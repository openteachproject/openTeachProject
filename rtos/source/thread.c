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



status_t threadSetPriority(threadId_t id, threadPriority_t priority) {
    return StatusError;
}



void _threadSetPrioritySystemCall(_threadSetPriorityArg_t *arg) {

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



        break;
    }



    if (returnValue == NULL) {
        free(newThread);
        free(threadName);
        free(threadStackLimit);
        free(threadReadyNode);
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



status_t threadTerminate(threadId_t id) {
    return StatusError;
}



void _threadTerminateSystemCall(_threadTerminateArg_t *arg) {

}



status_t threadYield(void) {
    return StatusError;
}



void _threadYieldSystemCall(_threadYieldArg_t *arg) {

}



status_t threadSuspend(threadId_t id) {
    return StatusError;
}



void _threadSuspendSystemCall(_threadSuspendArg_t *arg) {

}



status_t threadResume(threadId_t id) {
    return StatusError;
}



void _threadResumeSystemCall(_threadResumeArg_t *arg) {

}



status_t threadWait(threadId_t id, kernelTick_t numberOfTicks) {
    return StatusError;
}



void _threadWaitSystemCall(_threadWaitArg_t *arg) {

}



