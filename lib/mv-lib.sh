mkdir - p array;
git add array;
git mv -
    f array_allocate.c array_bytes.c array_cat.c array_cat0.c array_catb.c array_cate.c array_cats.c array_cats0
        .c array_equal.c array_fail.c array_get.c array_length.c array_reset.c array_start.c array_trunc
        .c array_truncate.c array mkdir -
    p buffer;
git add buffer;
git mv -
    f buffer_0.c buffer_0small.c buffer_1.c buffer_1small.c buffer_2.c buffer_close.c buffer_default.c buffer_dummyread
        .c buffer_dummyreadmmap.c buffer_dump.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromarray
        .c buffer_frombuf.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa
        .c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa
        .c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_getn
        .c buffer_getnewline_sa.c buffer_init.c buffer_init_free.c buffer_mmapprivate.c buffer_mmapread
        .c buffer_mmapread_fd.c buffer_munmap.c buffer_peek.c buffer_prefetch.c buffer_put.c buffer_put8long
        .c buffer_putalign.c buffer_putc.c buffer_puterror.c buffer_puterror2.c buffer_putflush.c buffer_putlong
        .c buffer_putlonglong.c buffer_putm_internal.c buffer_putm_internal_flush.c buffer_putnlflush.c buffer_putnspace
        .c buffer_puts.c buffer_putsa.c buffer_putsaflush.c buffer_putsalign.c buffer_putsflush.c buffer_putspace
        .c buffer_putulonglong.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_seek
        .c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c buffer mkdir -
    p byte;
git add byte;
git mv - f byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_rchr.c byte_zero.c byte mkdir - p dir;
git add dir;
git mv - f dir_close.c dir_name.c dir_open.c dir_read.c dir_time.c dir_type.c dir mkdir - p fmt;
git add fmt;
git mv -
    f fmt_8long.c fmt_octal.c fmt_asn1derlength.c fmt_asn1dertag.c fmt_double.c fmt_escapecharc.c fmt_escapecharhtml
        .c fmt_escapecharquotedprintable.c fmt_escapecharquotedprintableutf8.c fmt_escapecharxml.c fmt_fill
        .c fmt_httpdate.c fmt_human.c fmt_humank.c fmt_iso8601.c fmt_long.c fmt_longlong.c fmt_minus.c fmt_pad
        .c fmt_plusminus.c fmt_str.c fmt_strm_internal.c fmt_strn.c fmt_tohex.c fmt_ulonglong.c fmt_ulong.c fmt_ulong0
        .c fmt_ulonglong.c fmt_utf8.c fmt_xlong.c fmt_xlonglong.c fmt_xmlescape.c fmt mkdir -
    p hmap;
git add hmap;
git mv -
    f hmap_add.c hmap_add_tuple_with_data.c hmap_delete.c hmap_destroy.c hmap_free_data.c hmap_init.c hmap_internal
        .c hmap_is_locate.c hmap_print_list.c hmap_print_table.c hmap_print_tree.c hmap_search.c hmap_set
        .c hmap_truncate.c hmap mkdir -
    p http;
git add http;
git mv - f http_get.c http_init.c http_readable.c http_sendreq.c http_writeable.c http mkdir - p iarray;
git add iarray;
git mv - f iarray_allocate.c iarray_get.c iarray_init.c iarray mkdir - p io;
git add io;
git mv -
    f io_appendfile.c io_block.c io_canread.c io_canwrite.c io_check.c io_close.c io_closeonexec.c io_createfile
        .c io_debugstring.c io_dontwantread.c io_dontwantwrite.c io_eagain.c io_eagain_read.c io_eagain_write.c io_fd
        .c io_finishandshutdown.c io_getcookie.c io_mmapwritefile.c io_nonblock.c io_passfd.c io_pipe.c io_readfile
        .c io_readwritefile.c io_receivefd.c io_sendfile.c io_setcookie.c io_sigpipe.c io_socketpair.c io_timedout
        .c io_timeout.c io_timeouted.c io_tryread.c io_tryreadtimeout.c io_trywrite.c io_trywritetimeout.c io_wait
        .c io_waitread.c io_waituntil.c io_waituntil2.c io_waitwrite.c io_wantread.c io_wantwrite.c io mkdir -
    p list;
git add list;
git mv -
    f list_add_after.c list_add_before.c list_find.c list_find_remove.c list_init.c list_length.c list_move_head
        .c list_move_tail.c list_push.c list_remove.c list_swap.c list_unshift.c list mkdir -
    p mmap;
git add mmap;
git mv - f mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c mmap mkdir - p open;
git add open;
git mv - f open_append.c open_read.c open_rw.c open_trunc.c open mkdir - p rdir;
git add rdir;
git mv - f rdir_close.c rdir_open.c rdir_read.c rdir mkdir - p scan;
git add scan;
git mv -
    f scan_double.c scan_fromhex.c scan_uint.c scan_ulong.c scan_ulonglong.c scan_ulongn.c scan_ushort.c scan_xlong
        .c scan_xlonglong.c scan mkdir -
    p socket;
git add socket;
git mv -
    f socket_connect4.c socket_connect6.c socket_connected.c socket_ip4loopback.c socket_noipv6.c socket_tcp4
        .c socket_tcp4b.c socket_tcp6.c socket_tcp6b.c socket_v4mappedprefix.c socket_v6loopback.c socket mkdir -
    p str;
git add str;
git mv -
    f str_basename.c str_chr.c str_diff.c str_diffn.c str_dup.c str_istr.c str_len.c str_ptime.c str_rchr.c str_tok
        .c str mkdir -
    p stralloc;
git add stralloc;
git mv -
    f stralloc_append.c stralloc_append_sa.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0
        .c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs
        .c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready
        .c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c stralloc mkdir -
    p strarray;
git add strarray;
git mv - f strarray_push.c strarray_pushd.c strarray mkdir - p strlist;
git add strlist;
git mv -
    f strlist_at.c strlist_cat.c strlist_count.c strlist_dump.c strlist_index_of.c strlist_join.c strlist_push
        .c strlist_push_sa.c strlist_push_tokens.c strlist_push_unique.c strlist_pushb.c strlist_pushm_internal
        .c strlist_pushsa.c strlist_shift.c strlist_sort.c strlist_to_argv.c strlist_unshift.c strlist mkdir -
    p tai;
git add tai;
git mv - f tai_add.c tai_now.c tai_pack.c tai_sub.c tai_uint.c tai_unpack.c tai mkdir - p taia;
git add taia;
git mv -
    f taia_add.c taia_addsec.c taia_approx.c taia_frac.c taia_half.c taia_less.c taia_now.c taia_pack.c taia_sub
        .c taia_tai.c taia_uint.c taia_unpack.c taia mkdir -
    p uint16;
git add uint16;
git mv - f uint16_pack.c uint16_pack_big.c uint16_unpack.c uint16_unpack_big.c uint16 mkdir - p uint32;
git add uint32;
git mv - f uint32_pack.c uint32_pack_big.c uint32_unpack.c uint32_unpack_big.c uint32 mkdir - p uint64;
git add uint64;
git mv - f uint64_pack.c uint64_pack_big.c uint64_unpack.c uint64_unpack_big.c uint64
