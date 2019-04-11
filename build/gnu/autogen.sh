#!/bin/sh
#
THISDIR=`dirname "$0"`
set -e
set -x

cd $THISDIR/../..

aclocal --force -I config
autoheader --force
autoconf --force
