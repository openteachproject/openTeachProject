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
