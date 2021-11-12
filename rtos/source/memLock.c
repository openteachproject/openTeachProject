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
 * memLock.c
 *
 *  Created on: Nov 12, 2021
 *      Author: salar
 */





#include <malloc.h>
#include <rtos/include/rtosTypeDefinitions.h>
#include <rtos/include/kernel.h>
#include <rtos/include/rtos.h>






void __malloc_lock(struct _reent *reent) {

    _kernelStarted_t                    kernelStartedValue;
    _mutexId_t                          heapLockerMutex;
    _rtosStatus_t                       status;

    kernelStartedValue = _kernelGetStartedValue();

    if (kernelStartedValue == kernelStartedTrue) {

        heapLockerMutex = _kernelGetHeapLockerMutex();

        while(true) {

            status = mutexGet(heapLockerMutex, 0);

            if (status == StatusOk) {

                break;
            }
        }
    }
}

void __malloc_unlock(struct _reent *reent) {

    _kernelStarted_t                    kernelStartedValue;
    _mutexId_t                          heapLockerMutex;

    kernelStartedValue = _kernelGetStartedValue();

    if (kernelStartedValue == kernelStartedTrue) {

        heapLockerMutex = _kernelGetHeapLockerMutex();

        mutexRelease(heapLockerMutex);
    }
}


