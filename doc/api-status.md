# API and test status

What is checked, how, and what is left. Regenerate with `tests/make-api-status.py N`.

| check | command | result |
|---|---|---|
| unit tests | `build/.../tests/all_tests` (run in `tests/`) | 876 tests pass; `valgrind -q --error-exitcode=9` clean |
| every header compiles alone | `tests/check-headers.sh` | 0 failures |
| libowfat source compatibility | `tests/libowfat-compat/` (see `doc/libowfat-compat.md`) | 517 prototypes, 0 missing |
| one function per file | `tests/check-onefunc.py` | 48 files deviate from one function per file |
| genmakefile | `tests/genmakefile/{pair-headers,import-mplabx,project-file,infer-sources}.sh GENMAKEFILE` | all pass |
| functions not named by a test | this script | 123 |

## Functions not named by a test

Most are run by tests of bigger functions or hidden behind macros (`omf_record_foreach`, the `bmap_*` macros);
the rest are internal helpers, debug-only code (`DEBUG_CHARBUF`, the `cbmap` debug allocators) and code that
exists only for Windows (`win32_*`, `winsock*`, `terminal_wattr`, `_tpopen`).

* `buffer`: buffer_default buffer_dummyread_fromstr buffer_dummyreadbuf buffer_dummyreadmmap buffer_unixread buffer_unixwrite
* `cb`: cb_find_top_i cb_free_node
* `cbmap`: cbmap_data_node_destroy cbmap_default_allocator cbmap_default_deallocator cbmap_internal_node_destroy cbmap_internal_node_new cbmap_mem_allocated cbmap_mem_debug_calloc cbmap_mem_debug_free cbmap_mem_debug_mem cbmap_mem_debug_strdup cbmap_mem_debug_strndup cbmap_mem_mem_dup cbmap_mem_memalign cbmap_mem_posix_memalign cbmap_nop_allocator cbmap_nop_deallocator cbmap_recursive_delete cbmap_visit
* `charbuf`: charbuf_dump charbuf_stubborn_read
* `cpp`: cpp_const_expr cpp_convert_int cpp_convert_number cpp_error_at cpp_error_tok cpp_join_adjacent_string_literals cpp_note_skipped cpp_preprocess2 cpp_query_undefined cpp_read_line_marker cpp_read_macro_definition cpp_skip_cond_incl2 cpp_verror_at cpp_warn_tok
* `elf`: elf_symbol_r
* `fmt`: fmt_2digits fmt_hex2 fmt_oct3 fmt_strm_internal
* `gpio`: gpio_set_input_pull gpio_set_input_resistor_pull
* `http`: chunked_line http_body_flush http_open http_read http_read_header http_read_internal http_response_free http_response_read http_sendreq http_socket http_socket_read http_socket_write
* `io`: io_dontwantread_really io_dontwantwrite_really io_fd io_fd_canwrite io_fd_flags io_finishandshutdown io_getfds io_wantread_really io_wantwrite_really
* `omf`: omf_record_begin omf_record_end omf_record_next
* `path`: hasAccessRight
* `playlist`: playlist_entry_clear playlist_state_new
* `scan`: test_main
* `socket`: __winsock_init socket_fastopen_connect4 socket_fastopen_connect6 socket_mcjoin6 socket_mcleave6 socket_send6_flag winsock2errno wsa_socketpair
* `stralloc`: buffer_puts_escaped_args byte_scan stralloc_catm_internal stralloc_write
* `strarray`: strarray_sort_compare
* `terminal`: terminal_info terminal_rgb16 terminal_wattr
* `textbuf`: textbuf_read
* `textcode`: unittest_main
* `tls`: tls_bio_socket tls_context tls_errno tls_instance_handshake tls_instance_new tls_instance_return tls_verify
* `unix`: _dummy_add_pid _pclose _tLoadProg _tpopen daemon fnmatch ftruncate getdelim getpagesize glob globfree is_junction is_symlink pclose popen readlink wordexp wordfree
* `xml`: xml_find_with_attrs_l xml_pargs_free xml_pargs_make

## Files that break the one function per file rule

* lib/alloc/alloc.c alloc alloc_free
* lib/buffer/buffer_bzip.c buffer_bunzip buffer_bunzip_fd buffer_bzip buffer_bzip_fd
* lib/buffer/buffer_gzip.c buffer_gunzip buffer_gunzip_fd buffer_gzip buffer_gzip_fd
* lib/cbmap/cbmap_alloc.c cbmap_mem_allocated cbmap_mem_debug_calloc cbmap_mem_debug_free cbmap_mem_debug_mem cbmap_mem_debug_strdup cbmap_mem_debug_strndup cbmap_mem_mem_dup cbmap_mem_memalign cbmap_mem_posix_memalign
* lib/cbmap/cbmap_new.c cbmap_default_allocator cbmap_default_deallocator cbmap_new cbmap_new_with_custom_allocation cbmap_new_with_static_keys_and_values cbmap_nop_allocator cbmap_nop_deallocator
* lib/cbmap/memalign.c memalign_alloc memalign_alloc_aligned memalign_free
* lib/cpp/cpp_error.c cpp_error_at cpp_error_tok cpp_verror_at cpp_warn_tok
* lib/cpp/cpp_token.c cpp_token_append cpp_token_copy cpp_token_dump cpp_token_eof cpp_token_free cpp_token_new
* lib/dns/dns_debug.c dns_debug_hex dns_debug_msg dns_debug_num
* lib/dns/dns_dtda.c dns_domain_todot dns_domain_todot_cat dns_domain_todot_inline dns_domain_todot_utf8_cat
* lib/dns/dns_random.c dns_random dns_random_init
* lib/dns/dns_rcip.c dns_resolvconfip dns_resolvconfip_reset
* lib/dns/dns_transmit.c dns_transmit_free dns_transmit_get dns_transmit_io dns_transmit_start
* lib/env/env_put.c env_clear env_init env_put env_put2 env_put2b env_putb env_unset_internal
* lib/errmsg/errmsg_puts.c errmsg_flush errmsg_puts errmsg_start
* lib/fmt/fmt_escapecharxml.c fmt_escapecharhtml fmt_escapecharxml
* lib/http/http_socket.c http_socket http_socket_read http_socket_write
* lib/io/io_dontwantread.c io_dontwantread io_dontwantread_really
* lib/io/io_dontwantwrite.c io_dontwantwrite io_dontwantwrite_really
* lib/io/io_fd.c io_fd io_fd_canwrite io_fd_flags io_getfds
* lib/io/io_wantread.c io_wantread io_wantread_really
* lib/io/io_wantwrite.c io_wantwrite io_wantwrite_really
* lib/path/path_access.c hasAccessRight path_access
* lib/scan/scan_varint.c scan_pb_type0_int scan_varint
* lib/sig/sig_stack.c sig_pop sig_pusha
* lib/socket/socket_send6.c socket_send6 socket_send6_flag
* lib/socket/socket_udp6.c socket_udp6 socket_udp6b
* lib/socket/winsock_init.c __winsock_init
* lib/socket/winsock_socketpair.c wsa_socketpair
* lib/strarray/strarray_glob.c strarray_glob strarray_glob_b
* lib/textcode/scan_html.c scan_html scan_html_tagarg
* lib/tls/tls_init.c tls_context tls_init tls_verify
* lib/tls/tls_new.c tls_client tls_server
* lib/ucs/ucs_latin1.c ucs_from_latin1_char ucs_to_latin1_char
* lib/unix/getopt.c unix_getopt unix_getopt_long
* lib/unix/glob.c glob globfree
* lib/unix/pipe2.c pipe pipe2
* lib/unix/popen.c pclose popen
* lib/unix/readlink.c is_junction is_symlink readlink
* lib/unix/symlink.c link symlink
* lib/unix/win32_borland_popen.c _pclose _tpopen
* lib/unix/win32_glob.c glob globfree
* lib/unix/win32_loadprog.c _dummy_add_pid _tLoadProg
* lib/unix/win32_mkdtemp.c mkdtemp
* lib/unix/win32_pipe.c pipe
* lib/unix/win32_popen.c _pclose popen
* lib/unix/win32_setenv.c setenv
* lib/unix/wordexp.c wordexp wordfree
