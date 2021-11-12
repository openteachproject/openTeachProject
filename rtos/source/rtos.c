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
 * rtos.c
 *
 *  Created on: Sep 12, 2021
 *      Author: salar
 */



#include <rtos/include/rtosTypeDefinitions.h>
#include <rtos/include/systemCallTypeDefintions.h>
#include <rtos/include/kernel.h>
#include <rtos/include/rtos.h>



void idleThreadFunction(threadFunctionParameter_t parameter) {
    while(true) {

    }
}



static threadId_t rtosCreateIdleThread( threadName_t                name,
                                        threadFunctionPointer_t     functionPointer,
                                        threadFunctionParameter_t   functionParameter,
                                        threadPriority_t            priority,
                                        threadStackSize_t           stackSize
                                        )
{

    _threadId_t                         returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _threadCreateNewArg_t               threadCreateNewArg;

    threadCreateNewArg . threadNameArg = name;
    threadCreateNewArg . threadFunctionPointerArg = functionPointer;
    threadCreateNewArg . threadFunctionParameterArg = functionParameter;
    threadCreateNewArg . threadPriorityArg = priority;
    threadCreateNewArg . threadStackSizeArg = stackSize;

    systemCallArg . systemCallNumber = ThreadCreateNewSystemCallNumber;
    systemCallArg . specificSystemCallArg = &threadCreateNewArg;

    _kernelSystemCall(&systemCallArg);

    returnValue = threadCreateNewArg . returnValue;

    return returnValue;
}



rtosStatus_t rtosInitialize(void) {

    _rtosStatus_t                       returnValue;
    _mutexId_t                          heapLockerMutex;

    returnValue = _kernelInitialize();

    rtosCreateIdleThread("IdleThread", idleThreadFunction, NULL, PriorityIdle, 256);

    heapLockerMutex = mutexCreateNew("HeapLocker", MutexTypeRecursive);
    _kernelSetHeapLockerMutex(heapLockerMutex);

    return returnValue;
}

rtosStatus_t rtosStart(void) {

    return _kernelStart();
}


