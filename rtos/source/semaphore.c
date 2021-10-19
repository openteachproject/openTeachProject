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
 * semaphore.c
 *
 *  Created on: Sep 29, 2021
 *      Author: salar
 */



#include <rtos/include/rtosTypeDefinitions.h>
#include <rtos/include/systemCallTypeDefintions.h>
#include <rtos/include/kernel.h>
#include <rtos/include/list.h>
#include <rtos/include/rtos.h>



static _rtosStatus_t semaphoreGetFromInterrupt(_semaphoreId_t id);



static _rtosStatus_t semaphoreReleaseFromInterrupt(_semaphoreId_t id);



semaphoreName_t semaphoreGetName(semaphoreId_t id) {

    _semaphoreName_t                    returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _semaphoreGetNameArg_t              semaphoreGetNameArg;

    if (id == NULL) {
        returnValue = NULL;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = NULL;
        }
        else {

            semaphoreGetNameArg . semaphoreIdArg = id;

            systemCallArg . systemCallNumber = SemaphoreGetNameSystemCallNumber;
            systemCallArg . specificSystemCallArg = &semaphoreGetNameArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = semaphoreGetNameArg . returnValue;
        }
    }

    return returnValue;
}



void _semaphoreGetNameSystemCall(_semaphoreGetNameArg_t *arg) {

    _semaphoreControlBlock_t            *semaphore;
    _semaphoreId_t                      semaphoreId;
    _semaphoreName_t                    semaphoreName;

    semaphoreId = arg -> semaphoreIdArg;
    semaphore = (_semaphoreControlBlock_t*)semaphoreId;
    semaphoreName = semaphore -> name;

    arg -> returnValue = semaphoreName;
    return;
}



semaphoreId_t semaphoreCreateNew( semaphoreName_t name,
                                  semaphoreCount_t maxCount,
                                  semaphoreCount_t initialCount)
{
    _semaphoreId_t                      returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _semaphoreCreateNewArg_t            semaphoreCreateNewArg;

    if ( maxCount < 1 ||
         initialCount > maxCount)
    {
        returnValue = NULL;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = NULL;
        }
        else {

            semaphoreCreateNewArg . semaphoreNameArg = name;
            semaphoreCreateNewArg . semaphoreMaxCountArg = maxCount;
            semaphoreCreateNewArg . semaphoreInitialCountArg = initialCount;

            systemCallArg . systemCallNumber = SemaphoreCreateNewSystemCallNumber;
            systemCallArg . specificSystemCallArg = &semaphoreCreateNewArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = semaphoreCreateNewArg . returnValue;
        }
    }

    return returnValue;
}



void _semaphoreCreateNewSystemCall(_semaphoreCreateNewArg_t *arg) {

    _semaphoreId_t                      returnValue;
    _semaphoreControlBlock_t            *newSemaphore;
    _semaphoreId_t                      semaphoreId;
    _semaphoreName_t                    semaphoreName;
    _semaphoreWaitList_t                *semaphoreWaitList;

    while(true) {

        newSemaphore = (_semaphoreControlBlock_t*)malloc(sizeof(_semaphoreControlBlock_t));
        if (newSemaphore == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            semaphoreId = (_semaphoreId_t)newSemaphore;
            newSemaphore -> id = semaphoreId;
            returnValue = semaphoreId;
        }



        if (strlen(arg -> semaphoreNameArg) >= SemaphoreNameMaxLength) {
            (arg -> semaphoreNameArg)[SemaphoreNameMaxLength - 1] = 0x00;
        }
        semaphoreName = (_semaphoreName_t)malloc(strlen(arg -> semaphoreNameArg));
        if (semaphoreName == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            strcpy(semaphoreName, arg -> semaphoreNameArg);
            newSemaphore -> name = semaphoreName;
        }



        newSemaphore -> count = arg -> semaphoreInitialCountArg;
        newSemaphore -> maxCount = arg -> semaphoreMaxCountArg;
        newSemaphore -> initialCount = arg -> semaphoreInitialCountArg;



        semaphoreWaitList = _listCreateNewSemaphoreWaitList();
        if (semaphoreWaitList == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newSemaphore -> semaphoreWaitList = semaphoreWaitList;
        }

        break;
    }



    if (returnValue == NULL) {
        free(newSemaphore);
        free(semaphoreName);
        free(semaphoreWaitList);
    }



    arg -> returnValue =returnValue;
    return;
}



rtosStatus_t semaphoreDelete(semaphoreId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _semaphoreDeleteArg_t               semaphoreDeleteArg;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            semaphoreDeleteArg . semaphoreIdArg = id;

            systemCallArg . systemCallNumber = SemaphoreDeleteSystemCallNumber;
            systemCallArg . specificSystemCallArg = &semaphoreDeleteArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = semaphoreDeleteArg . returnValue;
        }
    }

    return returnValue;
}



void _semaphoreDeleteSystemCall(_semaphoreDeleteArg_t *arg) {

    _rtosStatus_t                       returnValue;
    _semaphoreControlBlock_t            *semaphore;
    _semaphoreId_t                      semaphoreId;
    _listSize_t                         listSize;
    _threadControlBlock_t               *waitThread;
    _threadId_t                         waitThreadId;
    _kernelWaitListNumber_t             waitListNumber;

    semaphoreId = arg -> semaphoreIdArg;
    semaphore = (_semaphoreControlBlock_t*)semaphoreId;

    listSize = _listGetSizeSemaphoreWaitList(semaphoreId);

    for(_listSize_t index = 0 ; index < listSize ; index++) {

        waitThreadId = _listGetMaxSemaphoreWaitList(semaphoreId);
        waitThread = (_threadControlBlock_t*)waitThreadId;

        _listDeleteFromSemaphoreWaitList(semaphoreId, waitThreadId);
        waitThread -> semaphoreWaitingFor = NULL;

        waitListNumber = (waitThread -> returnTick) % NumberOfWaitLists;
        waitThread -> returnTick = 0;
        _listDeleteFromWaitList(waitThreadId, waitListNumber);

        _listInsertToReadyList(waitThreadId);
        waitThread -> state = ThreadStateReady;

        if (_kernelCheckForContextSwitchAfterInsert(waitThreadId) == true) {

            _kernelContextSwitchRequest();
        }
        returnValue = StatusOk;
    }



    free(semaphore -> name);
    free(semaphore -> semaphoreWaitList);
    free(semaphore);



    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t semaphoreGet(semaphoreId_t id, kernelTick_t timeOut) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _semaphoreGetArg_t                  semaphoreGetArg;
    _threadControlBlock_t               *semaphoreCaller;
    _threadId_t                         semaphoreCallerId;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = semaphoreGetFromInterrupt(id);
        }
        else {

            semaphoreCallerId = _kernelGetCurrentRunningThreadId();
            semaphoreCaller = (_threadControlBlock_t*)semaphoreCallerId;

            semaphoreGetArg . semaphoreIdArg = id;
            semaphoreGetArg . timeOutArg = timeOut;
            semaphoreGetArg . semaphoreCallerIdArg = semaphoreCallerId;

            systemCallArg . systemCallNumber = SemaphoreGetSystemCallNumber;
            systemCallArg . specificSystemCallArg = &semaphoreGetArg;

            _kernelSystemCall(&systemCallArg);


            if ( semaphoreCaller -> returnedByRelease == ReturnedByReleaseSet &&
                 semaphoreGetArg . returnValue == StatusWait )
            {

                returnValue = StatusOk;
            }
            else if ( semaphoreCaller -> returnedByRelease == ReturnedByReleaseUnset &&
                      semaphoreGetArg . returnValue == StatusWait )
            {

                returnValue = StatusErrorTimeOut;
            }
            else {

                returnValue = semaphoreGetArg . returnValue;
            }
        }
    }

    return returnValue;
}



void _semaphoreGetSystemCall(_semaphoreGetArg_t *arg) {

    _rtosStatus_t                       returnValue;
    _semaphoreControlBlock_t            *semaphore;
    _semaphoreId_t                      semaphoreId;
    _semaphoreCount_t                   semaphoreCount;
    _threadControlBlock_t               *semaphoreCaller;
    _threadId_t                         semaphoreCallerId;
    _kernelTick_t                       returnTick;
    _kernelTick_t                       currentTick;
    _kernelTick_t                       timeOut;
    _kernelWaitListNumber_t             waitListNumber;
    _atomicResult_t                     result;

    semaphoreId = arg -> semaphoreIdArg;
    semaphore = (_semaphoreControlBlock_t*)semaphoreId;
    timeOut = arg -> timeOutArg;
    semaphoreCallerId = arg -> semaphoreCallerIdArg;
    semaphoreCaller = (_threadControlBlock_t*)semaphoreCallerId;

    while(true) {

        semaphoreCount = _kernelAtomicRead32((_atomicAddress_t)(&(semaphore -> count)));

        if (semaphoreCount > 0) {

            semaphoreCount--;
            result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

            if (result == AtomicWriteSuccessfull) {

                returnValue = StatusOk;

                break;
            }
        }
        else {

            if (timeOut == 0) {

                returnValue = StatusErrorResource;
            }
            else {

                semaphoreCount--;
                result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

                if (result == AtomicWriteSuccessfull) {

                    _listInsertToSemaphoreWaitList(semaphoreId, semaphoreCallerId);
                    semaphoreCaller -> semaphoreWaitingFor = semaphoreId;

                    currentTick = _kernelGetTick();
                    returnTick = currentTick + timeOut;
                    waitListNumber = returnTick % NumberOfWaitLists;

                    semaphoreCaller -> returnTick = returnTick;

                    _listDeleteFromReadyList(semaphoreCallerId);

                    _listInsertToWaitList(semaphoreCallerId, waitListNumber);
                    semaphoreCaller -> state = ThreadStateWaited;

                    semaphoreCaller -> returnedByRelease = ReturnedByReleaseUnset;

                    if (_kernelCheckForContextSwitchAfterDelete(semaphoreCallerId)) {

                        _kernelContextSwitchRequest();
                    }

                    returnValue = StatusWait;

                    break;
                }
            }
        }
    }

    arg -> returnValue = returnValue;
    return;
}

static _rtosStatus_t semaphoreGetFromInterrupt(_semaphoreId_t id) {

    _rtosStatus_t                            returnValue;
    _semaphoreControlBlock_t                 *semaphore;
    _semaphoreCount_t                        semaphoreCount;
    _atomicResult_t                          result;

    semaphore = (_semaphoreControlBlock_t*)id;

    while(true) {

        semaphoreCount = _kernelAtomicRead32((_atomicAddress_t)(&(semaphore -> count)));

        if (semaphoreCount > 0) {

            semaphoreCount--;
            result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

            if (result == AtomicWriteSuccessfull) {

                returnValue = StatusOk;

                break;
            }
        }
        else {

            result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

            if (result == AtomicWriteSuccessfull) {

                returnValue = StatusErrorResource;

                break;
            }
        }
    }

    return returnValue;
}



rtosStatus_t semaphoreRelease(semaphoreId_t id) {

    _rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _semaphoreReleaseArg_t              semaphoreReleaseArg;

    if (id == NULL) {
        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = semaphoreReleaseFromInterrupt(id);
        }
        else {

            semaphoreReleaseArg . semaphoreIdArg = id;

            systemCallArg . systemCallNumber = SemaphoreReleaseSystemCallNumber;
            systemCallArg . specificSystemCallArg = &semaphoreReleaseArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = semaphoreReleaseArg . returnValue;
        }
    }

    return returnValue;
}



void _semaphoreReleaseSystemCall(_semaphoreReleaseArg_t *arg) {

    _rtosStatus_t                       returnValue;
    _semaphoreControlBlock_t            *semaphore;
    _semaphoreId_t                      semaphoreId;
    _semaphoreCount_t                   semaphoreCount;
    _threadControlBlock_t               *waitThread;
    _threadId_t                         waitThreadId;
    _kernelWaitListNumber_t             waitListNumber;
    _atomicResult_t                     result;

    semaphoreId = arg -> semaphoreIdArg;
    semaphore = (_semaphoreControlBlock_t*)semaphoreId;

    while(true) {

        semaphoreCount = _kernelAtomicRead32((_atomicAddress_t)(&(semaphore -> count)));

        if (semaphoreCount >= 0) {

            if (semaphoreCount < (semaphore -> maxCount)) {

                semaphoreCount++;
                result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

                if (result == AtomicWriteSuccessfull) {

                    returnValue = StatusOk;

                    break;
                }
            }
            else {

                result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

                if (result == AtomicWriteSuccessfull) {

                    returnValue = StatusErrorResource;

                    break;
                }
            }
        }
        else {

            semaphoreCount++;
            result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

            if (result == AtomicWriteSuccessfull) {

                waitThreadId = _listGetMaxSemaphoreWaitList(semaphoreId);
                waitThread = (_threadControlBlock_t*)waitThreadId;

                _listDeleteFromSemaphoreWaitList(semaphoreId, waitThreadId);

                waitThread -> semaphoreWaitingFor = NULL;

                waitListNumber = (waitThread -> returnTick) % NumberOfWaitLists;
                waitThread -> returnTick = 0;
                _listDeleteFromWaitList(waitThreadId, waitListNumber);

                _listInsertToReadyList(waitThreadId);
                waitThread -> state = ThreadStateReady;
                waitThread -> returnedByRelease = ReturnedByReleaseSet;

                if (_kernelCheckForContextSwitchAfterInsert(waitThreadId)) {

                    _kernelContextSwitchRequest();
                }

                returnValue = StatusOk;

                break;
            }
        }
    }

    arg -> returnValue = returnValue;
    return;
}



static _rtosStatus_t semaphoreReleaseFromInterrupt(_semaphoreId_t id) {

    _rtosStatus_t                            returnValue;
    _semaphoreControlBlock_t                 *semaphore;
    _semaphoreCount_t                        semaphoreCount;
    _atomicResult_t                          result;
    _kernelInterruptRequest_t                *interruptRequestMemory;

    semaphore = (_semaphoreControlBlock_t*)id;

    while(true) {

        semaphoreCount = _kernelAtomicRead32((_atomicAddress_t)(&(semaphore -> count)));

        if (semaphoreCount >= 0) {

            if (semaphoreCount < (semaphore -> maxCount)) {

                semaphoreCount++;
                result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

                if (result == AtomicWriteSuccessfull) {

                    returnValue = StatusOk;

                    break;
                }
            }
            else {

                result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

                if (result == AtomicWriteSuccessfull) {

                    returnValue = StatusErrorResource;

                    break;
                }
            }
        }
        else {

            semaphoreCount++;
            result = _kernelAtomicWrite32((_atomicValue_t)semaphoreCount, (_atomicAddress_t)(&(semaphore -> count)));

            if (result == AtomicWriteSuccessfull) {

                interruptRequestMemory = _kernelInterruptRequestAlloc();

                *(interruptRequestMemory + 2) = (_kernelInterruptRequest_t)id;

                *(interruptRequestMemory + 0) = (_kernelInterruptRequest_t)SemaphoreReleaseFromInterruptSystemCallNumber;
                *(interruptRequestMemory + 1) = (_kernelInterruptRequest_t)(interruptRequestMemory + 2);

                _kernelSystemCall((_kernelSystemCallArg_t*)interruptRequestMemory);

                returnValue = StatusOk;

                break;
            }
        }
    }

    return returnValue;
}



void _semaphoreReleaseFromInterruptSystemCall(_semaphoreReleaseFromInterruptArg_t *arg) {

    _semaphoreId_t                      semaphoreId;
    _threadControlBlock_t               *waitThread;
    _threadId_t                         waitThreadId;
    _kernelWaitListNumber_t             waitListNumber;
    _kernelInterruptRequest_t           *interruptRequestPointer;

    semaphoreId = arg -> semaphoreIdArg;

    interruptRequestPointer = ((_kernelInterruptRequest_t*)arg) - 2;

    _kernelInterruptRequestFree((_kernelInterruptRequest_t*)(interruptRequestPointer));

    waitThreadId = _listGetMaxSemaphoreWaitList(semaphoreId);
    waitThread = (_threadControlBlock_t*)waitThreadId;

    _listDeleteFromSemaphoreWaitList(semaphoreId, waitThreadId);

    waitThread -> semaphoreWaitingFor = NULL;

    waitListNumber = (waitThread -> returnTick) % NumberOfWaitLists;
    waitThread -> returnTick = 0;
    _listDeleteFromWaitList(waitThreadId, waitListNumber);

    _listInsertToReadyList(waitThreadId);
    waitThread -> state = ThreadStateReady;
    waitThread -> returnedByRelease = ReturnedByReleaseSet;

    if (_kernelCheckForContextSwitchAfterInsert(waitThreadId)) {

        _kernelContextSwitchRequest();
    }

    return;
}


