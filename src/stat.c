/*
 * This file is part of the openfat project.
 *
 * Copyright (C) 2011  Department of Physics, University of Otago
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Get status of open file
 */
#include "openfat.h"

#include <string.h>
#include <sys/stat.h>

#include "blockdev.h"
#include "fat_core.h"
#include "direntry.h"

int fat_file_stat(struct fat_file_handle *h, struct stat *st)
{
	struct fat_sdirent *fatent;

	memset(st, 0, sizeof(*st));

	if(h->dirent_sector == 0) {
		/* Root directory */
		st->st_mode = S_IFDIR;
		return 0;
	}

	/* Read direntry sector */
	block_read_sectors(h->fat->dev, h->dirent_sector, 1, _fat_sector_buf);
	fatent = (void*)&_fat_sector_buf[h->dirent_offset];
	
	/* TODO: Fill in timestamps */

	if(fatent->attr & FAT_ATTR_DIRECTORY) {
		st->st_mode = S_IFDIR;
	} else {
		st->st_size = __get_le32(&fatent->size);
	}

	return 0;
}
