/*
 *OpenTeachProject
 *Copyright (C) 2021  OpenTeachProject <openteachproject@gmail.com>
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



/*
 * port.c
 *
 *  Created on: Aug 27, 2021
 *      Author: salar
 */
#include <rtos/include/port.h>
#include <rtos/include/kernel.h>
#include <ARMCM7.h>



static void                             _svcPendsvSetup(void);
static void                             _svcCall(void);
static void                             _setPendsv(void);
static bool                             _isIrqMode(void);
static bool                             _isIrqMasked(void);
static uint32_t                         _xpsrInitialValue(void);



_rtosStatus_t _portDeviceInitialize(void) {

    _svcPendsvSetup();
    return StatusOk;
}

_rtosStatus_t _portTickTimerStart(void) {

    SysTick_Config(16000000 / 1000);
    NVIC_SetPriority(SysTick_IRQn, 0);
    return StatusOk;
}

_rtosStatus_t _portThreadStackInitialize(_threadId_t threadId) {

    _threadControlBlock_t               *thread;
    _threadStackPointer_t               threadStackPointer;
    _threadStackPointer_t               threadStackTop;
    _threadFunctionPointer_t            threadFunctionPointer;
    _threadFunctionParameter_t          threadFunctionParameter;



    thread = (_threadControlBlock_t*)threadId;
    threadStackPointer = thread -> stackPointer;
    threadStackTop = thread -> stackTop;
    threadFunctionPointer = thread -> functionPointer;
    threadFunctionParameter = thread -> functionParameter;



    *(uint32_t*)(threadStackTop - 4) = _xpsrInitialValue();
    *(uint32_t*)(threadStackTop - 8) = (uint32_t)threadFunctionPointer;
    *(uint32_t*)(threadStackTop - 32) = (uint32_t)threadFunctionParameter;
    threadStackPointer = threadStackPointer - 64;


    thread -> stackPointer = threadStackPointer;



    return StatusOk;
}

_bool_t _portIsInInterrupt(void) {

    return (_isIrqMode() | _isIrqMasked());
}

_rtosStatus_t _portStartFirstThread(void) {

    __ASM (

            "BL             _kernelGetCurrentRunningThreadStackPointer          \n\t"
            "MOV            R3, R0                                              \n\t"
            "ADDS           R3, #64                                             \n\t"
            "MSR            PSP, R3                                             \n\t"



            "BL             _kernelGetCurrentRunningThreadFunctionPointer       \n\t"
            "MOV            R4, R0                                              \n\t"



            "BL             _kernelGetCurrentRunningThreadFunctionParameter     \n\t"
            "MOV            R5, R0                                              \n\t"



            "MRS            R3, CONTROL                                         \n\t"
            "ORR            R3, #3                                              \n\t"
            "MSR            CONTROL, R3                                         \n\t"



            "MOV            R0, R5                                              \n\t"
            "MOV            R3, R4                                              \n\t"
            "BX             R3                                                  \n\t"
            );


    
    return StatusOk;
}

_rtosStatus_t _portSupervisorInterruptTrigger(void) {

    _svcCall();
    return StatusOk;
}

_rtosStatus_t _portPreSupervisorInterruptTrigger(void) {

    _setPendsv();
    return StatusOk;
}

_atomicValue_t _portAtomicRead32(_atomicAddress_t address) {

    return __LDREXW(address);
}

_atomicResult_t _portAtomicWrite32(_atomicValue_t value, _atomicAddress_t address) {

    return __STREXW(value, address);
}

_rtosStatus_t _portAtomicClear(void) {

    __CLREX();
    return StatusOk;
}

void _svcPendsvSetup(void) {

    uint32_t                            svcPriority;
    uint32_t                            pendSvPriority;
    uint32_t                            priorityGrouping = NVIC_GetPriorityGrouping();

    if (priorityGrouping >= __NVIC_PRIO_BITS){
        pendSvPriority = (1 << (7 - priorityGrouping)) - 1;
        svcPriority = pendSvPriority - 1;
    }
    else {
        pendSvPriority = (1 << (8 - __NVIC_PRIO_BITS)) - 1;
        svcPriority = pendSvPriority - 1;
    }

    svcPriority = NVIC_EncodePriority(priorityGrouping, svcPriority, 0);
    pendSvPriority = NVIC_EncodePriority(priorityGrouping, pendSvPriority, 0);

    NVIC_SetPriority(SVCall_IRQn, svcPriority);
    NVIC_SetPriority(PendSV_IRQn, pendSvPriority);
}

void _setPendsv(void) {

    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

void _svcCall(void) {

    __ASM("SVC    0");
}

bool _isIrqMode(void) {

    return (__get_IPSR() != 0U);
}

bool _isIrqMasked(void) {

    return ((__get_PRIMASK() != 0U) || (__get_BASEPRI() != 0U));
}

uint32_t _xpsrInitialValue(void) {

    return (0x01000000U);
}
