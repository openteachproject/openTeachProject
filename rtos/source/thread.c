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
 * thread.c
 *
 *  Created on: Aug 26, 2021
 *      Author: salar
 */



#include <rtos/include/rtosTypeDefinitions.h>
#include <rtos/include/systemCallTypeDefintions.h>
#include <rtos/include/kernel.h>
#include <rtos/include/list.h>
#include <rtos/include/rtos.h>



threadId_t threadGetId(void) {

    _threadId_t                        returnValue;
    _kernelState_t                     kernelState;

    kernelState = _kernelGetState();
    if (_kernelIsInInterrupt() || kernelState != KernelStateRunning) {
        return NULL;
    }
    else {
        returnValue = _kernelGetCurrentRunningThreadId();
    }
    return returnValue;
}



threadName_t threadGetName(threadId_t id) {

    _threadName_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadGetNameArg_t                 threadGetNameArg;

    if (id == NULL) {
        returnValue = NULL;
    }
    else {
        if (_kernelIsInInterrupt()) {

            returnValue = NULL;
        }
        else {

            threadGetNameArg . threadIdArg = id;

            systemCallArg . systemCallNumber = ThreadGetNameSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadGetNameArg;

            _kernelSystemCall(&systemCallArg);
            returnValue = threadGetNameArg . returnValue;
        }
    }

    return returnValue;
}



void _threadGetNameSystemCall(_threadGetNameArg_t *arg) {

    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadName_t                       threadName;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    threadName = thread -> name;

    arg -> returnValue = threadName;
    return;
}



threadState_t threadGetState(threadId_t id) {

    _threadState_t                      returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadGetStateArg_t                threadGetStateArg;

    if (id == NULL) {
        returnValue = ThreadStateError;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = ThreadStateError;
        }
        else {

            threadGetStateArg . threadIdArg = id;

            systemCallArg . systemCallNumber = ThreadGetStateSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadGetStateArg;

            _kernelSystemCall(&systemCallArg);
            returnValue = threadGetStateArg . returnValue;
        }
    }

    return returnValue;
}



void _threadGetStateSystemCall(_threadGetStateArg_t *arg) {

    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadState_t                      threadState;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    threadState = thread -> state;

    arg -> returnValue = threadState;
    return;
}



threadPriority_t threadGetPriority(threadId_t id) {

    _threadPriority_t                   returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadGetPriorityArg_t             threadGetPriorityArg;

    if (id == NULL) {
        returnValue = PriorityError;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = PriorityError;
        }
        else {

            threadGetPriorityArg . threadIdArg = id;

            systemCallArg . systemCallNumber = ThreadGetPrioritySystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadGetPriorityArg;

            _kernelSystemCall(&systemCallArg);
            returnValue = threadGetPriorityArg . returnValue;
        }
    }

    return returnValue;
}



void _threadGetPrioritySystemCall(_threadGetPriorityArg_t *arg) {

    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadPriority_t                   threadPriority;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    threadPriority = thread -> priority;

    arg -> returnValue = threadPriority;

    return;
}



rtosStatus_t threadSetPriority(threadId_t id, threadPriority_t priority) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadSetPriorityArg_t             threadSetPriorityArg;

    if ( id == NULL ||
         priority >= NumberOfPriorityLevels ||
         priority == PriorityIdle)
    {

        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            threadSetPriorityArg . threadIdArg = id;
            threadSetPriorityArg . threadPriorityArg = priority;

            systemCallArg . systemCallNumber = ThreadSetPrioritySystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadSetPriorityArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = threadSetPriorityArg . returnValue;
        }
    }

    return returnValue;
}



void _threadSetPrioritySystemCall(_threadSetPriorityArg_t *arg) {

    _rtosStatus_t                       returnValue;
    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadPriority_t                   newPriority;
    _threadPriority_t                   oldPriority;
    _threadState_t                      threadState;
    _kernelState_t                      kernelState;
    _threadControlBlock_t               *currentRunningThread;
    _threadId_t                         currentRunningThreadId;
    _threadPriority_t                   currentRunningThreadPriority;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    oldPriority = thread -> priority;
    newPriority = arg -> threadPriorityArg;
    threadState = thread -> state;

    if (newPriority == oldPriority) {

        returnValue = StatusOk;
    }
    else {

        if (thread -> state == ThreadStateRunning || thread -> state == ThreadStateReady) {

            _listDeleteFromReadyList(threadId);

            thread -> priority = newPriority;
            thread -> semaphoreWaitNode -> key = newPriority;
            thread -> mutexWaitNode -> key = newPriority;
            thread -> memPoolWaitNode -> key = newPriority;

            _listInsertToReadyList(threadId);

            kernelState = _kernelGetState();

            if (kernelState == KernelStateRunning) {

                currentRunningThreadId = _kernelGetCurrentRunningThreadId();
                currentRunningThread = (_threadControlBlock_t*)currentRunningThreadId;
                currentRunningThreadPriority = currentRunningThread -> priority;

                if (threadId == currentRunningThreadId) {

                    _kernelContextSwitchRequest();
                }
                else if (threadId != currentRunningThreadId && newPriority > currentRunningThreadPriority) {

                    _kernelContextSwitchRequest();
                }
                else {

                }
            }

            returnValue = StatusOk;
        }
        else if (threadState == ThreadStateWaited) {

            if (thread -> semaphoreWaitingFor != NULL) {

                _listDeleteFromSemaphoreWaitList(thread -> semaphoreWaitingFor, threadId);

                thread -> priority = newPriority;
                thread -> semaphoreWaitNode -> key = newPriority;
                thread -> mutexWaitNode -> key = newPriority;
                thread -> memPoolWaitNode -> key = newPriority;

                _listInsertToSemaphoreWaitList(thread -> semaphoreWaitingFor, threadId);
            }
            else if (thread -> mutexWaitingFor != NULL) {

                _listDeleteFromMutexWaitList(thread -> mutexWaitingFor, threadId);

                thread -> priority = newPriority;
                thread -> semaphoreWaitNode -> key = newPriority;
                thread -> mutexWaitNode -> key = newPriority;
                thread -> memPoolWaitNode -> key = newPriority;

                _listInsertToMutexWaitList(thread -> mutexWaitingFor, threadId);
            }
            else if (thread -> memPoolWaitingFor != NULL) {

                _listDeleteFromMemPoolWaitList(thread -> memPoolWaitingFor, threadId);

                thread -> priority = newPriority;
                thread -> semaphoreWaitNode -> key = newPriority;
                thread -> mutexWaitNode -> key = newPriority;
                thread -> memPoolWaitNode -> key = newPriority;

                _listInsertToMemPoolWaitList(thread -> memPoolWaitingFor, threadId);
            }
            else {

                thread -> priority = newPriority;
            }

            returnValue = StatusOk;
        }
        else if (threadState == ThreadStateSuspended) {

            thread -> priority = newPriority;
            thread -> semaphoreWaitNode -> key = newPriority;
            thread -> mutexWaitNode -> key = newPriority;
            thread -> memPoolWaitNode -> key = newPriority;

            returnValue = StatusOk;
        }
        else if (threadState == ThreadStateTerminated) {

            returnValue = StatusErrorThreadState;
        }
        else {
            //
        }
    }

    arg -> returnValue = returnValue;
    return;
}



threadStackSize_t threadGetStackSize(threadId_t id) {

    _threadStackSize_t                  returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadGetStackSizeArg_t            threadGetStackSizeArg;

    if (id == NULL) {
        returnValue = 0;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = 0;
        }
        else {

            threadGetStackSizeArg . threadIdArg = id;

            systemCallArg . systemCallNumber = ThreadGetStackSizeSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadGetStackSizeArg;

            _kernelSystemCall(&systemCallArg);
            returnValue = threadGetStackSizeArg . returnValue;
        }
    }

    return returnValue;
}



void _threadGetStackSizeSystemCall(_threadGetStackSizeArg_t *arg) {

    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadStackSize_t                  threadStackSize;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    threadStackSize = thread -> stackSize;

    arg -> returnValue = threadStackSize;
    return;
}



threadId_t threadCreateNew( threadName_t                name,
                            threadFunctionPointer_t     functionPointer,
                            threadFunctionParameter_t   functionParameter,
                            threadPriority_t            priority,
                            threadStackSize_t           stackSize
                            )
{

    _threadId_t                            returnValue;
    _kernelSystemCallArg_t                 systemCallArg;
    _threadCreateNewArg_t                  threadCreateNewArg;

    if (    functionPointer == NULL ||
            priority >= NumberOfPriorityLevels ||
            priority == PriorityIdle ||
            stackSize > ThreadMaximumStackSize)
    {

        returnValue = NULL;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = NULL;
        }
        else {

            threadCreateNewArg . threadNameArg = name;
            threadCreateNewArg . threadFunctionPointerArg = functionPointer;
            threadCreateNewArg . threadFunctionParameterArg = functionParameter;
            threadCreateNewArg . threadPriorityArg = priority;
            threadCreateNewArg . threadStackSizeArg = stackSize;

            systemCallArg . systemCallNumber = ThreadCreateNewSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadCreateNewArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = threadCreateNewArg . returnValue;
        }
    }
    return returnValue;
}

void _threadCreateNewSystemCall(_threadCreateNewArg_t *arg) {

    _threadId_t                            returnValue;;
    _threadControlBlock_t                 *newThread;
    _threadId_t                            threadId;
    _threadName_t                          threadName;
    _threadStackPointer_t                  threadStackLimit;
    _threadStackPointer_t                  threadStackTop;
    _threadStackSize_t                     threadStackSize;
    _kernelReadyNode_t                    *threadReadyNode;
    _kernelSuspendedNode_t                *threadSuspendedNode;
    _kernelTerminatedNode_t               *threadTerminatedNode;
    _kernelWaitNode_t                     *threadWaitNode;
    _semaphoreWaitNode_t                  *semaphoreWaitNode;
    _mutexWaitNode_t                      *mutexWaitNode;
    _threadOwnedMutexList_t               *threadOwnedMutexList;
    _memPoolWaitNode_t                    *memPoolWaitNode;

    while(true) {
        newThread = (_threadControlBlock_t*)malloc(sizeof(_threadControlBlock_t));
        if (newThread == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            threadId = (_threadId_t)newThread;
            newThread -> id = threadId;
            returnValue = threadId;
        }



        if (strlen(arg -> threadNameArg) >= ThreadNameMaxLength) {
            (arg -> threadNameArg)[ThreadNameMaxLength - 1] = 0x00;
        }
        threadName = (_threadName_t)malloc(strlen(arg -> threadNameArg));
        if (threadName == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            strcpy(threadName, arg -> threadNameArg);
            newThread -> name = threadName;
        }



        newThread -> state = ThreadStateReady;

        newThread -> functionPointer = arg -> threadFunctionPointerArg;
        newThread -> functionParameter = arg -> threadFunctionParameterArg;

        newThread -> priority = arg -> threadPriorityArg;



        if ((arg -> threadStackSizeArg) == 0) {
            arg -> threadStackSizeArg = ThreadDefaultStackSize;
        }
        threadStackLimit = (_threadStackPointer_t)malloc(arg -> threadStackSizeArg);
        if (threadStackLimit == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            threadStackSize = arg -> threadStackSizeArg;
            threadStackTop = threadStackLimit + threadStackSize;

            newThread -> stackSize = threadStackSize;
            newThread -> stackTop = threadStackTop;
            newThread -> stackPointer = threadStackTop;

            _kernelThreadStackInitialize(threadId);
        }



        threadReadyNode = _listCreateNewReadyNode(threadId);
        if (threadReadyNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> readyNode = threadReadyNode;
        }

        threadWaitNode = _listCreateNewWaitNode(threadId);
        if (threadReadyNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> waitNode = threadWaitNode;
        }

        newThread -> returnTick = 0;

        threadSuspendedNode = _listCreateNewSuspendedNode(threadId);
        if (threadReadyNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> suspendedNode = threadSuspendedNode;
        }

        threadTerminatedNode = _listCreateNewTerminatedNode(threadId);
        if (threadReadyNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> terminatedNode = threadTerminatedNode;
        }

        semaphoreWaitNode = _listCreateNewSemaphoreWaitNode(threadId);
        if (semaphoreWaitNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> semaphoreWaitNode = semaphoreWaitNode;
        }

        newThread -> semaphoreWaitingFor = NULL;

        mutexWaitNode = _listCreateNewMutexWaitNode(threadId);
        if (mutexWaitNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> mutexWaitNode = mutexWaitNode;
        }

        newThread -> mutexWaitingFor = NULL;

        threadOwnedMutexList = _listCreateNewThreadOwnedMutexList();
        if (threadOwnedMutexList == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> threadOwnedMutexList = threadOwnedMutexList;
        }

        newThread -> returnedByRelease = ReturnedByReleaseUnset;

        memPoolWaitNode = _listCreateNewMemPoolWaitNode(threadId);
        if (memPoolWaitNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newThread -> memPoolWaitNode = memPoolWaitNode;
        }

        newThread -> memPoolWaitingFor = NULL;

        newThread -> returnedByFree = ReturnedByFreeUnset;

        newThread -> memPoolBlockReleasedByFree = NULL;



        break;
    }



    if (returnValue == NULL) {
        free(newThread);
        free(threadName);
        free(threadStackLimit);
        free(threadReadyNode);
        free(threadWaitNode);
        free(threadSuspendedNode);
        free(threadTerminatedNode);
        free(semaphoreWaitNode);
        free(mutexWaitNode);
        free(threadOwnedMutexList);
        free(memPoolWaitNode);
    }
    else {
        if (_listInsertToReadyList(threadId) == StatusOk) {
            if (_kernelCheckForContextSwitchAfterInsert(threadId) == true) {
                _kernelContextSwitchRequest();
            }
        }
        else {
            returnValue = NULL;
        }
    }



    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t threadTerminate(threadId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadTerminateArg_t               threadTerminateArg;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            threadTerminateArg . threadIdArg = id;

            systemCallArg . systemCallNumber = ThreadTerminateSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadTerminateArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = threadTerminateArg . returnValue;
        }
    }

    return returnValue;
}



void _threadTerminateSystemCall(_threadTerminateArg_t *arg) {

    _rtosStatus_t                       returnValue = StatusOk;
    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadState_t                      threadState;
    _listSize_t                         listSize;
    _kernelTick_t                       returnTick;
    _kernelWaitListNumber_t             waitListNumber;


    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    threadState = thread -> state;

    listSize = _listGetSizeThreadOwnedMutexList(threadId);

    if (listSize > 0) {

        returnValue = StatusErrorResource;

        arg -> returnValue = returnValue;
        return;
    }

    if (threadState == ThreadStateRunning || threadState == ThreadStateReady) {

        _listDeleteFromReadyList(threadId);

        _listInsertToTerminatedList(threadId);
        thread -> state = ThreadStateTerminated;

        if (_kernelCheckForContextSwitchAfterDelete(threadId)) {

            _kernelContextSwitchRequest();
        }

        returnValue = StatusOk;
    }
    else if (threadState == ThreadStateWaited) {

        if (thread -> semaphoreWaitingFor != NULL) {
            _listDeleteFromSemaphoreWaitList(thread -> semaphoreWaitingFor, threadId);
            thread -> semaphoreWaitingFor = NULL;

        }
        else if (thread -> mutexWaitingFor != NULL) {
            _listDeleteFromMutexWaitList(thread -> mutexWaitingFor, threadId);
            thread -> mutexWaitingFor = NULL;
        }
        else if (thread -> memPoolWaitingFor != NULL) {
            _listDeleteFromMemPoolWaitList(thread -> memPoolWaitingFor, threadId);
            thread -> memPoolWaitingFor = NULL;
        }
        else {
            //Thread has not waited for a resource
        }

        returnTick = thread -> returnTick;
        waitListNumber = returnTick % NumberOfWaitLists;
        thread -> returnTick = 0;
        _listDeleteFromWaitList(threadId, waitListNumber);

        _listInsertToTerminatedList(threadId);
        thread -> state = ThreadStateTerminated;

        returnValue = StatusOk;
    }
    else if (threadState == ThreadStateSuspended) {

        _listDeleteFromSuspendedList(threadId);

        _listInsertToTerminatedList(threadId);
        thread -> state = ThreadStateTerminated;

        returnValue = StatusOk;
    }
    else if (threadState == ThreadStateTerminated) {

        returnValue = StatusErrorThreadState;
    }
    else {
        //
    }

    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t threadYield(void) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadYieldArg_t                   threadYieldArg;


    if (_kernelIsInInterrupt()) {
        returnValue = StatusErrorInerrupt;
    }
    else {

        threadYieldArg . currentRunningThreadIdArg = _kernelGetCurrentRunningThreadId();

        systemCallArg . systemCallNumber = ThreadYieldSystemCallNumber;
        systemCallArg . specificSystemCallArg = &threadYieldArg;

        _kernelSystemCall(&systemCallArg);

        returnValue = threadYieldArg . returnValue;
    }

    return returnValue;
}



void _threadYieldSystemCall(_threadYieldArg_t *arg) {

    _threadId_t                         currentRunningThreadId;

    currentRunningThreadId = arg -> currentRunningThreadIdArg;

    if (currentRunningThreadId == _kernelGetCurrentRunningThreadId()) {

        _kernelContextSwitchRequest();
    }

    arg -> returnValue = StatusOk;
    return;
}



rtosStatus_t threadSuspend(threadId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadSuspendArg_t                 threadSuspendArg;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            threadSuspendArg . threadIdArg = id;

            systemCallArg . systemCallNumber = ThreadSuspendSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadSuspendArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = threadSuspendArg . returnValue;
        }
    }

    return returnValue;
}



void _threadSuspendSystemCall(_threadSuspendArg_t *arg) {

    _rtosStatus_t                       returnValue;
    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadState_t                      threadState;
    _kernelTick_t                       returnTick;
    _kernelWaitListNumber_t             waitListNumber;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    threadState = thread -> state;

    if (threadState == ThreadStateRunning || threadState == ThreadStateReady) {

        _listDeleteFromReadyList(threadId);

        _listInsertToSuspendedList(threadId);
        thread -> state = ThreadStateSuspended;

        if (_kernelCheckForContextSwitchAfterDelete(threadId)) {

            _kernelContextSwitchRequest();
        }

        returnValue = StatusOk;
    }
    else if (threadState == ThreadStateWaited) {

        if (thread -> semaphoreWaitingFor != NULL) {
            _listDeleteFromSemaphoreWaitList(thread -> semaphoreWaitingFor, threadId);
            thread -> semaphoreWaitingFor = NULL;

        }
        else if (thread -> mutexWaitingFor != NULL) {
            _listDeleteFromMutexWaitList(thread -> mutexWaitingFor, threadId);
            thread -> mutexWaitingFor = NULL;
        }
        else if (thread -> memPoolWaitingFor != NULL) {
            _listDeleteFromMemPoolWaitList(thread -> memPoolWaitingFor, threadId);
            thread -> memPoolWaitingFor = NULL;
        }
        else {
            //Thread has not waited for a resource
        }

        returnTick = thread -> returnTick;
        waitListNumber = returnTick % NumberOfWaitLists;
        thread -> returnTick = 0;
        _listDeleteFromWaitList(threadId, waitListNumber);

        _listInsertToSuspendedList(threadId);
        thread -> state = ThreadStateSuspended;

        returnValue = StatusOk;
    }
    else if (threadState == ThreadStateSuspended || threadState == ThreadStateTerminated) {

        returnValue = StatusErrorThreadState;
    }

    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t threadResume(threadId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadResumeArg_t                  threadResumeArg;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            threadResumeArg . threadIdArg = id;

            systemCallArg . systemCallNumber = ThreadResumeSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadResumeArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = threadResumeArg . returnValue;
        }
    }

    return returnValue;
}



void _threadResumeSystemCall(_threadResumeArg_t *arg) {

    _rtosStatus_t                       returnValue;
    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _threadState_t                      threadState;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;
    threadState = thread -> state;

    if (threadState == ThreadStateSuspended) {

        _listDeleteFromSuspendedList(threadId);

        _listInsertToReadyList(threadId);
        thread -> state = ThreadStateReady;

        if (_kernelCheckForContextSwitchAfterInsert(threadId)) {

            _kernelContextSwitchRequest();
        }

        returnValue = StatusOk;
    }
    else {

        returnValue = StatusErrorThreadState;
    }

    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t threadWait(threadId_t id, kernelTick_t numberOfTicks) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadWaitArg_t                    threadWaitArg;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            threadWaitArg . threadIdArg = id;
            threadWaitArg . numberOfTicksArg = numberOfTicks;

            systemCallArg . systemCallNumber = ThreadWaitSystemCallNumber;
            systemCallArg . specificSystemCallArg = &threadWaitArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = threadWaitArg . returnValue;
        }

    }

    return returnValue;
}



void _threadWaitSystemCall(_threadWaitArg_t *arg) {

    _threadControlBlock_t               *thread;
    _threadId_t                         threadId;
    _kernelTick_t                       returnTick;
    _kernelTick_t                       currentTick;
    _kernelTick_t                       numberOfTicks;
    _kernelWaitListNumber_t             waitListNumber;

    threadId = arg -> threadIdArg;
    thread = (_threadControlBlock_t*)threadId;

    numberOfTicks = arg -> numberOfTicksArg;

    currentTick = _kernelGetTick();
    returnTick = currentTick + numberOfTicks;
    waitListNumber = returnTick % NumberOfWaitLists;

    thread -> returnTick = returnTick;

    if (thread -> state == ThreadStateRunning) {

        _listDeleteFromReadyList(threadId);
    }
    else if (thread -> state == ThreadStateReady) {

        _listDeleteFromReadyList(threadId);
    }
    else {
        //Do noting
    }

    _listInsertToWaitList(threadId, waitListNumber);
    thread -> state = ThreadStateWaited;

    if (_kernelCheckForContextSwitchAfterDelete(threadId) == true) {

        _kernelContextSwitchRequest();
    }

    arg -> returnValue = StatusOk;
    return;
}



