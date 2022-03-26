file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz/cmake/xz-config.cmake"
     "include(\${CMAKE_CURRENT_LIST_DIR}/xz.cmake)")
file(
  WRITE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz/CMakeLists.txt"
  "cmake_minimum_required(VERSION 3.2)\n\nproject(xz VERSION 5.3.0 LANGUAGES C)\n\nif(MSVC)\n\tset(CMAKE_DEBUG_POSTFIX \"d\")\nendif()\n\nif(UNIX OR LINUX)\n\tadd_definitions(-DMYTHREAD_POSIX=1)\nendif(UNIX OR LINUX)\n\nif(LZMA_STATIC)\n\tset(LIB_TYPE STATIC)\nelse(LZMA_STATIC)\n\tset(LIB_TYPE )\nendif(LZMA_STATIC)\n\nadd_library(\n\tlzma\n\t\${LIB_TYPE}\n\tsrc/common/tuklib_cpucores.c\n\tsrc/common/tuklib_physmem.c\n\tsrc/liblzma/check/check.c\n\tsrc/liblzma/check/crc32_fast.c\n\tsrc/liblzma/check/crc32_table.c\n\tsrc/liblzma/check/crc64_fast.c\n\tsrc/liblzma/check/crc64_table.c\n\tsrc/liblzma/check/sha256.c\n\tsrc/liblzma/common/alone_decoder.c\n\tsrc/liblzma/common/alone_encoder.c\n\tsrc/liblzma/common/auto_decoder.c\n\tsrc/liblzma/common/block_buffer_decoder.c\n\tsrc/liblzma/common/block_buffer_encoder.c\n\tsrc/liblzma/common/block_decoder.c\n\tsrc/liblzma/common/block_encoder.c\n\tsrc/liblzma/common/block_header_decoder.c\n\tsrc/liblzma/common/block_header_encoder.c\n\tsrc/liblzma/common/block_util.c\n\tsrc/liblzma/common/common.c\n\tsrc/liblzma/common/easy_buffer_encoder.c\n\tsrc/liblzma/common/easy_decoder_memusage.c\n\tsrc/liblzma/common/easy_encoder.c\n\tsrc/liblzma/common/easy_encoder_memusage.c\n\tsrc/liblzma/common/easy_preset.c\n\tsrc/liblzma/common/filter_buffer_decoder.c\n\tsrc/liblzma/common/filter_buffer_encoder.c\n\tsrc/liblzma/common/filter_common.c\n\tsrc/liblzma/common/filter_decoder.c\n\tsrc/liblzma/common/filter_encoder.c\n\tsrc/liblzma/common/filter_flags_decoder.c\n\tsrc/liblzma/common/filter_flags_encoder.c\n\tsrc/liblzma/common/hardware_cputhreads.c\n\tsrc/liblzma/common/hardware_physmem.c\n\tsrc/liblzma/common/index.c\n\tsrc/liblzma/common/index_decoder.c\n\tsrc/liblzma/common/index_encoder.c\n\tsrc/liblzma/common/index_hash.c\n\tsrc/liblzma/common/outqueue.c\n\tsrc/liblzma/common/stream_buffer_decoder.c\n\tsrc/liblzma/common/stream_buffer_encoder.c\n\tsrc/liblzma/common/stream_decoder.c\n\tsrc/liblzma/common/stream_encoder.c\n\tsrc/liblzma/common/stream_encoder_mt.c\n\tsrc/liblzma/common/stream_flags_common.c\n\tsrc/liblzma/common/stream_flags_decoder.c\n\tsrc/liblzma/common/stream_flags_encoder.c\n\tsrc/liblzma/common/vli_decoder.c\n\tsrc/liblzma/common/vli_encoder.c\n\tsrc/liblzma/common/vli_size.c\n\tsrc/liblzma/delta/delta_common.c\n\tsrc/liblzma/delta/delta_decoder.c\n\tsrc/liblzma/delta/delta_encoder.c\n\tsrc/liblzma/lzma/fastpos_table.c\n\tsrc/liblzma/lzma/lzma2_decoder.c\n\tsrc/liblzma/lzma/lzma2_encoder.c\n\tsrc/liblzma/lzma/lzma_decoder.c\n\tsrc/liblzma/lzma/lzma_encoder.c\n\tsrc/liblzma/lzma/lzma_encoder_optimum_fast.c\n\tsrc/liblzma/lzma/lzma_encoder_optimum_normal.c\n\tsrc/liblzma/lzma/lzma_encoder_presets.c\n\tsrc/liblzma/lz/lz_decoder.c\n\tsrc/liblzma/lz/lz_encoder.c\n\tsrc/liblzma/lz/lz_encoder_mf.c\n\tsrc/liblzma/rangecoder/price_table.c\n\tsrc/liblzma/simple/arm.c\n\tsrc/liblzma/simple/armthumb.c\n\tsrc/liblzma/simple/ia64.c\n\tsrc/liblzma/simple/powerpc.c\n\tsrc/liblzma/simple/simple_coder.c\n\tsrc/liblzma/simple/simple_decoder.c\n\tsrc/liblzma/simple/simple_encoder.c\n\tsrc/liblzma/simple/sparc.c\n\tsrc/liblzma/simple/x86.c\n\tsrc/common/mythread.h\n\tsrc/common/sysdefs.h\n\tsrc/common/tuklib_common.h\n\tsrc/common/tuklib_config.h\n\tsrc/common/tuklib_cpucores.h\n\tsrc/common/tuklib_integer.h\n\tsrc/common/tuklib_physmem.h\n\tsrc/liblzma/api/lzma.h\n\tsrc/liblzma/api/lzma/base.h\n\tsrc/liblzma/api/lzma/bcj.h\n\tsrc/liblzma/api/lzma/block.h\n\tsrc/liblzma/api/lzma/check.h\n\tsrc/liblzma/api/lzma/container.h\n\tsrc/liblzma/api/lzma/delta.h\n\tsrc/liblzma/api/lzma/filter.h\n\tsrc/liblzma/api/lzma/hardware.h\n\tsrc/liblzma/api/lzma/index.h\n\tsrc/liblzma/api/lzma/index_hash.h\n\tsrc/liblzma/api/lzma/lzma12.h\n\tsrc/liblzma/api/lzma/stream_flags.h\n\tsrc/liblzma/api/lzma/version.h\n\tsrc/liblzma/api/lzma/vli.h\n\tsrc/liblzma/check/check.h\n\tsrc/liblzma/check/crc32_table_be.h\n\tsrc/liblzma/check/crc32_table_le.h\n\tsrc/liblzma/check/crc64_table_be.h\n\tsrc/liblzma/check/crc64_table_le.h\n\tsrc/liblzma/check/crc_macros.h\n\tsrc/liblzma/common/alone_decoder.h\n\tsrc/liblzma/common/block_buffer_encoder.h\n\tsrc/liblzma/common/block_decoder.h\n\tsrc/liblzma/common/block_encoder.h\n\tsrc/liblzma/common/common.h\n\tsrc/liblzma/common/easy_preset.h\n\tsrc/liblzma/common/filter_common.h\n\tsrc/liblzma/common/filter_decoder.h\n\tsrc/liblzma/common/filter_encoder.h\n\tsrc/liblzma/common/index.h\n\tsrc/liblzma/common/index_encoder.h\n\tsrc/liblzma/common/memcmplen.h\n\tsrc/liblzma/common/outqueue.h\n\tsrc/liblzma/common/stream_decoder.h\n\tsrc/liblzma/common/stream_flags_common.h\n\tsrc/liblzma/delta/delta_common.h\n\tsrc/liblzma/delta/delta_decoder.h\n\tsrc/liblzma/delta/delta_encoder.h\n\tsrc/liblzma/delta/delta_private.h\n\tsrc/liblzma/lzma/fastpos.h\n\tsrc/liblzma/lzma/lzma2_decoder.h\n\tsrc/liblzma/lzma/lzma2_encoder.h\n\tsrc/liblzma/lzma/lzma_common.h\n\tsrc/liblzma/lzma/lzma_decoder.h\n\tsrc/liblzma/lzma/lzma_encoder.h\n\tsrc/liblzma/lzma/lzma_encoder_private.h\n\tsrc/liblzma/lz/lz_decoder.h\n\tsrc/liblzma/lz/lz_encoder.h\n\tsrc/liblzma/lz/lz_encoder_hash.h\n\tsrc/liblzma/lz/lz_encoder_hash_table.h\n\tsrc/liblzma/rangecoder/price.h\n\tsrc/liblzma/rangecoder/range_common.h\n\tsrc/liblzma/rangecoder/range_decoder.h\n\tsrc/liblzma/rangecoder/range_encoder.h\n\tsrc/liblzma/simple/simple_coder.h\n\tsrc/liblzma/simple/simple_decoder.h\n\tsrc/liblzma/simple/simple_encoder.h\n\tsrc/liblzma/simple/simple_private.h\n\twindows/config.h)\n\ntarget_include_directories(\n\tlzma\n\tPRIVATE \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/api> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/check> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/common> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/lzma>\n\t\t\t\t\t\$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/lz> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/simple> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/delta> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/rangecoder> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/common> \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/common>\n\t\t\t\t\t\$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/windows>\n\tINTERFACE \$<INSTALL_INTERFACE:include>\n\tPUBLIC \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/api>)\n\ntarget_compile_definitions(lzma PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_STDC_NO_DEPRECATE HAVE_CONFIG_H)\n\ninclude(CMakePackageConfigHelpers)\nwrite_basic_package_version_file(\${CMAKE_CURRENT_BINARY_DIR}/\${PROJECT_NAME}-config-version.cmake VERSION \${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)\n\nif(NOT LZMA_NO_INSTALL)\n\tinstall(TARGETS lzma EXPORT lzma RUNTIME DESTINATION bin ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)\n\tinstall(DIRECTORY \${CMAKE_CURRENT_SOURCE_DIR}/src/liblzma/api/ DESTINATION include)\n\tinstall(EXPORT lzma FILE \${PROJECT_NAME}.cmake NAMESPACE \${PROJECT_NAME}:: DESTINATION lib/cmake/\${PROJECT_NAME})\n\tinstall(FILES cmake/\${PROJECT_NAME}-config.cmake \${CMAKE_CURRENT_BINARY_DIR}/\${PROJECT_NAME}-config-version.cmake DESTINATION lib/cmake/\${PROJECT_NAME})\nendif(NOT LZMA_NO_INSTALL)\n\nexport(TARGETS lzma FILE \${CMAKE_CURRENT_BINARY_DIR}/\${PROJECT_NAME}-config.cmake NAMESPACE \${PROJECT_NAME}::)\n"
)
