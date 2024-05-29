/***************************************************************************//**
* \file SchM_Can.c
*
* \brief
* SchM_Can stub implementation
*
********************************************************************************
* \copyright
* Copyright 2016-2022, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


/*==================[inclusions]============================================*/

#include <SchM_Can.h>    
#include <Interrupts_DisableRestore.h>
#include <Det.h>

/*==================[local variables]=======================================*/
/*==================[global variables]======================================*/

#define SCHM_START_SEC_VAR_UNSPECIFIED
#include <SchM_MemMap.h>

SCHM_DEFINE_EXCLUSIVE_AREA_VAR(Can)

#define SCHM_STOP_SEC_VAR_UNSPECIFIED
#include <SchM_MemMap.h>

/*==================[external function definitions]=========================*/
#define SCHM_START_SEC_CODE
#include <SchM_MemMap.h>

SCHM_DEFINE_EXCLUSIVE_AREA_FUNC(Can, CAN, 0)

#define SCHM_STOP_SEC_CODE
#include <SchM_MemMap.h>
/*==================[end of file]===========================================*/

