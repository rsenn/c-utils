/*
 * Copyright (C) 2002 Uwe Ohse, uwe@ohse.de
 * This is free software, licensed under the terms of the GNU General
 * Public License Version 2, of which a copy is stored at:
 *    http://www.ohse.de/uwe/licenses/GPL-2
 * Later versions may or may not apply, see
 *    http://www.ohse.de/uwe/licenses/
 * for information after a newer version has been published.
 */
#ifndef FTPLIB_H
#define FTPLIB_H

/*
 *  0: ok.
 * -1: error
 */
int ftplib_write_cmd_s(buffer* io, const unsigned char* s1);
int ftplib_write_cmd_ss(buffer* io, const unsigned char* s1, const unsigned char* s2);
/*
 *  1: ok.
 *  0: EOF.
 * -1: error.
 */
int ftplib_read(buffer* io, stralloc* ret);
int ftplib_read_oneline(buffer* io, stralloc* ret);
/*
 *  1: ok.
 *  0: EOF.
 * -1: error.
 * -2: parse error.
 * -3: ftpserver tried to redirect us to another host or an illegal port.
 */
int ftplib_pasv(
    buffer* io_i, buffer* io_o, unsigned long timeout, stralloc* allowed_ips, stralloc* message, unsigned int retries);
/* -1: error, else number of \0 terminated string in "dir".
 * fd will not be closed.
 */
int ftp_read_list(int fd, stralloc* dir);

#endif
