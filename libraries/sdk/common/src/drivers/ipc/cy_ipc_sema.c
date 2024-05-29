/***************************************************************************//**
* \file cy_ipc_sema.c
* \version 1.10
*
*  Description:
*   IPC Semaphore Driver - This source file contains the source code for the
*   semaphore level APIs for the IPC interface.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ipc/cy_ipc_drv.h"
#include "ipc/cy_ipc_sema.h"
#include "syslib/cy_syslib.h"
#include <string.h> /* The memset() definition */

/* Defines a mask to Check if semaphore count is a multiple of 32 */
#define CY_IPC_SEMA_PER_WORD_MASK    (CY_IPC_SEMA_PER_WORD - 1ul)

/* Pointer to IPC structure used for semaphores */
static volatile stc_IPC_STRUCT_t* cy_semaIpcStruct = NULL;
/*
* Internal IPC semaphore control data structure.
*/
typedef struct {
    uint32_t maxSema;      /* Maximum semaphores in system */
    uint32_t *arrayPtr;    /* Pointer to semaphores array  */
} cy_stc_ipc_sema_t;

/*******************************************************************************
* Function Name: Cy_IPC_Sema_Init
****************************************************************************//**
*
* This function initializes the semaphores subsystem. The user must create an
* array of unsigned 32-bit words to hold the semaphore bits. The number
* of semaphores will be the size of the array * 32. The total semaphores count
* will always be a multiple of 32.
*
* \note In a multi-CPU system this init function should be called with all
* initialized parameters on one CPU only to provide a pointer to SRAM that can
* be shared between all the CPUs in the system that will use semaphores.
* On other CPUs user must specify the IPC semaphores channel and pass 0 / NULL
* to count and memPtr parameters correspondingly.
*
* \param ipcChannel
* The IPC channel number used for semaphores
*
* \param count
*  The maximum number of semaphores to be supported (multiple of 32).
*
* \param memPtr
*  This points to the array of (count/32) words that contain the semaphore data.
*
* \return
*    \retval CY_IPC_SEMA_SUCCESS: Successfully initialized
*    \retval CY_IPC_SEMA_BAD_PARAM:     Memory pointer is NULL and count is not zero,
*                             or count not multiple of 32
*    \retval CY_IPC_SEMA_ERROR_LOCKED:  Could not acquire semaphores IPC channel
*
*******************************************************************************/
cy_en_ipcsema_status_t Cy_IPC_Sema_Init(uint32_t ipcChannel,
                                        uint32_t count, uint32_t memPtr[])
{
    /* Structure containing semaphores control data */
    static cy_stc_ipc_sema_t       cy_semaData;

    cy_en_ipcsema_status_t retStatus = CY_IPC_SEMA_BAD_PARAM;

    if (ipcChannel >= CY_IPC_CHANNELS)
    {
        retStatus = CY_IPC_SEMA_BAD_PARAM;
    }
    else
    {
        if( (NULL == memPtr) && (0u == count))
        {
            cy_semaIpcStruct = Cy_IPC_Drv_GetIpcBaseAddress(ipcChannel);

            retStatus = CY_IPC_SEMA_SUCCESS;
        }

        /* Check for non Null pointers and count value */
        else if ((NULL != memPtr) && (0u != count))
        {
            /* Check if semaphore count is a multiple of 32 */
            if( 0ul == (count & CY_IPC_SEMA_PER_WORD_MASK))
            {
                cy_semaIpcStruct = Cy_IPC_Drv_GetIpcBaseAddress(ipcChannel);

                cy_semaData.maxSema  = count;
                cy_semaData.arrayPtr = memPtr;

                /* Initialize all semaphores to released */
                (void)memset(cy_semaData.arrayPtr, 0, (count /8u));
                
                __DSB();

                /* Make sure semaphores start out released.  */
                /* Ignore the return value since it is OK if it was already released. */
                (void) Cy_IPC_Drv_LockRelease (cy_semaIpcStruct, CY_IPC_NO_NOTIFICATION);

                 /* Set the IPC Data with the pointer to the array. */
                if( CY_IPC_DRV_SUCCESS == Cy_IPC_Drv_SendMsgPtr (cy_semaIpcStruct, CY_IPC_NO_NOTIFICATION, &cy_semaData))
                {
                    if(CY_IPC_DRV_SUCCESS == Cy_IPC_Drv_LockRelease (cy_semaIpcStruct, CY_IPC_NO_NOTIFICATION))
                    {
                        retStatus = CY_IPC_SEMA_SUCCESS;
                    }
                    else
                    {
                        /* IPC channel not released, still semaphored */
                        retStatus = CY_IPC_SEMA_ERROR_LOCKED;
                    }
                }
                else
                {
                    /* Could not acquire semaphore channel */
                    retStatus = CY_IPC_SEMA_ERROR_LOCKED;
                }
            }
            else
            {
                retStatus = CY_IPC_SEMA_BAD_PARAM;
            }
        }
        else
        {
            retStatus = CY_IPC_SEMA_BAD_PARAM;
        }
    }

    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_Sema_Set
****************************************************************************//**
*
* This function tries to acquire a semaphore. If the
* semaphore is not available, this function returns immediately with
* CY_IPC_SEMA_LOCKED.
*
* It first acquires the IPC channel that is used for all the semaphores, sets
* the semaphore if it is cleared, then releases the IPC channel used for the semaphore.
*
* \param semaNumber
*  The semaphore number to acquire.
*
* \param preemptable
*  When this parameter is enabled the function can be preempted by another
*  task or other forms of context switching in an RTOS environment.
*
* \return
*    \retval CY_IPC_SEMA_SUCCESS:      The semaphore was set successfully
*    \retval CY_IPC_SEMA_LOCKED:       The semaphore channel is busy or locked
*                                      by another process
*    \retval CY_IPC_SEMA_NOT_ACQUIRED: Semaphore was already set
*    \retval CY_IPC_SEMA_OUT_OF_RANGE: The semaphore number is not valid
*    \retval CY_IPC_SEMA_ERROR_UNINIT: Driver has not been initialized yet
*
*
*******************************************************************************/
cy_en_ipcsema_status_t Cy_IPC_Sema_Set(uint32_t semaNumber, bool preemptable)
{
    uint32_t semaIndex;
    uint32_t semaMask;
    uint32_t interruptState = 0ul;

    cy_stc_ipc_sema_t      *semaStruct;
    cy_en_ipcsema_status_t  retStatus = CY_IPC_SEMA_LOCKED;


    if(cy_semaIpcStruct == NULL)
    {
        retStatus = CY_IPC_SEMA_ERROR_UNINIT;
    }
    else
    {
        /* Get pointer to structure */
        semaStruct = (cy_stc_ipc_sema_t *)Cy_IPC_Drv_ReadDataValue(cy_semaIpcStruct);
        
        if (semaNumber < semaStruct->maxSema)
        {
            semaIndex = semaNumber / CY_IPC_SEMA_PER_WORD;
            semaMask = (uint32_t)(1ul << (semaNumber - (semaIndex * CY_IPC_SEMA_PER_WORD) ));

            if (!preemptable)
            {
                interruptState = Cy_SysLib_EnterCriticalSection();
            }

            /* Check to make sure the IPC channel is released
               If so, check if specific channel can be locked. */
            if(CY_IPC_DRV_SUCCESS == Cy_IPC_Drv_LockAcquire (cy_semaIpcStruct))
            {
                __DSB();
                
                if((semaStruct->arrayPtr[semaIndex] & semaMask) == 0ul)
                {
                    semaStruct->arrayPtr[semaIndex] |= semaMask;
                    retStatus = CY_IPC_SEMA_SUCCESS;
                }
                else
                {
                    retStatus = CY_IPC_SEMA_NOT_ACQUIRED;
                }
                
                __DSB();

                /* Release, but do not trigger a release event */
                (void) Cy_IPC_Drv_LockRelease (cy_semaIpcStruct, CY_IPC_NO_NOTIFICATION);
            }

            if (!preemptable)
            {
                Cy_SysLib_ExitCriticalSection(interruptState);
            }
        }
        else
        {
            retStatus = CY_IPC_SEMA_OUT_OF_RANGE;
        }
    }
    
    return(retStatus);
}


/*******************************************************************************
* Function Name: Cy_IPC_Sema_Clear
****************************************************************************//**
*
* This functions tries to releases a semaphore.
*
* It first acquires the IPC channel that is used for all the semaphores, clears
* the semaphore if it is set, then releases the IPC channel used for the semaphores.
*
* \param semaNumber
*  The index of the semaphore to release.
*
* \param preemptable
*  When this parameter is enabled the function can be preempted by another
*  task or other forms of context switching in an RTOS environment.
*
* \return
*    \retval CY_IPC_SEMA_SUCCESS:         The semaphore was cleared successfully
*    \retval CY_IPC_SEMA_NOT_ACQUIRED:    The semaphore was already cleared
*    \retval CY_IPC_SEMA_LOCKED:          The semaphore channel was semaphored or busy
*    \retval CY_IPC_SEMA_OUT_OF_RANGE:    The semaphore number is not valid
*    \retval CY_IPC_SEMA_ERROR_UNINIT:    Driver has not been initialized yet
*
*
*******************************************************************************/
cy_en_ipcsema_status_t Cy_IPC_Sema_Clear(uint32_t semaNumber, bool preemptable)
{
    uint32_t semaIndex;
    uint32_t semaMask;
    uint32_t interruptState = 0ul;

    cy_stc_ipc_sema_t      *semaStruct;
    cy_en_ipcsema_status_t  retStatus = CY_IPC_SEMA_LOCKED;


    if(cy_semaIpcStruct == NULL)
    {
        retStatus = CY_IPC_SEMA_ERROR_UNINIT;
    }
    else
    {
        /* Get pointer to structure */
        semaStruct = (cy_stc_ipc_sema_t *)Cy_IPC_Drv_ReadDataValue(cy_semaIpcStruct);
        
        if (semaNumber < semaStruct->maxSema)
        {
            semaIndex = semaNumber / CY_IPC_SEMA_PER_WORD;
            semaMask = (uint32_t)(1ul << (semaNumber - (semaIndex * CY_IPC_SEMA_PER_WORD) ));

            if (!preemptable)
            {
                interruptState = Cy_SysLib_EnterCriticalSection();
            }

            /* Check to make sure the IPC channel is released
               If so, check if specific channel can be locked. */
            if(CY_IPC_DRV_SUCCESS == Cy_IPC_Drv_LockAcquire (cy_semaIpcStruct))
            {
                __DSB();
                
                if((semaStruct->arrayPtr[semaIndex] & semaMask) != 0ul)
                {
                    semaStruct->arrayPtr[semaIndex] &= ~semaMask;
                    retStatus = CY_IPC_SEMA_SUCCESS;
                }
                else
                {
                    retStatus = CY_IPC_SEMA_NOT_ACQUIRED;
                }
                
                __DSB();

                /* Release, but do not trigger a release event */
                (void) Cy_IPC_Drv_LockRelease (cy_semaIpcStruct, CY_IPC_NO_NOTIFICATION);
            }

            if (!preemptable)
            {
                Cy_SysLib_ExitCriticalSection(interruptState);
            }
        }
        else
        {
            retStatus = CY_IPC_SEMA_OUT_OF_RANGE;
        }
    }
    
    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_Sema_Status
****************************************************************************//**
*
* This function returns the status of the semaphore.
*
* \param semaNumber
*  The index of the semaphore to return status.
*
* \return
*     \retval CY_IPC_SEMA_STATUS_LOCKED:    The semaphore is in the set state.
*     \retval CY_IPC_SEMA_STATUS_UNLOCKED:  The semaphore is in the cleared state.
*     \retval CY_IPC_SEMA_OUT_OF_RANGE:     The semaphore number is not valid
*     \retval CY_IPC_SEMA_ERROR_UNINIT:     Driver has not been initialized yet
*
*
*******************************************************************************/
cy_en_ipcsema_status_t Cy_IPC_Sema_Status(uint32_t semaNumber)
{
    cy_en_ipcsema_status_t   retStatus;
    uint32_t semaIndex;
    uint32_t semaMask;
    cy_stc_ipc_sema_t      *semaStruct;

    if(cy_semaIpcStruct == NULL)
    {
        retStatus = CY_IPC_SEMA_ERROR_UNINIT;
    }
    else
    {
        /* Get pointer to structure */
        semaStruct = (cy_stc_ipc_sema_t *)Cy_IPC_Drv_ReadDataValue(cy_semaIpcStruct);
        
        if (semaNumber < semaStruct->maxSema)
        {
            /* Get the index into the semaphore array and calculate the mask */
            semaIndex = semaNumber / CY_IPC_SEMA_PER_WORD;
            semaMask = (uint32_t)(1ul << (semaNumber - (semaIndex * CY_IPC_SEMA_PER_WORD) ));

            if((semaStruct->arrayPtr[semaIndex] & semaMask) != 0ul)
            {
                retStatus =  CY_IPC_SEMA_STATUS_LOCKED;
            }
            else
            {
                retStatus =  CY_IPC_SEMA_STATUS_UNLOCKED;
            }
        }
        else
        {
            retStatus = CY_IPC_SEMA_OUT_OF_RANGE;
        }
    }
    
    return(retStatus);
}

/*******************************************************************************
* Function Name: Cy_IPC_Sema_GetMaxSems
****************************************************************************//**
*
* This function returns the number of semaphores in the semaphores subsystem.
*
* \param maxSema
*  Pointer to where the max semaphore number will be stored
*
* \return
*     \retval CY_IPC_SEMA_SUCCESS:      #maxSema will contain valid number
*     \retval CY_IPC_SEMA_ERROR_UNINIT: Driver has not been initialized yet
*
*******************************************************************************/
cy_en_ipcsema_status_t Cy_IPC_Sema_GetMaxSems (uint32_t *maxSema)
{
    cy_stc_ipc_sema_t      *semaStruct;
    cy_en_ipcsema_status_t retStatus = CY_IPC_SEMA_SUCCESS;

    if(cy_semaIpcStruct == NULL)
    {
        *maxSema = 0;
        retStatus = CY_IPC_SEMA_ERROR_UNINIT;
    }
    else
    {
        /* Get pointer to structure */
        semaStruct = (cy_stc_ipc_sema_t *)Cy_IPC_Drv_ReadDataValue(cy_semaIpcStruct);
        
        *maxSema = semaStruct->maxSema;
    }
    
    return retStatus;
}


/*******************************************************************************
* Function Name: Cy_IPC_Sema_IsReady
****************************************************************************//**
*
* This function checks whether the IPC Sema "master" (the core for which
* Cy_IPC_Sema_Init is called with 'count' != 0 and 'memPtr' != NULL) has completed
* its Cy_IPC_Sema_Init call. The decision is made by checking whether the IPC data
* register of the semaphore IPC struct has changed its value from its default 0.
*
*
* \return
*     \retval true:  IPC sema "master" is ready
*     \retval false: IPC sema "master" has not yet finished initialization or 
*                    driver on this core has not yet been initialized
*
*******************************************************************************/
bool Cy_IPC_Sema_IsReady (void)
{
    cy_stc_ipc_sema_t      *semaStruct;

    // Check first if Cy_IPC_Sema_Init has been called on this core at all...
    if(cy_semaIpcStruct != NULL)
    {
        /* Get pointer to structure */
        semaStruct = (cy_stc_ipc_sema_t *)Cy_IPC_Drv_ReadDataValue(cy_semaIpcStruct);
        
        if(semaStruct != NULL)
        {
            return true;
        }
    }
    
    return false;
}

/* [] END OF FILE */
