#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "buffer.h"
#include "stralloc.h"
static char buffer_1_out[BUFFER_OUTSIZE];
static buffer buffer_1 = BUFFER_INIT(write, 1, buffer_1_out, BUFFER_OUTSIZE);

int list_dir(stralloc *dir)
{
				DIR *d;
        struct dirent *de;
				char *idx;

				stralloc_zero(dir);
			  d	= opendir(dir->s);
				idx = dir->s + dir->len;
				stralloc_cats(dir, "/");

				while((de = readdir(d)))
				{
							idx[1] = '\0';

							stralloc_readyplus(dir, strlen(de->d_name)+1);
							strcpy(&idx[1], de->d_name);

								buffer_put(&buffer_1, dir->s,dir->len);
								buffer_put(&buffer_1, "\n", 1);
				buffer_flush(&buffer_1);
				}
				closedir(d);
}

int main(int argc, char *argv[])
{
				stralloc dir={0,0,0};
				stralloc_copys(&dir,argv[1]);
				list_dir(&dir);
				return 0;
}
