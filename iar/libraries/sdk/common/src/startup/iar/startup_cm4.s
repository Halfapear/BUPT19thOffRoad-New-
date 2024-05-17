;/**************************************************************************//**
; * @file     startup_cm4.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM4 Device Series
; ******************************************************************************/
;* \copyright
;* Copyright 2018-2021, Cypress Semiconductor Corporation.  All rights reserved.
;* You may use this file only in accordance with the license, terms, conditions,
;* disclaimers, and limitations in the end user license agreement accompanying
;* the software package with which this file was provided.
;*******************************************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        
;************************************************************************
;* Local definitions
;************************************************************************

; Set a sufficient startup stack size for correct operation of C startup code (startup.c)
STARTUP_STACK_SIZE_DOUBLE_WORDS    EQU 32

VTOR                               EQU 0xe000ed08


;************************************************************************
;* Import symbols
;************************************************************************

        EXTERN  CpuUserInt0_Handler
        EXTERN  CpuUserInt1_Handler
        EXTERN  CpuUserInt2_Handler
        EXTERN  CpuUserInt3_Handler
        EXTERN  CpuUserInt4_Handler
        EXTERN  CpuUserInt5_Handler
        EXTERN  CpuUserInt6_Handler
        EXTERN  CpuUserInt7_Handler
        EXTERN  __iar_program_start
        EXTERN  __iar_data_init3
        EXTERN  Startup_Init
        

;************************************************************************
;* Export symbols
;************************************************************************

        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size
        PUBLIC  __ramVectors
        PUBLIC  Cy_SaveIRQ
        PUBLIC  Cy_RestoreIRQ
        PUBLIC  Cy_u32StartupStackStartAddress
        PUBLIC  Cy_u32StartupStackEndAddress


;************************************************************************
;* Vector Table and RAM Vector Table
;************************************************************************
        
        SECTION CSTACK:DATA:NOROOT(3)
        
        ; align to 1024 byte, because CM4_VECTOR_TABLE_BASE register only supports address bits [31:10] (Note: for VTOR a 128-byte alignment would be ok)
        SECTION .intvec:CODE:ROOT(10)
        DATA

__iar_init$$done:   ; The vector table is not needed until after copy initialization is done

__vector_table:
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
__vector_table_0x1c:
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External interrupts              PowerMode    Description

        DCD     CpuUserInt0_Handler      ; DeepSleep    CPU User Interrupt #0
        DCD     CpuUserInt1_Handler      ; DeepSleep    CPU User Interrupt #1
        DCD     CpuUserInt2_Handler      ; DeepSleep    CPU User Interrupt #2
        DCD     CpuUserInt3_Handler      ; DeepSleep    CPU User Interrupt #3
        DCD     CpuUserInt4_Handler      ; DeepSleep    CPU User Interrupt #4
        DCD     CpuUserInt5_Handler      ; DeepSleep    CPU User Interrupt #5
        DCD     CpuUserInt6_Handler      ; DeepSleep    CPU User Interrupt #6
        DCD     CpuUserInt7_Handler      ; DeepSleep    CPU User Interrupt #7

        ; These IRQs can only be triggered by SW via NVIC regs
        DCD     CpuUserInt8_Handler      ; Active       CPU User Interrupt #8
        DCD     CpuUserInt9_Handler      ; Active       CPU User Interrupt #9
        DCD     CpuUserInt10_Handler     ; Active       CPU User Interrupt #10
        DCD     CpuUserInt11_Handler     ; Active       CPU User Interrupt #11
        DCD     CpuUserInt12_Handler     ; Active       CPU User Interrupt #12
        DCD     CpuUserInt13_Handler     ; Active       CPU User Interrupt #13
        DCD     CpuUserInt14_Handler     ; Active       CPU User Interrupt #14
        DCD     CpuUserInt15_Handler     ; Active       CPU User Interrupt #15
__Vectors_End:

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors

        ; use same alignment like vector table in ROM above (even though VTOR minimum requirement would be 128 bytes if not used in combination with CM4_VECTOR_TABLE_BASE register)
        SECTION .intvec_ram:DATA:ROOT(10)
__ramVectors:
        DS8     __Vectors_Size


;************************************************************************
;* Helper functions
;************************************************************************

        THUMB



; Saves and disables the interrupts

        PUBLIC Cy_SaveIRQ
        SECTION .text:CODE:REORDER:NOROOT(2)
Cy_SaveIRQ:
        MRS   r0, PRIMASK
        CPSID I
        BX    LR


;-----------------------------------------

; Restores the interrupts

        PUBLIC Cy_RestoreIRQ
        SECTION .text:CODE:REORDER:NOROOT(2)
Cy_RestoreIRQ:
        MSR   PRIMASK, r0
        BX    LR

;-----------------------------------------

; Define strong version to return zero for
; __iar_program_start to skip data sections
; initialization.

        PUBLIC __low_level_init
        SECTION .text:CODE:REORDER:NOROOT(2)
__low_level_init
        MOVS R0, #0
        BX LR

        
;************************************************************************
;* Start-up Code
;************************************************************************

        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler:

; Disable global interrupts
        CPSID   I
        
; Update Vector Table Offset Register with address of user ROM table
; (will be updated later to user RAM table address in C startup code)
        LDR  r0, =__vector_table
        LDR  r1, =VTOR
        STR  r0, [r1]
        DSB
        
; Initialize ECC of startup stack (needed for local variables in C startup code) by processing 8 bytes per loop iteration,
; because the ECC initialization feature uses this generic granularity that will cover any memory (SRAM/TCM) in any TVII device
; Prerequisite: Stack Pointer (SP) has not been modified (from the vector table init value) by above code (otherwise code must be adapted)
        MOVS r0, #0 ; clear value
        MOVS r1, #0 ; clear value
        LDR  r2, Cy_u32StartupStackStartAddress
startup_stack_ecc_init_loop:
        STRD r0, r1, [r2], #8
        CMP  r2, sp
        BNE  startup_stack_ecc_init_loop
        
; Call C startup code (no ANSI C context established yet!)
        LDR  r7, =Startup_Init
        BLX  r7

; Initialize data sections
        LDR  r7, =__iar_data_init3
        BLX  r7

        LDR  r7, =__iar_program_start
        BLX  r7

; Note: Control flow does not necessarily return here.
; On some tool-chains (e.g. IAR) control flow will never return from
; the system library.
Cy_Main_Exited:
        B    Cy_Main_Exited

;************************************************************************
;* Literal pool
;************************************************************************

        ALIGNROM 2
        
        LTORG

        DATA
        
        ALIGNROM 2

STARTUP_STACK_SIZE_BYTES    EQU (STARTUP_STACK_SIZE_DOUBLE_WORDS * 8)  ; Multiplication does not work in below data definition directive, so an additional define is created

Cy_u32StartupStackStartAddress:
        DCD (sfe(CSTACK) - STARTUP_STACK_SIZE_BYTES)

Cy_u32StartupStackEndAddress:
        DCD (sfe(CSTACK) - 1)

        

;************************************************************************
;* Default and weak implementation of interrupt handlers
;************************************************************************


        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
NMI_Handler:
        B NMI_Handler

;-----------------------------------------

        PUBWEAK Cy_SysLib_FaultHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Cy_SysLib_FaultHandler:
        B Cy_SysLib_FaultHandler

;-----------------------------------------

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
HardFault_Handler:
        IMPORT Cy_SysLib_FaultHandler
        MOVS r0, #4
        MOV  r1, lr
        TST  r0, r1
        BEQ  L_MSP
        MRS  r0, PSP
        B    L_API_call
L_MSP
        MRS  r0, MSP
L_API_call
        ; Storing LR content for Creator call stack trace
        PUSH {lr}
        LDR  r1, =Cy_SysLib_FaultHandler
        BLX  r1
        POP  {pc}

;-----------------------------------------

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
MemManage_Handler:
        B HardFault_Handler

;-----------------------------------------

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
BusFault_Handler:
        B HardFault_Handler

;-----------------------------------------

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
UsageFault_Handler:
        B HardFault_Handler

;-----------------------------------------

        IMPORT Cy_SysLib_SvcHandler
        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
SVC_Handler:
        MOVS r0, #4
        MOV  r1, lr
        TST  r0, r1
        BEQ  L_SVC_MSP
        MRS  r0, PSP
L_SVC_MSP:
        MRS  r0, MSP
        ; Storing LR content for Creator call stack trace
        PUSH {lr}
        LDR  r1, =Cy_SysLib_SvcHandler
        BLX  r1
        POP  {pc}

;-----------------------------------------

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
DebugMon_Handler:
        B DebugMon_Handler

;-----------------------------------------

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
PendSV_Handler:
        B PendSV_Handler

;-----------------------------------------

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
SysTick_Handler:
        B SysTick_Handler


;-----------------------------------------


        ; External interrupts
        ; Traveo II CPU User Interrupts 0-7 handlers are defined in the project interrupt mapping file
        ; Traveo II CPU User Interrupts 8-15 can only be used as SW interrupts and need to be defined by user (weak implementation provided below)


        PUBWEAK CpuUserInt8_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt8_Handler:
        B CpuUserInt8_Handler

;-----------------------------------------

        PUBWEAK CpuUserInt9_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt9_Handler:
        B CpuUserInt9_Handler

;-----------------------------------------

        PUBWEAK CpuUserInt10_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt10_Handler:
        B CpuUserInt10_Handler

;-----------------------------------------

        PUBWEAK CpuUserInt11_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt11_Handler:
        B CpuUserInt11_Handler

;-----------------------------------------

        PUBWEAK CpuUserInt12_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt12_Handler:
        B CpuUserInt12_Handler

;-----------------------------------------

        PUBWEAK CpuUserInt13_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt13_Handler:
        B CpuUserInt13_Handler

;-----------------------------------------

        PUBWEAK CpuUserInt14_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt14_Handler:
        B CpuUserInt14_Handler

;-----------------------------------------

        PUBWEAK CpuUserInt15_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
CpuUserInt15_Handler:
        B CpuUserInt15_Handler

;-----------------------------------------


;************************************************************************
;* File end
;************************************************************************

        END


