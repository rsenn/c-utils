/*
 * Copyright (C) 2002 Uwe Ohse,
 * uwe@ohse.de This is free software,
 * licensed under the terms of the GNU
 * General Public License Version 2, of
 * which a copy is stored at:
 *    http://www.ohse.de/uwe/licenses/GPL-2
 * Later versions may or may not apply,
 * see http://www.ohse.de/uwe/licenses/
 * for information after a newer version
 * has been published.
 */
#include "lib/buffer.h"
#include "lib/stralloc.h"
//#include "attributes.h"

#define COMMON_RELATED_INFO                                                                                                                                                                            \
  "\n"                                                                                                                                                                                                 \
  "The ftpcopy package has a mailing "                                                                                                                                                                 \
  "list. Send an empty mail to\n"                                                                                                                                                                      \
  "<ftpcopy-subscribe@lists.ohse.de> "                                                                                                                                                                 \
  "to subscribe to it.\n\n"                                                                                                                                                                            \
  "The ftpcopy homepage is at\n"                                                                                                                                                                       \
  "  "                                                                                                                                                                                                 \
  "http://www.ohse.de/uwe/"                                                                                                                                                                            \
  "ftpcopy.html"
#define COMMON_BUGREPORT_INFO                                                                                                                                                                          \
  "Please report bugs to "                                                                                                                                                                             \
  "<ftpcopy@lists.ohse.de>"

#define COMMON_OPT_user                                                                                                                                                                                \
  {                                                                                                                                                                                                    \
    'u', "user", uogo_string, 0, &o_user, 0,                                                                                                                                                           \
        "Use NAME to login on the "                                                                                                                                                                    \
        "ftp server.",                                                                                                                                                                                 \
        "The default is `anonymous'. "                                                                                                                                                                 \
        "Use an empty name to force "                                                                                                                                                                  \
        "the program to not "                                                                                                                                                                          \
        "log in.",                                                                                                                                                                                     \
        "NAME"                                                                                                                                                                                         \
  }
#define COMMON_OPT_account                                                                                                                                                                             \
  {                                                                                                                                                                                                    \
    0, "account", uogo_string, 0, &o_acct, 0,                                                                                                                                                          \
        "Send ACCOUNT as account "                                                                                                                                                                     \
        "name during login phase.",                                                                                                                                                                    \
        "Note: this is _not_ the "                                                                                                                                                                     \
        "user name, but the name of "                                                                                                                                                                  \
        "what could be called "                                                                                                                                                                        \
        "a subaccount implemented by "                                                                                                                                                                 \
        "a few servers. If you don't "                                                                                                                                                                 \
        "understand what "                                                                                                                                                                             \
        "it means you have a good "                                                                                                                                                                    \
        "chance to never need this "                                                                                                                                                                   \
        "option anyway. If you "                                                                                                                                                                       \
        "think you need it please "                                                                                                                                                                    \
        "try the --user option "                                                                                                                                                                       \
        "first.",                                                                                                                                                                                      \
        "ACCOUNT"                                                                                                                                                                                      \
  }
#define COMMON_OPT_pass                                                                                                                                                                                \
  {                                                                                                                                                                                                    \
    'p', "pass", uogo_string, 0, &o_pass, 0,                                                                                                                                                           \
        "Use PASS as password to "                                                                                                                                                                     \
        "login on the ftp server.",                                                                                                                                                                    \
        "The default is "                                                                                                                                                                              \
        "`anonymous@invalid.example'."                                                                                                                                                                 \
        " If an empty password is "                                                                                                                                                                    \
        "given the programm will not "                                                                                                                                                                 \
        "send a password to the "                                                                                                                                                                      \
        "server.",                                                                                                                                                                                     \
        "PASSWORD"                                                                                                                                                                                     \
  }
#define COMMON_OPT_tries                                                                                                                                                                               \
  {                                                                                                                                                                                                    \
    '\0', "tries", uogo_ulong, 0, &o_tries, 0,                                                                                                                                                         \
        "Number of tries to connect "                                                                                                                                                                  \
        "and log in.",                                                                                                                                                                                 \
        "The default is 1, meaning "                                                                                                                                                                   \
        "that the program "                                                                                                                                                                            \
        "will give up after the "                                                                                                                                                                      \
        "first error.\n"                                                                                                                                                                               \
        "This option was added in "                                                                                                                                                                    \
        "version 0.3.0.",                                                                                                                                                                              \
        0                                                                                                                                                                                              \
  }
#define COMMON_OPT_data_connect_retries                                                                                                                                                                \
  {                                                                                                                                                                                                    \
    '\0', "data-connect-retries", uogo_ulong, 0, &o_data_connect_retries, 0,                                                                                                                           \
        "Number of tries to connect "                                                                                                                                                                  \
        "to data port.",                                                                                                                                                                               \
        "The program will try to "                                                                                                                                                                     \
        "reach the data port (for "                                                                                                                                                                    \
        "retrieval of listings or "                                                                                                                                                                    \
        "data) that many times and "                                                                                                                                                                   \
        "will give up after that "                                                                                                                                                                     \
        "many errors in a row. "                                                                                                                                                                       \
        "The default is 5, meaning "                                                                                                                                                                   \
        "that the program "                                                                                                                                                                            \
        "will give up after the "                                                                                                                                                                      \
        "fifth error.\n"                                                                                                                                                                               \
        "This option was added in "                                                                                                                                                                    \
        "version 0.6.6. The old "                                                                                                                                                                      \
        "behaviour was to give "                                                                                                                                                                       \
        "up after the first error.",                                                                                                                                                                   \
        0                                                                                                                                                                                              \
  }
#define COMMON_OPT_sleep                                                                                                                                                                               \
  {                                                                                                                                                                                                    \
    '\0', "login-sleep", uogo_ulong, 0, &o_login_sleep, 0,                                                                                                                                             \
        "Seconds to sleep after a "                                                                                                                                                                    \
        "failed login.",                                                                                                                                                                               \
        "More precisely: the program "                                                                                                                                                                 \
        "will fall to sleep for this "                                                                                                                                                                 \
        "many seconds "                                                                                                                                                                                \
        "after a "                                                                                                                                                                                     \
        "try to connect or login has "                                                                                                                                                                 \
        "failed. The default is 5. A "                                                                                                                                                                 \
        "0 is treated as 1, "                                                                                                                                                                          \
        "and abuse, especially "                                                                                                                                                                       \
        "together with --tries, is "                                                                                                                                                                   \
        "likely to annory the "                                                                                                                                                                        \
        "servers adminstrators.\n"                                                                                                                                                                     \
        "This option was added in "                                                                                                                                                                    \
        "version 0.4.5.",                                                                                                                                                                              \
        0                                                                                                                                                                                              \
  }
#define COMMON_OPT_v4                                                                                                                                                                                  \
  {                                                                                                                                                                                                    \
    '4', "v4", uogo_flag, UOGO_NOARG, &o_v4_only, 1,                                                                                                                                                   \
        "Only use IPv4, even if v6 "                                                                                                                                                                   \
        "is available.",                                                                                                                                                                               \
        "This option effectively "                                                                                                                                                                     \
        "disallows the use of IPv6, "                                                                                                                                                                  \
        "except for DNS queries.\n"                                                                                                                                                                    \
        "It was added in version "                                                                                                                                                                     \
        "0.6.0.",                                                                                                                                                                                      \
        0                                                                                                                                                                                              \
  }
#define COMMON_OPT_v6                                                                                                                                                                                  \
  {                                                                                                                                                                                                    \
    '6', "v6", uogo_flag, UOGO_NOARG, &o_v6_only, 1,                                                                                                                                                   \
        "Only use IPv6, even if v4 "                                                                                                                                                                   \
        "is available.",                                                                                                                                                                               \
        "This option effectively "                                                                                                                                                                     \
        "disallows the use of IPv4, "                                                                                                                                                                  \
        "except for DNS queries.\n"                                                                                                                                                                    \
        "It was added in version "                                                                                                                                                                     \
        "0.6.0.",                                                                                                                                                                                      \
        0                                                                                                                                                                                              \
  }
#define COMMON_OPT_timeout                                                                                                                                                                             \
  {                                                                                                                                                                                                    \
    'T', "timeout", uogo_ulong, 0, &o_timeout, 0,                                                                                                                                                      \
        "Timeout to use for network "                                                                                                                                                                  \
        "read/write.",                                                                                                                                                                                 \
        "The default is 30 seconds "                                                                                                                                                                   \
        "and is usually sufficient.\n"                                                                                                                                                                 \
        "This option was added in "                                                                                                                                                                    \
        "version 0.3.8.",                                                                                                                                                                              \
        "SECONDS"                                                                                                                                                                                      \
  }
#define COMMON_OPT_ascii_listings                                                                                                                                                                      \
  {                                                                                                                                                                                                    \
    0, "ascii-listings", uogo_flag, UOGO_NOARG, &o_ascii_listings, 1,                                                                                                                                  \
        "Do directory listings in "                                                                                                                                                                    \
        "ASCII mode.",                                                                                                                                                                                 \
        "Use this option if the FTP "                                                                                                                                                                  \
        "server is unable to "                                                                                                                                                                         \
        "correctly list directories "                                                                                                                                                                  \
        "in binary mode, for "                                                                                                                                                                         \
        "example, if you see a "                                                                                                                                                                       \
        "message like this (usually "                                                                                                                                                                  \
        "on "                                                                                                                                                                                          \
        "one line):\n"                                                                                                                                                                                 \
        "`fatal: received unwanted "                                                                                                                                                                   \
        "answer to LIST: 426 Data "                                                                                                                                                                    \
        "connection: Illegal seek.'\n"                                                                                                                                                                 \
        "This option was added in "                                                                                                                                                                    \
        "version 0.5.2.",                                                                                                                                                                              \
        0                                                                                                                                                                                              \
  }
#define COMMON_OPT_list_options                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    'L', "list-options", uogo_string, 0, &o_list_options, 1, "Add OPTS to LIST command.",                                                                                                              \
        "This allows to pass "                                                                                                                                                                         \
        "arbitrary options to the "                                                                                                                                                                    \
        "FTP servers LIST command. "                                                                                                                                                                   \
        "Note that ftpcopy does "                                                                                                                                                                      \
        "not cope well with "                                                                                                                                                                          \
        "recursive directory "                                                                                                                                                                         \
        "listings.\n"                                                                                                                                                                                  \
        "This option was added in "                                                                                                                                                                    \
        "version 0.3.0.",                                                                                                                                                                              \
        "OPTS"                                                                                                                                                                                         \
  }
#define COMMON_OPT_eat_leading_spaces                                                                                                                                                                  \
  {                                                                                                                                                                                                    \
    '\0', "eat-leading-spaces", uogo_flag, 1, &o_eat_leading_spaces, 1,                                                                                                                                \
        "Delete leading spaces from "                                                                                                                                                                  \
        "file names.",                                                                                                                                                                                 \
        "This works around some "                                                                                                                                                                      \
        "cases which cannot be "                                                                                                                                                                       \
        "automatically solved.\n"                                                                                                                                                                      \
        "No leading spaces will be "                                                                                                                                                                   \
        "deleted on EPLF and MLSx "                                                                                                                                                                    \
        "listings.\n"                                                                                                                                                                                  \
        "This option was added in "                                                                                                                                                                    \
        "version 0.6.6.",                                                                                                                                                                              \
        0                                                                                                                                                                                              \
  }
#define COMMON_OPT_pasv_ip                                                                                                                                                                             \
  {                                                                                                                                                                                                    \
    0, "allow-pasv-ip", callback_ip, 0, callback_ip, 0,                                                                                                                                                \
        "Allow data connections to "                                                                                                                                                                   \
        "the address IP4.",                                                                                                                                                                            \
        "Normally ftpls only accepts "                                                                                                                                                                 \
        "data connections to the IP "                                                                                                                                                                  \
        "addresses it "                                                                                                                                                                                \
        "received as an answer to "                                                                                                                                                                    \
        "the DNS request, or the IP "                                                                                                                                                                  \
        "address in the URL. "                                                                                                                                                                         \
        "Sometimes this is not "                                                                                                                                                                       \
        "enough, especially when NAT "                                                                                                                                                                 \
        "or masquerading are "                                                                                                                                                                         \
        "active. ftpcopy then prints "                                                                                                                                                                 \
        "an error message `illegal "                                                                                                                                                                   \
        "redirect by FTP "                                                                                                                                                                             \
        "server'. With this option, "                                                                                                                                                                  \
        "which may be given more "                                                                                                                                                                     \
        "than once, you can "                                                                                                                                                                          \
        "add additional addresses to "                                                                                                                                                                 \
        "the internal list of "                                                                                                                                                                        \
        "allowed data "                                                                                                                                                                                \
        "connection targets. IP4 has "                                                                                                                                                                 \
        "to be an IPv4 address or a "                                                                                                                                                                  \
        "list of IPv4 "                                                                                                                                                                                \
        "addresses, separated by "                                                                                                                                                                     \
        "commas.\n"                                                                                                                                                                                    \
        "The environment variable "                                                                                                                                                                    \
        "FTPCOPY_ALLOW_PASV_IP "                                                                                                                                                                       \
        "serves the same purpose.\n"                                                                                                                                                                   \
        "Note: Do not use "                                                                                                                                                                            \
        "this option without "                                                                                                                                                                         \
        "thinking: FTP redirects may "                                                                                                                                                                 \
        "be used to launch "                                                                                                                                                                           \
        "denial of service attacks "                                                                                                                                                                   \
        "against innocent targets.\n"                                                                                                                                                                  \
        "This option was added in "                                                                                                                                                                    \
        "version 0.6.1.",                                                                                                                                                                              \
        "IP4"                                                                                                                                                                                          \
  }
#define COMMON_OPT_force_select                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    0, "force-select", uogo_flag, UOGO_NOARG, &iopause_force_select, 1, "Use select, not poll.",                                                                                                       \
        "Do not use the poll() "                                                                                                                                                                       \
        "system call even if it's "                                                                                                                                                                    \
        "available, but use select() "                                                                                                                                                                 \
        "instead. This allows the "                                                                                                                                                                    \
        "program to be used together "                                                                                                                                                                 \
        "with runsocks "                                                                                                                                                                               \
        "from the socks5 reference "                                                                                                                                                                   \
        "implementation.\n"                                                                                                                                                                            \
        "Please note that you will "                                                                                                                                                                   \
        "need a directly reachable "                                                                                                                                                                   \
        "name server anyway, as "                                                                                                                                                                      \
        "the DNS library in use does "                                                                                                                                                                 \
        "not support SOCKS (you can "                                                                                                                                                                  \
        "always use IP "                                                                                                                                                                               \
        "addresses).\n"                                                                                                                                                                                \
        "This option was added in "                                                                                                                                                                    \
        "version 0.3.8.",                                                                                                                                                                              \
        0                                                                                                                                                                                              \
  }

extern buffer io_i;
extern buffer io_o;
extern unsigned long o_loglevel;
extern unsigned long o_data_connect_retries;
extern int o_force_select;
extern int o_eat_leading_spaces;

extern unsigned long o_timeout;
extern unsigned long o_login_sleep;
extern stralloc remoteip;          /* list of IP addresses of
                                      the host */
extern stralloc pasv_response_ips; /* list of IP
                                      addresses
                                      allowed in
                                      PASV response
                                    */

int do_pasv(void);
int connect_auth(const char* host, const char* o_user, const char* o_pass, const char* o_acct, int tries);
char* ccread(void);
char* ccread_oneline(void);
void cmdwrite2(const char* s1, const unsigned char* s2);
void cmdwrite1(const char* s);
void x2(const char* where);
void sx2(const char* what);
void do_log1(const char* s1);
void do_log2(const char* s1, const char* s2);
void do_log3(const char* s1, const char* s2, const char* s3);
void do_log4(const char* s1, const char* s2, const char* s3, const char* s4);
void do_logmem(const char* s1, unsigned int l);
void warn_broken_mlsx(void);
stralloc* canon(stralloc* l_dir); /* feed me with 0
                                     terminated
                                     strings only */

void eof_or_error(int ec, int e, const char* s1, const char* s2, const char* s3) attribute_noreturn;
void sockrecbuf(int, unsigned long);
extern unsigned int window_x;
extern unsigned int window_y;
int inquire_window_size(void);
int setup_window_size(void);
