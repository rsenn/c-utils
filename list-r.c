#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "buffer.h"
#include "stralloc.h"
static char buffer_1_out[BUFFER_OUTSIZE];
static buffer buffer_1 = BUFFER_INIT(write, 1, buffer_1_out, BUFFER_OUTSIZE);
int list_dir_internal(stralloc *dir,  char type);

int list_dir(stralloc *dir)
{
        list_dir_internal(dir, DT_DIR);
        list_dir_internal(dir, -DT_DIR);
}

int list_dir_internal(stralloc *dir,  char type)
{
        DIR *d;
        struct dirent *de;
        char *idx;
        unsigned long l;

        stralloc_nul(dir);
        d  = opendir(dir->s);
        stralloc_cats(dir, "/");
        l = dir->len;
        while((de = readdir(d)))
        {
          dir->len = l;

          if(strcmp(de->d_name, ".") == 0) continue;
          if(strcmp(de->d_name, "..") == 0) continue;

          if(type > 0 && !(de->d_type & type)) continue;
          if(type < 0 && (de->d_type & (-type))) continue;

//        idx = dir->s + l;  *idx = '\0';

         stralloc_readyplus(dir, strlen(de->d_name)+1);
         strcpy(dir->s + dir->len, de->d_name);
         dir->len+=strlen(de->d_name);
        //      stralloc_cats(dir, de->d_name);
        //

        if(de->d_type & DT_DIR)
        {
                stralloc_cats(dir, "/");
        }
     

                buffer_put(&buffer_1, dir->s,dir->len);
                buffer_put(&buffer_1, "\n", 1);
             buffer_flush(&buffer_1);
      
          if(de->d_type & DT_DIR)
          {
                dir->len--;
                  list_dir_internal(dir,0);
          }
        }
        closedir(d);
}

int main(int argc, char *argv[])
{
        stralloc dir={0,0,0};
        stralloc_copys(&dir,argv[1]);
        list_dir_internal(&dir,0);
        return 0;
}
