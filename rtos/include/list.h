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
 * list.h
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */



#ifndef RTOS_INCLUDE_LIST_H_
#define RTOS_INCLUDE_LIST_H_



#include <rtos/include/rtosTypeDefinitions.h>



_kernelReadyNode_t                     *_listCreateNewReadyNode(_threadId_t id);
_kernelReadyList_t                     *_listCreateNewReadyList(void);
_rtosStatus_t                           _listInsertToReadyList(_threadId_t id);
_rtosStatus_t                           _listDeleteFromReadyList(_threadId_t id);
_threadId_t                             _listGetFirstReadyList(_threadPriority_t priority);
_threadId_t                             _listGetNextReadyList(_threadId_t id);
_rtosStatus_t                           _listPushIndexReadyList(_threadPriority_t priority);
_listSize_t                             _listGetSizeReadyList(_threadPriority_t priority);
_bool_t                                 _listIsEmptyReadyList(_threadPriority_t priority);



_kernelWaitNode_t                      *_listCreateNewWaitNode(_threadId_t id);
_kernelWaitList_t                      *_listCreateNewWaitList(void);
_rtosStatus_t                           _listInsertToWaitList(_threadId_t id, _kernelWaitListNumber_t waitListNumber);
_rtosStatus_t                           _listDeleteFromWaitList(_threadId_t id, _kernelWaitListNumber_t waitListNumber);
_threadId_t                             _listGetFirstWaitList(_kernelWaitListNumber_t waitListNumber);
_threadId_t                             _listGetNextWaitList(_kernelWaitListNumber_t waitListNumber, _threadId_t id);
_listSize_t                             _listGetTotalSizeOfWaitLists(void);
_listSize_t                             _listGetSizeWaitList(_kernelWaitListNumber_t waitListNumber);
_bool_t                                 _listIsEmptyWaitList(_kernelWaitListNumber_t waitListNumber);



_semaphoreWaitNode_t                   *_listCreateNewSemaphoreWaitNode(_threadId_t id);
_semaphoreWaitList_t                   *_listCreateNewSemaphoreWaitList(void);
_rtosStatus_t                           _listInsertToSemaphoreWaitList(_semaphoreId_t semaphoreId, _threadId_t threadId);
_rtosStatus_t                           _listDeleteFromSemaphoreWaitList(_semaphoreId_t semaphoreId, _threadId_t threadId);
_threadId_t                             _listGetMinSemaphoreWaitList(_semaphoreId_t id);
_threadId_t                             _listGetMaxSemaphoreWaitList(_semaphoreId_t id);
_listSize_t                             _listGetSizeSemaphoreWaitList(_semaphoreId_t id);
_bool_t                                 _listIsEmptySemaphoreWaitList(_semaphoreId_t id);



_mutexWaitNode_t                       *_listCreateNewMutexWaitNode(_threadId_t id);
_mutexWaitList_t                       *_listCreateNewMutexWaitList(void);
_rtosStatus_t                           _listInsertToMutexWaitList(_mutexId_t mutexId, _threadId_t threadId);
_rtosStatus_t                           _listDeleteFromMutexWaitList(_mutexId_t mutexId, _threadId_t threadId);
_threadId_t                             _listGetMinMutexWaitList(_mutexId_t id);
_threadId_t                             _listGetMaxMutexWaitList(_mutexId_t id);
_listSize_t                             _listGetSizeMutexWaitList(_mutexId_t id);
_bool_t                                 _listIsEmptyMutexWaitList(_mutexId_t id);



_threadOwnedMutexNode_t                *_listCreateNewThreadOwnedMutexNode(_mutexId_t id);
_threadOwnedMutexList_t                *_listCreateNewThreadOwnedMutexList(void);
_rtosStatus_t                           _listInsertToThreadOwnedMutexList(_threadId_t threadId, _mutexId_t mutexId);
_rtosStatus_t                           _listDeleteFromThreadOwnedMutexList(_threadId_t threadId, _mutexId_t mutexId);
_mutexId_t                              _listGetFirstThreadOwnedMutexList(_threadId_t id);
_mutexId_t                              _listGetNextThreadOwnedMutexList(_threadId_t threadId, _mutexId_t mutexId);
_listSize_t                             _listGetSizeThreadOwnedMutexList(_threadId_t id);
_bool_t                                 _listIsEmptyThreadOwnedMutexList(_threadId_t id);



_kernelSuspendedNode_t                 *_listCreateNewSuspendedNode(_threadId_t id);
_kernelSuspendedList_t                 *_listCreateNewSuspendedList(void);
_rtosStatus_t                           _listInsertToSuspendedList(_threadId_t id);
_rtosStatus_t                           _listDeleteFromSuspendedList(_threadId_t id);
_threadId_t                             _listGetFirstSuspendedList(void);
_threadId_t                             _listGetNextSuspendedList(_threadId_t id);
_listSize_t                             _listGetSizeSuspendedList(void);
_bool_t                                 _listIsEmptySuspendedList(void);



_kernelTerminatedNode_t                *_listCreateNewTerminatedNode(_threadId_t id);
_kernelTerminatedList_t                *_listCreateNewTerminatedList(void);
_rtosStatus_t                           _listInsertToTerminatedList(_threadId_t id);
_rtosStatus_t                           _listDeleteFromTerminatedList(_threadId_t id);
_threadId_t                             _listGetFirstTerminatedList(void);
_threadId_t                             _listGetNextTerminatedList(_threadId_t id);
_listSize_t                             _listGetSizeTerminatedList(void);
_bool_t                                 _listIsEmptyTerminatedList(void);



_memPoolWaitNode_t                     *_listCreateNewMemPoolWaitNode(_threadId_t id);
_memPoolWaitList_t                     *_listCreateNewMemPoolWaitList(void);
_rtosStatus_t                           _listInsertToMemPoolWaitList(_memPoolId_t memPoolId, _threadId_t threadId);
_rtosStatus_t                           _listDeleteFromMemPoolWaitList(_memPoolId_t memPoolId, _threadId_t threadId);
_threadId_t                             _listGetMinMemPoolWaitList(_memPoolId_t id);
_threadId_t                             _listGetMaxMemPoolWaitList(_memPoolId_t id);
_listSize_t                             _listGetSizeMemPoolWaitList(_memPoolId_t id);
_bool_t                                 _listIsEmptyMemPoolWaitList(_memPoolId_t id);



#endif /* RTOS_INCLUDE_LIST_H_ */
