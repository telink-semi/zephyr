/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bootutil/bootutil.h"
#include "bootutil_priv.h"
#include "bootutil/bootutil_log.h"

BOOT_LOG_MODULE_DECLARE(mcuboot);

// Modified version of the original function with the check for equal slot sizes removed.
/*
 * Slots are compatible when all sectors that store up to to size of the image
 * round up to sector size, in both slot's are able to fit in the scratch
 * area, and have sizes that are a multiple of each other (powers of two
 * presumably!).
 */
int
__wrap_boot_slots_compatible(struct boot_loader_state *state)
{
    size_t num_sectors_primary;
    size_t num_sectors_secondary;
    size_t sz0, sz1;
    size_t primary_slot_sz, secondary_slot_sz;
#ifndef MCUBOOT_OVERWRITE_ONLY
    size_t scratch_sz;
#endif
    size_t i, j;
    int8_t smaller;

    num_sectors_primary = boot_img_num_sectors(state, BOOT_PRIMARY_SLOT);
    num_sectors_secondary = boot_img_num_sectors(state, BOOT_SECONDARY_SLOT);
    if ((num_sectors_primary > BOOT_MAX_IMG_SECTORS) ||
        (num_sectors_secondary > BOOT_MAX_IMG_SECTORS)) {
        BOOT_LOG_WRN("Cannot upgrade: more sectors than allowed");
        return 0;
    }

#ifndef MCUBOOT_OVERWRITE_ONLY
    scratch_sz = boot_scratch_area_size(state);
#endif

    /*
     * The following loop scans all sectors in a linear fashion, assuring that
     * for each possible sector in each slot, it is able to fit in the other
     * slot's sector or sectors. Slot's should be compatible as long as any
     * number of a slot's sectors are able to fit into another, which only
     * excludes cases where sector sizes are not a multiple of each other.
     */
    i = sz0 = primary_slot_sz = 0;
    j = sz1 = secondary_slot_sz = 0;
    smaller = 0;
    while (i < num_sectors_primary || j < num_sectors_secondary) {
        if (sz0 == sz1) {
            sz0 += boot_img_sector_size(state, BOOT_PRIMARY_SLOT, i);
            sz1 += boot_img_sector_size(state, BOOT_SECONDARY_SLOT, j);
            i++;
            j++;
        } else if (sz0 < sz1) {
            sz0 += boot_img_sector_size(state, BOOT_PRIMARY_SLOT, i);
            /* Guarantee that multiple sectors of the secondary slot
             * fit into the primary slot.
             */
            if (smaller == 2) {
                BOOT_LOG_WRN("Cannot upgrade: slots have non-compatible sectors");
                return 0;
            }
            smaller = 1;
            i++;
        } else {
            sz1 += boot_img_sector_size(state, BOOT_SECONDARY_SLOT, j);
            /* Guarantee that multiple sectors of the primary slot
             * fit into the secondary slot.
             */
            if (smaller == 1) {
                BOOT_LOG_WRN("Cannot upgrade: slots have non-compatible sectors");
                return 0;
            }
            smaller = 2;
            j++;
        }
#ifndef MCUBOOT_OVERWRITE_ONLY
        if (sz0 == sz1) {
            primary_slot_sz += sz0;
            secondary_slot_sz += sz1;
            /* Scratch has to fit each swap operation to the size of the larger
             * sector among the primary slot and the secondary slot.
             */
            if (sz0 > scratch_sz || sz1 > scratch_sz) {
                BOOT_LOG_WRN("Cannot upgrade: not all sectors fit inside scratch");
                return 0;
            }
            smaller = sz0 = sz1 = 0;
        }
#endif
    }

    return 1;
}
