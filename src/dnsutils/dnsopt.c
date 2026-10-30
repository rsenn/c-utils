#include "dnsopt.h"
#include "../../lib/buffer.h"
#include "../../lib/dns.h"
#include "../../lib/env.h"
#include "../../lib/byte.h"
#include "../../lib/errmsg.h"
#include "../../lib/fmt.h"
#include "../../lib/getopt.h"
#include "../../lib/ip4.h"
#include "../../lib/ip6.h"
#include "../../lib/stralloc.h"
#include "../../lib/scan.h"
#include "../../lib/str.h"

#define MAXSERVERS 16

static struct {
  const char* name; /* as given on the command line */
  char ip[IP6_FMT];
} servers[MAXSERVERS];
static size_t nservers, cur;

/* addserver resolves name, an address or a host name, with the resolver in
 * effect so far and appends it to the server list. */
static void
addserver(const char* name) {
  static stralloc fqdn, out;
  char ip[16], tmp[IP6_FMT];
  const char* orig = name;
  size_t n, len = str_len(name);

  if(nservers == MAXSERVERS)
    die(100, "too many servers, at most 16", 0);

  if(name[0] == '[' && len > 1 && name[len - 1] == ']') {
    byte_copy(tmp, len - 2, name + 1);
    tmp[len - 2] = 0;
    name = tmp;
    len -= 2;
  }

  if((n = scan_ip4(name, ip)) && n == len && dns_family != 6)
    servers[nservers].ip[fmt_ip4(servers[nservers].ip, ip)] = 0;
  else if(dns_family != 4 && (n = scan_ip6(name, ip)) && n == len)
    servers[nservers].ip[fmt_ip6(servers[nservers].ip, ip)] = 0;
  else if(scan_ip4(name, ip) == len || scan_ip6(name, ip) == len)
    die(100, "address family of server does not match -4/-6: ", name, 0);
  else {
    if(!stralloc_copyb(&fqdn, name, len))
      die(111, "out of memory", 0);

    if(dns_family == 6 || dns_ip4(&out, &fqdn) == -1 || out.len < 4) {
      if(dns_family == 4 || dns_ip6(&out, &fqdn) == -1 || out.len < 16)
        diesys(111, "unable to resolve server ", name, 0);
      servers[nservers].ip[fmt_ip6(servers[nservers].ip, out.s)] = 0;
    } else
      servers[nservers].ip[fmt_ip4(servers[nservers].ip, out.s)] = 0;
  }
  servers[nservers++].name = orig;
  dns_resolvconfip_reset();
}

static void
usage(const char* prog, const char* args, int code) {
  buffer* b = code ? buffer_2 : buffer_1;

  buffer_puts(b, "usage: ");
  buffer_puts(b, prog);
  buffer_puts(b, " [options] @server... ");
  buffer_puts(b, args);
  buffer_puts(b,
              "\n\n"
              "  -s, --server=IP   resolver address (DNSCACHEIP)\n"
              "  -p, --port=PORT   resolver port (DNSCACHEPORT)\n"
              "  -t, --timeout=S   seconds per attempt, may be fractional\n"
              "  -T, --tcp         query over TCP\n"
              "  -c, --conf=FILE   read servers from FILE, not /etc/resolv.conf\n"
              "      --no-recurse  do not ask for recursion\n"
              "  -4                use IPv4 servers only\n"
              "  -6                use IPv6 servers only\n"
              "  -v, --verbose     show queries and replies\n"
#ifdef DEBUG_OUTPUT
              "  -d, --debug       dump traffic and internals (or set DEBUG)\n"
#endif
              "  -h, --help        show this help\n");
  buffer_flush(b);
  exit(code);
}

int
dnsopt_parse(int argc, char* const argv[], const char* args) {
  static const struct unix_longopt opts[] = {{"server", required_argument, 0, 's'},
                                             {"port", required_argument, 0, 'p'},
                                             {"timeout", required_argument, 0, 't'},
                                             {"tcp", no_argument, 0, 'T'},
                                             {"conf", required_argument, 0, 'c'},
                                             {"no-recurse", no_argument, 0, 'r'},
                                             {"ipv4", no_argument, 0, '4'},
                                             {"ipv6", no_argument, 0, '6'},
                                             {"verbose", no_argument, 0, 'v'},
                                             {"debug", no_argument, 0, 'd'},
                                             {"help", no_argument, 0, 'h'},
                                             {0, 0, 0, 0}};
  const char* prog = argv[0] ? argv[0] : "dns";
  const char* env;
  unsigned long port;
  size_t i = str_rchr(prog, '/');
  int c, index;

  if(prog[i])
    prog += i + 1;

  errmsg_iam(prog);

#ifdef DEBUG_OUTPUT
  if((env = env_get("DEBUG")) && *env && str_diff(env, "0"))
    dns_debug = dns_verbose = 1;
#endif

  unix_opterr = 0;

  while((c = unix_getopt_long(argc, argv, "s:p:t:c:T46vdh", opts, &index)) != -1) {
    switch(c) {
      case 's':
        /* DNSCACHE takes precedence over DNSCACHEIP in the resolver setup */
        env_unset("DNSCACHE");
        if(!env_set("DNSCACHEIP", unix_optarg))
          die(111, "out of memory", 0);
        break;
      case 'p':
        if(unix_optarg[scan_ulong(unix_optarg, &port)] || port == 0 || port > 65535)
          die(100, "invalid port: ", unix_optarg, 0);
        if(!env_set("DNSCACHEPORT", unix_optarg))
          die(111, "out of memory", 0);
        break;
      case 't': {
        /* seconds with up to 3 decimals, as milliseconds */
        unsigned long ms;
        size_t n = scan_ulong(unix_optarg, &ms), k = 0;

        ms *= 1000;
        if(n && unix_optarg[n] == '.') {
          const char* f = unix_optarg + n + 1;
          unsigned long scale = 100;

          for(; f[k] >= '0' && f[k] <= '9'; ++k, scale /= 10)
            ms += (f[k] - '0') * scale;
          n += 1 + k;
          if(!k)
            n = 0;
        }
        if(!n || unix_optarg[n] || !ms)
          die(100, "invalid timeout: ", unix_optarg, 0);
        dns_timeout_ms = ms;
        break;
      }
      case 'T': dns_tcp = 1; break;
      case 'r': dns_recurse = 0; break;
      case 'c':
        /* a server given by -s or in DNSCACHE would hide the file */
        env_unset("DNSCACHE");
        env_unset("DNSCACHEIP");
        if(!env_set("RESOLVCONF", unix_optarg))
          die(111, "out of memory", 0);
        break;
      case '4': dns_family = 4; break;
      case '6': dns_family = 6; break;
      case 'v': dns_verbose = 1; break;
#ifdef DEBUG_OUTPUT
      case 'd': dns_debug = dns_verbose = 1; break;
#endif
      case 'h': usage(prog, args, 0); break;
      default: usage(prog, args, 100); break;
    }
  }

  /* take the @server arguments out of argv, in any position */
  {
    int i, j = unix_optind;

    for(i = unix_optind; i < argc; ++i)
      if(argv[i][0] == '@')
        addserver(argv[i] + 1);
      else
        ((char**)argv)[j++] = argv[i];
    ((char**)argv)[j] = 0;

    if(j == unix_optind)
      usage(prog, args, 100);
  }
  return unix_optind;
}

static void
select_server(size_t i) {
  cur = i;
  env_unset("DNSCACHE");
  if(!env_set("DNSCACHEIP", servers[i].ip))
    die(111, "out of memory", 0);
  dns_resolvconfip_reset();
}

int
dnsopt_run(char* const names[], dnsopt_fn* fn) {
  int failed = 0;
  size_t i;

  for(; *names; ++names) {
    if(!nservers) {
      if(fn(*names))
        failed = 1;
      continue;
    }
    for(i = 0; i < nservers; ++i) {
      select_server(i);
      if(fn(*names))
        failed = 1;
    }
  }
  buffer_flush(buffer_1);
  return failed ? 111 : 0;
}

void
dnsopt_prefix(buffer* b) {
  if(nservers > 1) {
    buffer_puts(b, servers[cur].name);
    buffer_puts(b, ": ");
  }
}

void
dnsopt_warn(const char* what, const char* name) {
  buffer_flush(buffer_1);
  if(nservers)
    errmsg_warnsys(what, name, " at ", servers[cur].name, 0);
  else
    errmsg_warnsys(what, name, 0);
}
