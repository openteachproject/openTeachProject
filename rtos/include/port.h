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
 * port.h
 *
 *  Created on: Aug 24, 2021
 *      Author: salar
 */

#ifndef RTOS_INCLUDE_PORT_H_
#define RTOS_INCLUDE_PORT_H_



#include <rtos/include/rtosTypeDefinitions.h>



_status_t                               _portDeviceInitialize(void);
_status_t                               _portTickTimerStart(void);
_status_t                               _portThreadStackInitialize(_threadId_t id);
_bool_t                                 _portIsInInterrupt(void);
_status_t                               _portStartFirstThread(void);

_status_t                               _portSupervisorInterruptTrigger(void);
_status_t                               _portPreSupervisorInterruptTrigger(void);

_atomicValue_t                          _portAtomicRead32(_atomicAddress_t address);
_atomicResult_t                         _portAtomicWrite32(_atomicValue_t value, _atomicAddress_t address);
_status_t                               _portAtomicClear(void);



#endif /* RTOS_INCLUDE_PORT_H_ */
