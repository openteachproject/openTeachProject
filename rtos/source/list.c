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
 * list.c
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */



#include <rtos/include/list.h>
#include <rtos/include/kernel.h>



_kernelReadyNode_t* _listCreateNewReadyNode(_threadId_t id) {

    _kernelReadyNode_t                  *newNode;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        newNode = CircularQueueCreateNewNode(id, 0);
    }
    return newNode;
}

_kernelReadyList_t* _listCreateNewReadyList(void) {

    _kernelReadyList_t                  *newList;

    newList = CircularQueueCreateNewQueue();
    return newList;
}

_rtosStatus_t _listInsertToReadyList(_threadId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelReadyList_t                  *list;
    _kernelReadyNode_t                  *node;
    _kernelControlBlock_t               *kernel;
    _threadControlBlock_t               *thread;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> readyList[thread -> priority];
        node = thread -> readyNode;
        CircularQueueEnQueue(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromReadyList(_threadId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelReadyList_t                  *list;
    _kernelReadyNode_t                  *node;
    _kernelControlBlock_t               *kernel;
    _threadControlBlock_t               *thread;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> readyList[thread -> priority];
        node = thread -> readyNode;
        if (thread -> state == ThreadStateRunning || thread -> state == ThreadStateReady) {
            CircularQueueDeleteNode(list, node);
            returnValue = StatusOk;
        }
        else {
            returnValue = StatusErrorList;
        }
    }
    return returnValue;
}

_threadId_t _listGetFirstReadyList(_threadPriority_t priority) {

    _threadId_t                         firstThreadId;
    _kernelReadyList_t                  *list;
    _kernelReadyNode_t                  *node;
    _kernelControlBlock_t               *kernel;

    if (priority >= NumberOfPriorityLevels) {
        firstThreadId = NULL;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        list = kernel -> readyList[priority];
        if (CircularQueueGetSize(list) == 0) {
            firstThreadId = NULL;
        }
        else {
            node = CircularQueueHead(list);
            firstThreadId = (_threadId_t)(node -> element);
        }
    }
    return firstThreadId;
}

_threadId_t _listGetNextReadyList(_threadId_t id) {

    _threadId_t                         nextThreadId;
    _kernelReadyNode_t                  *node;
    _kernelReadyNode_t                  *nextNode;
    _threadControlBlock_t               *thread;

    if (id == NULL) {
        nextThreadId = NULL;
    }
    else {
        thread = (_threadControlBlock_t*)id;
        if (thread -> state == ThreadStateReady) {
            node = thread -> readyNode;
            nextNode = CircularQueueGetNextNode(node);
            nextThreadId = (_threadId_t)(nextNode -> element);
        }
        else {
            nextThreadId = NULL;
        }
    }
    return nextThreadId;
}

_rtosStatus_t _listPushIndexReadyList(_threadPriority_t priority) {

    _rtosStatus_t                       returnValue;
    _kernelReadyList_t                  *list;
    _kernelControlBlock_t               *kernel;

    if (priority >= NumberOfPriorityLevels) {
        returnValue = StatusErrorParameter;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        list = kernel -> readyList[priority];
        CircularQueuePushIndex(list);
        returnValue = StatusOk;
    }
    return returnValue;
}

_listSize_t _listGetSizeReadyList(_threadPriority_t priority) {

    _listSize_t                         listSize;
    _kernelReadyList_t                  *list;
    _kernelControlBlock_t               *kernel;

    if (priority >= NumberOfPriorityLevels) {
        listSize = 0;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        list = kernel -> readyList[priority];
        listSize = CircularQueueGetSize(list);
    }
    return listSize;
}

_bool_t _listIsEmptyReadyList(_threadPriority_t priority) {

    _bool_t                             returnValue;
    _kernelReadyList_t                  *list;
    _kernelControlBlock_t               *kernel;

    if (priority >= NumberOfPriorityLevels) {
        returnValue = false;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        list = kernel -> readyList[priority];
        if (CircularQueueGetSize(list) == 0) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}



_kernelWaitNode_t* _listCreateNewWaitNode(_threadId_t id) {

    _kernelWaitNode_t                    *newNode;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        newNode = QueueCreateNewNode(id, 0);
    }
    return newNode;
}

_kernelWaitList_t* _listCreateNewWaitList(void) {

    _kernelWaitList_t                    *newList;

    newList = QueueCreateNewQueue();
    return newList;
}

_rtosStatus_t _listInsertToWaitList(_threadId_t id, _kernelWaitListNumber_t waitListNumber) {

    _rtosStatus_t                        returnValue;
    _kernelWaitList_t                    *list;
    _kernelWaitNode_t                    *node;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL || waitListNumber >= NumberOfWaitLists) {
        returnValue = StatusErrorParameter;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> waitList[waitListNumber];
        node = thread -> waitNode;
        QueueEnqueue(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromWaitList(_threadId_t id, _kernelWaitListNumber_t waitListNumber) {

    _rtosStatus_t                        returnValue;
    _kernelWaitList_t                    *list;
    _kernelWaitNode_t                    *node;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL || waitListNumber >= NumberOfWaitLists) {
        returnValue = StatusErrorParameter;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> waitList[waitListNumber];
        node = thread -> waitNode;
        if (thread -> state == ThreadStateWaited) {
            QueueDeleteNode(list, node);
            returnValue = StatusOk;
        }
        else {
            returnValue = StatusErrorList;
        }
    }
    return returnValue;
}

_threadId_t _listGetFirstWaitList(_kernelWaitListNumber_t waitListNumber) {

    _threadId_t                          firstThreadId;
    _kernelWaitList_t                    *list;
    _kernelWaitNode_t                    *node;
    _kernelControlBlock_t                *kernel;

    if (waitListNumber >= NumberOfWaitLists) {
        firstThreadId = NULL;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        list = kernel -> waitList[waitListNumber];
        if (QueueGetSize(list) == 0) {
            firstThreadId = NULL;
        }
        else {
            node = QueueHead(list);
            firstThreadId = (_threadId_t)(node -> element);
        }
    }
    return firstThreadId;
}

_threadId_t _listGetNextWaitList(_kernelWaitListNumber_t waitListNumber, _threadId_t id) {

    _threadId_t                          nextThreadId;
    _kernelWaitList_t                    *list;
    _kernelWaitNode_t                    *node;
    _kernelWaitNode_t                    *nextNode;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (waitListNumber >= NumberOfWaitLists || id == NULL) {
        nextThreadId = NULL;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> waitList[waitListNumber];
        node = thread -> waitNode;
        if (thread -> state == ThreadStateWaited || thread -> state == ThreadStateReady) {
            if (QueueGetSize(list) == 1) {
                nextNode = node;
            }
            else {
                nextNode = QueueGetNextNode(node);
            }
            nextThreadId = (_threadId_t)(nextNode -> element);
        }
        else {
            nextThreadId = NULL;
        }
    }
    return nextThreadId;
}

_listSize_t _listGetTotalSizeWaitList(void) {

    _listSize_t                          listSize = 0;
    _kernelWaitList_t                    *list;
    _kernelControlBlock_t                *kernel;
    _kernelWaitListNumber_t              waitListNumber = 0;

    kernel = _kernelGetKernelControlBlock();
    while(waitListNumber < NumberOfWaitLists) {
        list = kernel -> waitList[waitListNumber];
        listSize = listSize + QueueGetSize(list);
        waitListNumber++;
    }
    return listSize;
}

_listSize_t _listGetSizeWaitList(_kernelWaitListNumber_t waitListNumber) {

    _listSize_t                          listSize;
    _kernelWaitList_t                    *list;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    list = kernel -> waitList[waitListNumber];
    listSize = QueueGetSize(list);
    return listSize;
}

_bool_t _listIsEmptyWaitList(_kernelWaitListNumber_t waitListNumber) {

    _bool_t                              returnValue;
    _kernelWaitList_t                    *list;
    _kernelControlBlock_t                *kernel;

    if (waitListNumber >= NumberOfWaitLists) {
        returnValue = false;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        list = kernel -> waitList[waitListNumber];
        if (QueueGetSize(list) == 0) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}



_semaphoreWaitNode_t* _listCreateNewSemaphoreWaitNode(_threadId_t id) {

    _semaphoreWaitNode_t                 *newNode;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        thread = (_threadControlBlock_t*)id;
        newNode = binaryTreeCreateNewNode(id, thread -> priority);
    }
    return newNode;
}

_semaphoreWaitList_t* _listCreateNewSemaphoreWaitList(void) {

    _semaphoreWaitList_t        *newList;

    newList = binaryTreeCreateNewTree();
    return newList;
}

_rtosStatus_t _listInsertToSemaphoreWaitList(_semaphoreId_t semaphoreId, _threadId_t threadId) {

    _rtosStatus_t                        returnValue;
    _semaphoreWaitList_t                 *list;
    _semaphoreWaitNode_t                 *node;
    _semaphoreControlBlock_t             *semaphore;
    _threadControlBlock_t                *thread;

    if (semaphoreId == NULL || threadId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        semaphore = (_semaphoreControlBlock_t*)semaphoreId;
        thread = (_threadControlBlock_t*)threadId;
        list = semaphore -> semaphoreWaitList;
        node = thread -> semaphoreWaitNode;
        binaryTreeInsert(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromSemaphoreWaitList(_semaphoreId_t semaphoreId, _threadId_t threadId) {

    _rtosStatus_t                        returnValue;
    _semaphoreWaitList_t                 *list;
    _semaphoreWaitNode_t                 *node;
    _semaphoreControlBlock_t             *semaphore;
    _threadControlBlock_t                *thread;

    if (semaphoreId == NULL || threadId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        semaphore = (_semaphoreControlBlock_t*)semaphoreId;
        thread = (_threadControlBlock_t*)threadId;
        list = semaphore -> semaphoreWaitList;
        node = thread -> semaphoreWaitNode;
        if (thread -> semaphoreWaitingFor == NULL) {
            returnValue = StatusErrorList;
        }
        else {
            binaryTreeDelete(list, node);
            returnValue = StatusOk;
        }
    }
    return returnValue;
}

_threadId_t _listGetMinSemaphoreWaitList(_semaphoreId_t id) {

    _threadId_t                          minThreadId;
    _semaphoreWaitList_t                 *list;
    _semaphoreWaitNode_t                 *node;
    _semaphoreControlBlock_t             *semaphore;

    if (id == NULL) {
        minThreadId = NULL;
    }
    else {
        semaphore = (_semaphoreControlBlock_t*)id;
        list = semaphore -> semaphoreWaitList;
        if (binaryTreeGetSize(list) == 0) {
            minThreadId = NULL;
        }
        else {
            node = binaryTreeMinimum(list, list -> rootNode);
            minThreadId = (_threadId_t)(node -> element);
        }
    }
    return minThreadId;
}

_threadId_t _listGetMaxSemaphoreWaitList(_semaphoreId_t id) {

    _threadId_t                          maxThreadId;
    _semaphoreWaitList_t                 *list;
    _semaphoreWaitNode_t                 *node;
    _semaphoreControlBlock_t             *semaphore;

    if (id == NULL) {
        maxThreadId = NULL;
    }
    else {
        semaphore = (_semaphoreControlBlock_t*)id;
        list = semaphore -> semaphoreWaitList;
        if (binaryTreeGetSize(list) == 0) {
            maxThreadId = NULL;
        }
        else {
            node = binaryTreeMaximum(list, list -> rootNode);
            maxThreadId = (_threadId_t)(node -> element);
        }
    }
    return maxThreadId;
}

_listSize_t _listGetSizeSemaphoreWaitList(_semaphoreId_t id) {

    _listSize_t                          listSize;
    _semaphoreWaitList_t                 *list;
    _semaphoreControlBlock_t             *semaphore;

    if (id == NULL) {
        listSize = 0;
    }
    else {
        semaphore = (_semaphoreControlBlock_t*)id;
        list = semaphore -> semaphoreWaitList;
        listSize = binaryTreeGetSize(list);
    }
    return listSize;
}

_bool_t _listIsEmptySemaphoreWaitList(_semaphoreId_t id) {

    _bool_t                              returnValue;
    _semaphoreWaitList_t                 *list;
    _semaphoreControlBlock_t             *semaphore;

    if (id == NULL) {
        returnValue = false;
    }
    else {
        semaphore = (_semaphoreControlBlock_t*)id;
        list = semaphore -> semaphoreWaitList;
        if (binaryTreeGetSize(list) == 0) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}



_mutexWaitNode_t* _listCreateNewMutexWaitNode(_threadId_t id) {

    _mutexWaitNode_t                     *newNode;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        thread = (_threadControlBlock_t*)id;
        newNode = binaryTreeCreateNewNode(id, thread -> priority);
    }
    return newNode;
}

_mutexWaitList_t* _listCreateNewMutexWaitList(void) {

    _mutexWaitList_t                     *newList;

    newList = binaryTreeCreateNewTree();
    return newList;
}

_rtosStatus_t _listInsertToMutexWaitList(_mutexId_t mutexId, _threadId_t threadId) {

    _rtosStatus_t                        returnValue;
    _mutexWaitList_t                     *list;
    _mutexWaitNode_t                     *node;
    _mutexControlBlock_t                 *mutex;
    _threadControlBlock_t                *thread;

    if (mutexId == NULL || threadId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        mutex = (_mutexControlBlock_t*)mutexId;
        thread = (_threadControlBlock_t*)threadId;
        list = mutex -> mutexWaitList;
        node = thread -> mutexWaitNode;
        binaryTreeInsert(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromMutexWaitList(_mutexId_t mutexId, _threadId_t threadId) {

    _rtosStatus_t                        returnValue;
    _mutexWaitList_t                     *list;
    _mutexWaitNode_t                     *node;
    _mutexControlBlock_t                 *mutex;
    _threadControlBlock_t                *thread;

    if (mutexId == NULL || threadId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        mutex = (_mutexControlBlock_t*)mutexId;
        thread = (_threadControlBlock_t*)threadId;
        list = mutex -> mutexWaitList;
        node = thread -> mutexWaitNode;
        if (thread -> mutexWaitingFor == NULL) {
            returnValue = StatusErrorList;
        }
        else {
            binaryTreeDelete(list, node);
            returnValue = StatusOk;
        }
    }
    return returnValue;
}

_threadId_t _listGetMinMutexWaitList(_mutexId_t id) {

    _threadId_t                          minThreadId;
    _mutexWaitList_t                     *list;
    _mutexWaitNode_t                     *node;
    _mutexControlBlock_t                 *mutex;

    if (id == NULL) {
        minThreadId = NULL;
    }
    else {
        mutex = (_mutexControlBlock_t*)id;
        list = mutex -> mutexWaitList;
        if (binaryTreeGetSize(list) == 0) {
            minThreadId = NULL;
        }
        else {
            node = binaryTreeMinimum(list, list -> rootNode);
            minThreadId = (_threadId_t)(node -> element);
        }
    }
    return minThreadId;
}

_threadId_t _listGetMaxMutexWaitList(_mutexId_t id) {

    _threadId_t                          maxThreadId;
    _mutexWaitList_t                     *list;
    _mutexWaitNode_t                     *node;
    _mutexControlBlock_t                 *mutex;

    if (id == NULL) {
        maxThreadId = NULL;
    }
    else {
        mutex = (_mutexControlBlock_t*)id;
        list = mutex -> mutexWaitList;
        if (binaryTreeGetSize(list) == 0) {
            maxThreadId = NULL;
        }
        else {
            node = binaryTreeMaximum(list, list -> rootNode);
            maxThreadId = (_threadId_t)(node -> element);
        }
    }
    return maxThreadId;
}

_listSize_t _listGetSizeMutexWaitList(_mutexId_t id) {

    _listSize_t                          listSize;
    _mutexWaitList_t                     *list;
    _mutexControlBlock_t                 *mutex;

    if (id == NULL) {
        listSize = 0;
    }
    else {
        mutex = (_mutexControlBlock_t*)id;
        list = mutex -> mutexWaitList;
        listSize = binaryTreeGetSize(list);
    }
    return listSize;
}

_bool_t _listIsEmptyMutexWaitList(_mutexId_t id) {

    _bool_t                              returnValue;
    _mutexWaitList_t                     *list;
    _mutexControlBlock_t                 *mutex;

    if (id == NULL) {
        returnValue = false;
    }
    else {
        mutex = (_mutexControlBlock_t*)id;
        list = mutex -> mutexWaitList;
        if (binaryTreeGetSize(list) == 0) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}



_threadOwnedMutexNode_t* _listCreateNewThreadOwnedMutexNode(_mutexId_t id) {

    _threadOwnedMutexNode_t              *newNode;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        newNode = QueueCreateNewNode(id, 0);
    }
    return newNode;
}

_threadOwnedMutexList_t* _listCreateNewThreadOwnedMutexList(void) {

    _threadOwnedMutexList_t              *newList;

    newList = QueueCreateNewQueue();
    return newList;
}

_rtosStatus_t _listInsertToThreadOwnedMutexList(_threadId_t threadId, _mutexId_t mutexId) {

    _rtosStatus_t                        returnValue;
    _threadOwnedMutexList_t              *list;
    _threadOwnedMutexNode_t              *node;
    _threadControlBlock_t                *thread;
    _mutexControlBlock_t                 *mutex;

    if (threadId == NULL || mutexId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        thread = (_threadControlBlock_t*)threadId;
        mutex = (_mutexControlBlock_t*)mutexId;
        list = thread -> threadOwnedMutexList;
        node = mutex -> threadOwnedMutexNode;
        QueueEnqueue(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromThreadOwnedMutexList(_threadId_t threadId, _mutexId_t mutexId) {

    _rtosStatus_t                        returnValue;
    _threadOwnedMutexList_t              *list;
    _threadOwnedMutexNode_t              *node;
    _threadControlBlock_t                *thread;
    _mutexControlBlock_t                 *mutex;

    if (threadId == NULL || mutexId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        thread = (_threadControlBlock_t*)threadId;
        mutex = (_mutexControlBlock_t*)mutexId;
        list = thread -> threadOwnedMutexList;
        node = mutex -> threadOwnedMutexNode;
        if (mutex -> owner == threadId) {
            QueueDeleteNode(list, node);
            returnValue = StatusOk;
        }
        else {
            returnValue = StatusErrorList;
        }
    }
    return returnValue;
}

_mutexId_t _listGetFirstThreadOwnedMutexList(_threadId_t id) {

    _mutexId_t                           firstMutexId;
    _threadOwnedMutexList_t              *list;
    _threadOwnedMutexNode_t              *node;
    _threadControlBlock_t                *thread;

    if ( id == NULL) {
        firstMutexId = NULL;
    }
    else {
        thread = (_threadControlBlock_t*)id;
        list = thread -> threadOwnedMutexList;
        if (QueueGetSize(list) == 0) {
            firstMutexId = NULL;
        }
        else {
            node = QueueHead(list);
            firstMutexId = (_mutexId_t)(node -> element);
        }
    }
    return firstMutexId;
}

_mutexId_t _listGetNextThreadOwnedMutexList(_threadId_t threadId, _mutexId_t mutexId) {

    _mutexId_t                           nextMutexId;
    _threadOwnedMutexList_t              *list;
    _threadOwnedMutexNode_t              *node;
    _threadOwnedMutexNode_t              *nextNode;
    _threadControlBlock_t                *thread;
    _mutexControlBlock_t                 *mutex;

    if (threadId == NULL || mutexId == NULL) {
        nextMutexId = NULL;
    }
    else {
        thread = (_threadControlBlock_t*)threadId;
        mutex = (_mutexControlBlock_t*)mutexId;
        list = thread -> threadOwnedMutexList;
        node = mutex -> threadOwnedMutexNode;
        if (mutex -> owner == threadId) {
            if (QueueGetSize(list) == 1) {
                nextNode = node;
            }
            else {
                nextNode = QueueGetNextNode(node);
            }
            nextMutexId = (_mutexId_t)(nextNode -> element);
        }
        else {
            mutexId = NULL;
        }
    }
    return nextMutexId;
}

_listSize_t _listGetSizeThreadOwnedMutexList(_threadId_t id) {

    _listSize_t                          listSize;
    _threadOwnedMutexList_t              *list;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        listSize = 0;
    }
    else {
        thread = (_threadId_t)id;
        list = thread -> threadOwnedMutexList;
        listSize = QueueGetSize(list);
    }
    return listSize;
}

_bool_t _listIsEmptyThreadOwnedMutexList(_threadId_t id) {

    _bool_t                              returnValue;
    _threadOwnedMutexList_t              *list;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        returnValue = false;
    }
    else {
        thread = (_threadId_t)id;
        list = thread -> threadOwnedMutexList;
        if (QueueGetSize(list) == 0) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}

_kernelSuspendedNode_t* _listCreateNewSuspendedNode(_threadId_t id) {

    _kernelSuspendedNode_t               *newNode;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        newNode = QueueCreateNewNode(id, 0);
    }
    return newNode;
}

_kernelSuspendedList_t* _listCreateNewSuspendedList(void) {

    _kernelSuspendedList_t               *newList;

    newList = QueueCreateNewQueue();
    return newList;
}

_rtosStatus_t _listInsertToSuspendedList(_threadId_t id) {

    _rtosStatus_t                        returnValue;
    _kernelSuspendedList_t               *list;
    _kernelSuspendedNode_t               *node;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> suspendedList;
        node = thread -> suspendedNode;
        QueueEnqueue(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromSuspendedList(_threadId_t id) {

    _rtosStatus_t                        returnValue;
    _kernelSuspendedList_t               *list;
    _kernelSuspendedNode_t               *node;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        returnValue = StatusOk;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> suspendedList;
        node = thread -> suspendedNode;
        if (thread -> state == ThreadStateSuspended) {
            QueueDeleteNode(list, node);
            returnValue = StatusOk;
        }
        else {
            returnValue = StatusErrorList;
        }
    }
    return returnValue;
}

_threadId_t _listGetFirstSuspendedList(void) {

    _threadId_t                          firstThreadId;
    _kernelSuspendedList_t               *list;
    _kernelSuspendedNode_t               *node;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    list = kernel -> suspendedList;
    if (QueueGetSize(list) == 0) {
        firstThreadId = NULL;
    }
    else {
        node = QueueHead(list);
        firstThreadId = (_threadId_t)(node -> element);
    }
    return firstThreadId;
}

_threadId_t _listGetNextSuspendedList(_threadId_t id) {

    _threadId_t                          nextThreadId;
    _kernelSuspendedList_t               *list;
    _kernelSuspendedNode_t               *node;
    _kernelSuspendedNode_t               *nextNode;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        nextThreadId = NULL;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> suspendedList;
        node = thread -> suspendedNode;
        if (thread -> state == ThreadStateSuspended) {
            if (QueueGetSize(list) == 1) {
                nextNode = node;
            }
            else {
                nextNode = QueueGetNextNode(node);
            }
            nextThreadId = (_threadId_t)(nextNode -> element);
        }
        else {
            nextThreadId = NULL;
        }
    }
    return nextThreadId;
}

_listSize_t _listGetSizeSuspendedList(void) {

    _listSize_t                          listSize;
    _kernelSuspendedList_t               *list;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    list = kernel -> suspendedList;
    listSize = QueueGetSize(list);
    return listSize;
}

_bool_t _listIsEmptySuspendedList(void) {

    _bool_t                              returnValue;
    _kernelSuspendedList_t               *list;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    list = kernel -> suspendedList;
    if (QueueGetSize(list) == 0) {
        returnValue = true;
    }
    else {
        returnValue = false;
    }
    return returnValue;
}

_kernelTerminatedNode_t* _listCreateNewTerminatedNode(_threadId_t id) {

    _kernelTerminatedNode_t              *newNode;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        newNode = QueueCreateNewNode(id, 0);
    }
    return newNode;
}

_kernelTerminatedList_t* _listCreateNewTerminatedList(void) {

    _kernelTerminatedList_t              *newList;

    newList = QueueCreateNewQueue();
    return newList;
}

_rtosStatus_t _listInsertToTerminatedList(_threadId_t id) {

    _rtosStatus_t                        returnValue;
    _kernelTerminatedList_t              *list;
    _kernelTerminatedNode_t              *node;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> terminatedList;
        node = thread -> terminatedNode;
        QueueEnqueue(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromTerminatedList(_threadId_t id) {

    _rtosStatus_t                        returnValue;
    _kernelTerminatedList_t              *list;
    _kernelTerminatedNode_t              *node;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        returnValue = StatusOk;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> terminatedList;
        node = thread -> terminatedNode;
        if (thread -> state == ThreadStateTerminated) {
            QueueDeleteNode(list, node);
            returnValue = StatusOk;
        }
        else {
            returnValue = StatusErrorList;
        }
    }
    return returnValue;
}

_threadId_t _listGetFirstTerminatedList(void) {

    _threadId_t                          firstThreadId;
    _kernelTerminatedList_t              *list;
    _kernelTerminatedNode_t              *node;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    list = kernel -> terminatedList;
    if (QueueGetSize(list) == 0) {
        firstThreadId = NULL;
    }
    else {
        node = QueueHead(list);
        firstThreadId = (_threadId_t)(node -> element);
    }
    return firstThreadId;
}

_threadId_t _listGetNextTerminatedList(_threadId_t id) {

    _threadId_t                          nextThreadId;
    _kernelTerminatedList_t              *list;
    _kernelTerminatedNode_t              *node;
    _kernelTerminatedNode_t              *nextNode;
    _kernelControlBlock_t                *kernel;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        nextThreadId = NULL;
    }
    else {
        kernel = _kernelGetKernelControlBlock();
        thread = (_threadControlBlock_t*)id;
        list = kernel -> terminatedList;
        node = thread -> terminatedNode;
        if (thread -> state == ThreadStateTerminated) {
            if (QueueGetSize(list) == 1) {
                nextNode = node;
            }
            else {
                nextNode = QueueGetNextNode(node);
            }
            nextThreadId = (_threadId_t)(nextNode -> element);
        }
        else {
            nextThreadId = NULL;
        }
    }
    return nextThreadId;
}

_listSize_t _listGetSizeTerminatedList(void) {

    _listSize_t                          listSize;
    _kernelTerminatedList_t              *list;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    list = kernel -> terminatedList;
    listSize = QueueGetSize(list);
    return listSize;
}

_bool_t _listIsEmptyTerminatedList(void) {

    _bool_t                              returnValue;
    _kernelTerminatedList_t              *list;
    _kernelControlBlock_t                *kernel;

    kernel = _kernelGetKernelControlBlock();
    list = kernel -> terminatedList;
    if (QueueGetSize(list) == 0) {
        returnValue = true;
    }
    else {
        returnValue = false;
    }
    return returnValue;
}



_memPoolWaitNode_t* _listCreateNewMemPoolWaitNode(_threadId_t id) {

    _memPoolWaitNode_t                   *newNode;
    _threadControlBlock_t                *thread;

    if (id == NULL) {
        newNode = NULL;
    }
    else {
        thread = (_threadControlBlock_t*)id;
        newNode = binaryTreeCreateNewNode(id, thread -> priority);
    }
    return newNode;
}

_memPoolWaitList_t* _listCreateNewMemPoolWaitList(void) {

    _memPoolWaitList_t                   *newList;

    newList = binaryTreeCreateNewTree();
    return newList;
}

_rtosStatus_t _listInsertToMemPoolWaitList(_memPoolId_t memPoolId, _threadId_t threadId) {

    _rtosStatus_t                        returnValue;
    _memPoolWaitList_t                   *list;
    _memPoolWaitNode_t                   *node;
    _memPoolControlBlock_t               *memPool;
    _threadControlBlock_t                *thread;

    if (memPoolId == NULL || threadId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        memPool = (_memPoolControlBlock_t*)memPoolId;
        thread = (_threadControlBlock_t*)threadId;
        list = memPool -> memPoolWaitList;
        node = thread -> memPoolWaitNode;
        binaryTreeInsert(list, node);
        returnValue = StatusOk;
    }
    return returnValue;
}

_rtosStatus_t _listDeleteFromMemPoolWaitList(_memPoolId_t memPoolId, _threadId_t threadId) {

    _rtosStatus_t                        returnValue;
    _memPoolWaitList_t                   *list;
    _memPoolWaitNode_t                   *node;
    _memPoolControlBlock_t               *memPool;
    _threadControlBlock_t                *thread;

    if (memPoolId == NULL || threadId == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {
        memPool = (_memPoolControlBlock_t*)memPoolId;
        thread = (_threadControlBlock_t*)threadId;
        list = memPool -> memPoolWaitList;
        node = thread -> memPoolWaitNode;
        if (thread -> memPoolWaitingFor == NULL) {
            returnValue = StatusErrorList;
        }
        else {
            binaryTreeDelete(list, node);
            returnValue = StatusOk;
        }
    }
    return returnValue;
}

_threadId_t _listGetMinMemPoolWaitList(_memPoolId_t id) {

    _threadId_t                          minThreadId;
    _memPoolWaitList_t                   *list;
    _memPoolWaitNode_t                   *node;
    _memPoolControlBlock_t               *memPool;

    if (id == NULL) {
        minThreadId = NULL;
    }
    else {
        memPool = (_memPoolControlBlock_t*)id;
        list = memPool -> memPoolWaitList;
        if (binaryTreeGetSize(list) == 0) {
            minThreadId = NULL;
        }
        else {
            node = binaryTreeMinimum(list, list -> rootNode);
            minThreadId = (_threadId_t)(node -> element);
        }
    }
    return minThreadId;
}

_threadId_t _listGetMaxMemPoolWaitList(_memPoolId_t id) {

    _threadId_t                          maxThreadId;
    _memPoolWaitList_t                   *list;
    _memPoolWaitNode_t                   *node;
    _memPoolControlBlock_t               *memPool;

    if (id == NULL) {
        maxThreadId = NULL;
    }
    else {
        memPool = (_memPoolControlBlock_t*)id;
        list = memPool -> memPoolWaitList;
        if (binaryTreeGetSize(list) == 0) {
            maxThreadId = NULL;
        }
        else {
            node = binaryTreeMaximum(list, list -> rootNode);
            maxThreadId = (_threadId_t)(node -> element);
        }
    }
    return maxThreadId;
}

_listSize_t _listGetSizeMemPoolWaitList(_memPoolId_t id) {

    _listSize_t                          listSize;
    _memPoolWaitList_t                   *list;
    _memPoolControlBlock_t               *memPool;

    if (id == NULL) {
        listSize = 0;
    }
    else {
        memPool = (_memPoolControlBlock_t*)id;
        list = memPool -> memPoolWaitList;
        listSize = binaryTreeGetSize(list);
    }
    return listSize;
}

_bool_t _listIsEmptyMemPoolWaitList(_memPoolId_t id) {

    _bool_t                              returnValue;
    _memPoolWaitList_t                   *list;
    _memPoolControlBlock_t               *memPool;

    if (id == NULL) {
        returnValue = false;
    }
    else {
        memPool = (_memPoolControlBlock_t*)id;
        list = memPool -> memPoolWaitList;
        if (binaryTreeGetSize(list) == 0) {
            returnValue = true;
        }
        else {
            returnValue = false;
        }
    }
    return returnValue;
}

