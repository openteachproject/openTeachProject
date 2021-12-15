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
 * userQueue.c
 *
 *  Created on: Dec 11, 2021
 *      Author: salar
 */



#include <rtos/include/rtosTypeDefinitions.h>
#include <rtos/include/rtos.h>
#include <rtos/include/kernel.h>



userQueueName_t userQueueGetName(userQueueId_t id) {

    _userQueueName_t                       returnValue;
    _userQueueControlBlock_t              *userQueue;

    userQueue = (_userQueueControlBlock_t*)id;
    returnValue = userQueue -> name;

    return returnValue;
}



userQueueSize_t userQueueGetSize(userQueueId_t id) {

    _userQueueSize_t                       returnValue;
    _userQueueControlBlock_t              *userQueue;

    userQueue = (_userQueueControlBlock_t*)id;
    returnValue = userQueue -> bufferSize;

    return returnValue;
}



userQueueId_t userQueueCreateNew(userQueueName_t name, userQueueSize_t bufferSize) {

    _userQueueId_t                         returnValue;
    _userQueueControlBlock_t              *newUserQueue;
    _userQueueId_t                         userQueueId;
    _userQueueName_t                       userQueueName;
    _userQueueAddress_t                    userQueueBufferAddress;
    _semaphoreId_t                         sendSemaphoreId;
    _semaphoreId_t                         receiveSemaphoreId;

    if (_kernelIsInInterrupt()) {

        returnValue = NULL;
    }
    else {

        while (true) {

            newUserQueue = (_userQueueControlBlock_t*)malloc(sizeof(_userQueueControlBlock_t));
            if (newUserQueue == NULL) {

                returnValue = NULL;
                break;
            }
            else {

                userQueueId = (_userQueueId_t)newUserQueue;
                newUserQueue -> id = userQueueId;
                returnValue = userQueueId;
            }



            if (strlen(name) >= UserQueueNameMaxLength) {
                (name)[MutexNameMaxLength - 1] = 0x00;
            }

            userQueueName = (_userQueueName_t)malloc(strlen(name));
            if (userQueueName == NULL) {

                returnValue = NULL;
                break;
            }
            else {

                strcpy(userQueueName, name);
                newUserQueue -> name = userQueueName;
            }



            userQueueBufferAddress = (_userQueueAddress_t)malloc(bufferSize);
            if (userQueueBufferAddress == NULL) {

                returnValue = NULL;
                break;
            }
            else {

                newUserQueue -> bufferAddress = userQueueBufferAddress;
            }



            newUserQueue -> bufferSize = bufferSize;



            sendSemaphoreId = semaphoreCreateNew("queueSendSemaphore", 1, 1);
            if (sendSemaphoreId == NULL) {

                returnValue = NULL;
                break;
            }
            else {

                newUserQueue -> sendSemaphoreId = sendSemaphoreId;
            }



            receiveSemaphoreId = semaphoreCreateNew("queueReceiveSemaphore", 1, 0);
            if (receiveSemaphoreId == NULL) {

                returnValue = NULL;
                break;
            }
            else {

                newUserQueue -> receiveSemaphoreId = receiveSemaphoreId;
            }



            break;
        }



        if (returnValue == NULL) {

            free(userQueueName);
            free(userQueueBufferAddress);
            free(sendSemaphoreId);
            free(receiveSemaphoreId);
            free(newUserQueue);
        }
    }



    return returnValue;
}



rtosStatus_t userQueueDelete(userQueueId_t id) {

    _rtosStatus_t                          returnValue;
    _userQueueControlBlock_t              *userQueue;

    userQueue = (_userQueueControlBlock_t*)id;

    if (_kernelIsInInterrupt()) {

        returnValue = StatusErrorInerrupt;
    }
    else {

        free(userQueue -> name);
        free(userQueue -> bufferAddress);
        free(userQueue -> sendSemaphoreId);
        free(userQueue -> receiveSemaphoreId);
        free(userQueue);

        returnValue = StatusOk;
    }



    return returnValue;
}



rtosStatus_t userQueueSend(    userQueueId_t id,
                               userQueueAddress_t bufferAddress,
                               userQueueSize_t bufferSize,
                               kernelTick_t timeOut)
{

    _rtosStatus_t                          returnValue;
    _userQueueControlBlock_t              *userQueue;

    if (id == NULL) {

        returnValue = StatusErrorParameter;
    }
    else {

        userQueue = (_userQueueControlBlock_t*)id;



        if (bufferSize > (userQueue -> bufferSize)) {

            returnValue =  StatusErrorResource;
        }
        else {

            semaphoreGet(userQueue -> sendSemaphoreId, timeOut);

            memcpy(userQueue -> bufferAddress, bufferAddress, bufferSize);

            semaphoreRelease(userQueue -> receiveSemaphoreId);

            returnValue = StatusOk;
        }
    }



    return returnValue;
}



rtosStatus_t userQueueReceive(    userQueueId_t id,
                                  userQueueAddress_t bufferAddress,
                                  userQueueSize_t bufferSize,
                                  kernelTick_t timeOut)
{

    _rtosStatus_t                          returnValue;
    _userQueueControlBlock_t              *userQueue;

    if (id == NULL) {

        returnValue = StatusErrorParameter;
    }
    else {

        userQueue = (_userQueueControlBlock_t*)id;



        if ((userQueue -> bufferSize) > bufferSize) {

            returnValue =  StatusErrorResource;
        }
        else {

            semaphoreGet(userQueue -> receiveSemaphoreId, timeOut);

            memcpy(bufferAddress, userQueue -> bufferAddress, bufferSize);

            semaphoreRelease(userQueue -> sendSemaphoreId);

            returnValue = StatusOk;
        }
    }



    return returnValue;
}


