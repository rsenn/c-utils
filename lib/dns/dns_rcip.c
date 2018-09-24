#include "../windoze.h"
#include "../byte.h"
#include "../dns.h"
#include "../ip4.h"
#include "../ip6.h"
#include "../open.h"
#include "../taia.h"
#include <stdlib.h>
#if WINDOWS
#include <windows.h>
#include <iphlpapi.h>
#include <winsock2.h>
#endif

static stralloc data;

static int
init(char ip[256]) {
  unsigned long int i;
  unsigned long int j;
  int iplen = 0;
  char* x;
#if WINDOWS
  FIXED_INFO *pFixedInfo;
  ULONG ulOutBufLen;
#endif

  x = getenv("DNSCACHEIP");

#if WINDOWS


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
  if(!x) {
    /* Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable */

    pFixedInfo = (FIXED_INFO *) MALLOC(sizeof(FIXED_INFO));
    if(pFixedInfo) {
      ulOutBufLen = sizeof(FIXED_INFO);
      if(GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pFixedInfo);
        pFixedInfo = (FIXED_INFO *) MALLOC(ulOutBufLen);
      }
    }
    if(pFixedInfo) {
      if(GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) {
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
        if(!i) break;
        x += i;
        iplen += 16;
      }
    }

  if(!iplen) {
    i = openreadclose("/etc/resolv.conf", &data, 64);
    if(i == (unsigned long int) -1) return -1;
    if(i) {
      if(!stralloc_append(&data, "\n")) return -1;
      i = 0;
      for(j = 0; j < data.len; ++j)
        if(data.s[j] == '\n') {
          if(byte_equal("nameserver ", 11, data.s + i) || byte_equal("nameserver\t", 11, data.s + i)) {
            i += 10;
            while((data.s[i] == ' ') || (data.s[i] == '\t')) ++i;
            if(iplen <= 60)
              if(scan_ip6(data.s + i, ip + iplen)) {
                iplen += 16;
              }
          }
          i = j + 1;
        }
    }
  }

  if(!iplen) {
    byte_copy(ip, 16, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1");
    iplen = 16;
  }
  byte_zero(ip + iplen, 256 - iplen);
  return 0;
}

static int ok;
static unsigned int uses;
static struct taia deadline;
static char ip[256]; /* defined if ok */

int
dns_resolvconfip(char s[256]) {
  struct taia now;

  taia_now(&now);
  if(taia_less(&deadline, &now)) ok = 0;
  if(!uses) ok = 0;

  if(!ok) {
    if(init(ip) == -1) return -1;
    taia_uint(&deadline, 600);
    taia_add(&deadline, &now, &deadline);
    uses = 10000;
    ok = 1;
  }

  --uses;
  byte_copy(s, 256, ip);
  return 0;
}
