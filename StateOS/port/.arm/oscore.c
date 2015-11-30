/******************************************************************************

    @file    State Machine OS: oscore.c
    @author  Rajmund Szymanski
    @date    29.10.2015
    @brief   StateOS port file for ARM Cotrex-M uC.

 ******************************************************************************

    StateOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of StateOS distribution.

    StateOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    StateOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <oskernel.h>
#include <stddef.h>

#ifdef  __CC_ARM
#if     OS_ROBIN

/* -------------------------------------------------------------------------- */

__asm void PendSV_Handler( void )
{
	PRESERVE8
	IMPORT System
	IMPORT core_tsk_handler

	ldr   r0,   =System
	ldr   r0,  [ r0, #__cpp(offsetof(sys_t, cur)) ]
	mrs   r1,    PSP
#if __FPU_USED
	tst   lr,   #16                     ; fpu used?
	it    eq
 vstmdbeq r1!, { s16 - s31 }
#endif
#if __CORTEX_M < 3
	subs  r1,   #36
	stm   r1!, { r4  - r7 }
	mov   r3,    r8
	mov   r4,    r9
	mov   r5,    r10
	mov   r6,    r11
	mov   r7,    lr
	stm   r1!, { r3  - r7 }
	subs  r1,   #36
#else
	stmdb r1!, { r4  - r11, lr }
#endif
	str   r1,  [ r0, #__cpp(offsetof(tsk_t, sp)) ]
	bl    core_tsk_handler
	ldr   r1,  [ r0, #__cpp(offsetof(tsk_t, sp)) ]
#if __CORTEX_M < 3
	adds  r1,   #16
	ldm   r1!, { r3  - r7 }
	mov   r8,    r3
	mov   r9,    r4
	mov   r10,   r5
	mov   r11,   r6
	mov   lr,    r7
	subs  r1,   #36
	ldm   r1!, { r4  - r7 }
	adds  r1,   #20
#else
	ldmia r1!, { r4  - r11, lr }
#endif
#if __FPU_USED
	tst   lr,   #16                     ; fpu used?
	it    eq
 vldmiaeq r1!, { s16 - s31 }
#endif
	msr   PSP,   r1
	bx    lr

	ALIGN
}

/* -------------------------------------------------------------------------- */

#else // OS_ROBIN == 0

/* -------------------------------------------------------------------------- */

__asm void port_ctx_switch( void )
{
	PRESERVE8
	IMPORT System
	IMPORT core_tsk_handler

#if __CORTEX_M < 3
	mov   r0,    r8
	mov   r1,    r9
	mov   r2,    r10
	mov   r3,    r11
	push       { r0  - r3, lr }
	push       { r4  - r7 }
#else
	push       { r4  - r11, lr }
#endif
#if __FPU_USED
	vpush      { s16 - s31 }
#endif
	ldr   r0,   =System
	ldr   r0,  [ r0, #__cpp(offsetof(sys_t, cur)) ]
	mov   r1,    sp
	str   r1,  [ r0, #__cpp(offsetof(tsk_t, sp)) ]
	bl    core_tsk_handler
	ldr   r1,  [ r0, #__cpp(offsetof(tsk_t, sp)) ]
	mov   sp,    r1
#if __FPU_USED
	vpop       { s16 - s31 }
#endif
#if __CORTEX_M < 3
	pop        { r4  - r7 }
	pop        { r0  - r3 }
	mov   r8,    r0
	mov   r9,    r1
	mov   r10,   r2
	mov   r11,   r3
	pop        { pc }
#else
	pop        { r4  - r11, pc }
#endif

	ALIGN
}

/* -------------------------------------------------------------------------- */

#endif // OS_ROBIN

/* -------------------------------------------------------------------------- */

__attribute__(( noreturn ))
__asm void port_tsk_start( void )
{
	PRESERVE8
	IMPORT System
	IMPORT core_ctx_switch
	EXPORT port_tsk_break

priv_tsk_start

	ldr   r0,   =System
	ldr   r0,  [ r0, #__cpp(offsetof(sys_t, cur)) ]
	ldr   r1,  [ r0, #__cpp(offsetof(tsk_t, top)) ]
	mov   sp,    r1
#if OS_LOCK_LEVEL
#if OS_LOCK_LEVEL == 1
	cpsie i
#else
	movs  r3,   #0
	msr   BASEPRI, r3
#endif
#endif
	ldr   r3,  [ r0, #__cpp(offsetof(tsk_t, state)) ]
	blx   r3

port_tsk_break

	bl    core_ctx_switch
	b     priv_tsk_start

	ALIGN
}

/* -------------------------------------------------------------------------- */

#endif // __CC_ARM