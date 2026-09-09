# Telink Zephyr SDK Release Note

[![Version](https://img.shields.io/badge/Version-tl_v1.4.0--v4.1.0-blue?style=flat-square)](https://github.com/telink-semi/tl_zephyr/releases/tag/tl_v1.4.0-v4.1.0)
[![License](https://img.shields.io/badge/License-Apache%202.0-red?style=flat-square)](../../../LICENSE)
[![Zephyr](https://img.shields.io/badge/Zephyr-v4.1.0-green?style=flat-square)](https://github.com/zephyrproject-rtos/zephyr/releases/tag/v4.1.0)

***

- **Release Type:** Public-Release
- **Tag Version:** tl\_v1.4.0-v4.1.0

<!-- - **Branch:** dev-tlk_v4.1 -->
<!-- - **Target Commit:** 0858e43f05a91d84ab2fed77f3849ff589510b24 -->

***

## 📖 Introduction

This release is based on the latest commit of the `dev-tlk_v4.1` branch and supports the new TL521X (TL5218X) chip series.

### Based on Upstream Zephyr v4.1.0

This release is based on upstream Zephyr **v4.1.0** (tag [`v4.1.0`](https://github.com/zephyrproject-rtos/zephyr/releases/tag/v4.1.0)). For the full list of upstream changes, see:

- [Zephyr v4.1.0 Release Notes](https://docs.zephyrproject.org/latest/releases/release-notes-4.1.html)
- [Zephyr v4.1.0 Migration Guide](https://docs.zephyrproject.org/latest/releases/migration-guide-4.1.html) — recommended reading when upgrading from a previous Zephyr version, as upstream v4.1.0 introduced several API and Kconfig changes (Bluetooth HCI driver API rewrite, pipe API rework, removed/deprecated options, etc.).

For environment setup, see the [Telink Zephyr SDK Getting Started Guide](../getting_started/index.md).

***

## 📋 Version Information

### Zephyr SDK & Toolchain

| Component                 | Version              |
| ------------------------- | -------------------- |
| **Host OS (recommended)** | Ubuntu 24.04 LTS     |
| **Zephyr SDK Version**    | Telink Zephyr v4.1.0 |
| **Zephyr SDK**            | 0.17.0               |
| **Toolchain**             | riscv64-zephyr-elf   |

> **Note:** Only the `riscv64-zephyr-elf` toolchain (from Zephyr SDK 0.17.0) is validated for Telink SoCs. The experimental IAR compiler support introduced in upstream Zephyr v4.1.0 is **not** tested with this release. GCC-based `riscv64-zephyr-elf` is the recommended and only supported toolchain. Ubuntu 24.04 LTS is the recommended host OS.

### Telink SDK

| Property         | Value                    |
| ---------------- | ------------------------ |
| **Tag Name**     | tl\_v1.4.0-v4.1.0        |
| **Release Type** | Public-Release           |

### Chip & Hardware Versions

📦 **Chip Versions**

| Chip Family | Versions |
| ----------- | -------- |
| TL521X      | A0       |

🔧 **Hardware EVK Versions**

| Chip   | EVK Version     |
| ------ | --------------- |
| TL521X | C1T416A20\_V1.0 |

***

## ✨ Highlights

| Category         | Details                       |
| ---------------- | ----------------------------- |
| **Refactoring**  | Split TL521X drivers out of the shared TLX drivers into dedicated drivers |

This release adds support for the new TL521X (TL5218X) chip series.

**Supported configurations:**

| SoC    | Configuration           |
| ------ | ----------------------- |
| TL521X | BLE-only or Thread-only |

***

## 🆕 New Features

- ✅ Split TL521X drivers out of the shared TLX drivers into dedicated drivers;

***

## 🐛 Bug Fixes

| Issue | Component | Description |
| ----- | --------- | ----------- |
| [#802](https://github.com/telink-semi/tl_zephyr/pull/802) | Bluetooth | Fix Matter commissioning failure after the BLE to Thread switch |
| [#802](https://github.com/telink-semi/tl_zephyr/pull/802) | Serial    | Fix a splitting issue found during the TL521X driver split |
| [#802](https://github.com/telink-semi/tl_zephyr/pull/802) | MCUBoot   | Re-add `MCUBOOT_START_OFFSET` for B9X/W9X, which was lost during the shared TLX Kconfig move |
| [#832](https://github.com/telink-semi/tl_zephyr/pull/832) | SoC       | Pull up the SWS pin after startup and retention reset |
| [#836](https://github.com/telink-semi/tl_zephyr/pull/836) | SoC       | Reset the radio at boot to support switching from Zigbee to Matter |

***

## 🔧 API & Kconfig Changes

### Telink-Specific Changes

The following Telink-specific Kconfig changes may affect existing applications when upgrading to this release:

| Change | Impact | Migration |
| ------ | ------ | --------- |
| BLE controller library naming changed from SoC-based (`_dual_core`/`_single_core`/`_general`) to role-based (`_peripheral`/`_central`/`_multirole`), selected via the new `TL_BLE_CTRL_VARIANT` Kconfig choice ([#830](https://github.com/telink-semi/tl_zephyr/pull/830)) | **BREAKING** — existing BLE applications must choose a role-based variant | Select the `TL_BLE_CTRL_VARIANT` value that matches the application role |
| Removed the per-SoC hardcoded `SUSPEND_EXIT_LATENCY_US` macros; suspend-exit latency is now set dynamically by the BLE library at runtime ([#830](https://github.com/telink-semi/tl_zephyr/pull/830)) | Applications relying on the removed macros break | Rely on the BLE library's runtime suspend-exit latency handling |
| Restructured the SoC layout: TL321X/TL322X/TL323X/TL521X/TL721X/TL5X moved to the new `soc/telink/tlx/` family with dedicated Kconfig and linker scripts, and TL521X got independent drivers ([#802](https://github.com/telink-semi/tl_zephyr/pull/802)) | Out-of-tree Kconfigs referencing the legacy family symbol may break | Update to the new SoC Kconfig symbols; a hidden `SOC_RISCV_TELINK_TLX` alias is kept for compatibility |
| Changed the `tl5218x` board devicetree to the new `telink,tl521x-*` compatibles ([#802](https://github.com/telink-semi/tl_zephyr/pull/802)) | Out-of-tree board dts/overlays using the old `telink,tlx-*` compatibles break | Update devicetree compatibles to `telink,tl521x-*` |
| Added a TL521X retention Kconfig and scoped `HWINFO_TELINK_TLX` to TL321X/TL322X/TL323X/TL721X ([#802](https://github.com/telink-semi/tl_zephyr/pull/802)) | TL521X now uses its own retention option; the tlx hwinfo driver is not built for TL521X | Select the TL521X retention option as needed; no change for TL321X/TL322X/TL323X/TL721X |

### Upstream Inherited Changes

This release inherits all upstream Zephyr v4.1.0 API and Kconfig changes.

For the complete list, see the upstream [Zephyr v4.1.0 Release Notes — API Changes](https://docs.zephyrproject.org/latest/releases/release-notes-4.1.html#api-changes) and the [Migration Guide](https://docs.zephyrproject.org/latest/releases/migration-guide-4.1.html).

***

## 📦 Updates / Dependencies

| Component                 | Repository                                                   | Commit                                                       | Notes                                                        |
| ------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **Telink BLE SDK**        | [telink-semi/tl\_ble\_sdk\_zephyr](https://github.com/telink-semi/tl_ble_sdk_zephyr) | [`de6f125`](https://github.com/telink-semi/tl_ble_sdk_zephyr/commit/de6f125c8ae6d29c4a640ab3526b40ff8c3f0a20) | Required by TL521X; fetch via `./hal_v2/fetch_sdk.sh` (hal\_v2) or `west blobs fetch hal_telink` (hal\_v1) |
| **Telink HAL Zephyr**     | [telink-semi/hal\_telink](https://github.com/telink-semi/hal_telink) | [`bd870dc`](https://github.com/telink-semi/hal_telink/commit/bd870dc273989756f908077761a5e3adbd7d108f) | Contains both hal\_v1 and hal\_v2 sources                    |
| **MCUBoot**               | [telink-semi/tl\_mcuboot](https://github.com/telink-semi/tl_mcuboot) | [`ce0da85`](https://github.com/telink-semi/tl_mcuboot/commit/ce0da85c39c749df49b0ec62b33d2ecdea24c927) | Bootloader; required for OTA/DFU                             |
| **OpenThread Telink**     | [telink-semi/tl\_openthread](https://github.com/telink-semi/tl_openthread) | [`542aaab`](https://github.com/telink-semi/tl_openthread/commit/542aaab44e1308e1a8a24573dfbd413fade342ee) | OpenThread source adapted for Telink                         |
| **OpenThread Telink Lib** | [telink-semi/tl\_openthread\_libs](https://github.com/telink-semi/tl_openthread_libs) | [`f69c186`](https://github.com/telink-semi/tl_openthread_libs/commit/f69c186d65a41259480e87ccf9d2a7f665249778) | Pre-built OpenThread library for Telink                      |
| **Telink XZ (LZMA)**      | [telink-semi/tl\_xz](https://github.com/telink-semi/tl_xz) | [`831f338`](https://github.com/telink-semi/tl_xz/commit/831f338fd6784661d3bec62fd01060ee4d7d373d) | LZMA compression library module (modules/lib/lzma)           |

### Telink HAL Zephyr (hal\_telink) `tl_v1.4.0-v4.0.4.8`

The HAL module is pinned at tag `tl_v1.4.0-v4.0.4.8` (commit [`bd870dc`](https://github.com/telink-semi/hal_telink/commit/bd870dc273989756f908077761a5e3adbd7d108f)). Compared with the previous release tag (`tl_v1.3.0-v4.0.4.8`), it includes the following changes:

| Change | Description |
| ------ | ----------- |
| Support the new `tlx` SoC family ([#203](https://github.com/telink-semi/hal_telink/pull/203)) | Adapt the CMake build system for `CONFIG_SOC_FAMILY_TELINK_TLX`: `SOC_FAMILY` is set to `tlx` so the controller driver path resolves correctly; update the mbedtls ECP acceleration and IEEE802.15.4 include conditionals; keep the legacy TLX Kconfig symbols working for backward compatibility |
| Role-based BLE controller library variants ([#205](https://github.com/telink-semi/hal_telink/pull/205)) | Replace the `COMPILE_TL_LIB_GENERAL` option with the `TL_BLE_CTRL_VARIANT` Kconfig choice (`TL_BLE_CTRL_PERIPHERAL` / `TL_BLE_CTRL_CENTRAL` / `TL_BLE_CTRL_MULTIROLE`), auto-selected from the BLE role configs; controller library names change from SoC-based (`_dual_core` / `_single_core` / `_general` / `_concurrent`) to role-based (`_peripheral` / `_central` / `_multirole`) |
| Dynamic suspend-exit latency ([#205](https://github.com/telink-semi/hal_telink/pull/205)) | Remove the per-SoC hardcoded `SUSPEND_EXIT_LATENCY_US` macros from `tlx_bt_init.c`; the BLE library now records the suspend-exit start tick (`blc_ll_get_suspend_exit_start_tick`) for more accurate runtime latency handling |
| Driver source sync ([#202](https://github.com/telink-semi/hal_telink/pull/202)) | Sync the hal_v2 wrapper with the latest Telink BLE SDK driver source changes (e.g. RF power control in `tl_rf_power.c`) |

### Telink Zephyr OpenThread (tl_openthread) `tl_v1.4.0-v1.4`

The Telink OpenThread module for Zephyr, not a mirror of the offical OpenThread repository. The OpenThread module is pinned at tag `tl_v1.4.0-v1.4` (commit [`542aaab`](https://github.com/telink-semi/tl_openthread/commit/542aaab44e1308e1a8a24573dfbd413fade342ee)). For Zephyr 4.1, the corresponding OpenThread module is commit [`3ae741f`](https://github.com/telink-semi/tl_openthread/commit/3ae741f95e7dfb391dec35c48742862049eb62e8).Relative to the base version, this release tag includes the following changes:

| Change | Description |
| ------ | ----------- |
| Add openthread sed code into ramcode ([#2](https://github.com/telink-semi/tl_openthread/pull/2)) | Put OpenThread SED proc call-tree into ramcode on chips with enough RAM (e.g., `TL721X`) to reduce wake‑up runtime and lower power consumption. This only applies when `CONFIG_PM` is enabled. |

***

## ⚠️ Known Issues and Limitations

- No known issues for the TL521X platform in this release.

***

## 📌 Important Notes

- Use `west update` to automatically pull `telink_ble_sdk` along with other modules.
- Alternatively, manually fetch `telink_ble_sdk` by running `./hal_v2/fetch_sdk.sh {REPO_URL} {COMMIT_HASH}` inside `modules/hal/telink/` to pull or update the BLE stack to the version pinned by this release, where `{COMMIT_HASH}` is the `telink_ble_sdk` revision in `west.yml`. For this release, that is: `./hal_v2/fetch_sdk.sh https://github.com/telink-semi/tl_ble_sdk_zephyr.git de6f125c8ae6d29c4a640ab3526b40ff8c3f0a20`
- For full environment setup instructions, refer to the [Telink Zephyr SDK Getting Started Guide](../getting_started/index.md) or the [Telink Matter Developer Guide](https://doc.telink-semi.cn/doc/en/software/res/sdk/matter/telink_matter_developer_guide_en/).

***

## 📊 Resource Usage (Code Size)

This section shows the RAM and ROM usage for various Zephyr samples on Telink platforms.

**Build environment:**

| Property         | Value                                                                              |
| ---------------- | ---------------------------------------------------------------------------------- |
| Zephyr SDK       | 0.17.0                                                                             |
| Toolchain        | `riscv64-zephyr-elf`                                                               |
| Extra CMake flag | `-DCONFIG_COMPILER_WARNINGS_AS_ERRORS=y`                                           |
| Debug logging    | Enabled (per sample default)                                                       |
| Reproduce        | `west build -p auto -b <board> <sample> -- -DCONFIG_COMPILER_WARNINGS_AS_ERRORS=y` |

> **Note:** The numbers below are from CI builds with the configuration above.

### Supported Boards

| Board   | Chip Family |
| ------- | ----------- |
| tl5218x | TL521X      |

***

### Zephyr Samples Support Matrix

The table below summarizes build and test status for Zephyr samples in this
release. Samples are located under `samples/` in the Zephyr tree (tests are
under `tests/`).

> ✅ = Supported and Tested &nbsp;&nbsp; 🟡 = Supported but Untested
> &nbsp;&nbsp; (builds successfully, not functionally validated)
> &nbsp;&nbsp; · = Untested (not built or not applicable)

| Sample                       | TL521X |
| ---------------------------- | :----: |
| **basic/blinky**             |   ✅    |
| **basic/button**             |   ✅    |
| **basic/fade\_led**          |   ✅    |
| **hello\_world**             |   ✅    |
| **bluetooth/peripheral\_ht** |   ✅    |
| **net/openthread/cli**       |   ✅    |
| **net/sockets/echo\_client** |   ✅    |
| **net/sockets/echo\_server** |   ·    |
| **crypto/mbedtls**           |   ✅    |
| **drivers/adc/adc\_dt**      |   ✅    |
| **drivers/spi\_flash**       |   ·    |
| **drivers/watchdog**         |   ✅    |
| **sensor/sht3xd**            |   ✅    |

> **Notes on Sample Support**
>
> - **Tested combinations (✅):** All samples listed as ✅ have been built and functionally validated on TL521X.
> - **Supported but untested (🟡):** None in this release.
> - **Untested (·):** Combinations marked · are not built in this release, either because the sample is not applicable to the TL521X chip family or because the build failed.

***

### TL521X (tl5218x)

📈 **Resource Usage Details**

| Sample                               | RAM\_ILM\_N                | ROM                       | RAM                         |
| ------------------------------------ | -------------------------- | ------------------------- | --------------------------- |
| **samples/basic/blinky**             | 5186 B (3.96% of 128 KB)   | 31186 B (2.97% of 1 MB)   | 19200 B (14.65% of 128 KB)  |
| **samples/basic/button**             | 5186 B (3.96% of 128 KB)   | 31826 B (3.04% of 1 MB)   | 19216 B (14.66% of 128 KB)  |
| **samples/basic/fade\_led**          | 5186 B (3.96% of 128 KB)   | 45186 B (4.31% of 1 MB)   | 22680 B (17.30% of 128 KB)  |
| **samples/bluetooth/peripheral\_ht** | 18572 B (14.17% of 128 KB) | 203664 B (19.42% of 1 MB) | 41908 B (31.97% of 128 KB)  |
| **samples/crypto/mbedtls**           | 5186 B (3.96% of 128 KB)   | 131526 B (12.54% of 1 MB) | 25908 B (19.77% of 128 KB)  |
| **samples/drivers/adc/adc\_dt**      | 5186 B (3.96% of 128 KB)   | 41422 B (3.95% of 1 MB)   | 20084 B (15.32% of 128 KB)  |
| **samples/drivers/watchdog**         | 5186 B (3.96% of 128 KB)   | 39542 B (3.77% of 1 MB)   | 19272 B (14.70% of 128 KB)  |
| **samples/hello\_world**             | 5186 B (3.96% of 128 KB)   | 30710 B (2.93% of 1 MB)   | 19200 B (14.65% of 128 KB)  |
| **samples/net/openthread/cli**       | 12034 B (9.18% of 128 KB)  | 571700 B (54.52% of 1 MB) | 104912 B (80.04% of 128 KB) |
| **samples/net/sockets/echo\_client** | 19434 B (14.83% of 128 KB) | 267072 B (25.47% of 1 MB) | 59408 B (45.32% of 128 KB)  |
| **samples/sensor/sht3xd**            | 5262 B (4.01% of 128 KB)   | 44794 B (4.27% of 1 MB)   | 19244 B (14.68% of 128 KB)  |

***

### 📝 Additional Notes

- **Memory Regions:** May vary between chip variants; check individual board configurations
- **Full CI Data:** For complete resource usage information across all samples (including Bluetooth, OpenThread, and MCUBoot), refer to CI build artifacts
- **Production Optimizations:** For production builds, disable debug logging and enable appropriate optimizations to reduce RAM/ROM usage
- **Bluetooth & OpenThread:** For Bluetooth LE and OpenThread-specific resource usage, see the respective CI workflow files in `.github/workflows/`
- **Build Config:** All builds use `-DCONFIG_COMPILER_WARNINGS_AS_ERRORS=y` as in the CI pipelines

***

## 🔗 Resources

Made by Telink Semiconductor

- [Website](https://www.telink-semi.com/)
- [Forum](https://forum.telink-semi.cn/)
- [Documentation](https://doc.telink-semi.cn/)
- [Telink Zephyr SDK Getting Started Guide](../getting_started/index.md)
- [Zephyr v4.1.0 Release Notes](https://docs.zephyrproject.org/latest/releases/release-notes-4.1.html)
- [Zephyr v4.1.0 Migration Guide](https://docs.zephyrproject.org/latest/migration/migration-4.1.html)
