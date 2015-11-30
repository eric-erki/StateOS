/******************************************************************************

    @file    State Machine OS: osconfig.h
    @author  Rajmund Szymanski
    @date    29.10.2015
    @brief   StateOS config file for STM32F4 uC.

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

#pragma once

#include <stm32f4xx.h>

// ----------------------------
// cpu frequency in Hz
// default value: 168000000
#define CPU_FREQUENCY 168000000

// ----------------------------
// os frequency in Hz
// OS_TIMER == 0 (SysTick) => dafault value:    1000
// OS_TIMER >  0 (TIM2, 5) => dafault value: 1000000 (tickless mode)
#define  OS_FREQUENCY   1000000

// ----------------------------
// system timer
// 0:SysTick, 2:TIM2, 5:TIM5
// default value: 0 (SysTick)
#define  OS_TIMER             2

// ----------------------------
// system mode, round-robin frequency in Hz
// OS_ROBIN == 0 => os works in cooperative mode
// OS_ROBIN >  0 => os works in preemptive mode, OS_ROBIN indicates round-robin frequency
// default value: 0
#define  OS_ROBIN          1000

// ----------------------------
// critical sections protection level
// OS_LOCK_LEVEL == 0 => there is no protection of critical sections (do not use system functions in interrupt procedures)
// OS_LOCK_LEVEL == 1 => entrance to a critical section blocks all interrupts
// OS_LOCK_LEVEL >  1 => (CM3 and above) entrance to a critical section blocks interrupts with priority lower (the priority value greater) than OS_LOCK_LEVEL
// OS_ROBIN      >  0 => required value: 1 (critical sections must be protected)
// else               => default  value: 0
#define  OS_LOCK_LEVEL        1

// ----------------------------
// os heap size in bytes
// OS_HEAP_SIZE == 0 => functions 'xxx_create' use of a simplified procedure memory allocation on the heap that is not compatible with 'calloc'
// OS_HEAP_SIZE == 1 => functions 'xxx_create' use 'calloc' provided with the compiler libraries
// OS_HEAP_SIZE >  1 => functions 'xxx_create' allocate memory on a dedicated system heap, OS_HEAP_SIZE indicates size of the heap
// default value: 0
#define  OS_HEAP_SIZE         0

// ----------------------------
// default task stack size in bytes
// default value: 256
#define  OS_STACK_SIZE      256