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
_status_t                               _listInsertToReadyList(_threadId_t id);
_status_t                               _listDeleteFromReadyList(_threadId_t id);
_threadId_t                             _listGetFirstReadyList(_threadPriority_t priority);
_threadId_t                             _listGetNextReadyList(_threadId_t id);
_status_t                               _listPushIndexReadyList(_threadPriority_t priority);
_listSize_t                             _listGetSizeReadyList(_threadPriority_t priority);
_bool_t                                 _listIsEmptyReadyList(_threadPriority_t priority);



_kernelWaitNode_t                      *_listCreateNewWaitNode(_threadId_t id);
_kernelWaitList_t                      *_listCreateNewWaitList(void);
_status_t                               _listInsertToWaitList(_threadId_t id, _kernelWaitListNumber_t waitListNumber);
_status_t                               _listDeleteFromWaitList(_threadId_t id, _kernelWaitListNumber_t waitListNumber);
_threadId_t                             _listGetFirstWaitList(_kernelWaitListNumber_t waitListNumber);
_threadId_t                             _listGetNextWaitList(_kernelWaitListNumber_t waitListNumber, _threadId_t id);
_listSize_t                             _listGetTotalSizeOfWaitLists(void);
_listSize_t                             _listGetSizeWaitList(_kernelWaitListNumber_t waitListNumber);
_bool_t                                 _listIsEmptyWaitList(_kernelWaitListNumber_t waitListNumber);



_semaphoreWaitNode_t                   *_listCreateNewSemaphoreWaitNode(_threadId_t id);
_semaphoreWaitList_t                   *_listCreateNewSemaphoreWaitList(void);
_status_t                               _listInsertToSemaphoreWaitList(_semaphoreId_t semaphoreId, _threadId_t threadId);
_status_t                               _listDeleteFromSemaphoreWaitList(_semaphoreId_t semaphoreId, _threadId_t threadId);
_threadId_t                             _listGetMinSemaphoreWaitList(_semaphoreId_t id);
_threadId_t                             _listGetMaxSemaphoreWaitList(_semaphoreId_t id);
_listSize_t                             _listGetSizeSemaphoreWaitList(_semaphoreId_t id);
_bool_t                                 _listIsEmptySemaphoreWaitList(_semaphoreId_t id);



#endif /* RTOS_INCLUDE_LIST_H_ */
