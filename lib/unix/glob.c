#include "../windoze.h"

#if WINDOWS_NATIVE
#include "../glob.h"

#include "../byte.h"
#include "../str.h"

#include <errno.h>
#include <stdlib.h>
#if WINDOWS
#include <windows.h>
#endif

#define NUM_ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

/*
 * Helper functions
 */

static char const*
strrpbrk(char const* s, char const* charset) {
  char* part = NULL;
  char const* pch;

  for(pch = charset; *pch; ++pch) {
    char* p = strrchr(s, *pch);

    if(NULL != p) {
      if(NULL == part) {
        part = p;
      } else {
        if(part < p) {
          part = p;
        }
      }
    }
  }

  return part;
}

/*
 * API functions
 */

/* It gives you back the matched contents of your pattern, so for Win32, th
 * directories must be included
 */

int
glob(char const* pattern, int flags, int (*errfunc)(char const*, int), glob_t* pglob) {
  int result;
  char szRelative[1 + _MAX_PATH];
  char const* file_part;
  WIN32_FIND_DATAA find_data;
  HANDLE hFind;
  char* buffer;
  char szPattern2[1 + _MAX_PATH];
  char szPattern3[1 + _MAX_PATH];
  char const* effectivePattern = pattern;
  char const* leafMost;
  const int bMagic = (NULL != strpbrk(pattern, "?*"));
  int bNoMagic = 0;
  int bMagic0;
  size_t maxMatches = ~(size_t)(0);

  if(flags & GLOB_NOMAGIC) {
    bNoMagic = !bMagic;
  }

  if(flags & GLOB_LIMIT) {
    maxMatches = (size_t)pglob->gl_matchc;
  }

  if(flags & GLOB_TILDE) {
    if('~' == pattern[0] && ('\0' == pattern[1] || '/' == pattern[1] || '\\' == pattern[1])) {
      DWORD dw;

      (void)lstrcpyA(&szPattern2[0], "%HOMEDRIVE%%HOMEPATH%");

      dw = ExpandEnvironmentStringsA(&szPattern2[0], &szPattern3[0], NUM_ELEMENTS(szPattern3) - 1);

      if(0 != dw) {
        (void)lstrcpynA(&szPattern3[0] + dw - 1, &pattern[1], (int)(NUM_ELEMENTS(szPattern3) - dw));
        szPattern3[NUM_ELEMENTS(szPattern3) - 1] = '\0';

        effectivePattern = szPattern3;
      }
    }
  }

  file_part = strrpbrk(effectivePattern, "\\/");

  if(NULL != file_part) {
    leafMost = ++file_part;

    (void)lstrcpyA(szRelative, effectivePattern);
    szRelative[file_part - effectivePattern] = '\0';
  } else {
    szRelative[0] = '\0';
    leafMost = effectivePattern;
  }

  bMagic0 = (leafMost == strpbrk(leafMost, "?*"));

  hFind = FindFirstFileA(effectivePattern, &find_data);
  buffer = NULL;

  pglob->gl_pathc = 0;
  pglob->gl_pathv = NULL;

  if(0 == (flags & GLOB_DOOFFS)) {
    pglob->gl_offs = 0;
  }

  if(hFind == INVALID_HANDLE_VALUE) {
    if(NULL != errfunc) {
      (void)errfunc(effectivePattern, (int)GetLastError());
    }

    result = GLOB_NOMATCH;
  } else {
    int cbCurr = 0;
    size_t cbAlloc = 0;
    size_t cMatches = 0;

    result = 0;

    do {
      int cch;
      size_t new_cbAlloc;

      if(bMagic0 && 0 == (flags & GLOB_PERIOD)) {
        if('.' == find_data.cFileName[0]) {
          continue;
        }
      }

      if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
#ifdef GLOB_ONLYFILE
        if(flags & GLOB_ONLYFILE) {
          continue;
        }
#endif /* GLOB_ONLYFILE */

        if(bMagic0 && GLOB_NODOTSDIRS == (flags & GLOB_NODOTSDIRS)) {
          /* Pattern must begin with '.' to match either dots directory */
          if(0 == lstrcmpA(".", find_data.cFileName) || 0 == lstrcmpA("..", find_data.cFileName)) {
            continue;
          }
        }

        if(flags & GLOB_MARK) {
#if 0
                    if(find_data.cFileName[0] >= 'A' && find_data.cFileName[0] <= 'M')
#endif /* 0 */
          (void)lstrcatA(find_data.cFileName, "/");
        }
      } else {
        if(flags & GLOB_ONLYDIR) {
          /* Skip all further actions, and get the next entry */
#if 0
                    if(find_data.cFileName[0] >= 'A' && find_data.cFileName[0] <= 'M')
#endif /* 0 */
          continue;
        }
      }

      cch = lstrlenA(find_data.cFileName);
      if(NULL != file_part) {
        cch += file_part - effectivePattern;
      }

      new_cbAlloc = (size_t)cbCurr + cch + 1;
      if(new_cbAlloc > cbAlloc) {
        char* new_buffer;

        new_cbAlloc *= 2;

        new_cbAlloc = (new_cbAlloc + 31) & ~(31);

        new_buffer = (char*)realloc(buffer, new_cbAlloc);

        if(new_buffer == NULL) {
          result = GLOB_NOSPACE;
          free(buffer);
          buffer = NULL;
          break;
        }

        buffer = new_buffer;
        cbAlloc = new_cbAlloc;
      }

      (void)lstrcpynA(buffer + cbCurr, szRelative, 1 + (file_part - effectivePattern));
      (void)lstrcatA(buffer + cbCurr, find_data.cFileName);
      cbCurr += cch + 1;

      ++cMatches;
    } while(FindNextFileA(hFind, &find_data) && cMatches != maxMatches);

    (void)FindClose(hFind);

    if(result == 0) {
      /* Now expand the buffer, to fit in all the pointers. */
      size_t cbPointers = (1 + cMatches + pglob->gl_offs) * sizeof(char*);
      char* new_buffer = (char*)realloc(buffer, cbAlloc + cbPointers);

      if(new_buffer == NULL) {
        result = GLOB_NOSPACE;
        free(buffer);
      } else {
        char** pp;
        char** begin;
        char** end;
        char* next_str;

        buffer = new_buffer;

        (void)byte_copyr(new_buffer + cbPointers, cbAlloc, new_buffer);

        /* Handle the offsets. */
        begin = (char**)new_buffer;
        end = begin + pglob->gl_offs;

        for(; begin != end; ++begin) {
          *begin = NULL;
        }

        /* Sort, or no sort. */
        pp = (char**)new_buffer + pglob->gl_offs;
        begin = pp;
        end = begin + cMatches;

        if(flags & GLOB_NOSORT) {
          /* The way we need in order to test the removal of dots in the
           * findfile_sequence. */
          *end = NULL;
          for(begin = pp, next_str = buffer + cbPointers; begin != end; --end) {
            *(end - 1) = next_str;

            /* Find the next s. */
            next_str += 1 + lstrlenA(next_str);
          }
        } else {
          /* The normal way. */
          for(begin = pp, next_str = buffer + cbPointers; begin != end; ++begin) {
            *begin = next_str;

            /* Find the next s. */
            next_str += 1 + lstrlenA(next_str);
          }
          *begin = NULL;
        }

        /* Return results to caller. */
        pglob->gl_pathc = (int)cMatches;
        pglob->gl_matchc = (int)cMatches;
        pglob->gl_flags = 0;
        if(bMagic) {
          pglob->gl_flags |= GLOB_MAGCHAR;
        }
        pglob->gl_pathv = (char**)new_buffer;
      }
    }

    if(0 == cMatches) {
      result = GLOB_NOMATCH;
    }
  }

  if(GLOB_NOMATCH == result) {
    if((flags & GLOB_TILDE_CHECK) && effectivePattern == szPattern3) {
      result = GLOB_NOMATCH;
    } else if(bNoMagic || (flags & GLOB_NOCHECK)) {
      size_t cbNeeded = ((2 + pglob->gl_offs) * sizeof(char*)) + (1 + strlen(effectivePattern));
      char** pp = (char**)realloc(buffer, cbNeeded);

      if(NULL == pp) {
        result = GLOB_NOSPACE;
        free(buffer);
      } else {
        /* Handle the offsets. */
        char** begin = pp;
        char** end = pp + pglob->gl_offs;

        for(; begin != end; ++begin) {
          *begin = NULL;
        }

        /* Synthesis the pattern result. */

        pp[pglob->gl_offs] = (char*)(pp + 2 + pglob->gl_offs);
        pp[pglob->gl_offs + 1] = NULL;

        str_copy(pp[pglob->gl_offs], effectivePattern);

        /* Return results to caller. */
        pglob->gl_pathc = 1;
        pglob->gl_matchc = 1;
        pglob->gl_flags = 0;
        if(bMagic) {
          pglob->gl_flags |= GLOB_MAGCHAR;
        }
        pglob->gl_pathv = pp;

        result = 0;
      }
    }
  } else if(0 == result) {
    if((size_t)pglob->gl_matchc == maxMatches) {
      result = GLOB_NOSPACE;
    }
  }

  return result;
}

void
globfree(glob_t* pglob) {
  if(pglob != NULL) {
    free(pglob->gl_pathv);
    pglob->gl_pathc = 0;
    pglob->gl_pathv = NULL;
  }
}

#elif !defined(HAVE_GLOB)
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <fnmatch.h>
#include <dirent.h>
#include <pwd.h>

#include "../glob.h"

/* If i18n, should be using strcoll */
static int
cmp_func(const void* a, const void* b) {
  const char* const s1 = *(const char* const* const)a;
  const char* const s2 = *(const char* const* const)b;
  if(s1 == NULL)
    return 1;
  if(s2 == NULL)
    return -1;
  return strcoll(s1, s2);
}

/* Like `glob', but PATTERN is a final pathname component,
   and matches are searched for in DIRECTORY.
   The GLOB_NOSORT bit in FLAGS is ignored.  No sorting is ever done.
   The GLOB_APPEND flag is assumed to be set (always appends).
   Prepends DIRECTORY in constructed PGLOB. */
static void
close_dir_keep_errno(DIR* dp) {
  int save = errno;
  if(dp)
    closedir(dp);
  errno = save;
}

static int
add_entry(const char* name, glob_t* pglob, int* nfound) {
  pglob->gl_pathv = (char**)realloc(pglob->gl_pathv, (pglob->gl_pathc + pglob->gl_offs + 2) * sizeof(char*));
  if(pglob->gl_pathv == NULL)
    return 1;
  pglob->gl_pathv[pglob->gl_offs + pglob->gl_pathc] = strdup(name);
  pglob->gl_pathv[pglob->gl_offs + pglob->gl_pathc + 1] = NULL;
  pglob->gl_pathc++;
  (*nfound)++;
  return 0;
}

static void
build_fullname(char* fullname, const char* directory, const char* filename) {
  char* dest = fullname;
  if(directory[0] == '/' && !directory[1]) {
    *dest = '/';
    ++dest;
  } else if(directory[0] != '.' || directory[1]) {
    strcpy(dest, directory);
    dest = strchr(dest, 0);
    *dest = '/';
    ++dest;
  }
  strcpy(dest, filename);
}

static int
glob_in_dir(
    const char* pattern, const char* directory, int flags, int errfunc(const char* epath, int eerrno), glob_t* pglob) {
  DIR* dp = opendir(directory);
  int nfound = 0;

  int i;
  char* ptr;

  if(!dp) {
    if(errno != ENOTDIR && ((errfunc != NULL && (*errfunc)(directory, errno)) || (flags & GLOB_ERR)))
      return GLOB_ABORTED;
  } else {
    int fnm_flags = ((!(flags & GLOB_PERIOD) ? FNM_PERIOD : 0) | ((flags & GLOB_NOESCAPE) ? FNM_NOESCAPE : 0));
    struct dirent* ep;
    while((ep = readdir(dp))) {
      i = strlen(directory) + strlen(ep->d_name) + 2;
      ptr = (char*)alloca(i);
      build_fullname(ptr, directory, ep->d_name);
      if(flags & GLOB_ONLYDIR) {
        struct stat statr;
        if(stat(ptr, &statr) || !S_ISDIR(statr.st_mode))
          continue;
      }
      if(fnmatch(pattern, ep->d_name, fnm_flags) == 0)
        if(add_entry(ptr, pglob, &nfound))
          goto memory_error;
    }
  }

  close_dir_keep_errno(dp);

  if(nfound != 0)
    pglob->gl_flags = flags;
  else if(flags & GLOB_NOCHECK) {
    /* nfound == 0 */
    i = strlen(directory) + strlen(pattern) + 2;
    ptr = (char*)alloca(i);
    build_fullname(ptr, directory, pattern);
    if(add_entry(ptr, pglob, &nfound))
      goto memory_error;
  }

  return (nfound == 0) ? GLOB_NOMATCH : 0;

memory_error:
  /* We're in trouble since we can't free the already allocated memory. [allocated from strdup(filame)]
   * Well, after all, when malloc returns NULL we're already in a bad mood, and no doubt the
   * program will manage to segfault by itself very soon :-). */
  close_dir_keep_errno(dp);
  return GLOB_NOSPACE;
}

#define __GLOB_FLAGS \
  (GLOB_ABEND | GLOB_ABORTED | GLOB_APPEND | GLOB_BRACE | GLOB_DOOFFS | GLOB_ERR | GLOB_LIMIT | GLOB_MAGCHAR | \
   GLOB_MARK | GLOB_NOCHECK | GLOB_NODOTSDIRS | GLOB_NOESCAPE | GLOB_NOMAGIC | GLOB_NOMATCH | GLOB_NOSORT | \
   GLOB_NOSPACE | GLOB_NOSYS | GLOB_ONLYDIR | GLOB_ONLYFILE | GLOB_PERIOD | GLOB_TILDE | GLOB_TILDE_CHECK)

int
glob(const char* pattern, int flags, int errfunc(const char* epath, int eerrno), glob_t* pglob) {
  char* pattern_;
  char* filename;
  char* dirname;
  size_t oldcount;
  struct stat statr;

  size_t i; /* tmp variables are declared here to save a bit of object space */
  int j, k; /* */
  char *ptr, *ptr2;

  if(pattern == NULL || pglob == NULL || (flags & ~__GLOB_FLAGS) != 0) {
    errno = EINVAL;
    return -1;
  }

  if(!(flags & GLOB_DOOFFS))
    pglob->gl_offs = 0;

  /* Duplicate pattern so I can make modif to it later (to handle
         TILDE stuff replacing old contents, and to null-terminate the
         directory) */
  pattern_ = alloca(strlen(pattern) + 1);
  strcpy(pattern_, pattern);

  /* Check for TILDE stuff */
  if((flags & (GLOB_TILDE | GLOB_TILDE_CHECK)) && pattern_[0] == '~') {
    char* home_dir = NULL;
    if(pattern_[1] == '\0' || pattern_[1] == '/') {
      /* She's asking for ~, her homedir */
      home_dir = getenv("HOME");
    } else {
      /* She's asking for another one's homedir */
      struct passwd* p;
      ptr2 = alloca(strlen(pattern_) + 1);
      strcpy(ptr2, pattern_ + 1);
      ptr = strchr(ptr2, '/');
      if(ptr != NULL)
        *ptr = '\0';
      if(((p = getpwnam(ptr2)) != NULL))
        home_dir = p->pw_dir;
    }
    if(home_dir != NULL) {
      i = strlen(home_dir) + strlen(pattern_); /* pessimistic (the ~ case) */
      ptr = alloca(i);
      strncpy(ptr, home_dir, i);
      ptr2 = pattern_ + 1;
      while(*ptr2 != '/' && *ptr2 != '\0')
        ptr2++;
      strncat(ptr, ptr2, i);
      pattern_ = ptr;
    } else if(flags & GLOB_TILDE_CHECK)
      return GLOB_NOMATCH;
  }

  /* Find the filename */
  filename = strrchr(pattern_, '/');

  if(filename == NULL) {
    /* We have no '/' in the pattern */
    filename = pattern_;
    dirname = (char*)".";
  } else if(filename == pattern_) {
    /* "/pattern".  */
    dirname = (char*)"/";
    filename++;
  } else {
    dirname = pattern_;
    filename++;
    /* allow dirname to be null terminated */
    *(filename - 1) = '\0';

    if(filename[0] == '\0' && strcmp(pattern_, "/")) {
      /* "pattern/".  Expand "pattern", appending slashes.  */
      j = glob(dirname, flags | GLOB_MARK, errfunc, pglob);
      if(j == 0)
        pglob->gl_flags = ((pglob->gl_flags & ~GLOB_MARK) | (flags & GLOB_MARK));
      return j;
    }
  }

  /* Reserve memory for pglob */
  if(!(flags & GLOB_APPEND)) {
    pglob->gl_pathc = 0;
    if(!(flags & GLOB_DOOFFS))
      pglob->gl_pathv = NULL;
    else {
      pglob->gl_pathv = (char**)malloc((pglob->gl_offs + 1) * sizeof(char*));
      if(pglob->gl_pathv == NULL)
        return GLOB_NOSPACE;
      for(i = 0; i <= pglob->gl_offs; i++)
        pglob->gl_pathv[i] = NULL;
    }
  }

  oldcount = pglob->gl_pathc + pglob->gl_offs;

  /* Begin real work */
  if(!strcmp(dirname, "/") || !strcmp(dirname, ".") ||
     (!strchr(dirname, '*') && !strchr(dirname, '?') && !strchr(dirname, '['))) {
    /* Approx of a terminal state, glob directly in dir. */
    j = glob_in_dir(filename, dirname, flags, errfunc, pglob);
    if(j != 0)
      return j;
  } else {
    /* We are not in a terminal state, so we have to glob for
       the directory, and then glob for the pattern in each
       directory found. */
    glob_t dirs;

    j = glob(dirname,
             ((flags & (GLOB_ERR | GLOB_NOCHECK | GLOB_NOESCAPE | GLOB_ALTDIRFUNC)) | GLOB_NOSORT | GLOB_ONLYDIR),
             errfunc,
             &dirs);
    if(j != 0)
      return j;

    /* We have successfully globbed the directory name.
       For each name we found, call glob_in_dir on it and FILENAME,
       appending the results to PGLOB.  */
    for(i = 0; i < dirs.gl_pathc; i++) {
      j = glob_in_dir(filename, dirs.gl_pathv[i], ((flags | GLOB_APPEND) & ~GLOB_NOCHECK), errfunc, pglob);
      if(j == GLOB_NOMATCH)
        /* No matches in this directory.  Try the next.  */
        continue;
      if(j != 0) {
        globfree(&dirs);
        globfree(pglob);
        return j;
      }
    }

    /* We have ignored the GLOB_NOCHECK flag in the `glob_in_dir' calls.
       But if we have not found any matching entry and the GLOB_NOCHECK
       flag was set we must return the list consisting of the disrectory
       names followed by the filename.  */
    if(pglob->gl_pathc + pglob->gl_offs == oldcount) {
      /* No matches.  */
      if(flags & GLOB_NOCHECK) {
        for(i = 0; i < dirs.gl_pathc; i++) {
          if(stat(dirs.gl_pathv[i], &statr) || !S_ISDIR(statr.st_mode))
            continue;

          /* stat is okay, we will add the entry, but before let's resize the pathv */
          j = pglob->gl_pathc + pglob->gl_offs;
          pglob->gl_pathv = (char**)realloc(pglob->gl_pathv, (j + 2) * sizeof(char*));
          if(pglob->gl_pathv == NULL) {
            globfree(&dirs);
            return GLOB_NOSPACE;
          }

          /* okay now we add the new entry */
          k = strlen(dirs.gl_pathv[i]) + strlen(filename) + 2;
          if((pglob->gl_pathv[j] = malloc(k)) == NULL) {
            globfree(&dirs);
            globfree(pglob);
            return GLOB_NOSPACE;
          }
          build_fullname(pglob->gl_pathv[j], dirs.gl_pathv[i], filename);
          pglob->gl_pathc++;
          pglob->gl_pathv[j + 1] = NULL;
        }
      } else {
        globfree(&dirs);
        return GLOB_NOMATCH;
      }
    }

    globfree(&dirs);
  }

  if(flags & GLOB_MARK) {
    for(i = oldcount; i < pglob->gl_pathc + pglob->gl_offs; i++)
      if(!stat(pglob->gl_pathv[i], &statr) && S_ISDIR(statr.st_mode)) {
        size_t len = strlen(pglob->gl_pathv[i]) + 2;
        ptr = realloc(pglob->gl_pathv[i], len);
        if(ptr == NULL) {
          globfree(pglob);
          return GLOB_NOSPACE;
        }
        strcpy(&ptr[len - 2], "/");
        pglob->gl_pathv[i] = ptr;
      }
  }

  if(!(flags & GLOB_NOSORT)) {
    qsort(&pglob->gl_pathv[oldcount], pglob->gl_pathc + pglob->gl_offs - oldcount, sizeof(char*), cmp_func);
  }

  return 0;
}

/* Free storage allocated in PGLOB by a previous `glob' call.  */
void
globfree(glob_t* pglob) {
  if(pglob->gl_pathv != NULL) {
    size_t i;
    for(i = 0; i < pglob->gl_pathc; i++)
      if(pglob->gl_pathv[pglob->gl_offs + i] != NULL)
        free((void*)pglob->gl_pathv[pglob->gl_offs + i]);
    free((void*)pglob->gl_pathv);
  }
}
#endif
