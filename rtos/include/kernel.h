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
 * kernel.h
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */



#ifndef RTOS_INCLUDE_KERNEL_H_
#define RTOS_INCLUDE_KERNEL_H_



#include <rtos/include/rtosTypeDefinitions.h>



_kernelControlBlock_t                  *_kernelControlBlock;



_kernelControlBlock_t                  *_kernelGetKernelControlBlock(void);

_status_t                               _kernelSystemCall(_kernelSystemCallArg_t *systemCallArg);
_status_t                               _kernelSystemCallHandler(void);
_status_t                               _kernelSystemCallByNumber(_kernelSystemCallArg_t *systemCallArg);
_status_t                               _kernelContextSwitch(void);
_status_t                               _kernelWaitListHandler(void);

_status_t                               _kernelInitialize(void);
_status_t                               _kernelStart(void);

_bool_t                                 _kernelCheckForContextSwitchAfterInsert(_threadId_t threadId);
_bool_t                                 _kernelCheckForContextSwitchAfterDelete(_threadId_t threadId);


_kernelVersion_t                        _kernelGetVersion(void);
_status_t                               _kernelSetVersion(_kernelVersion_t version);

_kernelState_t                          _kernelGetState(void);
_status_t                               _kernelSetState(_kernelState_t state);

_kernelTick_t                           _kernelGetTick(void);
_status_t                               _kernelSetTick(_kernelTick_t tick);
_status_t                               _kernelIncreaseTick(void);

_threadId_t                             _kernelGetCurrentRunningThreadId(void);
_status_t                               _kernelSetCurrentRunningThreadId(_threadId_t threadId);

_threadStackPointer_t                   _kernelGetCurrentRunningThreadStackPointer(void);
_status_t                               _kernelSetCurrentRunningThreadStackPointer(_threadStackPointer_t stackPointer);

_threadFunctionPointer_t                _kernelGetCurrentRunningThreadFunctionPointer(void);
_threadFunctionParameter_t              _kernelGetCurrentRunningThreadFunctionParameter(void);

_status_t                               _kernelSystemCallListInitialize(void);
_status_t                               _kernelReadyListInitialize(void);
_status_t                               _kernelWaitListInitialize(void);
_status_t                               _kernelSuspendedListInitialize(void);
_status_t                               _kernelTerminatedListInitialize(void);

_status_t                               _kernelSystemCallRequest(void);
_status_t                               _kernelContextSwitchRequest(void);
_status_t                               _kernelWaitListHandlerRequest(void);

_kernelRequest_t                        _kernelGetSystemCallRequest(void);
_status_t                               _kernelSetSystemCallRequest(void);
_status_t                               _kernelUnsetSystemCallRequest(void);

_kernelRequest_t                        _kernelGetContextSwitchRequest(void);
_status_t                               _kernelSetContextSwitchRequest(void);
_status_t                               _kernelUnsetContextSwitchReuest(void);

_kernelRequest_t                        _kernelGetWaitListHandlerRequest(void);
_status_t                               _kernelSetWaitListHandlerRequest(void);
_status_t                               _kernelUnsetWaitListHandlerReuest(void);

_bool_t                                 _kernelIsInInterrupt(void);

_status_t                               _kernelThreadStackInitialize(_threadId_t threadId);

_atomicValue_t                          _kernelAtomicRead32(_atomicAddress_t address);
_atomicResult_t                         _kernelAtomicWrite32(_atomicValue_t value, _atomicAddress_t address);
_status_t                               _kernelAtomicClear(void);



#endif /* RTOS_INCLUDE_KERNEL_H_ */
