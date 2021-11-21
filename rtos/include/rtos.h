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
 * rtos.h
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */



#ifndef RTOS_INCLUDE_RTOS_H_
#define RTOS_INCLUDE_RTOS_H_



#include <rtos/include/rtosTypeDefinitions.h>



typedef _kernelState_t                      kernelState_t;
typedef _kernelTick_t                       kernelTick_t;
typedef _rtosStatus_t                       rtosStatus_t;
typedef _bool_t                             bool_t;



typedef _threadControlBlock_t               threadControlBlock_t;
typedef _threadId_t                         threadId_t;
typedef _threadName_t                       threadName_t;
typedef _threadState_t                      threadState_t;
typedef _threadFunctionParameter_t          threadFunctionParameter_t;
typedef _threadFunctionPointer_t            threadFunctionPointer_t;
typedef _threadPriority_t                   threadPriority_t;
typedef _threadStackSize_t                  threadStackSize_t;
typedef _threadStackPointer_t               threadStackPointer_t;



typedef _semaphoreId_t                      semaphoreId_t;
typedef _semaphoreName_t                    semaphoreName_t;
typedef _semaphoreCount_t                   semaphoreCount_t;



typedef _mutexId_t                          mutexId_t;
typedef _mutexName_t                        mutexName_t;
typedef _mutexLock_t                        mutexLock_t;
typedef _mutexType_t                        mutexType_t;



typedef _memPoolId_t                        memPoolId_t;
typedef _memPoolName_t                      memPoolName_t;
typedef _memPoolSize_t                      memPoolSize_t;
typedef _memPoolCount_t                     memPoolCount_t;
typedef _memPoolPointer_t                   memPoolPointer_t;





rtosStatus_t                                rtosInitialize(void);
rtosStatus_t                                rtosStart(void);





threadId_t                                  threadGetId(void);
threadName_t                                threadGetName(threadId_t id);
threadState_t                               threadGetState(threadId_t id);
threadPriority_t                            threadGetPriority(threadId_t id);
rtosStatus_t                                threadSetPriority(threadId_t id, threadPriority_t priority);
threadStackSize_t                           threadGetStackSize(threadId_t id);
threadId_t                                  threadCreateNew(    threadName_t                name,
                                                                threadFunctionPointer_t     functionPointer,
                                                                threadFunctionParameter_t   functionParameter,
                                                                threadPriority_t            priority,
                                                                threadStackSize_t           stackSize
                                                                );
rtosStatus_t                                threadTerminate(threadId_t id);
rtosStatus_t                                threadYield(void);
rtosStatus_t                                threadSuspend(threadId_t id);
rtosStatus_t                                threadResume(threadId_t id);
rtosStatus_t                                threadWait(threadId_t id, kernelTick_t numberOfTicks);



semaphoreName_t                             semaphoreGetName(semaphoreId_t id);
semaphoreId_t                               semaphoreCreateNew(    semaphoreName_t name,
                                                                   semaphoreCount_t maxCount,
                                                                   semaphoreCount_t initialCount);
rtosStatus_t                                semaphoreDelete(semaphoreId_t id);
rtosStatus_t                                semaphoreGet(semaphoreId_t id, kernelTick_t timeOut);
rtosStatus_t                                semaphoreRelease(semaphoreId_t id);



mutexName_t                                 mutexGetName(mutexId_t id);
mutexId_t                                   mutexCreateNew(mutexName_t name, mutexType_t type);
rtosStatus_t                                mutexDelete(mutexId_t id);
rtosStatus_t                                mutexGet(mutexId_t id, kernelTick_t timeOut);
rtosStatus_t                                mutexRelease(mutexId_t id);



memPoolName_t                               memPoolGetName(memPoolId_t id);
memPoolId_t                                 memPoolCreateNew(   memPoolName_t name,
                                                                memPoolSize_t blockSize,
                                                                memPoolCount_t maxBlockCount);
rtosStatus_t                                memPoolDelete(memPoolId_t id);
memPoolPointer_t                            memPoolAllocate(memPoolId_t id, _kernelTick_t timeOut);
rtosStatus_t                                memPoolFree(memPoolId_t id, memPoolPointer_t block);



#endif /* RTOS_INCLUDE_RTOS_H_ */
