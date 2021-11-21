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
 * memPool.c
 *
 *  Created on: Nov 20, 2021
 *      Author: salar
 */



#include <rtos/include/rtosTypeDefinitions.h>
#include <rtos/include/systemCallTypeDefintions.h>
#include <rtos/include/kernel.h>
#include <rtos/include/list.h>
#include <rtos/include/rtos.h>



static _memPoolPointer_t _memPoolAllocateFromInterrupt(_memPoolId_t id);

static _rtosStatus_t _memPoolFreeFromInterrupt(_memPoolId_t id, _memPoolPointer_t block);



memPoolName_t memPoolGetName(memPoolId_t id) {

    _memPoolName_t                      returnValue;;
    _kernelSystemCallArg_t              systemCallArg;
    _memPoolGetNameArg_t                memPoolGetNameArg;

    if (id == NULL) {

        returnValue = NULL;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = NULL;
        }
        else {

            memPoolGetNameArg . memPoolIdArg = id;

            systemCallArg . systemCallNumber = MemPoolGetNameSystemCallNumber;
            systemCallArg . specificSystemCallArg = &memPoolGetNameArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = memPoolGetNameArg . returnValue;
        }
    }

    return returnValue;
}



void _memPoolGetNameSystemCall(_memPoolGetNameArg_t *arg) {

    _memPoolControlBlock_t              *memPool;
    _memPoolId_t                         memPoolId;
    _memPoolName_t                       memPoolName;

    memPoolId = arg -> memPoolIdArg;
    memPool = (_memPoolControlBlock_t*)memPoolId;
    memPoolName = memPool -> name;

    arg -> returnValue = memPoolName;
    return;
}



memPoolId_t memPoolCreateNew(    memPoolName_t name,
                                 memPoolSize_t blockSize,
                                 memPoolCount_t maxBlockCount)
{

    _memPoolId_t                        returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _memPoolCreateNewArg_t              memPoolCreateNewArg;


    if (_kernelIsInInterrupt()) {

        returnValue = NULL;
    }
    else {

        memPoolCreateNewArg . memPoolNameArg = name;
        memPoolCreateNewArg . memPoolBlockSizeArg = blockSize;
        memPoolCreateNewArg . memPoolBlockMaxCountArg = maxBlockCount;

        systemCallArg . systemCallNumber = MemPoolCreateNewSystemCallNumber;
        systemCallArg . specificSystemCallArg = &memPoolCreateNewArg;

        _kernelSystemCall(&systemCallArg);

        returnValue = memPoolCreateNewArg . returnValue;
    }

    return returnValue;
}



void _memPoolCreateNewSystemCall(_memPoolCreateNewArg_t *arg) {

   _memPoolId_t                        returnValue;
   _memPoolControlBlock_t             *newMemPool;
   _memPoolId_t                        memPoolId;
   _memPoolName_t                      memPoolName;
   _memPoolSize_t                      blockSize;
   _memPoolCount_t                     blockCount;
   _memPoolPointer_t                   firstBlockAddress;
   _memPoolLock_t                     *lockArray;
   _memPoolWaitList_t                 *memPoolWaitList;



   blockSize = arg -> memPoolBlockSizeArg;
   blockCount = arg -> memPoolBlockMaxCountArg;



    while(true) {

        newMemPool = (_memPoolControlBlock_t*)malloc(sizeof(_memPoolControlBlock_t));
        if (newMemPool == NULL) {

            returnValue = NULL;
            break;
        }
        else {

            memPoolId = (memPoolId_t)newMemPool;
            newMemPool -> id = memPoolId;
            returnValue = memPoolId;
        }



        if (strlen(arg -> memPoolNameArg) >= MemPoolNameMaxLength) {
            (arg -> memPoolNameArg)[MemPoolNameMaxLength - 1] = 0x00;
        }
        memPoolName = (_memPoolName_t)malloc(strlen(arg -> memPoolNameArg));
        if (memPoolName == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            strcpy(memPoolName, arg -> memPoolNameArg);
            newMemPool -> name = memPoolName;
        }



        newMemPool -> blockSize = blockSize;

        newMemPool -> maxBlockCount = blockCount;



        firstBlockAddress = (_memPoolPointer_t)malloc(blockCount * blockSize);
        if (firstBlockAddress == NULL) {

            returnValue = NULL;
            break;
        }
        else {

            newMemPool -> firstBlockAddress = firstBlockAddress;
        }



        lockArray = (_memPoolLock_t*)malloc(blockCount * sizeof(_memPoolLock_t));
        if (lockArray == NULL) {

            returnValue = NULL;
            break;
        }
        else {

            newMemPool -> lockArray = lockArray;

            for (uint32_t i = 0 ; i < blockCount ; i++) {

                lockArray[i] = MemPoolUnLocked;
            }
        }

        newMemPool -> lockArrayIndex = 0;



        memPoolWaitList = _listCreateNewMemPoolWaitList();
        if (memPoolWaitList == NULL) {

            returnValue = NULL;
            break;
        }
        else {

            newMemPool -> memPoolWaitList = memPoolWaitList;
        }



        break;
    }



    if (returnValue == NULL) {

        free(newMemPool);
        free(memPoolName);
        free(firstBlockAddress);
        free(lockArray);
        free(memPoolWaitList);
    }



    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t memPoolDelete(memPoolId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _memPoolDeleteArg_t                 memPoolDeleteArg;

    if (id == NULL) {

        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            memPoolDeleteArg . memPoolIdArg = id;

            systemCallArg . systemCallNumber = MemPoolDeleteSystemCallNumber;
            systemCallArg . specificSystemCallArg = &memPoolDeleteArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = memPoolDeleteArg . returnValue;
        }
    }

    return returnValue;
}



void _memPoolDeleteSystemCall(_memPoolDeleteArg_t *arg) {

    _memPoolControlBlock_t             *memPool;
    _memPoolId_t                        memPoolId;

    memPoolId = arg -> memPoolIdArg;
    memPool = (_memPoolControlBlock_t*)memPoolId;

    free(memPool -> name);
    free(memPool -> firstBlockAddress);
    free(memPool -> lockArray);
    free(memPool -> memPoolWaitList);



    arg -> returnValue = StatusOk;
    return;
}



memPoolPointer_t memPoolAllocate(memPoolId_t id, _kernelTick_t timeOut) {

    _memPoolPointer_t                   returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _memPoolAllocateArg_t               memPoolAllocateArg;
    _threadControlBlock_t              *memPoolCaller;
    _threadId_t                         memPoolCallerId;

    if (id == NULL) {

        returnValue = NULL;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = _memPoolAllocateFromInterrupt(id);
        }
        else {

            memPoolAllocateArg . memPoolIdArg = id;
            memPoolAllocateArg . timeOutArg = timeOut;
            memPoolAllocateArg . memPoolCallerIdArg = _kernelGetCurrentRunningThreadId();

            systemCallArg . systemCallNumber = MemPoolAllocateSystemCallNumber;
            systemCallArg . specificSystemCallArg = &memPoolAllocateArg;

            _kernelSystemCall(&systemCallArg);


            memPoolCallerId = _kernelGetCurrentRunningThreadId();
            memPoolCaller = (threadControlBlock_t*)memPoolCallerId;

            if (   memPoolCaller -> returnedByFree == ReturnedByFreeSet &&
                   memPoolAllocateArg . returnValue == NULL)
            {

                returnValue = memPoolCaller -> memPoolBlockReleasedByFree;
            }
            else if (   memPoolCaller -> returnedByRelease == ReturnedByReleaseUnset &&
                        memPoolAllocateArg . returnValue == NULL )
            {

                returnValue = NULL;
            }
            else {

                returnValue = memPoolAllocateArg . returnValue;
            }
        }
    }

    return returnValue;
}



void _memPoolAllocateSystemCall(_memPoolAllocateArg_t *arg) {

    _memPoolPointer_t                   returnValue = NULL;
    _memPoolControlBlock_t             *memPool;
    _memPoolId_t                        memPoolId;
    _memPoolCount_t                     maxBlockCount;
    _memPoolSize_t                      blockSize;
    _memPoolPointer_t                   firstBlockAddress;
    _memPoolSize_t                      blockOffset;
    _memPoolLock_t                     *lockArray;
    _memPoolIndex_t                     lockArrayIndex;
    _threadControlBlock_t              *memPoolCaller;
    _threadId_t                         memPoolCallerId;
    _kernelTick_t                       returnTick;
    _kernelTick_t                       currentTick;
    _kernelTick_t                       timeOut;
    _kernelWaitListNumber_t             waitListNumber;
    _atomicResult_t                     result;

    memPoolId = arg -> memPoolIdArg;
    memPoolCallerId = arg -> memPoolCallerIdArg;
    timeOut = arg -> timeOutArg;

    memPool = (_memPoolControlBlock_t*)memPoolId;
    maxBlockCount = memPool -> maxBlockCount;
    blockSize = memPool -> blockSize;
    firstBlockAddress = memPool -> firstBlockAddress;
    lockArray = memPool -> lockArray;
    lockArrayIndex = memPool -> lockArrayIndex;
    memPoolCaller = (_threadControlBlock_t*)memPoolCallerId;



    for(uint32_t index = 0 ; index < maxBlockCount ; index++) {

        lockArrayIndex = (_memPoolIndex_t)_kernelAtomicRead32((_atomicAddress_t)&(memPool -> lockArrayIndex));

        if (lockArray[lockArrayIndex] == MemPoolUnLocked) {

            result = (_atomicResult_t)_kernelAtomicWrite32(MemPoolLocked, (_atomicAddress_t)&(lockArray[lockArrayIndex]));
            if (result == AtomicWriteSuccessfull) {

                blockOffset = lockArrayIndex * blockSize;
                returnValue = firstBlockAddress + blockOffset;

                break;
            }
            else {

                index--;
            }
        }
        else {

            lockArrayIndex = (lockArrayIndex + 1) % maxBlockCount;

            result = (_atomicResult_t)_kernelAtomicWrite32(lockArrayIndex, (_atomicAddress_t)&(memPool -> lockArrayIndex));

            if (result == AtomicWriteUnsuccessfull) {

                index--;
            }
        }
    }



    if (returnValue == NULL) {

        if (timeOut != 0) {

            memPoolCaller = (_threadControlBlock_t*)memPoolCallerId;

            _listInsertToMemPoolWaitList(memPoolId, memPoolCallerId);

            memPoolCaller -> memPoolWaitingFor = memPoolId;

            currentTick = _kernelGetTick();
            returnTick = currentTick + timeOut;
            waitListNumber = returnTick % NumberOfWaitLists;

            memPoolCaller -> returnTick = returnTick;

            _listDeleteFromReadyList(memPoolCallerId);

            _listInsertToWaitList(memPoolCallerId, waitListNumber);
            memPoolCaller -> state = ThreadStateWaited;

            memPoolCaller -> returnedByFree = ReturnedByFreeUnset;

            if (_kernelCheckForContextSwitchAfterDelete(memPoolCallerId)) {

                _kernelContextSwitchRequest();
            }
            returnValue = NULL;
        }
    }



    arg -> returnValue = returnValue;
    return;
}



static _memPoolPointer_t _memPoolAllocateFromInterrupt(_memPoolId_t id) {

    _memPoolPointer_t                   returnValue = NULL;
    _memPoolControlBlock_t             *memPool;
    _memPoolCount_t                     maxBlockCount;
    _memPoolIndex_t                     lockArrayIndex;
    _memPoolSize_t                      blockSize;
    _memPoolSize_t                      blockOffset;
    _memPoolPointer_t                   firstBlockAddress;
    _memPoolLock_t                     *lockArray;
    _atomicResult_t                     result;

    memPool = (_memPoolControlBlock_t*)id;
    maxBlockCount = memPool -> maxBlockCount;
    lockArray = memPool -> lockArray;
    blockSize = memPool -> blockSize;
    firstBlockAddress = memPool -> firstBlockAddress;

    for(uint32_t index = 0 ; index < maxBlockCount ; index++) {

        lockArrayIndex = (_memPoolIndex_t)_kernelAtomicRead32((_atomicAddress_t)&(memPool -> lockArrayIndex));

        if (lockArray[lockArrayIndex] == MemPoolUnLocked) {

            result = (_atomicResult_t)_kernelAtomicWrite32(MemPoolLocked, (_atomicAddress_t)&(lockArray[lockArrayIndex]));
            if (result == AtomicWriteSuccessfull) {

                blockOffset = lockArrayIndex * blockSize;
                returnValue = firstBlockAddress + blockOffset;

                break;
            }
            else {

                index--;
            }
        }
        else {

            lockArrayIndex = (lockArrayIndex + 1) % maxBlockCount;

            result = (_atomicResult_t)_kernelAtomicWrite32(lockArrayIndex, (_atomicAddress_t)&(memPool -> lockArrayIndex));

            if (result == AtomicWriteUnsuccessfull) {

                index--;
            }
        }
    }

    return returnValue;
}



rtosStatus_t memPoolFree(memPoolId_t id, memPoolPointer_t block) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _memPoolFreeArg_t                   memPoolFreeArg;

    if (id == NULL || block == NULL) {

        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = _memPoolFreeFromInterrupt(id, block);
        }
        else {

            memPoolFreeArg . memPoolIdArg = id;
            memPoolFreeArg . blockArg = block;

            systemCallArg . systemCallNumber = MemPoolFreeSystemCallNumber;
            systemCallArg . specificSystemCallArg = &memPoolFreeArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = memPoolFreeArg . returnValue;
        }
    }

    return returnValue;
}



void _memPoolFreeSystemCall(_memPoolFreeArg_t *arg) {

    _rtosStatus_t                       returnValue;
    _memPoolControlBlock_t             *memPool;
    _memPoolId_t                        memPoolId;
    _memPoolPointer_t                   block;
    _memPoolSize_t                      blockSize;
    _memPoolLock_t                     *lockArray;
    _memPoolIndex_t                     lockArrayIndex;
    _memPoolPointer_t                   firstBlockAddress;
    _listSize_t                         memPoolWaitListSize;
    _threadId_t                         waitThreadId;
    _threadControlBlock_t              *waitThread;
    _kernelWaitListNumber_t             waitListNumber;

    memPoolId = arg -> memPoolIdArg;
    block = arg -> blockArg;

    memPool = (_memPoolControlBlock_t*)memPoolId;
    firstBlockAddress = memPool -> firstBlockAddress;
    blockSize = memPool -> blockSize;
    lockArray = memPool -> lockArray;

    memPoolWaitListSize = _listGetSizeMemPoolWaitList(memPoolId);

    if (memPoolWaitListSize > 0) {

        waitThreadId = _listGetMaxMemPoolWaitList(memPoolId);
        waitThread = (_threadControlBlock_t*)waitThreadId;

        waitThread -> memPoolBlockReleasedByFree = block;

        _listDeleteFromMemPoolWaitList(memPoolId, waitThreadId);

        waitThread -> memPoolWaitingFor = NULL;

        waitListNumber = (waitThread -> returnTick) % NumberOfWaitLists;
        waitThread -> returnTick = 0;
        _listDeleteFromWaitList(waitThreadId, waitListNumber);

        _listInsertToReadyList(waitThreadId);
        waitThread -> state = ThreadStateReady;
        waitThread -> returnedByFree = ReturnedByFreeSet;

        if (_kernelCheckForContextSwitchAfterInsert(waitThreadId)) {

            _kernelContextSwitchRequest();
        }

        returnValue = StatusOk;
    }
    else {

        lockArrayIndex = ((_memPoolSize_t)(block - firstBlockAddress)) / blockSize;
        lockArray[lockArrayIndex] = MemPoolUnLocked;

        returnValue = StatusOk;
    }



    arg -> returnValue = returnValue;
    return;
}



static _rtosStatus_t _memPoolFreeFromInterrupt(_memPoolId_t id, _memPoolPointer_t block) {

    _memPoolControlBlock_t             *memPool;
    _memPoolPointer_t                   firstBlockAddress;
    _memPoolSize_t                      blockSize;
    _memPoolLock_t                     *lockArray;
    _memPoolIndex_t                     lockArrayIndex;
    _kernelInterruptRequest_t          *interruptRequestMemory;


    memPool = (_memPoolControlBlock_t*)id;
    firstBlockAddress = memPool -> firstBlockAddress;
    blockSize = memPool -> blockSize;
    lockArray = memPool -> lockArray;

    lockArrayIndex = ((_memPoolSize_t)(block - firstBlockAddress)) / blockSize;
    lockArray[lockArrayIndex] = MemPoolUnLocked;



    interruptRequestMemory = _kernelInterruptRequestAlloc();

    *(interruptRequestMemory + 2) = (_kernelInterruptRequest_t)id;

    *(interruptRequestMemory + 0) = (_kernelInterruptRequest_t)MemPoolFreeFromInterruptSystemCallNumber;
    *(interruptRequestMemory + 1) = (_kernelInterruptRequest_t)(interruptRequestMemory + 2);

    _kernelSystemCall((_kernelSystemCallArg_t*)interruptRequestMemory);

    return StatusOk;
}



void _memPoolFreeFromInterruptSystemCall(_memPoolFreeFromInterruptArg_t *arg) {

    _memPoolControlBlock_t              *memPool;
    _memPoolId_t                        memPoolId;
    _memPoolSize_t                      maxBlockCount;
    _memPoolLock_t                     *lockArray;
    _memPoolIndex_t                     lockArrayIndex;
    _memPoolSize_t                      blockOffset;
    _memPoolSize_t                      blockSize;
    _memPoolPointer_t                   firstBlockAddress;
    _memPoolPointer_t                   block = NULL;
    _threadControlBlock_t              *waitThread;
    _threadId_t                         waitThreadId;
    _kernelWaitListNumber_t             waitListNumber;
    _kernelInterruptRequest_t          *interruptRequestPointer;
    _listSize_t                         memPoolWaitListSize;
    _atomicResult_t                     result;

    memPoolId = arg -> memPoolIdArg;

    memPool = (_memPoolControlBlock_t*)memPoolId;
    maxBlockCount = memPool -> maxBlockCount;
    lockArray = memPool -> lockArray;
    lockArrayIndex = memPool -> lockArrayIndex;
    blockSize = memPool -> blockSize;
    firstBlockAddress = memPool -> firstBlockAddress;

    interruptRequestPointer = ((_kernelInterruptRequest_t*)arg) - 2;

    _kernelInterruptRequestFree((_kernelInterruptRequest_t*)(interruptRequestPointer));



    memPoolWaitListSize = _listGetSizeMemPoolWaitList(memPoolId);

    if (memPoolWaitListSize > 0) {

        for(uint32_t index = 0 ; index < maxBlockCount ; index++) {

            lockArrayIndex = (_memPoolIndex_t)_kernelAtomicRead32((_atomicAddress_t)&(memPool -> lockArrayIndex));

            if (lockArray[lockArrayIndex] == MemPoolUnLocked) {

                result = (_atomicResult_t)_kernelAtomicWrite32(MemPoolLocked, (_atomicAddress_t)&(lockArray[lockArrayIndex]));
                if (result == AtomicWriteSuccessfull) {

                    blockOffset = lockArrayIndex * blockSize;
                    block = firstBlockAddress + blockOffset;

                    break;
                }
                else {

                    index--;
                }
            }
            else {

                lockArrayIndex = (lockArrayIndex + 1) % maxBlockCount;

                result = (_atomicResult_t)_kernelAtomicWrite32(lockArrayIndex, (_atomicAddress_t)&(memPool -> lockArrayIndex));

                if (result == AtomicWriteUnsuccessfull) {

                    index--;
                }
            }
        }



        if (block == NULL) {

            //
        }
        else {

            waitThreadId = _listGetMaxMemPoolWaitList(memPoolId);
            waitThread = (_threadControlBlock_t*)waitThreadId;

            waitThread -> memPoolBlockReleasedByFree = block;

            _listDeleteFromMemPoolWaitList(memPoolId, waitThreadId);

            waitThread -> memPoolWaitingFor = NULL;

            waitListNumber = (waitThread -> returnTick) % NumberOfWaitLists;
            waitThread -> returnTick = 0;
            _listDeleteFromWaitList(waitThreadId, waitListNumber);

            _listInsertToReadyList(waitThreadId);
            waitThread -> state = ThreadStateReady;
            waitThread -> returnedByFree = ReturnedByFreeSet;

            if (_kernelCheckForContextSwitchAfterInsert(waitThreadId)) {

                _kernelContextSwitchRequest();
            }
        }
    }
}



