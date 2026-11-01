static unsigned long long rs=88172645463325252ull;
static unsigned long long rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static const char alpha[]="0123456789abcdefxXeE+-. \t\n\r:,;ABCZ\x80\xc3\xa9\xe2\xff\x01\x7f%";
int main(int argc,char**argv){ int iters=argc>1?atoi(argv[1]):20000; size_t i,t; int bad=0; static char oa[600],ob[600];
 for(t=0;t<sizeof T/sizeof T[0];++t){ int nb=0;
  for(i=0;i<(size_t)iters;++i){ unsigned char in[48]; size_t n=rnd()%40,k; unsigned long long v; int mode=rnd()%4;
   for(k=0;k<n;++k){ in[k]= mode==0? (unsigned char)rnd() : (unsigned char)alpha[rnd()%(sizeof alpha-1)]; }
   if(mode==3 && n){ for(k=0;k<n;++k) in[k]="0123456789"[rnd()%10]; }
   in[n]=0; switch(rnd()%5){case 0:v=rnd()&0xff;break;case 1:v=rnd();break;case 2:v=rnd()>>(rnd()%64);break;case 3:v=(rnd()%2)?~0ull-(rnd()%3):1ull<<(rnd()%64);break;default:v=rnd()%1000;}
   oa[0]=ob[0]=0; T[t].a(in,n,v,oa,sizeof oa); T[t].b(in,n,v,ob,sizeof ob);
   if(strcmp(oa,ob)){ if(nb<2){ printf("DIFF %s v=%llu n=%zu in=",T[t].n,v,n); for(k=0;k<n;++k)printf("%02x",in[k]); printf("\n   libowfat: %s\n   c-utils : %s\n",oa,ob);} ++nb; }
  }
  if(nb){ printf("== %s: %d/%d differ\n",T[t].n,nb,iters); ++bad; }
 }
 printf("%zu functions compared, %d with differences\n",sizeof T/sizeof T[0],bad); return bad!=0; }
