zephyr_library()

zephyr_library_sources(
    st25_api.c
    tagtype3_wrapper.c
    tagtype4_wrapper.c
    lib_wrapper.c
    tagtype5_wrapper.c
    lib_NDEF_config.c
    bsp_nfctag.c
    st25dv-i2c_zephyr.c
    st25dvxxkc.c
    st25dvxxkc_reg.c
)
