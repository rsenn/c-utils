#include <time.h>
#include <ctype.h>
#include <string.h>
#include "../str.h"

#if(defined(__MSYS__) && __MSYS__ == 1)
#define isblank(c) ((c)==' '||(c)=='\t')
#endif

int isleap(int year);

extern const short  __spm [];

static const char*  months [12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

static int getint(const char** s,int max) {
  int i,j;
  for (i=j=0; j<max; ++j) {
    if(!isdigit(**s)) {
      if(j==0) return -1; else break;
    }
    i=i*10+**s-'0';
    ++*s;
  }
  return i;
}

char* str_ptime(const char* s,const char* format, struct tm* tm) {
  int i,j;
  register time_t  day;
  while(*format) {
    switch (*format) {
    case ' ': case '\t':
      /* match zero or more white space in input string */
      while(*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') ++s;
      ++format;
      break;
    case '%':
      ++format;
      switch (*format) {
      case '%': if(*s=='%') ++s; else return 0; break;
      case 'a': case 'A': /* weekday; we just skip */
	for (i=0; i<3; ++i)
	  if(isalpha(*s)) ++s;
	break;
      case 'b': case 'B': case 'h':
	for (i=0; i<12; ++i) {
	  if(strncasecmp(s,months[i],j=str_len(months[i])))
	    if(strncasecmp(s,months[i],j=3))
	      j=0;
	  if(j) { break; };
	}
	if(!j) return 0;
	s+=j;
	tm->tm_mon=i;
	break;
      case 'c':
	s=str_ptime(s,"%b %a %d %k:%M:%S %Z %Y",tm);
	break;
      case 'C':
	i=getint(&s,2);
	if(i==-1) return 0;
	tm->tm_year=(tm->tm_year%100)+(i*100);
	break;
      case 'd': case 'e':
	i=getint(&s,2);
	if(i==-1 || i>31) return 0;
	tm->tm_mday=i;
	break;
      case 'D':
	s=str_ptime(s,"%m/%d/%y",tm);
	break;
      case 'H': case 'k':
	i=getint(&s,2);
	if(i==-1 || i>23) return 0;
	tm->tm_hour=i;
	break;
      case 'I': case 'l':
	i=getint(&s,2);
	if(i==-1 || i>12) return 0;
	tm->tm_hour=(tm->tm_hour/12)*12+i;
	break;
      case 'j':
	getint(&s,3);	/* not used */
	break;
      case 'm':
	i=getint(&s,2);
	if(i<=0 || i>12) return 0;
	tm->tm_mon=i-1;
	break;
      case 'M':
	i=getint(&s,2);
	if(i==-1 || i>59) return 0;
	tm->tm_min=i;
	break;
      case 'n': case 't':
	while(isblank(*s)) ++s;
	break;
      case 'p': case 'P':
	if(*s=='p' || *s=='P') tm->tm_hour=(tm->tm_hour%12)+12;
	s+=2;
	break;
      case 'r':
	s=str_ptime(s,"%I:%M:%S %p",tm);
	break;
      case 'R':
	s=str_ptime(s,"%H:%M",tm);
	break;
      case 'S':
	i=getint(&s,2);
	if(i==-1 || i>60) return 0;
	tm->tm_sec=i;
	break;
      case 'T':
	s=str_ptime(s,"%H:%M:%S",tm);
	break;
      case 'U': case 'W':
	if(getint(&s,2)==-1) return 0;
	break;
      case 'w':
	if(*s<'0' || *s>'6') return 0;
	++s;
	break;
      case 'x':
	s=str_ptime(s,"%b %a %d",tm);
	break;
      case 'X':
	s=str_ptime(s,"%k:%M:%S",tm);
	break;
      case 'y':
	i=getint(&s,2);
	if(i<0) return 0;
	tm->tm_year=(i<69)?i+100:i;
	break;
      case 'Y':
	i=getint(&s,4);
	if(i==-1) return 0;
	tm->tm_year=i-1900;
	break;
      case 'Z':
	/* time zone.  Not sure what I'm expected to do here. We'll just
	 * skip to the next whitespace */
	while(*s!=' ' && *s!='\t') ++s;
	break;
      }
      ++format;
      break;
    default:
      if(*s != *format) return 0;
      ++format; ++s;
      break;
    }
  }

  day  = (tm->tm_year - 70) * 365 + (tm->tm_year - 69) / 4;
  day += tm->tm_yday = __spm [tm->tm_mon] + tm->tm_mday-1 + (isleap (tm->tm_year+1900) & (tm->tm_mon > 1));
  tm->tm_wday = (day + 4) % 7;

  return(char*)s;
}
