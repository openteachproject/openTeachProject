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
 * systemCallTypeDefintions.h
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */



#ifndef RTOS_INCLUDE_SYSTEMCALLTYPEDEFINTIONS_H_
#define RTOS_INCLUDE_SYSTEMCALLTYPEDEFINTIONS_H_



#include <rtos/include/rtosTypeDefinitions.h>



#define ThreadSystemCallNumberBase                    0
#define ThreadGetIdSystemCallNumber                   ThreadSystemCallNumberBase + 0
#define ThreadGetNameSystemCallNumber                 ThreadSystemCallNumberBase + 1
#define ThreadGetStateSystemCallNumber                ThreadSystemCallNumberBase + 2
#define ThreadGetPrioritySystemCallNumber             ThreadSystemCallNumberBase + 3
#define ThreadSetPrioritySystemCallNumber             ThreadSystemCallNumberBase + 4
#define ThreadGetStackSizeSystemCallNumber            ThreadSystemCallNumberBase + 5
#define ThreadCreateNewSystemCallNumber               ThreadSystemCallNumberBase + 6
#define ThreadTerminateSystemCallNumber               ThreadSystemCallNumberBase + 7
#define ThreadYieldSystemCallNumber                   ThreadSystemCallNumberBase + 8
#define ThreadResumeSystemCallNumber                  ThreadSystemCallNumberBase + 9
#define ThreadSuspendSystemCallNumber                 ThreadSystemCallNumberBase + 10
#define ThreadWaitSystemCallNumber                    ThreadSystemCallNumberBase + 11



#define SemaphoreSystemCallNumberBase                 256
#define SemaphoreGetNameSystemCallNumber              SemaphoreSystemCallNumberBase + 0
#define SemaphoreCreateNewSystemCallNumber            SemaphoreSystemCallNumberBase + 1
#define SemaphoreDeleteSystemCallNumber               SemaphoreSystemCallNumberBase + 2
#define SemaphoreGetSystemCallNumber                  SemaphoreSystemCallNumberBase + 3
#define SemaphoreReleaseSystemCallNumber              SemaphoreSystemCallNumberBase + 4



#define MutexSystemCallNumberBase                     512
#define MutexGetNameSystemCallNumber                  MutexSystemCallNumberBase + 0
#define MutexCreateNewSystemCallNumber                MutexSystemCallNumberBase + 1
#define MutexDeleteSystemCallNumber                   MutexSystemCallNumberBase + 2
#define MutexGetSystemCallNumber                      MutexSystemCallNumberBase + 3
#define MutexReleaseSystemCallNumber                  MutexSystemCallNumberBase + 4



typedef struct _threadGetIdArg                        _threadGetIdArg_t;
typedef struct _threadGetNameArg                      _threadGetNameArg_t;
typedef struct _threadGetStateArg                     _threadGetStateArg_t;
typedef struct _threadGetPriorityArg                  _threadGetPriorityArg_t;
typedef struct _threadSetPriorityArg                  _threadSetPriorityArg_t;
typedef struct _threadGetStackSizeArg                 _threadGetStackSizeArg_t;
typedef struct _threadCreateNewArg                    _threadCreateNewArg_t;
typedef struct _threadTerminateArg                    _threadTerminateArg_t;
typedef struct _threadYieldArg                        _threadYieldArg_t;
typedef struct _threadSuspendArg                      _threadSuspendArg_t;
typedef struct _threadResumeArg                       _threadResumeArg_t;
typedef struct _threadWaitArg                         _threadWaitArg_t;



typedef struct _semaphoreGetNameArg                   _semaphoreGetNameArg_t;
typedef struct _semaphoreCreateNewArg                 _semaphoreCreateNewArg_t;
typedef struct _semaphoreDeleteArg                    _semaphoreDeleteArg_t;
typedef struct _semaphoreGetArg                       _semaphoreGetArg_t;
typedef struct _semaphoreReleaseArg                   _semaphoreReleaseArg_t;



typedef struct _mutexGetNameArg                       _mutexGetNameArg_t;
typedef struct _mutexCreateNewArg                     _mutexCreateNewArg_t;
typedef struct _mutexDeleteArg                        _mutexDeleteArg_t;
typedef struct _mutexGetArg                           _mutexGetArg_t;
typedef struct _mutexReleaseArg                       _mutexReleaseArg_t;



struct _threadGetNameArg {
    _threadId_t                                       threadIdArg;
    _threadName_t                                     returnValue;
};
struct _threadGetStateArg {
    _threadId_t                                       threadIdArg;
    _threadState_t                                    returnValue;
};
struct _threadGetPriorityArg {
    _threadId_t                                       threadIdArg;
    _threadPriority_t                                 returnValue;
};
struct _threadSetPriorityArg {
    _threadId_t                                       threadIdArg;
    _threadPriority_t                                 threadPriorityArg;
    _rtosStatus_t                                     returnValue;
};
struct _threadGetStackSizeArg {
    _threadId_t                                       threadIdArg;
    _threadStackSize_t                                returnValue;
};
struct _threadCreateNewArg {
    _threadName_t                                     threadNameArg;
    _threadFunctionPointer_t                          threadFunctionPointerArg;
    _threadFunctionParameter_t                        threadFunctionParameterArg;
    _threadPriority_t                                 threadPriorityArg;
    _threadStackSize_t                                threadStackSizeArg;
    _threadId_t                                       returnValue;
};
struct _threadTerminateArg {
    _threadId_t                                       threadIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _threadYieldArg {
    _threadId_t                                       currentRunningThreadIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _threadSuspendArg {
    _threadId_t                                       threadIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _threadResumeArg {
    _threadId_t                                       threadIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _threadWaitArg {
    _threadId_t                                       threadIdArg;
    _kernelTick_t                                     numberOfTicksArg;
    _rtosStatus_t                                     returnValue;
};



struct _semaphoreGetNameArg {
    _semaphoreId_t                                    semaphoreIdArg;
    _semaphoreName_t                                  returnValue;
};
struct _semaphoreCreateNewArg {
    _semaphoreName_t                                  semaphoreNameArg;
    _semaphoreCount_t                                 semaphoreMaxCountArg;
    _semaphoreCount_t                                 semaphoreInitialCountArg;
    _semaphoreId_t                                    returnValue;
};
struct _semaphoreDeleteArg {
    _semaphoreId_t                                    semaphoreIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _semaphoreGetArg {
    _semaphoreId_t                                    semaphoreIdArg;
    _kernelTick_t                                     timeOutArg;
    _threadId_t                                       semaphoreCallerIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _semaphoreReleaseArg {
    _semaphoreId_t                                    semaphoreIdArg;
    _rtosStatus_t                                     returnValue;
};



struct _mutexGetNameArg {
    _mutexId_t                                        mutexIdArg;
    _mutexName_t                                      returnValue;
};
struct _mutexCreateNewArg {
    _mutexName_t                                      mutexNameArg;
    _mutexType_t                                      mutexTypeArg;
    _mutexId_t                                        returnValue;
};
struct _mutexDeleteArg {
    _mutexId_t                                        mutexIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _mutexGetArg {
    _mutexId_t                                        mutexIdArg;
    _kernelTick_t                                     timeOutArg;
    _threadId_t                                       mutexCallerIdArg;
    _rtosStatus_t                                     returnValue;
};
struct _mutexReleaseArg {
    _mutexId_t                                        mutexIdArg;
    _threadId_t                                       mutexCallerIdArg;
    _rtosStatus_t                                     returnValue;
};



void _threadGetNameSystemCall(_threadGetNameArg_t *arg);
void _threadGetStateSystemCall(_threadGetStateArg_t *arg);
void _threadGetPrioritySystemCall(_threadGetPriorityArg_t *arg);
void _threadSetPrioritySystemCall(_threadSetPriorityArg_t *arg);
void _threadGetStackSizeSystemCall(_threadGetStackSizeArg_t *arg);
void _threadCreateNewSystemCall(_threadCreateNewArg_t *arg);
void _threadTerminateSystemCall(_threadTerminateArg_t *arg);
void _threadYieldSystemCall(_threadYieldArg_t *arg);
void _threadSuspendSystemCall(_threadSuspendArg_t *arg);
void _threadResumeSystemCall(_threadResumeArg_t *arg);
void _threadWaitSystemCall(_threadWaitArg_t *arg);



void _semaphoreGetNameSystemCall(_semaphoreGetNameArg_t *arg);
void _semaphoreCreateNewSystemCall(_semaphoreCreateNewArg_t *arg);
void _semaphoreDeleteSystemCall(_semaphoreDeleteArg_t *arg);
void _semaphoreGetSystemCall(_semaphoreGetArg_t *arg);
void _semaphoreReleaseSystemCall(_semaphoreReleaseArg_t *arg);



void _mutexGetNameSystemCall(_mutexGetNameArg_t *arg);
void _mutexCreateNewSystemCall(_mutexCreateNewArg_t *arg);
void _mutexDeleteSystemCall(_mutexDeleteArg_t *arg);
void _mutexGetSystemCall(_mutexGetArg_t *arg);
void _mutexReleaseSystemCall(_mutexReleaseArg_t *arg);



#endif /* RTOS_INCLUDE_SYSTEMCALLTYPEDEFINTIONS_H_ */
