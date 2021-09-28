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
    _status_t                                         returnValue;
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
    _status_t                                         returnValue;
};
struct _threadYieldArg {
    _threadId_t                                       currentRunningThreadIdArg;
    _status_t                                         returnValue;
};
struct _threadSuspendArg {
    _threadId_t                                       threadIdArg;
    _status_t                                         returnValue;
};
struct _threadResumeArg {
    _threadId_t                                       threadIdArg;
    _status_t                                         returnValue;
};
struct _threadWaitArg {
    _threadId_t                                       threadIdArg;
    _kernelTick_t                                     numberOfTicksArg;
    _status_t                                         returnValue;
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



#endif /* RTOS_INCLUDE_SYSTEMCALLTYPEDEFINTIONS_H_ */
