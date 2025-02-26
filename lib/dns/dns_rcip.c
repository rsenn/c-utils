#define USE_WS2_32 1

#include "../socket_internal.h"
#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include "../ip4.h"
#include "../ip6.h"
#include "../open.h"
#include "../taia.h"
#include "../stralloc.h"
#include "../scan.h"

#include <stdlib.h>

#if WINDOWS
#if !defined(__ORANGEC__) && !defined(__CYGWIN__)
#include <winsock2.h>
#endif
#include <windows.h>

#define MAX_HOSTNAME_LEN 128
#define MAX_DOMAIN_NAME_LEN 128
#define MAX_SCOPE_ID_LEN 256
typedef struct {
  char String[4 * 4];
} IP_ADDRESS_STRING, *PIP_ADDRESS_STRING, IP_MASK_STRING, *PIP_MASK_STRING;
typedef struct _IP_ADDR_STRING {
  struct _IP_ADDR_STRING* Next;
  IP_ADDRESS_STRING IpAddress;
  IP_MASK_STRING IpMask;
  DWORD Context;
} IP_ADDR_STRING, *PIP_ADDR_STRING;

typedef struct {
  char HostName[MAX_HOSTNAME_LEN + 4];
  char DomainName[MAX_DOMAIN_NAME_LEN + 4];
  PIP_ADDR_STRING CurrentDnsServer;
  IP_ADDR_STRING DnsServerList;
  UINT NodeType;
  char ScopeId[MAX_SCOPE_ID_LEN + 4];
  UINT EnableRouting;
  UINT EnableProxy;
  UINT EnableDns;
} FIXED_INFO, *PFIXED_INFO;
#endif

static stralloc data;

static int
init(char ip[256]) {
  unsigned long int i;
  unsigned long int j;
  int iplen = 0;
  char* x;
#if WINDOWS
  FIXED_INFO* pFixedInfo;
  ULONG ulOutBufLen;
  typedef DWORD(WINAPI get_network_params_fn)(PFIXED_INFO pFixedInfo, PULONG pOutBufLen);
  static get_network_params_fn* get_network_params;

  if(get_network_params == 0) {
    HANDLE iphlpapi = LoadLibraryA("iphlpapi.dll");

    if(iphlpapi != INVALID_HANDLE_VALUE) {
      if((get_network_params = (get_network_params_fn*)GetProcAddress(iphlpapi, "GetNetworkParams")) == 0)
        return -1;
    }
  }
#endif

  x = getenv("DNSCACHEIP");

#if WINDOWS

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

  if(!x) {
    /* Make an initial call to GetAdaptersInfo to get the necessary size
     * into the ulOutBufLen variable */

    pFixedInfo = (FIXED_INFO*)MALLOC(sizeof(FIXED_INFO));

    if(pFixedInfo) {
      ulOutBufLen = sizeof(FIXED_INFO);

      if((*get_network_params)(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pFixedInfo);
        pFixedInfo = (FIXED_INFO*)MALLOC(ulOutBufLen);
      }
    }

    if(pFixedInfo) {
      if((*get_network_params)(pFixedInfo, &ulOutBufLen) == NO_ERROR) {
        x = pFixedInfo->DnsServerList.IpAddress.String;
      }
    }
  }
#endif

  if(x)

    while(iplen <= 60) {
      if(*x == '.')
        ++x;
      else {
        i = scan_ip6(x, ip + iplen);

        if(!i)
          break;
        x += i;
        iplen += 16;
      }
    }

  if(!iplen) {
    const char* rcf;
    rcf = getenv("RESOLVCONF");

    if(rcf == NULL)
      rcf = "/etc/resolv.conf";
    i = openreadclose(rcf, &data, 64);

    if(i == (unsigned long int)-1)
      return -1;

    if(i) {
      size_t i, n, p = 0;

      if(!stralloc_append(&data, "\n"))
        return -1;
      i = 0;

      do {
        p += scan_whitenskip(&data.s[p], data.len - p);

        if(!case_diffb(&data.s[p], 10, "nameserver")) {
          if(p + 10 < data.len)
            p += 10;
          p += scan_whitenskip(&data.s[p], data.len - p);

          if(iplen <= 60)

            if((i = scan_ip6(data.s + p, ip + iplen)))
              iplen += 16;
        }
        p += byte_chr(&data.s[p], data.len - p, '\n');
      } while(++p < data.len);
    }
  }

  if(!iplen) {
    byte_copy(ip, 16, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1");
    iplen = 16;
  }
  byte_zero(ip + iplen, 256 - iplen);
  return iplen;
}

static int ok;
static unsigned int uses;
static struct taia deadline;
static char ip[256]; /* defined if ok */

int
dns_resolvconfip(char s[256]) {
  struct taia now;

  taia_now(&now);

  if(taia_less(&deadline, &now))
    ok = 0;

  if(!uses)
    ok = 0;

  if(!ok) {
    if(init(ip) == -1)
      return -1;
    taia_uint(&deadline, 600);
    taia_add(&deadline, &now, &deadline);
    uses = 10000;
    ok = 1;
  }

  --uses;
  byte_copy(s, 256, ip);
  return 0;
}
