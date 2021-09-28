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
#define ThreadCreateNewSystemCallNumber               ThreadSystemCallNumberBase + 1



typedef struct _threadCreateNewArg                    _threadCreateNewArg_t;



struct _threadCreateNewArg {
    _threadName_t                                     threadNameArg;
    _threadFunctionPointer_t                          threadFunctionPointerArg;
    _threadFunctionParameter_t                        threadFunctionParameterArg;
    _threadPriority_t                                 threadPriorityArg;
    _threadStackSize_t                                threadStackSizeArg;
    _threadId_t                                       returnValue;
};



void _threadCreateNewSystemCall(_threadCreateNewArg_t *arg);



#endif /* RTOS_INCLUDE_SYSTEMCALLTYPEDEFINTIONS_H_ */
