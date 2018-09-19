#!/bin/sh
#
# 20180919
set -e
set -x

aclocal --force -I config
autoheader --force
autoconf --force
