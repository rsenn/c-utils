if(BUILD_BZIP2)
  message(STATUS "Building libbz2 from 3rdparty/bzip2")
  set(BZIP2_VERSION "1.0.8")
  set(BZIP2_DISABLE_TESTS TRUE)
  set(BZIP2_STATIC TRUE)
  set(BZIP2_NO_INSTALL TRUE)
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2/blocksort.c")
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bzip2-${BZIP2_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2")
      file(DOWNLOAD "https://sourceware.org/pub/bzip2/bzip2-${BZIP2_VERSION}.tar.gz" "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2-${BZIP2_VERSION}.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2-${BZIP2_VERSION}.tar.gz")

    file(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2-${BZIP2_VERSION}" "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2")
    file(
      WRITE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2/blocksort.c"
      "\n/*-------------------------------------------------------------*/\n/*--- Block sorting machinery                               ---*/\n/*---                                           blocksort.c ---*/\n/*-------------------------------------------------------------*/\n\n/* ------------------------------------------------------------------\n   This file is part of bzip2/libbzip2, a program and library for\n   lossless, block-sorting data compression.\n\n   bzip2/libbzip2 version 1.0.6 of 6 September 2010\n   Copyright (C) 1996-2010 Julian Seward <jseward@bzip.org>\n\n   Please read the WARNING, DISCLAIMER and PATENTS sections in the \n   README file.\n\n   This program is released under the terms of the license contained\n   in the file LICENSE.\n   ------------------------------------------------------------------ */\n\n\n#include \"bzlib_private.h\"\n\n/*---------------------------------------------*/\n/*--- Fallback O(N log(N)^2) sorting        ---*/\n/*--- algorithm, for repetitive blocks      ---*/\n/*---------------------------------------------*/\n\n/*---------------------------------------------*/\nstatic \n__inline__\nvoid fallbackSimpleSort ( UInt32* fmap, \n                          UInt32* eclass, \n                          Int32   lo, \n                          Int32   hi )\n{\n   Int32 i, j, tmp;\n   UInt32 ec_tmp;\n\n   if (lo == hi) return;\n\n   if (hi - lo > 3) {\n      for ( i = hi-4; i >= lo; i-- ) {\n         tmp = fmap[i];\n         ec_tmp = eclass[tmp];\n         for ( j = i+4; j <= hi && ec_tmp > eclass[fmap[j]]; j += 4 )\n            fmap[j-4] = fmap[j];\n         fmap[j-4] = tmp;\n      }\n   }\n\n   for ( i = hi-1; i >= lo; i-- ) {\n      tmp = fmap[i];\n      ec_tmp = eclass[tmp];\n      for ( j = i+1; j <= hi && ec_tmp > eclass[fmap[j]]; j++ )\n         fmap[j-1] = fmap[j];\n      fmap[j-1] = tmp;\n   }\n}\n\n\n/*---------------------------------------------*/\n#define fswap(zz1, zz2) \\\n   { Int32 zztmp = zz1; zz1 = zz2; zz2 = zztmp; }\n\n#define fvswap(zzp1, zzp2, zzn)       \\\n{                                     \\\n   Int32 yyp1 = (zzp1);               \\\n   Int32 yyp2 = (zzp2);               \\\n   Int32 yyn  = (zzn);                \\\n   while (yyn > 0) {                  \\\n      fswap(fmap[yyp1], fmap[yyp2]);  \\\n      yyp1++; yyp2++; yyn--;          \\\n   }                                  \\\n}\n\n\n#define fmin(a,b) ((a) < (b)) ? (a) : (b)\n\n#define fpush(lz,hz) { stackLo[sp] = lz; \\\n                       stackHi[sp] = hz; \\\n                       sp++; }\n\n#define fpop(lz,hz) { sp--;              \\\n                      lz = stackLo[sp];  \\\n                      hz = stackHi[sp]; }\n\n#define FALLBACK_QSORT_SMALL_THRESH 10\n#define FALLBACK_QSORT_STACK_SIZE   100\n\n\nstatic\nvoid fallbackQSort3 ( UInt32* fmap, \n                      UInt32* eclass,\n                      Int32   loSt, \n                      Int32   hiSt )\n{\n   Int32 unLo, unHi, ltLo, gtHi, n, m;\n   Int32 sp, lo, hi;\n   UInt32 med, r, r3;\n   Int32 stackLo[FALLBACK_QSORT_STACK_SIZE];\n   Int32 stackHi[FALLBACK_QSORT_STACK_SIZE];\n\n   r = 0;\n\n   sp = 0;\n   fpush ( loSt, hiSt );\n\n   while (sp > 0) {\n\n      AssertH ( sp < FALLBACK_QSORT_STACK_SIZE - 1, 1004 );\n\n      fpop ( lo, hi );\n      if (hi - lo < FALLBACK_QSORT_SMALL_THRESH) {\n         fallbackSimpleSort ( fmap, eclass, lo, hi );\n         continue;\n      }\n\n      /* Random partitioning.  Median of 3 sometimes fails to\n         avoid bad cases.  Median of 9 seems to help but \n         looks rather expensive.  This too seems to work but\n         is cheaper.  Guidance for the magic constants \n         7621 and 32768 is taken from Sedgewick's algorithms\n         book, chapter 35.\n      */\n      r = ((r * 7621) + 1) % 32768;\n      r3 = r % 3;\n      if (r3 == 0) med = eclass[fmap[lo]]; else\n      if (r3 == 1) med = eclass[fmap[(lo+hi)>>1]]; else\n                   med = eclass[fmap[hi]];\n\n      unLo = ltLo = lo;\n      unHi = gtHi = hi;\n\n      while (1) {\n         while (1) {\n            if (unLo > unHi) break;\n            n = (Int32)eclass[fmap[unLo]] - (Int32)med;\n            if (n == 0) { \n               fswap(fmap[unLo], fmap[ltLo]); \n               ltLo++; unLo++; \n               continue; \n            };\n            if (n > 0) break;\n            unLo++;\n         }\n         while (1) {\n            if (unLo > unHi) break;\n            n = (Int32)eclass[fmap[unHi]] - (Int32)med;\n            if (n == 0) { \n               fswap(fmap[unHi], fmap[gtHi]); \n               gtHi--; unHi--; \n               continue; \n            };\n            if (n < 0) break;\n            unHi--;\n         }\n         if (unLo > unHi) break;\n         fswap(fmap[unLo], fmap[unHi]); unLo++; unHi--;\n      }\n\n      AssertD ( unHi == unLo-1, \"fallbackQSort3(2)\" );\n\n      if (gtHi < ltLo) continue;\n\n      n = fmin(ltLo-lo, unLo-ltLo); fvswap(lo, unLo-n, n);\n      m = fmin(hi-gtHi, gtHi-unHi); fvswap(unLo, hi-m+1, m);\n\n      n = lo + unLo - ltLo - 1;\n      m = hi - (gtHi - unHi) + 1;\n\n      if (n - lo > hi - m) {\n         fpush ( lo, n );\n         fpush ( m, hi );\n      } else {\n         fpush ( m, hi );\n         fpush ( lo, n );\n      }\n   }\n}\n\n#undef fmin\n#undef fpush\n#undef fpop\n#undef fswap\n#undef fvswap\n#undef FALLBACK_QSORT_SMALL_THRESH\n#undef FALLBACK_QSORT_STACK_SIZE\n\n\n/*---------------------------------------------*/\n/* Pre:\n      nblock > 0\n      eclass exists for [0 .. nblock-1]\n      ((UChar*)eclass) [0 .. nblock-1] holds block\n      ptr exists for [0 .. nblock-1]\n\n   Post:\n      ((UChar*)eclass) [0 .. nblock-1] holds block\n      All other areas of eclass destroyed\n      fmap [0 .. nblock-1] holds sorted order\n      bhtab [ 0 .. 2+(nblock/32) ] destroyed\n*/\n\n#define       SET_BH(zz)  bhtab[(zz) >> 5] |= (1 << ((zz) & 31))\n#define     CLEAR_BH(zz)  bhtab[(zz) >> 5] &= ~(1 << ((zz) & 31))\n#define     ISSET_BH(zz)  (bhtab[(zz) >> 5] & (1 << ((zz) & 31)))\n#define      WORD_BH(zz)  bhtab[(zz) >> 5]\n#define UNALIGNED_BH(zz)  ((zz) & 0x01f)\n\nstatic\nvoid fallbackSort ( UInt32* fmap, \n                    UInt32* eclass, \n                    UInt32* bhtab,\n                    Int32   nblock,\n                    Int32   verb )\n{\n   Int32 ftab[257];\n   Int32 ftabCopy[256];\n   Int32 H, i, j, k, l, r, cc, cc1;\n   Int32 nNotDone;\n   Int32 nBhtab;\n   UChar* eclass8 = (UChar*)eclass;\n\n   /*--\n      Initial 1-char radix sort to generate\n      initial fmap and initial BH bits.\n   --*/\n   if (verb >= 4)\n      VPrintf0 ( \"        bucket sorting ...\\n\" );\n   for (i = 0; i < 257;    i++) ftab[i] = 0;\n   for (i = 0; i < nblock; i++) ftab[eclass8[i]]++;\n   for (i = 0; i < 256;    i++) ftabCopy[i] = ftab[i];\n   for (i = 1; i < 257;    i++) ftab[i] += ftab[i-1];\n\n   for (i = 0; i < nblock; i++) {\n      j = eclass8[i];\n      k = ftab[j] - 1;\n      ftab[j] = k;\n      fmap[k] = i;\n   }\n\n   nBhtab = 2 + (nblock / 32);\n   for (i = 0; i < nBhtab; i++) bhtab[i] = 0;\n   for (i = 0; i < 256; i++) SET_BH(ftab[i]);\n\n   /*--\n      Inductively refine the buckets.  Kind-of an\n      \"exponential radix sort\" (!), inspired by the\n      Manber-Myers suffix array construction algorithm.\n   --*/\n\n   /*-- set sentinel bits for block-end detection --*/\n   for (i = 0; i < 32; i++) { \n      SET_BH(nblock + 2*i);\n      CLEAR_BH(nblock + 2*i + 1);\n   }\n\n   /*-- the log(N) loop --*/\n   H = 1;\n   while (1) {\n\n      if (verb >= 4) \n         VPrintf1 ( \"        depth %6d has \", H );\n\n      j = 0;\n      for (i = 0; i < nblock; i++) {\n         if (ISSET_BH(i)) j = i;\n         k = fmap[i] - H; if (k < 0) k += nblock;\n         eclass[k] = j;\n      }\n\n      nNotDone = 0;\n      r = -1;\n      while (1) {\n\n   /*-- find the next non-singleton bucket --*/\n         k = r + 1;\n         while (ISSET_BH(k) && UNALIGNED_BH(k)) k++;\n         if (ISSET_BH(k)) {\n            while (WORD_BH(k) == 0xffffffff) k += 32;\n            while (ISSET_BH(k)) k++;\n         }\n         l = k - 1;\n         if (l >= nblock) break;\n         while (!ISSET_BH(k) && UNALIGNED_BH(k)) k++;\n         if (!ISSET_BH(k)) {\n            while (WORD_BH(k) == 0x00000000) k += 32;\n            while (!ISSET_BH(k)) k++;\n         }\n         r = k - 1;\n         if (r >= nblock) break;\n\n         /*-- now [l, r] bracket current bucket --*/\n         if (r > l) {\n            nNotDone += (r - l + 1);\n            fallbackQSort3 ( fmap, eclass, l, r );\n\n            /*-- scan bucket and generate header bits-- */\n            cc = -1;\n            for (i = l; i <= r; i++) {\n               cc1 = eclass[fmap[i]];\n               if (cc != cc1) { SET_BH(i); cc = cc1; };\n            }\n         }\n      }\n\n      if (verb >= 4) \n         VPrintf1 ( \"%6d unresolved strings\\n\", nNotDone );\n\n      H *= 2;\n      if (H > nblock || nNotDone == 0) break;\n   }\n\n   /*-- \n      Reconstruct the original block in\n      eclass8 [0 .. nblock-1], since the\n      previous phase destroyed it.\n   --*/\n   if (verb >= 4)\n      VPrintf0 ( \"        reconstructing block ...\\n\" );\n   j = 0;\n   for (i = 0; i < nblock; i++) {\n      while (ftabCopy[j] == 0) j++;\n      ftabCopy[j]--;\n      eclass8[fmap[i]] = (UChar)j;\n   }\n   AssertH ( j < 256, 1005 );\n}\n\n#undef       SET_BH\n#undef     CLEAR_BH\n#undef     ISSET_BH\n#undef      WORD_BH\n#undef UNALIGNED_BH\n\n\n/*---------------------------------------------*/\n/*--- The main, O(N^2 log(N)) sorting       ---*/\n/*--- algorithm.  Faster for \"normal\"       ---*/\n/*--- non-repetitive blocks.                ---*/\n/*---------------------------------------------*/\n\n/*---------------------------------------------*/\nstatic\n__inline__\nBool mainGtU ( UInt32  i1, \n               UInt32  i2,\n               UChar*  block, \n               UInt16* quadrant,\n               UInt32  nblock,\n               Int32*  budget )\n{\n   Int32  k;\n   UChar  c1, c2;\n   UInt16 s1, s2;\n\n   AssertD ( i1 != i2, \"mainGtU\" );\n   /* 1 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 2 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 3 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 4 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 5 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 6 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 7 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 8 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 9 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 10 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 11 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 12 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n\n   k = nblock + 8;\n\n   do {\n      /* 1 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 2 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 3 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 4 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 5 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 6 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 7 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 8 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n\n      if (i1 >= nblock) i1 -= nblock;\n      if (i2 >= nblock) i2 -= nblock;\n\n      k -= 8;\n      (*budget)--;\n   }\n      while (k >= 0);\n\n   return False;\n}\n\n\n/*---------------------------------------------*/\n/*--\n   Knuth's increments seem to work better\n   than Incerpi-Sedgewick here.  Possibly\n   because the number of elems to sort is\n   usually small, typically <= 20.\n--*/\nstatic\nInt32 incs[14] = { 1, 4, 13, 40, 121, 364, 1093, 3280,\n                   9841, 29524, 88573, 265720,\n                   797161, 2391484 };\n\nstatic\nvoid mainSimpleSort ( UInt32* ptr,\n                      UChar*  block,\n                      UInt16* quadrant,\n                      Int32   nblock,\n                      Int32   lo, \n                      Int32   hi, \n                      Int32   d,\n                      Int32*  budget )\n{\n   Int32 i, j, h, bigN, hp;\n   UInt32 v;\n\n   bigN = hi - lo + 1;\n   if (bigN < 2) return;\n\n   hp = 0;\n   while (incs[hp] < bigN) hp++;\n   hp--;\n\n   for (; hp >= 0; hp--) {\n      h = incs[hp];\n\n      i = lo + h;\n      while (True) {\n\n         /*-- copy 1 --*/\n         if (i > hi) break;\n         v = ptr[i];\n         j = i;\n         while ( mainGtU ( \n                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget \n                 ) ) {\n            ptr[j] = ptr[j-h];\n            j = j - h;\n            if (j <= (lo + h - 1)) break;\n         }\n         ptr[j] = v;\n         i++;\n\n         /*-- copy 2 --*/\n         if (i > hi) break;\n         v = ptr[i];\n         j = i;\n         while ( mainGtU ( \n                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget \n                 ) ) {\n            ptr[j] = ptr[j-h];\n            j = j - h;\n            if (j <= (lo + h - 1)) break;\n         }\n         ptr[j] = v;\n         i++;\n\n         /*-- copy 3 --*/\n         if (i > hi) break;\n         v = ptr[i];\n         j = i;\n         while ( mainGtU ( \n                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget \n                 ) ) {\n            ptr[j] = ptr[j-h];\n            j = j - h;\n            if (j <= (lo + h - 1)) break;\n         }\n         ptr[j] = v;\n         i++;\n\n         if (*budget < 0) return;\n      }\n   }\n}\n\n\n/*---------------------------------------------*/\n/*--\n   The following is an implementation of\n   an elegant 3-way quicksort for strings,\n   described in a paper \"Fast Algorithms for\n   Sorting and Searching Strings\", by Robert\n   Sedgewick and Jon L. Bentley.\n--*/\n\n#define mswap(zz1, zz2) \\\n   { Int32 zztmp = zz1; zz1 = zz2; zz2 = zztmp; }\n\n#define mvswap(zzp1, zzp2, zzn)       \\\n{                                     \\\n   Int32 yyp1 = (zzp1);               \\\n   Int32 yyp2 = (zzp2);               \\\n   Int32 yyn  = (zzn);                \\\n   while (yyn > 0) {                  \\\n      mswap(ptr[yyp1], ptr[yyp2]);    \\\n      yyp1++; yyp2++; yyn--;          \\\n   }                                  \\\n}\n\nstatic \n__inline__\nUChar mmed3 ( UChar a, UChar b, UChar c )\n{\n   UChar t;\n   if (a > b) { t = a; a = b; b = t; };\n   if (b > c) { \n      b = c;\n      if (a > b) b = a;\n   }\n   return b;\n}\n\n#define mmin(a,b) ((a) < (b)) ? (a) : (b)\n\n#define mpush(lz,hz,dz) { stackLo[sp] = lz; \\\n                          stackHi[sp] = hz; \\\n                          stackD [sp] = dz; \\\n                          sp++; }\n\n#define mpop(lz,hz,dz) { sp--;             \\\n                         lz = stackLo[sp]; \\\n                         hz = stackHi[sp]; \\\n                         dz = stackD [sp]; }\n\n\n#define mnextsize(az) (nextHi[az]-nextLo[az])\n\n#define mnextswap(az,bz)                                        \\\n   { Int32 tz;                                                  \\\n     tz = nextLo[az]; nextLo[az] = nextLo[bz]; nextLo[bz] = tz; \\\n     tz = nextHi[az]; nextHi[az] = nextHi[bz]; nextHi[bz] = tz; \\\n     tz = nextD [az]; nextD [az] = nextD [bz]; nextD [bz] = tz; }\n\n\n#define MAIN_QSORT_SMALL_THRESH 20\n#define MAIN_QSORT_DEPTH_THRESH (BZ_N_RADIX + BZ_N_QSORT)\n#define MAIN_QSORT_STACK_SIZE 100\n\nstatic\nvoid mainQSort3 ( UInt32* ptr,\n                  UChar*  block,\n                  UInt16* quadrant,\n                  Int32   nblock,\n                  Int32   loSt, \n                  Int32   hiSt, \n                  Int32   dSt,\n                  Int32*  budget )\n{\n   Int32 unLo, unHi, ltLo, gtHi, n, m, med;\n   Int32 sp, lo, hi, d;\n\n   Int32 stackLo[MAIN_QSORT_STACK_SIZE];\n   Int32 stackHi[MAIN_QSORT_STACK_SIZE];\n   Int32 stackD [MAIN_QSORT_STACK_SIZE];\n\n   Int32 nextLo[3];\n   Int32 nextHi[3];\n   Int32 nextD [3];\n\n   sp = 0;\n   mpush ( loSt, hiSt, dSt );\n\n   while (sp > 0) {\n\n      AssertH ( sp < MAIN_QSORT_STACK_SIZE - 2, 1001 );\n\n      mpop ( lo, hi, d );\n      if (hi - lo < MAIN_QSORT_SMALL_THRESH || \n          d > MAIN_QSORT_DEPTH_THRESH) {\n         mainSimpleSort ( ptr, block, quadrant, nblock, lo, hi, d, budget );\n         if (*budget < 0) return;\n         continue;\n      }\n\n      med = (Int32) \n            mmed3 ( block[ptr[ lo         ]+d],\n                    block[ptr[ hi         ]+d],\n                    block[ptr[ (lo+hi)>>1 ]+d] );\n\n      unLo = ltLo = lo;\n      unHi = gtHi = hi;\n\n      while (True) {\n         while (True) {\n            if (unLo > unHi) break;\n            n = ((Int32)block[ptr[unLo]+d]) - med;\n            if (n == 0) { \n               mswap(ptr[unLo], ptr[ltLo]); \n               ltLo++; unLo++; continue; \n            };\n            if (n >  0) break;\n            unLo++;\n         }\n         while (True) {\n            if (unLo > unHi) break;\n            n = ((Int32)block[ptr[unHi]+d]) - med;\n            if (n == 0) { \n               mswap(ptr[unHi], ptr[gtHi]); \n               gtHi--; unHi--; continue; \n            };\n            if (n <  0) break;\n            unHi--;\n         }\n         if (unLo > unHi) break;\n         mswap(ptr[unLo], ptr[unHi]); unLo++; unHi--;\n      }\n\n      AssertD ( unHi == unLo-1, \"mainQSort3(2)\" );\n\n      if (gtHi < ltLo) {\n         mpush(lo, hi, d+1 );\n         continue;\n      }\n\n      n = mmin(ltLo-lo, unLo-ltLo); mvswap(lo, unLo-n, n);\n      m = mmin(hi-gtHi, gtHi-unHi); mvswap(unLo, hi-m+1, m);\n\n      n = lo + unLo - ltLo - 1;\n      m = hi - (gtHi - unHi) + 1;\n\n      nextLo[0] = lo;  nextHi[0] = n;   nextD[0] = d;\n      nextLo[1] = m;   nextHi[1] = hi;  nextD[1] = d;\n      nextLo[2] = n+1; nextHi[2] = m-1; nextD[2] = d+1;\n\n      if (mnextsize(0) < mnextsize(1)) mnextswap(0,1);\n      if (mnextsize(1) < mnextsize(2)) mnextswap(1,2);\n      if (mnextsize(0) < mnextsize(1)) mnextswap(0,1);\n\n      AssertD (mnextsize(0) >= mnextsize(1), \"mainQSort3(8)\" );\n      AssertD (mnextsize(1) >= mnextsize(2), \"mainQSort3(9)\" );\n\n      mpush (nextLo[0], nextHi[0], nextD[0]);\n      mpush (nextLo[1], nextHi[1], nextD[1]);\n      mpush (nextLo[2], nextHi[2], nextD[2]);\n   }\n}\n\n#undef mswap\n#undef mvswap\n#undef mpush\n#undef mpop\n#undef mmin\n#undef mnextsize\n#undef mnextswap\n#undef MAIN_QSORT_SMALL_THRESH\n#undef MAIN_QSORT_DEPTH_THRESH\n#undef MAIN_QSORT_STACK_SIZE\n\n\n/*---------------------------------------------*/\n/* Pre:\n      nblock > N_OVERSHOOT\n      block32 exists for [0 .. nblock-1 +N_OVERSHOOT]\n      ((UChar*)block32) [0 .. nblock-1] holds block\n      ptr exists for [0 .. nblock-1]\n\n   Post:\n      ((UChar*)block32) [0 .. nblock-1] holds block\n      All other areas of block32 destroyed\n      ftab [0 .. 65536 ] destroyed\n      ptr [0 .. nblock-1] holds sorted order\n      if (*budget < 0), sorting was abandoned\n*/\n\n#define BIGFREQ(b) (ftab[((b)+1) << 8] - ftab[(b) << 8])\n#define SETMASK (1 << 21)\n#define CLEARMASK (~(SETMASK))\n\nstatic\nvoid mainSort ( UInt32* ptr, \n                UChar*  block,\n                UInt16* quadrant, \n                UInt32* ftab,\n                Int32   nblock,\n                Int32   verb,\n                Int32*  budget )\n{\n   Int32  i, j, k, ss, sb;\n   Int32  runningOrder[256];\n   Bool   bigDone[256];\n   Int32  copyStart[256];\n   Int32  copyEnd  [256];\n   UChar  c1;\n   Int32  numQSorted;\n   UInt16 s;\n   if (verb >= 4) VPrintf0 ( \"        main sort initialise ...\\n\" );\n\n   /*-- set up the 2-byte frequency table --*/\n   for (i = 65536; i >= 0; i--) ftab[i] = 0;\n\n   j = block[0] << 8;\n   i = nblock-1;\n   for (; i >= 3; i -= 4) {\n      quadrant[i] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i]) << 8);\n      ftab[j]++;\n      quadrant[i-1] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i-1]) << 8);\n      ftab[j]++;\n      quadrant[i-2] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i-2]) << 8);\n      ftab[j]++;\n      quadrant[i-3] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i-3]) << 8);\n      ftab[j]++;\n   }\n   for (; i >= 0; i--) {\n      quadrant[i] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i]) << 8);\n      ftab[j]++;\n   }\n\n   /*-- (emphasises close relationship of block & quadrant) --*/\n   for (i = 0; i < BZ_N_OVERSHOOT; i++) {\n      block   [nblock+i] = block[i];\n      quadrant[nblock+i] = 0;\n   }\n\n   if (verb >= 4) VPrintf0 ( \"        bucket sorting ...\\n\" );\n\n   /*-- Complete the initial radix sort --*/\n   for (i = 1; i <= 65536; i++) ftab[i] += ftab[i-1];\n\n   s = block[0] << 8;\n   i = nblock-1;\n   for (; i >= 3; i -= 4) {\n      s = (s >> 8) | (block[i] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i;\n      s = (s >> 8) | (block[i-1] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i-1;\n      s = (s >> 8) | (block[i-2] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i-2;\n      s = (s >> 8) | (block[i-3] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i-3;\n   }\n   for (; i >= 0; i--) {\n      s = (s >> 8) | (block[i] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i;\n   }\n\n   /*--\n      Now ftab contains the first loc of every small bucket.\n      Calculate the running order, from smallest to largest\n      big bucket.\n   --*/\n   for (i = 0; i <= 255; i++) {\n      bigDone     [i] = False;\n      runningOrder[i] = i;\n   }\n\n   {\n      Int32 vv;\n      Int32 h = 1;\n      do h = 3 * h + 1; while (h <= 256);\n      do {\n         h = h / 3;\n         for (i = h; i <= 255; i++) {\n            vv = runningOrder[i];\n            j = i;\n            while ( BIGFREQ(runningOrder[j-h]) > BIGFREQ(vv) ) {\n               runningOrder[j] = runningOrder[j-h];\n               j = j - h;\n               if (j <= (h - 1)) goto zero;\n            }\n            zero:\n            runningOrder[j] = vv;\n         }\n      } while (h != 1);\n   }\n\n   /*--\n      The main sorting loop.\n   --*/\n\n   numQSorted = 0;\n\n   for (i = 0; i <= 255; i++) {\n\n      /*--\n         Process big buckets, starting with the least full.\n         Basically this is a 3-step process in which we call\n         mainQSort3 to sort the small buckets [ss, j], but\n         also make a big effort to avoid the calls if we can.\n      --*/\n      ss = runningOrder[i];\n\n      /*--\n         Step 1:\n         Complete the big bucket [ss] by quicksorting\n         any unsorted small buckets [ss, j], for j != ss.  \n         Hopefully previous pointer-scanning phases have already\n         completed many of the small buckets [ss, j], so\n         we don't have to sort them at all.\n      --*/\n      for (j = 0; j <= 255; j++) {\n         if (j != ss) {\n            sb = (ss << 8) + j;\n            if ( ! (ftab[sb] & SETMASK) ) {\n               Int32 lo = ftab[sb]   & CLEARMASK;\n               Int32 hi = (ftab[sb+1] & CLEARMASK) - 1;\n               if (hi > lo) {\n                  if (verb >= 4)\n                     VPrintf4 ( \"        qsort [0x%x, 0x%x]   \"\n                                \"done %d   this %d\\n\",\n                                ss, j, numQSorted, hi - lo + 1 );\n                  mainQSort3 ( \n                     ptr, block, quadrant, nblock, \n                     lo, hi, BZ_N_RADIX, budget \n                  );   \n                  numQSorted += (hi - lo + 1);\n                  if (*budget < 0) return;\n               }\n            }\n            ftab[sb] |= SETMASK;\n         }\n      }\n\n      AssertH ( !bigDone[ss], 1006 );\n\n      /*--\n         Step 2:\n         Now scan this big bucket [ss] so as to synthesise the\n         sorted order for small buckets [t, ss] for all t,\n         including, magically, the bucket [ss,ss] too.\n         This will avoid doing Real Work in subsequent Step 1's.\n      --*/\n      {\n         for (j = 0; j <= 255; j++) {\n            copyStart[j] =  ftab[(j << 8) + ss]     & CLEARMASK;\n            copyEnd  [j] = (ftab[(j << 8) + ss + 1] & CLEARMASK) - 1;\n         }\n         for (j = ftab[ss << 8] & CLEARMASK; j < copyStart[ss]; j++) {\n            k = ptr[j]-1; if (k < 0) k += nblock;\n            c1 = block[k];\n            if (!bigDone[c1])\n               ptr[ copyStart[c1]++ ] = k;\n         }\n         for (j = (ftab[(ss+1) << 8] & CLEARMASK) - 1; j > copyEnd[ss]; j--) {\n            k = ptr[j]-1; if (k < 0) k += nblock;\n            c1 = block[k];\n            if (!bigDone[c1]) \n               ptr[ copyEnd[c1]-- ] = k;\n         }\n      }\n\n      AssertH ( (copyStart[ss]-1 == copyEnd[ss])\n                || \n                /* Extremely rare case missing in bzip2-1.0.0 and 1.0.1.\n                   Necessity for this case is demonstrated by compressing \n                   a sequence of approximately 48.5 million of character \n                   251; 1.0.0/1.0.1 will then die here. */\n                (copyStart[ss] == 0 && copyEnd[ss] == nblock-1),\n                1007 )\n\n      for (j = 0; j <= 255; j++) ftab[(j << 8) + ss] |= SETMASK;\n\n      /*--\n         Step 3:\n         The [ss] big bucket is now done.  Record this fact,\n         and update the quadrant descriptors.  Remember to\n         update quadrants in the overshoot area too, if\n         necessary.  The \"if (i < 255)\" test merely skips\n         this updating for the last bucket processed, since\n         updating for the last bucket is pointless.\n\n         The quadrant array provides a way to incrementally\n         cache sort orderings, as they appear, so as to \n         make subsequent comparisons in fullGtU() complete\n         faster.  For repetitive blocks this makes a big\n         difference (but not big enough to be able to avoid\n         the fallback sorting mechanism, exponential radix sort).\n\n         The precise meaning is: at all times:\n\n            for 0 <= i < nblock and 0 <= j <= nblock\n\n            if block[i] != block[j], \n\n               then the relative values of quadrant[i] and \n                    quadrant[j] are meaningless.\n\n               else {\n                  if quadrant[i] < quadrant[j]\n                     then the string starting at i lexicographically\n                     precedes the string starting at j\n\n                  else if quadrant[i] > quadrant[j]\n                     then the string starting at j lexicographically\n                     precedes the string starting at i\n\n                  else\n                     the relative ordering of the strings starting\n                     at i and j has not yet been determined.\n               }\n      --*/\n      bigDone[ss] = True;\n\n      if (i < 255) {\n         Int32 bbStart  = ftab[ss << 8] & CLEARMASK;\n         Int32 bbSize   = (ftab[(ss+1) << 8] & CLEARMASK) - bbStart;\n         Int32 shifts   = 0;\n\n         while ((bbSize >> shifts) > 65534) shifts++;\n\n         for (j = bbSize-1; j >= 0; j--) {\n            Int32 a2update     = ptr[bbStart + j];\n            UInt16 qVal        = (UInt16)(j >> shifts);\n            quadrant[a2update] = qVal;\n            if (a2update < BZ_N_OVERSHOOT)\n               quadrant[a2update + nblock] = qVal;\n         }\n         AssertH ( ((bbSize-1) >> shifts) <= 65535, 1002 );\n      }\n\n   }\n\n   if (verb >= 4)\n      VPrintf3 ( \"        %d pointers, %d sorted, %d scanned\\n\",\n                 nblock, numQSorted, nblock - numQSorted );\n}\n\n#undef BIGFREQ\n#undef SETMASK\n#undef CLEARMASK\n\n\n/*---------------------------------------------*/\n/* Pre:\n      nblock > 0\n      arr2 exists for [0 .. nblock-1 +N_OVERSHOOT]\n      ((UChar*)arr2)  [0 .. nblock-1] holds block\n      arr1 exists for [0 .. nblock-1]\n\n   Post:\n      ((UChar*)arr2) [0 .. nblock-1] holds block\n      All other areas of block destroyed\n      ftab [ 0 .. 65536 ] destroyed\n      arr1 [0 .. nblock-1] holds sorted order\n*/\nvoid BZ2_blockSort ( EState* s )\n{\n   UInt32* ptr    = s->ptr; \n   UChar*  block  = s->block;\n   UInt32* ftab   = s->ftab;\n   Int32   nblock = s->nblock;\n   Int32   verb   = s->verbosity;\n   Int32   wfact  = s->workFactor;\n   UInt16* quadrant;\n   Int32   budget;\n   Int32   budgetInit;\n   Int32   i;\n\n   if (nblock < 10000) {\n      fallbackSort ( s->arr1, s->arr2, ftab, nblock, verb );\n   } else {\n      /* Calculate the location for quadrant, remembering to get\n         the alignment right.  Assumes that &(block[0]) is at least\n         2-byte aligned -- this should be ok since block is really\n         the first section of arr2.\n      */\n      i = nblock+BZ_N_OVERSHOOT;\n      if (i & 1) i++;\n      quadrant = (UInt16*)(&(block[i]));\n\n      /* (wfact-1) / 3 puts the default-factor-30\n         transition point at very roughly the same place as \n         with v0.1 and v0.9.0.  \n         Not that it particularly matters any more, since the\n         resulting compressed stream is now the same regardless\n         of whether or not we use the main sort or fallback sort.\n      */\n      if (wfact < 1  ) wfact = 1;\n      if (wfact > 100) wfact = 100;\n      budgetInit = nblock * ((wfact-1) / 3);\n      budget = budgetInit;\n\n      mainSort ( ptr, block, quadrant, ftab, nblock, verb, &budget );\n      if (verb >= 3) \n         VPrintf3 ( \"      %d work, %d block, ratio %5.2f\\n\",\n                    budgetInit - budget,\n                    nblock, \n                    (float)(budgetInit - budget) /\n                    (float)(nblock==0 ? 1 : nblock) ); \n      if (budget < 0) {\n         if (verb >= 2) \n            VPrintf0 ( \"    too repetitive; using fallback\"\n                       \" sorting algorithm\\n\" );\n         fallbackSort ( s->arr1, s->arr2, ftab, nblock, verb );\n      }\n   }\n\n   s->origPtr = -1;\n   for (i = 0; i < s->nblock; i++)\n      if (ptr[i] == 0)\n         { s->origPtr = i; break; };\n\n   AssertH( s->origPtr != -1, 1003 );\n}\n\n\n/*-------------------------------------------------------------*/\n/*--- end                                       blocksort.c ---*/\n/*-------------------------------------------------------------*/\n"
    )
  endif()
  include("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2.cmake")

  set(LIBBZ2_SOURCES 3rdparty/bzip2/blocksort.c 3rdparty/bzip2/bzlib.c 3rdparty/bzip2/compress.c 3rdparty/bzip2/crctable.c 3rdparty/bzip2/decompress.c 3rdparty/bzip2/huffman.c 3rdparty/bzip2/randtable.c)
  add_library(bz2 ${LIBBZ2_SOURCES})
  set_target_properties(bz2 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bzip2" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bzip2")

  set(BZIP2_LIBRARY_DEBUG bz2 CACHE FILEPATH "")
  set(BZIP2_LIBRARY_RELEASE bz2 CACHE FILEPATH "")
  set(BZIP2_LIBRARY bz2 CACHE FILEPATH "")
  set(BZIP2_LIBRARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/bzip2" CACHE PATH "BZip2 library directory")
  set(BZIP2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/bzip2" CACHE PATH "")

  link_directories(BEFORE "${BZIP2_LIBRARY_DIR}")

endif(BUILD_BZIP2)
