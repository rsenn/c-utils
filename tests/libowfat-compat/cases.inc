#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define OUT(...) do{ size_t l_=strlen(out); snprintf(out+l_,osz-l_,__VA_ARGS__);}while(0)
static void hex(char*out,size_t osz,const char*b,size_t n){size_t i;for(i=0;i<n;++i)OUT("%02x",(unsigned char)b[i]);}
#define T(name) void PFX(t_##name)(const unsigned char* in,size_t n,unsigned long long v,char*out,size_t osz)
#define BUF char b[128]; memset(b,0x55,sizeof b); out[0]=0;
#define SB char s[64]; { size_t k=n<63?n:63; memcpy(s,in,k); s[k]=0; }

T(fmt_ulong){BUF size_t r=fmt_ulong(b,(unsigned long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_long){BUF size_t r=fmt_long(b,(long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_xlong){BUF size_t r=fmt_xlong(b,(unsigned long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_xlonglong){BUF size_t r=fmt_xlonglong(b,v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_8long){BUF size_t r=fmt_8long(b,(unsigned long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_ulonglong){BUF size_t r=fmt_ulonglong(b,v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_longlong){BUF size_t r=fmt_longlong(b,(long long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_ulong0){BUF size_t r=fmt_ulong0(b,(unsigned long)(v&0xffffff),(size_t)(n&7)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_minus){BUF size_t r=fmt_minus(b,(long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_plusminus){BUF size_t r=fmt_plusminus(b,(long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_str){BUF SB size_t r=fmt_str(b,s); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_strn){BUF SB size_t r=fmt_strn(b,s,(size_t)(v&15)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_pad){BUF SB size_t r=fmt_pad(b,s,strlen(s),(size_t)(v&15),(size_t)((v>>4)&31)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_fill){BUF size_t r=fmt_fill(b,(size_t)(v&15),(size_t)((v>>4)&31),(size_t)((v>>9)&31)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_utf8){BUF size_t r=fmt_utf8(b,(uint32_t)(v&0x7fffffff)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_escapecharjson){BUF size_t r=fmt_escapecharjson(b,(uint32_t)(v&0x1fffff)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_escapecharxml){BUF size_t r=fmt_escapecharxml(b,(uint32_t)(v&0x1fffff)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_escapecharhtml){BUF size_t r=fmt_escapecharhtml(b,(uint32_t)(v&0x1fffff)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_escapecharc){BUF size_t r=fmt_escapecharc(b,(uint32_t)(v&0x1fffff)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_varint){BUF size_t r=fmt_varint(b,v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_pb_tag){BUF size_t r=fmt_pb_tag(b,(size_t)(v&0xffff),(unsigned char)((v>>16)&7)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_pb_type0_sint){BUF size_t r=fmt_pb_type0_sint(b,(long long)v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_pb_type1_double){BUF double d; memcpy(&d,&v,8); size_t r=fmt_pb_type1_double(b,d); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_pb_type2_string){BUF SB size_t r=fmt_pb_type2_string(b,s,strlen(s)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_pb_string){BUF SB size_t r=fmt_pb_string(b,(size_t)(v&0xff),s,strlen(s)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_netstring){BUF SB size_t r=fmt_netstring(b,s,strlen(s)); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_human){BUF size_t r=fmt_human(b,v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_humank){BUF size_t r=fmt_humank(b,v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_asn1dertag){BUF size_t r=fmt_asn1dertag(b,v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_asn1derlength){BUF size_t r=fmt_asn1derlength(b,v); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_httpdate){BUF time_t t=(time_t)(v%4102444800ull); size_t r=fmt_httpdate(b,t); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(fmt_iso8601){BUF time_t t=(time_t)(v%4102444800ull); size_t r=fmt_iso8601(b,t); OUT("%zu:",r); hex(out,osz,b,r+1);}

#define SC(name,type,fmtstr,cast) T(name){ SB type x=0; size_t r=name(s,&x); OUT("%zu:" fmtstr,r,(cast)x);}
SC(scan_ulong,unsigned long,"%lu",unsigned long)
SC(scan_xlong,unsigned long,"%lu",unsigned long)
SC(scan_8long,unsigned long,"%lu",unsigned long)
SC(scan_long,long,"%ld",long)
SC(scan_ulonglong,unsigned long long,"%llu",unsigned long long)
SC(scan_xlonglong,unsigned long long,"%llu",unsigned long long)
SC(scan_8longlong,unsigned long long,"%llu",unsigned long long)
SC(scan_longlong,long long,"%lld",long long)
SC(scan_uint,unsigned int,"%u",unsigned)
SC(scan_int,int,"%d",int)
SC(scan_ushort,unsigned short,"%u",unsigned)
SC(scan_short,short,"%d",int)
SC(scan_xshort,unsigned short,"%u",unsigned)
SC(scan_8short,unsigned short,"%u",unsigned)
SC(scan_xint,unsigned int,"%u",unsigned)
SC(scan_8int,unsigned int,"%u",unsigned)
SC(scan_plusminus,int,"%d",int)
SC(scan_double,double,"%.17g",double)
T(scan_ulongn){SB unsigned long x=0; size_t r=scan_ulongn(s,(size_t)(v&15),&x); OUT("%zu:%lu",r,x);}
T(scan_longn){SB long x=0; size_t r=scan_longn(s,(size_t)(v&15),&x); OUT("%zu:%ld",r,x);}
T(scan_xlongn){SB unsigned long x=0; size_t r=scan_xlongn(s,(size_t)(v&15),&x); OUT("%zu:%lu",r,x);}
T(scan_8longn){SB unsigned long x=0; size_t r=scan_8longn(s,(size_t)(v&15),&x); OUT("%zu:%lu",r,x);}
T(scan_whitenskip){SB size_t r=scan_whitenskip(s,(size_t)(v&63)); OUT("%zu",r);}
T(scan_nonwhitenskip){SB size_t r=scan_nonwhitenskip(s,(size_t)(v&63)); OUT("%zu",r);}
T(scan_charsetnskip){SB size_t r=scan_charsetnskip(s,"abc",strlen(s)); OUT("%zu",r);}
T(scan_noncharsetnskip){SB size_t r=scan_noncharsetnskip(s,"abc",strlen(s)); OUT("%zu",r);}
T(scan_utf8){uint32_t x=0; size_t r=scan_utf8((const char*)in,n,&x); OUT("%zu:%u",r,(unsigned)x);}
T(scan_utf8_sem){uint32_t x=0; size_t r=scan_utf8_sem((const char*)in,n,&x); OUT("%zu:%u",r,(unsigned)x);}
T(scan_varint){unsigned long long x=0; size_t r=scan_varint((const char*)in,n,&x); OUT("%zu:%llu",r,x);}
T(scan_asn1derlength){unsigned long long x=0; size_t r=scan_asn1derlength((const char*)in,n,&x); OUT("%zu:%llu",r,x);}
T(scan_asn1dertag){unsigned long long x=0; size_t r=scan_asn1dertag((const char*)in,n,&x); OUT("%zu:%llu",r,x);}
T(scan_pb_type0_sint){signed long long x=0; size_t r=scan_pb_type0_sint((const char*)in,n,&x); OUT("%zu:%lld",r,x);}
T(scan_pb_type0_int){unsigned long long x=0; size_t r=scan_pb_type0_int((const char*)in,n,&x); OUT("%zu:%llu",r,x);}
T(scan_pb_tag){size_t f=0; unsigned char ty=0; size_t r=scan_pb_tag((const char*)in,n,&f,&ty); OUT("%zu:%zu:%u",r,f,ty);}
T(scan_pb_type1_fixed64){uint64_t x=0; size_t r=scan_pb_type1_fixed64((const char*)in,n,&x); OUT("%zu:%llx",r,(unsigned long long)x);}
T(scan_pb_type5_fixed32){uint32_t x=0; size_t r=scan_pb_type5_fixed32((const char*)in,n,&x); OUT("%zu:%x",r,(unsigned)x);}
T(scan_pb_type2_stringlen){const char*p=0; size_t sl=0; size_t r=scan_pb_type2_stringlen((const char*)in,n,&p,&sl); OUT("%zu:%zu:%zd",r,sl,p?p-(const char*)in:-1);}
T(scan_netstring){char*p=0; size_t sl=0; size_t r=scan_netstring((const char*)in,n,&p,&sl); OUT("%zu:%zu:%zd",r,sl,p?p-(char*)in:-1);}
T(scan_fromhex){OUT("%d",scan_fromhex((int)(v&255)));}
T(scan_httpdate){SB time_t t=0; size_t r=scan_httpdate(s,&t); OUT("%zu:%lld",r,(long long)t);}
T(scan_iso8601){SB struct timespec t={0,0}; size_t r=scan_iso8601(s,&t); OUT("%zu:%lld:%ld",r,(long long)t.tv_sec,r?t.tv_nsec:0);}

T(str_len){SB OUT("%zu",str_len(s));}
T(str_chr){SB OUT("%zu",str_chr(s,(char)(v&255)));}
T(str_rchr){SB OUT("%zu",str_rchr(s,(char)(v&255)));}
T(str_diff){SB char u[64]; size_t k=(size_t)(v&31); memset(u,0,sizeof u); memcpy(u,in+(n>k?k:0),n>k?n-k:0); OUT("%d",str_diff(s,u)>0?1:str_diff(s,u)<0?-1:0);}
T(str_start){SB OUT("%d",str_start(s,"ab"));}
T(str_equal){SB OUT("%d",str_equal(s,"ab"));}
T(str_diffn){SB OUT("%d",str_diffn(s,"abc",(size_t)(v&7))>0?1:str_diffn(s,"abc",(size_t)(v&7))<0?-1:0);}
T(str_copy){BUF SB size_t r=str_copy(b,s); OUT("%zu:",r); hex(out,osz,b,r+1);}
T(byte_chr){OUT("%zu",byte_chr(in,n,(char)(v&255)));}
T(byte_rchr){OUT("%zu",byte_rchr(in,n,(char)(v&255)));}
T(byte_diff){size_t k=n/2; int d=byte_diff(in,k,in+k); OUT("%d",d>0?1:d<0?-1:0);}
T(byte_equal){size_t k=n/2; OUT("%d",byte_equal(in,k,in+k));}
T(byte_zero){BUF byte_zero(b,(size_t)(v&63)); hex(out,osz,b,64);}
T(byte_copy){BUF size_t k=n<100?n:100; byte_copy(b,k,in); hex(out,osz,b,k+1);}
T(byte_copyr){BUF size_t k=n<100?n:100; byte_copyr(b,k,in); hex(out,osz,b,k+1);}
T(case_diffs){SB int d=case_diffs(s,"AbC"); OUT("%d",d>0?1:d<0?-1:0);}
T(case_starts){SB OUT("%d",case_starts(s,"AbC"));}
T(case_diffb){int d=case_diffb(in,n/2,in+n/2); OUT("%d",d>0?1:d<0?-1:0);}
T(uint16_pack_unpack){char b[2]; uint16_t x; uint16_pack(b,(uint16_t)v); hex(out,osz,b,2); uint16_unpack(b,&x); OUT(":%u",x); uint16_pack_big(b,(uint16_t)v); hex(out,osz,b,2); uint16_unpack_big(b,&x); OUT(":%u",x);}
T(uint32_pack_unpack){char b[4]; uint32_t x; uint32_pack(b,(uint32_t)v); hex(out,osz,b,4); uint32_unpack(b,&x); OUT(":%u",x); uint32_pack_big(b,(uint32_t)v); hex(out,osz,b,4); uint32_unpack_big(b,&x); OUT(":%u",x);}
T(uint64_pack_unpack){char b[8]; uint64_t x; uint64_pack(b,v); hex(out,osz,b,8); uint64_unpack(b,&x); OUT(":%llu",(unsigned long long)x); uint64_pack_big(b,v); hex(out,osz,b,8); uint64_unpack_big(b,&x); OUT(":%llu",(unsigned long long)x);}
