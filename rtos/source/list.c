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

_status_t _listInsertToReadyList(_threadId_t id) {

    _status_t                           returnValue;
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

_status_t _listDeleteFromReadyList(_threadId_t id) {

    _status_t                           returnValue;
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

_status_t _listPushIndexReadyList(_threadPriority_t priority) {

    _status_t                           returnValue;
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
