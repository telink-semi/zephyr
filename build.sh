#west build -p -b tl3228x ~/zephyrproject/zephyr/samples/subsys/usb/hid-keyboard -d ~/zephyrproject/build/build_hid_keyboard_tl3228x
west build -p -b tl3228x_dc ~/zephyrproject/zephyr/samples/bluetooth/peripheral_magnetic_keyboard/ -d ~/zephyrproject/build/build_peripheral_magnetic_keyboard_tl3228x  -- -DCONFIG_COMPILE_SDK="n"

