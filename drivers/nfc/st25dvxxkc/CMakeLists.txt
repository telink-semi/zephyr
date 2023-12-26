zephyr_library()

zephyr_library_sources(
    tagtype3_wrapper.c
    tagtype4_wrapper.c
    lib_wrapper.c
    tagtype5_wrapper.c
    lib_NDEF_config.c
    stm32l4s5i_iot01_nfctag.c
    stm32l4s5i_iot01.c
    st25dvxxkc.c
    st25dvxxkc_reg.c
)

zephyr_library_include_directories(${ZEPHYR_BASE}/include/zephyr/drivers/nfc/st25dvxxkc)
