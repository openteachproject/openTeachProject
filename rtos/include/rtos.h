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
typedef _status_t                           status_t;
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





status_t                                    rtosInitialize(void);
status_t                                    rtosStart(void);





threadId_t                                  threadGetId(void);
threadName_t                                threadGetName(threadId_t id);
threadState_t                               threadGetState(threadId_t id);
threadPriority_t                            threadGetPriority(threadId_t id);
status_t                                    threadSetPriority(threadId_t id, threadPriority_t priority);
threadStackSize_t                           threadGetStackSize(threadId_t id);
threadId_t                                  threadCreateNew(    threadName_t                name,
                                                                threadFunctionPointer_t     functionPointer,
                                                                threadFunctionParameter_t   functionParameter,
                                                                threadPriority_t            priority,
                                                                threadStackSize_t           stackSize
                                                                );
status_t                                    threadTerminate(threadId_t id);
status_t                                    threadYield(void);
status_t                                    threadSuspend(threadId_t id);
status_t                                    threadResume(threadId_t id);
status_t                                    threadWait(threadId_t id, kernelTick_t numberOfTicks);



#endif /* RTOS_INCLUDE_RTOS_H_ */
