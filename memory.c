/* SPDX-License-Identifier: ISC */
/* Copyright (c) 2015, IBM
 *           (c) 2020, NEC Laboratories Europe GmbH, NEC Corporation.
 * Author(s): Dan Williams <djwillia@us.ibm.com>
 *            Simon Kuenzer <simon.kuenzer@neclab.eu>
 *            Santiago Pagani <santiagopagani@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <uk/plat/memory.h>
#include <uk/plat/common/sections.h>
#include <uk/essentials.h>
#include <raspi/sysregs.h>
#include <stddef.h>

int ukplat_memregion_count(void)
{
	return 7;
}

int ukplat_memregion_get(int i, struct ukplat_memregion_desc **m)
{
	static struct ukplat_memregion_desc mrd[7];

	UK_ASSERT(m);

	switch (i) {
	case 0: /* stack */
		mrd[i].pbase = 0;
		mrd[i].vbase = 0;
		mrd[i].len =   __TEXT;
		mrd[i].type = UKPLAT_MEMRT_STACK ;
		mrd[i].flags = UKPLAT_MEMRF_READ | UKPLAT_MEMRF_WRITE ;
#if CONFIG_UKPLAT_MEMRNAME
		strncpy(mrd[i].name,"stack",sizeof(mrd[i].name)-1);
#endif
		*m = &mrd[i];
		break;
	case 1: /* text */
		mrd[i].pbase = __TEXT;
		mrd[i].vbase = __TEXT;
		mrd[i].len = __ETEXT - __TEXT;
		mrd[i].type = UKPLAT_MEMRT_KERNEL;
		mrd[i].flags = UKPLAT_MEMRF_READ;
#if CONFIG_UKPLAT_MEMRNAME
		strncpy(mrd[i].name,"text",sizeof(mrd[i].name)-1);
#endif
		*m = &mrd[i];
		break;
	case 2: /* rodata */
		mrd[i].pbase = __RODATA;
		mrd[i].vbase = __RODATA;
		mrd[i].len = __ERODATA - __RODATA;
		mrd[i].type = UKPLAT_MEMRT_KERNEL;
		mrd[i].flags = UKPLAT_MEMRF_READ;
#if CONFIG_UKPLAT_MEMRNAME
		strncpy(mrd[i].name, "rodata", sizeof(mrd[i].name) - 1);
#endif
		*m = &mrd[i];
		break;
	case 3: /* ctors */
		mrd[i].pbase = __CTORS;
		mrd[i].vbase = __CTORS;
		mrd[i].len = __ECTORS - __CTORS;
		mrd[i].type = UKPLAT_MEMRT_KERNEL;
		mrd[i].flags = UKPLAT_MEMRF_READ;
#if CONFIG_UKPLAT_MEMRNAME
		strncpy(mrd[i].name, "ctors", sizeof(mrd[i].name) - 1);
#endif
		*m = &mrd[i];
		break;
	case 4: /* data */
		mrd[i].pbase = __DATA;
		mrd[i].vbase = __DATA;
		mrd[i].len = __EDATA - __DATA;
		mrd[i].type = UKPLAT_MEMRT_KERNEL;
		mrd[i].flags = UKPLAT_MEMRF_READ | UKPLAT_MEMRF_WRITE;
#if CONFIG_UKPLAT_MEMRNAME
		strncpy(mrd[i].name, "data", sizeof(mrd[i].name) - 1);
#endif
		*m = &mrd[i];
		break;
	case 5: /* bss */
		mrd[i].pbase = __BSS_START;
		mrd[i].vbase = __BSS_START;
		mrd[i].len = __END - __BSS_START;
		mrd[i].type = UKPLAT_MEMRT_KERNEL;
		mrd[i].flags = UKPLAT_MEMRF_READ | UKPLAT_MEMRF_WRITE;
#if CONFIG_UKPLAT_MEMRNAME
		strncpy(mrd[i].name, "bss", sizeof(mrd[i].name) - 1);
#endif
		*m = &mrd[i];
		break;
	case 6: /* heap */
		mrd[i].pbase = __END;
		mrd[i].vbase = __END;
		mrd[i].len = (MMIO_BASE/2 - 1) - __END;
		mrd[i].type = UKPLAT_MEMRT_FREE;
		mrd[i].flags = 0;
#if CONFIG_UKPLAT_MEMRNAME
		strncpy(mrd[i].name, "heap", sizeof(mrd[i].name) - 1);
#endif
		*m = &mrd[i];
		break;
	default:
		if (i < 0 || i >= ukplat_memregion_count())
			return -1;

		*m = &mrd[i - 7];
		break;
	}

	return 0;
}

int _ukplat_mem_mappings_init(void)
{
	return 0;
}

void ukplat_stack_set_current_thread(void *thread_addr __unused)
{
}
