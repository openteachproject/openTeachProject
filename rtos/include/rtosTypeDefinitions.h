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
 * rtosTypeDefinitions.h
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */



#ifndef RTOS_INCLUDE_RTOSTYPEDEFINITIONS_H_
#define RTOS_INCLUDE_RTOSTYPEDEFINITIONS_H_



#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>



#include <dataStructure/include/queue.h>
#include <dataStructure/include/circularQueue.h>
#include <dataStructure/include/binaryTree.h>



#define KernelMajorVersion                  0
#define KernelMinorVersion                  0
#define KernelPatchVersion                  1

#define SystemCallListLength                4

#define ThreadNameMaxLength                 32

#define NumberOfWaitLists                   8

#define ThreadDefaultStackSize              1024
#define ThreadMaximumStackSize              32 * 1024

#define SemaphoreNameMaxLength              32





enum _kernelState
{
    KernelStateInitialized                  = 0,
    KernelStateRunning                      = 1,
    KernelStateStoped                       = 2,
    KernelStateError                        = 0xFFFFFFFF
};



enum _threadPriority
{
    PriorityIdle                            = 0,
    PriorityLow                             = 1,
    PriorityBelowNormal                     = 2,
    PriorityNormal                          = 3,
    PriorityAboveNormal                     = 4,
    PriorityHigh                            = 5,
    PriorityRealTime                        = 6,
    PriorityISR                             = 7,
    NumberOfPriorityLevels                  = 8,
    PriorityError                           = 0xFFFFFFFF
};



enum _kernelRequest
{
    KernelRequestSet                        = 1,
    KernelRequestUnSet                      = 0,
    KernelRequestError                      = 0xFFFFFFFF
};



enum _status
{
    StatusOk                                = 0,
    StatusError                             = 1,
    StatusErrorList                         = 2,
    StatusErrorInerrupt                     = 3,
    StatusErrorParameter                    = 4,
    StatusErrorHeapMemory                   = 5,
    StatusErrorKernelResource               = 6,
    StatusErrorThreadState                  = 7,
    StatusErrorResource                     = 8,
    StatusErrorTimeOut                      = 9,
    StatusWait                              = 10,
    StatusErrorKernelState                  = 0xFFFFFFFF
};



enum _atomicResult
{
    AtomicWriteSuccessfull                  = 0,
    AtomicWriteUnsuccessfull                = 1,
    AtomicWriteError                        = 0xFFFFFFFF
};



enum _threadState
{
    ThreadStateRunning                      = 0,
    ThreadStateReady                        = 1,
    ThreadStateWaited                       = 2,
    ThreadStateError                        = 0xFFFFFFFF
};



enum _returnedByRelease
{
    ReturnedByReleaseUnset                  = 0,
    ReturnedByReleaseSet                    = 1,
    ReturnedByReleaseError                  = 0xFFFFFFFF
};




typedef struct _kernelControlBlock          _kernelControlBlock_t;
typedef void*                               _kernelId_t;
typedef struct _kernelVersion               _kernelVersion_t;
typedef uint32_t                            _kernelVersionPart_t;
typedef enum _kernelState                   _kernelState_t;
typedef uint64_t                            _kernelTick_t;
typedef struct _kernelSystemCallArg         _kernelSystemCallArg_t;
typedef uint32_t                            _kernelSystemCallListIndex_t;
typedef uint32_t                            _kernelSystemCallNumber_t;
typedef circularQueue_t                     _kernelReadyList_t;
typedef circularQueueNode_t                 _kernelReadyNode_t;
typedef queue_t                             _kernelWaitList_t;
typedef queueNode_t                         _kernelWaitNode_t;
typedef uint32_t                            _kernelWaitListNumber_t;
typedef enum _kernelRequest                 _kernelRequest_t;
typedef enum _status                        _status_t;
typedef bool                                _bool_t;
typedef uint32_t                            _listSize_t;
typedef uint32_t                            _listKey_t;
typedef uint32_t                            _atomicValue_t;
typedef uint32_t*                           _atomicAddress_t;
typedef enum _atomicResult                  _atomicResult_t;
typedef enum _returnedByRelease             _returnedByRelease_t;



typedef struct _threadControlBlock          _threadControlBlock_t;
typedef void*                               _threadId_t;
typedef char*                               _threadName_t;
typedef enum _threadState                   _threadState_t;
typedef void*                               _threadFunctionParameter_t;
typedef void                                (*_threadFunctionPointer_t)(_threadFunctionParameter_t parameter);
typedef enum _threadPriority                _threadPriority_t;
typedef uint32_t                            _threadStackSize_t;
typedef uint8_t*                            _threadStackPointer_t;



typedef struct _semaphoreControlBlock       _semaphoreControlBlock_t;
typedef void*                               _semaphoreId_t;
typedef char*                               _semaphoreName_t;
typedef uint32_t                            _semaphoreCount_t;
typedef binaryTree_t                        _semaphoreWaitList_t;
typedef binaryTreeNode_t                    _semaphoreWaitNode_t;





struct _kernelVersion {
    _kernelVersionPart_t                    major;
    _kernelVersionPart_t                    minor;
    _kernelVersionPart_t                    patch;
};

struct _kernelSystemCallArg {
    _kernelSystemCallNumber_t               systemCallNumber;
    void*                                   specificSystemCallArg;
};

struct _kernelControlBlock {
    _kernelVersion_t                        version;
    _kernelState_t                          state;
    _kernelTick_t                           tick;
    _threadId_t                             currentRunningThreadId;
    _kernelSystemCallArg_t                  **systemCallList;
    _kernelSystemCallListIndex_t            systemCallListIndex;
    _kernelReadyList_t                      *readyList[NumberOfPriorityLevels];
    _kernelWaitList_t                       *waitList[NumberOfWaitLists];
    _kernelRequest_t                        systemCallHandlerRequest;
    _kernelRequest_t                        contextSwitchRequest;
    _kernelRequest_t                        waitListHandlerRequest;
};



struct _threadControlBlock {
    _threadId_t                             id;
    _threadName_t                           name;
    _threadState_t                          state;
    _threadFunctionParameter_t              functionParameter;
    _threadFunctionPointer_t                functionPointer;
    _threadPriority_t                       priority;
    _threadStackSize_t                      stackSize;
    _threadStackPointer_t                   stackTop;
    _threadStackPointer_t                   stackPointer;
    _kernelReadyNode_t                      *readyNode;
    _kernelWaitNode_t                       *waitNode;
    _kernelTick_t                           returnTick;
    _semaphoreWaitNode_t                    *semaphoreWaitNode;
    _semaphoreId_t                          semaphoreWaitingFor;
    _returnedByRelease_t                    returnedByRelease;
};



struct _semaphoreControlBlock {
    _semaphoreId_t                          id;
    _semaphoreName_t                        name;
    _semaphoreCount_t                       count;
    _semaphoreCount_t                       maxCount;
    _semaphoreCount_t                       initialCount;
    _semaphoreWaitList_t                    *semaphoreWaitList;
};



#endif /* RTOS_INCLUDE_RTOSTYPEDEFINITIONS_H_ */