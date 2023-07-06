/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Santiago Pagani <santiagopagani@gmail.com>
 *
 * Copyright (c) 2020, NEC Laboratories Europe GmbH, NEC Corporation.
 *                     All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THIS HEADER MAY NOT BE EXTRACTED OR MODIFIED IN ANY WAY.
 */

#include <uk/plat/bootstrap.h>
#include <uk/plat/time.h>
#include <uk/plat/common/cpu.h>
#if CONFIG_RASPI_LCD
	#include <raspi/lfb.h>
#endif
#if CONFIG_RASPI_TOUCHSCREEN
	#include <raspi/touchscreen.h>
#endif
#include <raspi/console.h>
#include <raspi/time.h>
#include <uk/print.h>
#include <uk/arch/types.h>
#include <uk/assert.h>
#include <uk/errptr.h>
#include <uk/config.h>

//smcc_psci_callfn_t smcc_psci_call;

static uint64_t assembly_entry;
static uint64_t hardware_init_done;

uint64_t _libraspiplat_get_reset_time(void)
{
	return assembly_entry;
}

uint64_t _libraspiplat_get_hardware_init_time(void)
{
	return hardware_init_done;
}

void _libraspiplat_entry(uint64_t low0, uint64_t hi0, uint64_t low1, uint64_t hi1)
{
	if (hi0 == hi1) {
		assembly_entry = ((hi0 << 32)&0xFFFFFFFF00000000) | (low0&0xFFFFFFFF);
	} else {
		assembly_entry = ((hi1 << 32)&0xFFFFFFFF00000000) | (low1&0xFFFFFFFF);
	}

    _libraspiplat_init_console();

	#if CONFIG_RASPI_LCD
    	lfb_init();
	#endif

	#if CONFIG_RASPI_TOUCHSCREEN
		touchscreen_init();
	#endif
	
	hardware_init_done = get_system_timer();

	/*
	 * Enter Unikraft
	 */
	ukplat_entry(0, 0);
}
