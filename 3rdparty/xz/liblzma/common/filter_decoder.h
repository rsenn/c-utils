///////////////////////////////////////////////////////////////////////////////
//
/// \file       filter_decoder.c
/// \brief      Filter ID mapping to filter-specific functions
//
//  Author:     Lasse Collin
//
//  This file has been put into the public domain.
//  You can do whatever you want with this file.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef LZMA_FILTER_DECODER_H
#define LZMA_FILTER_DECODER_H

#include "common.h"


#define HAVE_DECODER_LZMA1
#define HAVE_DECODER_LZMA2
#if defined(_X86_) || defined(__x86_64__)
#define HAVE_DECODER_X86
#elif defined(__thumb__)
#define HAVE_DECODER_ARMTHUMB 
#elif defined(__arm__)
#define HAVE_DECODER_ARM
#endif

extern lzma_ret lzma_raw_decoder_init(
		lzma_next_coder *next, const lzma_allocator *allocator,
		const lzma_filter *options);

#endif
