TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig

TARGET = pkgconf

INCLUDEPATH += . $$PWD/lib

PKGCONFIG += liblzma
DEFINES += HAVE_LIBLZMA=1

PKGCONFIG += zlib
DEFINES += HAVE_ZLIB=1


SOURCES = lib/socket/init.c pkgconf.c \
    lib/strlist/strlist_froms.c \
    lib/strlist/strlist_at.c \
    lib/strlist/strlist_cat.c \
    lib/strlist/strlist_contains_sa.c \
    lib/strlist/strlist_contains.c \
    lib/strlist/strlist_count.c \
    lib/strlist/strlist_dump.c \
    lib/strlist/strlist_froms.c \
    lib/strlist/strlist_index_of.c \
    lib/strlist/strlist_join.c \
    lib/strlist/strlist_joins.c \
    lib/strlist/strlist_push_sa.c \
    lib/strlist/strlist_push_tokens.c \
    lib/strlist/strlist_push_unique_sa.c \
    lib/strlist/strlist_push_unique.c \
    lib/strlist/strlist_push.c \
    lib/strlist/strlist_pushb.c \
    lib/strlist/strlist_pushm_internal.c \
    lib/strlist/strlist_pushsa.c \
    lib/strlist/strlist_range.c \
    lib/strlist/strlist_shift.c \
    lib/strlist/strlist_sort.c \
    lib/strlist/strlist_to_argv.c \
    lib/strlist/strlist_unshift.c
HEADERS = 
