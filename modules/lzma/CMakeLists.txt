# Copyright (c) 2024 Telink Semiconductor
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_LZMA)

  set(LZMA_DIR ${ZEPHYR_CURRENT_MODULE_DIR})

  add_library(liblzma STATIC
    ${LZMA_DIR}/src/liblzma/common/common.c
  )

  # target_compile_options(liblzma PRIVATE -nostdinc)

  zephyr_library_include_directories(
    ${LZMA_DIR}/
    ${LZMA_DIR}/src/common
    ${LZMA_DIR}/src/liblzma/api
    ${LZMA_DIR}/src/liblzma/lz
    ${LZMA_DIR}/src/liblzma/lzma
    ${LZMA_DIR}/src/liblzma/rangecoder
    ${LZMA_DIR}/src/liblzma/simple
  )

  target_include_directories(liblzma PRIVATE
    configs/
    ${LZMA_DIR}/
    ${LZMA_DIR}/src/common
    ${LZMA_DIR}/src/liblzma/api
    ${LZMA_DIR}/src/liblzma/common
    ${LZMA_DIR}/src/liblzma/lz
    ${LZMA_DIR}/src/liblzma/lzma
    ${LZMA_DIR}/src/liblzma/rangecoder
    ${LZMA_DIR}/src/liblzma/simple
    ${LZMA_DIR}/src/liblzma/delta
  )

  target_compile_definitions(liblzma PRIVATE HAVE_CONFIG_H)
  target_link_libraries(liblzma PRIVATE zephyr_interface)

endif()
