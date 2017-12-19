//=---------------------------------------------------------------------=
//
// $Id: utf8.h,v 1.1 2011/01/10 10:50:54 matt-beard Exp $ $Name:  $
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Metaglue Corporation.
// All rights reserved.
//
//=--
#ifndef __UTF8_h__
#define __UTF8_h__

#include <wchar.h>

// UTF8 versions of mbs/wcs libc functions
// (because MSFT and GNU take different approaches from each other)

#ifndef MG_UTF8
#define MG_UTF8 1
#endif

#if defined(MG_UTF8)

// convert a wchar_t to a utf8 (multibyte)char
// follows signature of int wctomb( char *m, wchar_t w )
// requires m to point to a buffer of length 4 or more
int wctou8(char* m, wchar_t w);

// number of char required to represent a single wchar_t in utf8
// follows signature of mblen( const wchar_t, 1