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
 * coreInterruptHandler.S
 *
 *  Created on: Aug 28, 2021
 *      Author: salar
 */



            .section .text
            .align    1
            .syntax unified
            .global    PendSV_Handler
            .type    PendSV_Handler, %function
PendSV_Handler:



            PUSH                        {R0, LR}
            B                           .LCheckForContextSwitchStart



.LCheckForContextSwitchStart:
            BL                          _kernelGetContextSwitchRequest
            SUBS                        R0, #1
            IT                          EQ
            BEQ                         .LContextSwitch
.LContextSwitchEnd:



            B                           .LPendSvHandlerExit



.LContextSwitch:
            MRS                         R3, PSP
            STMDB                       R3!, {R4-R11}
            MSR                         PSP, R3
            MOV                         R0, R3
            BL                          _kernelSetCurrentRunningThreadStackPointer
            BL                          _kernelContextSwitch
            BL                          _kernelGetCurrentRunningThreadStackPointer
            MOV                         R3, R0
            LDMIA                       R3!, {R4-R11}
            MSR                         PSP, R3
            BL                          _kernelUnsetContextSwitchReuest
            B                           .LContextSwitchEnd



.LPendSvHandlerExit:
            POP                         {R0, LR}
            BX                          LR
            .size                       PendSV_Handler, .-PendSV_Handler



            .section .text
            .align    1
            .syntax unified
            .global    SVC_Handler
            .type    SVC_Handler, %function
SVC_Handler:



            PUSH                        {R0, LR}



.LSystemCallStart:
            BL                          _kernelGetSystemCallRequest
            SUBS                        R0, #1
            IT                          EQ
            BEQ                         .LSyetemCallHandler
.LSystemCallEnd:



            B                           .LSvcHandlerExit



.LSyetemCallHandler:
            BL                          _kernelSystemCallHandler
            BL                          _kernelUnsetSystemCallRequest
            B                           .LSystemCallEnd



.LSvcHandlerExit:
            POP                         {R0, LR}
            BX                          LR
            .size                       SVC_Handler, .-SVC_Handler



            .section .text
            .align    1
            .syntax unified
            .global    SysTick_Handler
            .type    SysTick_Handler, %function
SysTick_Handler:



            PUSH                        {R0, LR}
            BL                          _kernelIncreaseTick
            BL                          _kernelContextSwitchRequest
            POP                         {R0, LR}
            BX                          LR



            .size        SysTick_Handler, .-SysTick_Handler



            .end



