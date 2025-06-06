#include "../unix.h"
#include "../util.h"

static const char* const unix_errnos[] = {
    "(success)",     "EPERM",        "ENOENT",      "ESRCH",           "EINTR",           "EIO",          "ENXIO",        "E2BIG",        "ENOEXEC",
    "EBADF",         "ECHILD",       "EAGAIN",      "ENOMEM",          "EACCES",          "EFAULT",       "ENOTBLK",      "EBUSY",        "EEXIST",
    "EXDEV",         "ENODEV",       "ENOTDIR",     "EISDIR",          "EINVAL",          "ENFILE",       "EMFILE",       "ENOTTY",       "ETXTBSY",
    "EFBIG",         "ENOSPC",       "ESPIPE",      "EROFS",           "EMLINK",          "EPIPE",        "EDOM",         "ERANGE",       "EDEADLK",
    "ENAMETOOLONG",  "ENOLCK",       "ENOSYS",      "ENOTEMPTY",       "ELOOP",           "(errno: 41)",  "ENOMSG",       "EIDRM",        "ECHRNG",
    "EL2NSYNC",      "EL3HLT",       "EL3RST",      "ELNRNG",          "EUNATCH",         "ENOCSI",       "EL2HLT",       "EBADE",        "EBADR",
    "EXFULL",        "ENOANO",       "EBADRQC",     "EBADSLT",         "(errno: 58)",     "EBFONT",       "ENOSTR",       "ENODATA",      "ETIME",
    "ENOSR",         "ENONET",       "ENOPKG",      "EREMOTE",         "ENOLINK",         "EADV",         "ESRMNT",       "ECOMM",        "EPROTO",
    "EMULTIHOP",     "EDOTDOT",      "EBADMSG",     "EOVERFLOW",       "ENOTUNIQ",        "EBADFD",       "EREMCHG",      "ELIBACC",      "ELIBBAD",
    "ELIBSCN",       "ELIBMAX",      "ELIBEXEC",    "EILSEQ",          "ERESTART",        "ESTRPIPE",     "EUSERS",       "ENOTSOCK",     "EDESTADDRREQ",
    "EMSGSIZE",      "EPROTOTYPE",   "ENOPROTOOPT", "EPROTONOSUPPORT", "ESOCKTNOSUPPORT", "EOPNOTSUPP",   "EPFNOSUPPORT", "EAFNOSUPPORT", "EADDRINUSE",
    "EADDRNOTAVAIL", "ENETDOWN",     "ENETUNREACH", "ENETRESET",       "ECONNABORTED",    "ECONNRESET",   "ENOBUFS",      "EISCONN",      "ENOTCONN",
    "ESHUTDOWN",     "ETOOMANYREFS", "ETIMEDOUT",   "ECONNREFUSED",    "EHOSTDOWN",       "EHOSTUNREACH", "EALREADY",     "EINPROGRESS",  "ESTALE",
    "EUCLEAN",       "ENOTNAM",      "ENAVAIL",     "EISNAM",          "EREMOTEIO",       "EDQUOT",       "ENOMEDIUM",    "EMEDIUMTYPE",  "ECANCELED",
    "ENOKEY",        "EKEYEXPIRED",  "EKEYREVOKED", "EKEYREJECTED",
};

const char*
unix_errno(int err) {
  if(err >= 0 && err < countof(unix_errnos))
    return unix_errnos[err];

  return "(unknown errno)";
}
