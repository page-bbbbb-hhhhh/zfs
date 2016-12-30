/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2016, Brendon Humphrey (brendon.humphrey@mac.com). All rights reserved.
 */

#include <libnvpair.h>
#include <libdiskmgt.h>
#include "disks_private.h"

int
inuse_corestorage(char *slice, nvlist_t *attrs, int *errp)
{
	struct DU_CS_Info info;
	int in_use = 0;

	init_diskutil_cs_info(&info);	
	get_diskutil_cs_info(slice, &info);

	if (diskutil_cs_info_valid(&info)) {
		if (is_cs_disk(&info)) {
			// slice is a physical cs device
			if (is_physical_volume(&info)) {
				libdiskmgt_add_str(attrs, DM_USED_BY,
				    DM_USE_CORESTORAGE_PV, errp);
				libdiskmgt_add_str(attrs, DM_USED_NAME,
				    slice, errp);
				in_use = 1;
			}

			// slice is a logical cs device, conditions precluding use:
			// -- converting
			// -- locked
			// -- !Online
			if (is_logical_volume(&info)) {
				if (is_locked(&info)) {
					libdiskmgt_add_str(attrs, DM_USED_BY,
					    DM_USE_CORESTORAGE_LOCKED_LV, errp);
					libdiskmgt_add_str(attrs, DM_USED_NAME,
					    slice, errp);
					in_use = 1;
				} else if (!is_converted(&info)) {
					char *lv_status = get_LV_status(&info);
					libdiskmgt_add_str(attrs, DM_USED_BY,
					    DM_USE_CORESTORAGE_CONVERTING_LV, errp);
					libdiskmgt_add_str(attrs, DM_USED_NAME,
					    lv_status, errp);
					free(lv_status);
					in_use = 1;
				} else if (!is_online(&info)) {
					char *lv_status = get_LV_status(&info);
					libdiskmgt_add_str(attrs, DM_USED_BY,
					    DM_USE_CORESTORAGE_OFFLINE_LV, errp);
					libdiskmgt_add_str(attrs, DM_USED_NAME,
					    lv_status, errp);
					free(lv_status);
					in_use = 1;
				}			
			}
		}
	}
	
	destroy_diskutil_cs_info(&info);
		
	return (in_use);
}
