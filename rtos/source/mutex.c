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
 * mutex.c
 *
 *  Created on: Sep 29, 2021
 *      Author: salar
 */



#include <rtos/include/rtosTypeDefinitions.h>
#include <rtos/include/systemCallTypeDefintions.h>
#include <rtos/include/kernel.h>
#include <rtos/include/list.h>
#include <rtos/include/rtos.h>



mutexName_t mutexGetName(mutexId_t id) {

    _mutexName_t                        returnValue;;
    _kernelSystemCallArg_t              systemCallArg;
    _mutexGetNameArg_t                  mutexGetNameArg;
    _mutexControlBlock_t                *mutex;

    mutex = (_mutexControlBlock_t*)id;

    if (id == NULL || mutex -> resourceState == ResourceStateDeleted) {

        returnValue = NULL;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = NULL;
        }
        else {

            mutexGetNameArg . mutexIdArg = id;

            systemCallArg . systemCallNumber = MutexGetNameSystemCallNumber;
            systemCallArg . specificSystemCallArg = &mutexGetNameArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = mutexGetNameArg . returnValue;
        }
    }

    return returnValue;
}



void _mutexGetNameSystemCall(_mutexGetNameArg_t *arg) {

    _mutexControlBlock_t                *mutex;
    _mutexId_t                          mutexId;
    _mutexName_t                        mutexName;

    mutexId = arg -> mutexIdArg;
    mutex = (_mutexControlBlock_t*)mutexId;
    mutexName = mutex -> name;

    arg -> returnValue = mutexName;
    return;
}



mutexId_t mutexCreateNew(mutexName_t name, mutexType_t type) {

    _mutexId_t                          returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _mutexCreateNewArg_t                mutexCreateNewArg;


    if (_kernelIsInInterrupt()) {

        returnValue = NULL;
    }
    else {

        mutexCreateNewArg . mutexNameArg = name;
        mutexCreateNewArg . mutexTypeArg = MutexTypeSingleshot;

        systemCallArg . systemCallNumber = MutexCreateNewSystemCallNumber;
        systemCallArg . specificSystemCallArg = &mutexCreateNewArg;

        _kernelSystemCall(&systemCallArg);

        returnValue = mutexCreateNewArg . returnValue;
    }

    return returnValue;
}



void _mutexCreateNewSystemCall(_mutexCreateNewArg_t *arg) {

    _mutexId_t                          returnValue;
    _mutexControlBlock_t                *newMutex;
    _mutexId_t                          mutexId;
    _mutexName_t                        mutexName;
    _mutexWaitList_t                    *mutexWaitList;
    _threadOwnedMutexNode_t             *threadOwnedMutexNode;

    while(true) {

        newMutex = (_mutexControlBlock_t*)malloc(sizeof(_mutexControlBlock_t));
        if (newMutex == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            mutexId = (mutexId_t)newMutex;
            newMutex -> id = mutexId;
            returnValue = mutexId;
        }



        if (strlen(arg -> mutexNameArg) >= MutexNameMaxLength) {
            (arg -> mutexNameArg)[MutexNameMaxLength - 1] = 0x00;
        }
        mutexName = (_mutexName_t)malloc(strlen(arg -> mutexNameArg));
        if (mutexName == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            strcpy(mutexName, arg -> mutexNameArg);
            newMutex -> name = mutexName;
        }



        newMutex -> lock = MutexUnlocked;

        newMutex -> type = MutexTypeSingleshot;

        newMutex -> owner = NULL;



        mutexWaitList = _listCreateNewMutexWaitList();
        if (mutexWaitList == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newMutex -> mutexWaitList = mutexWaitList;
        }



        threadOwnedMutexNode = _listCreateNewThreadOwnedMutexNode(mutexId);
        if (threadOwnedMutexNode == NULL) {
            returnValue = NULL;
            break;
        }
        else {
            newMutex -> threadOwnedMutexNode = threadOwnedMutexNode;
        }

        newMutex -> resourceState = ResourceStateActive;

        break;
    }



    if (returnValue == NULL) {
        free(newMutex);
        free(mutexName);
        free(threadOwnedMutexNode);
        free(mutexWaitList);
    }



    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t mutexDelete(mutexId_t id) {
	_rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _mutexDeleteArg_t                   mutexDeleteArg;
    _mutexControlBlock_t                *mutex;

    mutex = (_mutexControlBlock_t*)id;

    if (id == NULL || mutex -> resourceState == ResourceStateDeleted) {

        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            mutexDeleteArg . mutexIdArg = id;

            systemCallArg . systemCallNumber = MutexDeleteSystemCallNumber;
            systemCallArg . specificSystemCallArg = &mutexDeleteArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = mutexDeleteArg . returnValue;
        }
    }

    return returnValue;
}



void _mutexDeleteSystemCall(_mutexDeleteArg_t *arg) {

	_rtosStatus_t                       returnValue;
    _mutexControlBlock_t                *mutex;
    _mutexId_t                          mutexId;

    mutexId = arg -> mutexIdArg;
    mutex = (_mutexControlBlock_t*)mutexId;

    if (mutex -> lock == MutexLocked) {

        returnValue = StatusErrorResource;
    }
    else {

        mutex -> resourceState = ResourceStateDeleted;
        free(mutex -> name);
        free(mutex -> threadOwnedMutexNode);
        free(mutex -> mutexWaitList);
    }



    arg -> returnValue = returnValue;
    return;
}



rtosStatus_t mutexGet(mutexId_t id, kernelTick_t timeOut) {

	_rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _mutexGetArg_t                      mutexGetArg;
    _threadId_t                         mutexCallerId;
    _threadControlBlock_t               *mutexCaller;
    _mutexControlBlock_t                *mutex;

    mutex = (_mutexControlBlock_t*)id;

    if (id == NULL || mutex -> resourceState == ResourceStateDeleted) {

        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {

            returnValue = StatusErrorInerrupt;
        }
        else {

            mutexGetArg . mutexIdArg = id;
            mutexGetArg . timeOutArg = timeOut;
            mutexGetArg . mutexCallerIdArg = _kernelGetCurrentRunningThreadId();

            systemCallArg . systemCallNumber = MutexGetSystemCallNumber;
            systemCallArg . specificSystemCallArg = &mutexGetArg;

            _kernelSystemCall(&systemCallArg);


            mutexCallerId = _kernelGetCurrentRunningThreadId();
            mutexCaller = (threadControlBlock_t*)mutexCallerId;

            if ( mutexCaller -> returnedByRelease == ReturnedByReleaseSet &&
                 mutexGetArg . returnValue == StatusWait )
            {

                returnValue = StatusOk;
            }
            else if ( mutexCaller -> returnedByRelease == ReturnedByReleaseUnset &&
                      mutexGetArg . returnValue == StatusWait )
            {

                returnValue = StatusErrorTimeOut;
            }
            else {

                returnValue = mutexGetArg . returnValue;
            }
        }
    }

    return returnValue;
}



void _mutexGetSystemCall(_mutexGetArg_t *arg) {

	_rtosStatus_t                       returnValue;
    _mutexControlBlock_t                *mutex;
    _mutexId_t                          mutexId;
    _mutexLock_t                        mutexLock;
    _threadControlBlock_t               *mutexCaller;
    _threadId_t                         mutexCallerId;
    _kernelTick_t                       returnTick;
    _kernelTick_t                       currentTick;
    _kernelTick_t                       timeOut;
    _kernelWaitListNumber_t             waitListNumber;

    mutexId = arg -> mutexIdArg;
    mutex = (_mutexControlBlock_t*)mutexId;
    timeOut = arg -> timeOutArg;
    mutexCallerId = arg -> mutexCallerIdArg;
    mutexLock = mutex -> lock;

    if (mutexLock == MutexUnlocked) {

        mutex -> lock = MutexLocked;
        mutex -> owner = mutexCallerId;
        _listInsertToThreadOwnedMutexList(mutexCallerId, mutexId);
        returnValue = StatusOk;
    }
    else {

        if (mutex -> owner == mutexCallerId) {
            returnValue = StatusOk;
        }
        else {

            if (timeOut == 0) {
                returnValue = StatusErrorResource;
            }
            else {

                mutexCaller = (_threadControlBlock_t*)mutexCallerId;

                _listInsertToMutexWaitList(mutexId, mutexCallerId);

                mutexCaller -> mutexWaitingFor = mutexId;

                currentTick = _kernelGetTick();
                returnTick = currentTick + timeOut;
                waitListNumber = returnTick % NumberOfWaitLists;

                mutexCaller -> returnTick = returnTick;

                _listDeleteFromReadyList(mutexCallerId);

                _listInsertToWaitList(mutexCallerId, waitListNumber);
                mutexCaller -> state = ThreadStateWaited;

                mutexCaller -> returnedByRelease = ReturnedByReleaseUnset;

                if (_kernelCheckForContextSwitchAfterDelete(mutexCallerId)) {

                _kernelContextSwitchRequest();
                }
                returnValue = StatusWait;
            }
        }
    }

    arg -> returnValue = returnValue;
    return;
}




rtosStatus_t mutexRelease(mutexId_t id) {

	_rtosStatus_t                       returnValue;
    _kernelSystemCallArg_t              systemCallArg;
    _mutexReleaseArg_t                  mutexReleaseArg;
    _mutexControlBlock_t                *mutex;

    mutex = (_mutexControlBlock_t*)id;

    if (id == NULL || mutex -> resourceState == ResourceStateDeleted) {

        returnValue = StatusErrorParameter;
    }
    else {

        if (_kernelIsInInterrupt()) {
            returnValue = StatusErrorInerrupt;
        }
        else {

            mutexReleaseArg . mutexIdArg = id;
            mutexReleaseArg . mutexCallerIdArg = _kernelGetCurrentRunningThreadId();

            systemCallArg . systemCallNumber = MutexReleaseSystemCallNumber;
            systemCallArg . specificSystemCallArg = &mutexReleaseArg;

            _kernelSystemCall(&systemCallArg);

            returnValue = mutexReleaseArg . returnValue;
        }
    }

    return returnValue;
}



void _mutexReleaseSystemCall(_mutexReleaseArg_t *arg) {

	_rtosStatus_t                       returnValue;
    _mutexControlBlock_t                *mutex;
    _mutexId_t                          mutexId;
    _mutexLock_t                        mutexLock;
    _threadId_t                         mutexOwner;
    _threadId_t                         mutexCallerId;
    _threadControlBlock_t               *waitThread;
    _threadId_t                         waitThreadId;
    _kernelWaitListNumber_t             waitListNumber;

    mutexId = arg -> mutexIdArg;
    mutex = (_mutexControlBlock_t*)mutexId;
    mutexLock = mutex -> lock;
    mutexOwner = mutex -> owner;
    mutexCallerId = arg -> mutexCallerIdArg;

    if (mutexLock == MutexUnlocked) {
        returnValue = StatusErrorResource;
    }
    else {

        if (mutexOwner == mutexCallerId) {

            _listDeleteFromThreadOwnedMutexList(mutexCallerId, mutexId);

            if (_listIsEmptyMutexWaitList(mutexId)) {
                mutex -> lock = MutexUnlocked;
                mutex -> owner = NULL;
                returnValue = StatusOk;
            }
            else {

                waitThreadId = _listGetMaxMutexWaitList(mutexId);
                waitThread = (_threadControlBlock_t*)waitThreadId;

                _listInsertToThreadOwnedMutexList(waitThreadId, mutexId);
                mutex -> owner = waitThreadId;

                _listDeleteFromMutexWaitList(mutexId, waitThreadId);

                waitThread -> mutexWaitingFor = NULL;

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
            }
        }
        else {
            returnValue = StatusErrorResource;
        }
    }

    arg -> returnValue = returnValue;
    return;
}


