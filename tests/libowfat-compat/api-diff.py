# usage: api-diff.py LIBOWFAT_SRC_DIR [CUTILS_LIB_DIR [LIBOWFAT_A]]
# compares every function prototype declared in libowfat's headers with
# the declaration in c-utils' lib/*.h (arguments, return type, macros).
import re,glob,os,subprocess,sys
OWDIR=sys.argv[1] if len(sys.argv)>1 else '/usr/include'
CUDIR=sys.argv[2] if len(sys.argv)>2 and sys.argv[2] else os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..','lib')
OW=glob.glob(os.path.join(OWDIR,'*.h'))
CU=glob.glob(os.path.join(CUDIR,'*.h'))
def strip(t):
    t=re.sub(r'/\*.*?\*/',' ',t,flags=re.S); t=re.sub(r'//[^\n]*','',t); t=re.sub(r'\\\n',' ',t)
    macros=set(re.findall(r'^[ \t]*#[ \t]*define[ \t]+(\w+)',t,re.M))
    t=re.sub(r'^[ \t]*#[^\n]*$','',t,flags=re.M)
    t=re.sub(r'extern\s*"C"\s*\{','',t)
    # remove brace bodies
    out=[];d=0
    for ch in t:
        if ch=='{':
            d+=1
            if d==1: out.append('{}')
        elif ch=='}':
            d=max(0,d-1)
        elif d==0: out.append(ch)
    return ''.join(out),macros
def stmts(t):
    # statements separated by ';' or by the '{}' marker
    t=t.replace('{}',';{};')
    return [s.strip() for s in t.split(';') if s.strip() and s.strip()!='{}']
ATT=re.compile(r'\batt_\w+(\s*\([^)]*\))?|__attribute__\s*\(\(.*?\)\)|\b__\w+__\b')
def parse(path):
    t,m=strip(open(path,errors='ignore').read())
    out={}
    for s in stmts(t):
        s=ATT.sub('',s)
        mm=re.match(r'^(?:extern\s+)?([\w\s\*]+?)\b(\w+)\s*\((.*)\)\s*$',s,re.S)
        if not mm: continue
        ret,name,args=mm.groups()
        ret=' '.join(ret.split())
        if not ret or name in ('if','while','for','switch','return','sizeof','defined'): continue
        out.setdefault(name,[]).append((ret,args.strip()))
    return out,m
def split_args(s):
    s=s.strip()
    if s in('','void'): return []
    parts=[];d=0;cur=''
    for ch in s:
        if ch=='(':d+=1
        if ch==')':d-=1
        if ch==',' and d==0: parts.append(cur);cur=''
        else: cur+=ch
    parts.append(cur); return [p.strip() for p in parts]
CANON={'uint64_t':'unsigned long','uint64':'unsigned long','int64_t':'long','int64':'long','uint32_t':'unsigned int','uint32':'unsigned int','int32_t':'int','int32':'int','uint16_t':'unsigned short','uint16':'unsigned short','int16_t':'short','int16':'short','uint8_t':'unsigned char','uint8':'unsigned char','size_t':'unsigned long','ssize_t':'long','fd_type':'int','__UINT64_TYPE__':'unsigned long','__INT64_TYPE__':'long','unsigned long int':'unsigned long','long int':'long','unsigned int':'unsigned int','signed long long':'long long','signed long':'long','unsigned long long int':'unsigned long long','long long int':'long long'}
def pointee(a):
    a=re.sub(r'\b(const|extern|static|inline|register|volatile|restrict|struct|enum)\b','',a)
    if '(' in a: return None
    if '*' not in a: return None
    base=a[:a.index('*')].strip()
    stars=a.count('*')
    base=' '.join(base.split())
    for k in sorted(CANON,key=len,reverse=True):
        base=re.sub(r'\b'+re.escape(k)+r'\b','\0',base) if False else base
    base=CANON.get(base,base)
    if base in ('signed','unsigned'): base+=' int'
    if base=='signed char': base='char?'
    return base+'*'*stars
def klass(a):
    a=re.sub(r'\b(const|extern|static|inline|register|volatile|restrict|struct|enum|unsigned|signed)\b','',a).strip()
    if '(' in a and '*' in a: return 'fnptr'
    if '[' in a or '*' in a: return 'ptr'
    if a=='...': return 'va'
    return 'val'
ow={};owm=set();cu={};cum=set()
for h in OW:
    p,m=parse(h); owm|=m
    for k,v in p.items(): ow.setdefault(k,[]).extend([(os.path.basename(h),)+x for x in v])
for h in CU:
    p,m=parse(h); cum|=m
    for k,v in p.items(): cu.setdefault(k,[]).extend([(os.path.basename(h),)+x for x in v])
syms=set()
if len(sys.argv)>3:   # optional: libowfat.a, to tell exported symbols from header-only declarations
    syms=set(l.split()[-1] for l in subprocess.run(['nm','--defined-only','-g',sys.argv[3]],capture_output=True,text=True).stdout.splitlines() if len(l.split())==3)
miss=[];diff=[];ok=0
for name,decls in sorted(ow.items()):
    if name not in syms and name not in owm: 
        if not any(True for d in decls): continue
    if name.startswith('__') or name.startswith('att_'): continue
    c=cu.get(name)
    if not c:
        if name in cum: ok+=1
        else: miss.append((name,decls[0][0],'sym' if name in syms else 'decl'))
        continue
    o=decls[0];cc=c[0]
    oa=split_args(o[2]);ca=split_args(cc[2])
    why=[]
    if len(oa)!=len(ca) and not(oa and oa[-1]=='...'): why.append('%d args vs %d'%(len(oa),len(ca)))
    else:
        for i,(x,y) in enumerate(zip(oa,ca)):
            if klass(x)!=klass(y): why.append('arg%d %s vs %s'%(i+1,klass(x),klass(y)))
            else:
                px,py=pointee(x),pointee(y)
                if px and py and px!=py and 'void' not in px and 'void' not in py and px!='char*' and py!='char*' and 'char' not in px+py: why.append('arg%d pointee %s vs %s'%(i+1,px,py))
    rk=lambda r:'void' if re.search(r'\bvoid$',r) and '*' not in r else ('ptr' if '*' in r else 'val')
    if rk(o[1])!=rk(cc[1]): why.append('returns %s vs %s'%(o[1],cc[1]))
    if why: diff.append((name,o[0],cc[0],'; '.join(why),o[1]+' ('+o[2][:80]+')',cc[1]+' ('+cc[2][:80]+')'))
    else: ok+=1
print("checked %d: same shape %d, missing %d, differ %d"%(ok+len(miss)+len(diff),ok,len(miss),len(diff)))
print("\n== MISSING (not declared, not a macro, in c-utils lib/*.h)")
for n,h,k in miss: print("  %-30s %-14s [%s]"%(n,h,k))
print("\n== DIFFER")
for r in diff: print("  %s  (%s vs %s): %s\n     owf: %s\n     c-u: %s"%r)
