#ifndef DNSOPT_H
#define DNSOPT_H

#include "../../lib/buffer.h"

/* dnsopt_parse handles the command line common to the dns client programs:
 *
 *   -s, --server=IP   resolver address, sets DNSCACHEIP
 *   -p, --port=PORT   resolver port, sets DNSCACHEPORT
 *   -t, --timeout=S   seconds per attempt, fractional such as 0.5
 *   -T, --tcp         send the queries over TCP
 *   -c, --conf=FILE   take the servers from FILE instead of /etc/resolv.conf
 *   --no-recurse      clear the recursion desired bit
 *   -4, -6            use only IPv4 or only IPv6 resolver servers
 *   -v, --verbose     show queries and replies on stderr
 *   -d, --debug       dump traffic and internals (DEBUG_OUTPUT build only,
 *                     the environment variable DEBUG does the same)
 *   -h, --help        show usage and exit
 *   @server           ask this server, an address or a host name, anywhere
 *                     on the command line, may be given more than once
 *
 * The environment is set and the @server names are resolved before this
 * returns, so before the first query. The @server arguments are removed
 * from argv. Returns the index of the first name, exits on error or when
 * there is no name. args is the usage text after the program name, such as
 * "name...". */
int dnsopt_parse(int argc, char* const argv[], const char* args);

/* dnsopt_fn looks up one name and prints the result. Returns 0 on success,
 * -1 after it reported a failure with dnsopt_warn. */
typedef int dnsopt_fn(const char* name);

/* dnsopt_run calls fn for each name in turn, and for each name once per
 * @server, selecting the server first. A failure does not stop the run.
 * Returns 0, or 111 if any lookup failed. */
int dnsopt_run(char* const names[], dnsopt_fn* fn);

/* dnsopt_prefix puts "server: " to b when more than one @server was given,
 * call it at the start of each output line of a lookup. */
void dnsopt_prefix(buffer* b);

/* dnsopt_warn reports a failed lookup on stderr: what, name, the server if
 * there is one, and the error of errno. */
void dnsopt_warn(const char* what, const char* name);

#endif /* defined(DNSOPT_H) */
