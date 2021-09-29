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
 * kernel.c
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */



#include <rtos/include/kernel.h>
#include <rtos/include/systemCallTypeDefintions.h>
#include <rtos/include/list.h>
#include <rtos/include/port.h>



_kernelControlBlock_t* _kernelGetKernelControlBlock(void) {

    return _kernelControlBlock;
}

_status_t _kernelSystemCall(_kernelSystemCallArg_t *systemCallArg) {

    _kernelSystemCallListIndex_t        index;
    _kernelSystemCallArg_t              **systemCallList;
    _atomicResult_t                     result;
    _kernelControlBlock_t               *kernel;

    kernel = _kernelGetKernelControlBlock();
    systemCallList = kernel -> systemCallList;
    while(true) {
        index = _kernelAtomicRead32(&(kernel -> systemCallListIndex));
        if (systemCallList[index] == NULL) {
            result = _kernelAtomicWrite32((_atomicValue_t)systemCallArg, (_atomicAddress_t)(&systemCallList[index]));
            if (result == AtomicWriteSuccessfull) {
                break;
            }
        }
        else {
            index++;
            _kernelAtomicWrite32(index, &(kernel -> systemCallListIndex));
        }
    }

    _kernelSystemCallRequest();

    return StatusOk;
}

_status_t _kernelSystemCallHandler(void) {

    _kernelControlBlock_t               *kernel;
    _kernelSystemCallArg_t              *systemCallArg;
    _kernelSystemCallArg_t              **systemCallList;
    _kernelSystemCallListIndex_t        systemCallListIndex;
    _kernelSystemCallListIndex_t        index;

    kernel = _kernelGetKernelControlBlock();
    systemCallList = kernel -> systemCallList;
    systemCallListIndex = kernel -> systemCallListIndex;

    for (index = 0 ; index <= systemCallListIndex ; index++) {

        systemCallArg = systemCallList[index];

        if (systemCallArg != NULL) {

            _kernelSystemCallByNumber(systemCallArg);

            systemCallList[index] = NULL;
        }
    }

    kernel -> systemCallListIndex = 0;

    return StatusOk;;
}

_status_t _kernelSystemCallByNumber(_kernelSystemCallArg_t *systemCallArg) {
    
    void                                *specificSystemCallArg;
    _kernelSystemCallNumber_t           systemCallNumber;

    specificSystemCallArg = systemCallArg -> specificSystemCallArg;
    systemCallNumber = systemCallArg -> systemCallNumber;

    switch (systemCallNumber) {

        case ThreadGetNameSystemCallNumber:
            _threadGetNameSystemCall(specificSystemCallArg);
            break;
        case ThreadGetStateSystemCallNumber:
            _threadGetStateSystemCall(specificSystemCallArg);
            break;
        case ThreadGetPrioritySystemCallNumber:
            _threadGetPrioritySystemCall(specificSystemCallArg);
            break;
        case ThreadSetPrioritySystemCallNumber:
            _threadSetPrioritySystemCall(specificSystemCallArg);
            break;
        case ThreadGetStackSizeSystemCallNumber:
            _threadGetStackSizeSystemCall(specificSystemCallArg);
            break;
        case ThreadCreateNewSystemCallNumber:
            _threadCreateNewSystemCall(specificSystemCallArg);
            break;
        case ThreadTerminateSystemCallNumber:
            _threadTerminateSystemCall(specificSystemCallArg);
            break;
        case ThreadYieldSystemCallNumber:
            _threadYieldSystemCall(specificSystemCallArg);
            break;
        case ThreadSuspendSystemCallNumber:
            _threadSuspendSystemCall(specificSystemCallArg);
            break;
        case ThreadResumeSystemCallNumber:
            _threadResumeSystemCall(specificSystemCallArg);
            break;
        case ThreadWaitSystemCallNumber:
            _threadWaitSystemCall(specificSystemCallArg);
            break;



        case SemaphoreGetNameSystemCallNumber:
            _semaphoreGetNameSystemCall(specificSystemCallArg);
            break;
        case SemaphoreCreateNewSystemCallNumber:
            _semaphoreCreateNewSystemCall(specificSystemCallArg);
            break;
        case SemaphoreDeleteSystemCallNumber:
            _semaphoreDeleteSystemCall(specificSystemCallArg);
            break;
        case SemaphoreGetSystemCallNumber:
            _semaphoreGetSystemCall(specificSystemCallArg);
            break;
        case SemaphoreReleaseSystemCallNumber:
            _semaphoreReleaseSystemCall(specificSystemCallArg);
            break;



        default:
            break;
    }
    return StatusOk;;
}

_status_t _kernelContextSwitch(void) {

    _threadControlBlock_t                *currentRunningThread;
    _threadId_t                          currentRunningThreadId;
    _threadControlBlock_t                *nextRunningThread;
    _threadId_t                          nextRunningThreadId;
    _threadPriority_t                    priority;

    currentRunningThreadId = _kernelGetCurrentRunningThreadId();
    currentRunningThread = (_threadControlBlock_t*)currentRunningThreadId;


    /*
     * Check all of ready lists from highest priority to find most prior
     * thread and put it in next running thread.
     * */
    priority = NumberOfPriorityLevels - 1;
    while(priority >= 0) {
        if (_listIsEmptyReadyList(priority) == true) {
            priority--;
        }
        else {
            nextRunningThreadId = _listGetFirstReadyList(priority);
            _listPushIndexReadyList(priority);
            break;
        }
    }


    /*
     * If next running thread is not equal to the current running thread change
     * current running thread in kernel control block and replace with next
     * running thread.
     * */
    if (nextRunningThreadId == currentRunningThreadId) {
        //Context Switch is not needed.
    }
    else {


        /*
         * Sometime current running thread inserted to suspended, wait or
         * terminate list and isn't in ready state and you can't change the
         * state to ready.
         * */
        if (currentRunningThread -> state == ThreadStateRunning) {
            currentRunningThread -> state = ThreadStateReady;
        }
        nextRunningThread = (_threadControlBlock_t*)nextRunningThreadId;
        nextRunningThread -> state = ThreadStateRunning;
        _kernelSetCurrentRunningThreadId(nextRunningThreadId);
    }
    return StatusOk;
}

_status_t _kernelWaitListHandler(void) {

    _kernelWaitListNumber_t             waitListNumber;
    _listSize_t                         listSize;
    _kernelTick_t                       currentTick;
    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadId_t                         nextThreadId;
    _kernelTick_t                       returnTick;

    currentTick = _kernelGetTick();
    waitListNumber = currentTick % NumberOfWaitLists;

    listSize = _listGetSizeWaitList(waitListNumber);

    if (listSize > 0) {

        threadId = _listGetFirstWaitList(waitListNumber);

        for (_listSize_t index = 0 ; index < listSize ; index++) {

            thread = (_threadControlBlock_t*)threadId;
            returnTick = thread -> returnTick;

            if (currentTick == returnTick) {

                if (thread -> semaphoreWaitingFor != NULL) {
                    _listDeleteFromSemaphoreWaitList(thread -> semaphoreWaitingFor, threadId);
                    thread -> semaphoreWaitingFor = NULL;

                }
                else {
                    //Thread has not waited for a resource
                }

                nextThreadId = _listGetNextWaitList(waitListNumber, threadId);
                _listDeleteFromWaitList(threadId, waitListNumber);
                thread -> returnTick = 0;

                _listInsertToReadyList(threadId);
                thread -> state = ThreadStateReady;

                if (_kernelCheckForContextSwitchAfterInsert(threadId) == true) {
                    _kernelContextSwitchRequest();
                }

            }
            else {
                nextThreadId = _listGetNextWaitList(waitListNumber, threadId);
            }
            threadId = nextThreadId;
        }
    }

    return StatusOk;
}

_status_t _kernelInitialize(void) {

    _status_t                           returnValue = StatusOk;
    _kernelControlBlock_t               *kernel;
    _kernelVersion_t                    kernelVersion;

    kernel = (_kernelControlBlock_t*)malloc(sizeof(_kernelControlBlock_t));
    if (kernel == NULL) {
        return StatusErrorHeapMemory;
    }
    else {
        _kernelControlBlock = kernel;
    }

    kernelVersion . major = KernelMajorVersion;
    kernelVersion . minor = KernelMinorVersion;
    kernelVersion . patch = KernelPatchVersion;
    _kernelSetVersion(kernelVersion);

    _kernelSetState(KernelStateInitialized);

    _kernelSetTick(0);

    _kernelSetCurrentRunningThreadId(NULL);

    kernel -> systemCallHandlerRequest = KernelRequestUnSet;
    kernel -> contextSwitchRequest = KernelRequestUnSet;


    /*
     * Initialize all of kernel data structures. If even one of them was
     * unsuccessful delete all of initialized data structures.
     * */
    while(true) {

        returnValue = _kernelSystemCallListInitialize();
        if (returnValue != StatusOk) {
            break;
        }
        kernel -> systemCallListIndex = 0;

        returnValue = _kernelReadyListInitialize();
        if (returnValue != StatusOk) {
            break;
        }

        returnValue = _kernelWaitListInitialize();
        if (returnValue != StatusOk) {
            break;
        }

        break;
    }
    if (returnValue != StatusOk) {

        for(_listSize_t index = 0 ; index < SystemCallListLength ; index++) {
            free(kernel -> systemCallList[index]);
        }

        for(_listSize_t index = 0 ; index < NumberOfPriorityLevels ; index++) {
            free(kernel -> readyList[index]);
        }

        for(_listSize_t index = 0 ; index < NumberOfWaitLists ; index++) {
            free(kernel -> waitList[index]);
        }
    }

    _portDeviceInitialize();

    return returnValue;
}
_status_t _kernelStart(void) {

    _status_t                            returnValue;
    _threadControlBlock_t                *firstThread;
    _threadId_t                          firstThreadId;
    _threadPriority_t                    priority;


    /*
     * Find most prior thread and set it as first running thread.
     * */
    priority = NumberOfPriorityLevels - 1;
    while(priority >= 0) {
        if (_listIsEmptyReadyList(priority) == true) {
            priority--;
        }
        else {
            firstThreadId = _listGetFirstReadyList(priority);
            _listPushIndexReadyList(priority);
            break;
        }
    }

    if (firstThreadId == NULL) {
        returnValue = StatusErrorKernelResource;
    }
    else {
        firstThread = (_threadControlBlock_t*)firstThreadId;
        firstThread -> state = ThreadStateRunning;

        _kernelSetCurrentRunningThreadId(firstThreadId);
        _kernelSetState(KernelStateRunning);

        _portTickTimerStart();

        _portStartFirstThread();
    }

    return returnValue;
}

_bool_t _kernelCheckForContextSwitchAfterInsert(_threadId_t threadId) {

    _bool_t                              returnValue;
    _kernelState_t                       kernelState;
    _threadControlBlock_t                *thread;
    _threadControlBlock_t                *currentRunningThread;
    _threadId_t                          currentRunningThreadId;
    _threadPriority_t                    threadPriority;
    _threadPriority_t                    currentRunningThreadPriority;

    kernelState = _kernelGetState();
    if (kernelState == KernelStateRunning) {
        thread = (_threadControlBlock_t*)threadId;
        currentRunningThreadId = _kernelGetCurrentRunningThreadId();
        currentRunningThread = (_threadControlBlock_t*)currentRunningThreadId;
        threadPriority = thread -> priority;
        currentRunningThreadPriority = currentRunningThread -> priority;
        if (threadPriority > currentRunningThreadPriority) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}

_bool_t _kernelCheckForContextSwitchAfterDelete(_threadId_t threadId) {

    _bool_t                               returnValue;
    _kernelState_t                        kernelState;
    _threadId_t                           currentRunningThreadId;

    kernelState = _kernelGetState();
    if (kernelState == KernelStateRunning) {
        currentRunningThreadId = _kernelGetCurrentRunningThreadId();
        if (threadId == currentRunningThreadId) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}

_kernelVersion_t _kernelGetVersion(void) {

    _kernelVersion_t                    returnValue;
    _kernelControlBlock_t               *kernel;

    kernel = _kernelGetKernelControlBlock();
    returnValue = kernel -> version;
    return returnValue;
}

_status_t _kernelSetVersion(_kernelVersion_t version) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> version = version;
    return StatusOk;
}

_kernelState_t _kernelGetState(void) {

    _kernelState_t                       returnValue;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    returnValue = kernel -> state;
    return returnValue;
}

_status_t _kernelSetState(_kernelState_t state) {
    _kernelControlBlock_t                *kernel;
    kernel = _kernelGetKernelControlBlock();
    kernel -> state = state;
    return StatusOk;
}

_kernelTick_t _kernelGetTick(void) {

    _kernelTick_t                        returnValue;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    returnValue = kernel -> tick;
    return returnValue;
}

_status_t _kernelSetTick(_kernelTick_t tick) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> tick = tick;
    return StatusOk;
}

_status_t _kernelIncreaseTick(void) {

    _kernelControlBlock_t                *kernel;
    _kernelTick_t                        tick;

    kernel = _kernelGetKernelControlBlock();
    tick = kernel -> tick;
    tick++;
    kernel -> tick = tick;
    return StatusOk;
}

_threadId_t _kernelGetCurrentRunningThreadId(void) {

    _threadId_t                          returnValue;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    returnValue = kernel -> currentRunningThreadId;
    return returnValue;
}

_status_t _kernelSetCurrentRunningThreadId(_threadId_t threadId) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> currentRunningThreadId = threadId;
    return StatusOk;
}

_threadStackPointer_t _kernelGetCurrentRunningThreadStackPointer(void) {

    _threadStackPointer_t                returnValue;
    _threadControlBlock_t                *currentRunningThread;
    _threadId_t                          currentRunningThreadId;

    currentRunningThreadId = _kernelGetCurrentRunningThreadId();
    currentRunningThread = (_threadControlBlock_t*)currentRunningThreadId;
    returnValue = currentRunningThread -> stackPointer;
    return returnValue;
}

_status_t _kernelSetCurrentRunningThreadStackPointer(_threadStackPointer_t stackPointer) {

    _threadControlBlock_t                *currentRunningThread;
    _threadId_t                          currentRunningThreadId;

    currentRunningThreadId = _kernelGetCurrentRunningThreadId();
    currentRunningThread = (_threadControlBlock_t*)currentRunningThreadId;
    currentRunningThread -> stackPointer = stackPointer;
    return StatusOk;
}

_threadFunctionPointer_t _kernelGetCurrentRunningThreadFunctionPointer(void) {

    _threadFunctionPointer_t            returnValue;
    _threadControlBlock_t               *currentRunningThread;
    _threadId_t                         currentRunningThreadId;

    currentRunningThreadId = _kernelGetCurrentRunningThreadId();
    currentRunningThread = (_threadControlBlock_t*)currentRunningThreadId;
    returnValue = currentRunningThread -> functionPointer;
    return returnValue;
}

_threadFunctionParameter_t _kernelGetCurrentRunningThreadFunctionParameter(void) {

    _threadFunctionParameter_t           returnValue;
    _threadControlBlock_t                *currentRunningThread;
    _threadId_t                          currentRunningThreadId;

    currentRunningThreadId = _kernelGetCurrentRunningThreadId();
    currentRunningThread = (_threadControlBlock_t*)currentRunningThreadId;
    returnValue = currentRunningThread -> functionParameter;
    return returnValue;
}

_status_t _kernelSystemCallListInitialize(void) {

    _status_t                            returnValue = StatusOk;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();


    /*
     * Allocate memory to system call list(array). If heap memory allocation was
     * unsuccessful delete all of allocated memories.
     * */
    kernel -> systemCallList = (_kernelSystemCallArg_t**)malloc(SystemCallListLength * sizeof(_kernelSystemCallArg_t*));
    if(kernel -> systemCallList == NULL) {
        returnValue = StatusErrorHeapMemory;
    }
    else {
        for(uint32_t i = 0 ; i < SystemCallListLength ; i++) {
            kernel -> systemCallList[i] = NULL;
        }
    }



    return returnValue;
}

_status_t _kernelReadyListInitialize(void) {

    _status_t                            returnValue;
    _kernelControlBlock_t                *kernel;
    _kernelReadyList_t                   *list;

    returnValue = StatusOk;
    kernel = _kernelGetKernelControlBlock();


    /*
     * Allocate memory to ready list(array). If list creation was unsuccessful
     * delete all of created lists.
     * */
    for(_listSize_t index = 0 ; index < NumberOfPriorityLevels ; index++) {
        list = _listCreateNewReadyList();
        if (list == NULL) {
            returnValue = StatusErrorHeapMemory;
            break;
        }
        else {
            kernel -> readyList[index] = list;
        }
    }
    if (returnValue == StatusErrorHeapMemory) {
        for(_listSize_t index = 0 ; index < NumberOfPriorityLevels ; index++) {
            free(kernel -> readyList[index]);
        }
    }



    return returnValue;
}

_status_t _kernelWaitListInitialize(void) {
    _status_t                            returnValue;
    _kernelControlBlock_t                *kernel;
    _kernelWaitList_t                    *list;

    returnValue = StatusOk;
    kernel = _kernelGetKernelControlBlock();


    /*
     * Allocate memory to wait list(array). If list creation was unsuccessful
     * delete all of created lists.
     * */
    for(_listSize_t index = 0 ; index < NumberOfWaitLists ; index++) {
        list = _listCreateNewWaitList();
        if (list == NULL) {
            returnValue = StatusErrorHeapMemory;
            break;
        }
        else {
            kernel -> waitList[index] = list;
        }
    }
    if (returnValue == StatusErrorHeapMemory) {
        for(_listSize_t index = 0 ; index < NumberOfWaitLists ; index++) {
            free(kernel -> waitList[index]);
        }
    }



    return returnValue;
}

_status_t _kernelSystemCallRequest(void) {

    _kernelSetSystemCallRequest();

    _portSupervisorInterruptTrigger();

    return StatusOk;
}

_status_t _kernelContextSwitchRequest(void) {

    _kernelSetContextSwitchRequest();
    _portPreSupervisorInterruptTrigger();
    return StatusOk;
}

_status_t _kernelWaitListHandlerRequest(void) {

    _kernelSetWaitListHandlerRequest();
    _portPreSupervisorInterruptTrigger();
    return StatusOk;
}

_kernelRequest_t _kernelGetSystemCallRequest(void) {

    _kernelRequest_t                    returnValue;
    _kernelControlBlock_t               *kernel;

    kernel = _kernelGetKernelControlBlock();
    returnValue = kernel -> systemCallHandlerRequest;
    return returnValue;
}

_status_t _kernelSetSystemCallRequest(void) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> systemCallHandlerRequest = KernelRequestSet;
    return StatusOk;
}

_status_t _kernelUnsetSystemCallRequest(void) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> systemCallHandlerRequest = KernelRequestUnSet;
    return StatusOk;
}

_kernelRequest_t _kernelGetContextSwitchRequest(void) {

    _kernelRequest_t                    returnValue;
    _kernelControlBlock_t               *kernel;

    kernel = _kernelGetKernelControlBlock();
    returnValue = kernel -> contextSwitchRequest;
    return returnValue;
}

_status_t _kernelSetContextSwitchRequest(void) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> contextSwitchRequest = KernelRequestSet;
    return StatusOk;
}

_status_t _kernelUnsetContextSwitchReuest(void) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> contextSwitchRequest = KernelRequestUnSet;
    return StatusOk;
}

_kernelRequest_t _kernelGetWaitListHandlerRequest(void) {

    _kernelRequest_t                    returnValue;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    returnValue = kernel -> waitListHandlerRequest;
    return returnValue;
}

_status_t _kernelSetWaitListHandlerRequest(void) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> waitListHandlerRequest = KernelRequestSet;
    return StatusOk;
}

_status_t _kernelUnsetWaitListHandlerReuest(void) {

    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    kernel -> waitListHandlerRequest = KernelRequestUnSet;
    return StatusOk;
}

_bool_t _kernelIsInInterrupt(void) {

    return _portIsInInterrupt();
}

_status_t _kernelThreadStackInitialize(_threadId_t threadId) {

    return _portThreadStackInitialize(threadId);
}

_atomicValue_t _kernelAtomicRead32(_atomicAddress_t address) {

    return _portAtomicRead32(address);
}

_atomicResult_t _kernelAtomicWrite32(_atomicValue_t value, _atomicAddress_t address) {

    return _portAtomicWrite32(value, address);
}

_status_t _kernelAtomicClear(void) {

    return _portAtomicClear();
}


