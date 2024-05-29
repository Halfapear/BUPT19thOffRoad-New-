/***************************************************************************//**
* \file cy_scb_ezi2c.c
* \version 1.0
*
* Provides EZI2C API implementation of the SCB driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_scb_ezi2c.h"

#if defined(__cplusplus)
extern "C" {
#endif


/***************************************
*        Function Prototypes
***************************************/

static void HandleErrors      (volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context);
static void HandleAddress     (volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context);
static void UpdateRxFifoLevel (volatile stc_SCB_t *base, uint32_t bufSize);
static void HandleDataReceive (volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context);
static void HandleDataTransmit(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context);
static void HandleStop        (volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context);
static void UpdateAddressMask (volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t const *context);


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Init
****************************************************************************//**
*
* Initializes the SCB for the EZI2C operation.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param config
* The pointer to the configuration structure \ref cy_stc_scb_ezi2c_config_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \return
* \ref cy_en_scb_ezi2c_status_t
*
* \note
* Ensure that the SCB block is disabled before calling this function.
*
*******************************************************************************/
cy_en_scb_ezi2c_status_t Cy_SCB_EZI2C_Init(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_config_t const *config,
                                           cy_stc_scb_ezi2c_context_t *context)
{
    cy_en_scb_ezi2c_status_t retStatus = CY_SCB_EZI2C_BAD_PARAM;
    un_SCB_CTRL_t         temp_CTRL         = { 0ul };
    un_SCB_I2C_CTRL_t     temp_I2C_CTRL     = { 0ul };
    un_SCB_RX_CTRL_t      temp_RX_CTRL      = { 0ul };
    un_SCB_TX_CTRL_t      temp_TX_CTRL      = { 0ul };

    if ((NULL != base) && (NULL != config) && (NULL != context))
    {
        /* Configure the EZI2C interface */
        temp_CTRL.stcField.u1ADDR_ACCEPT = ((config->numberOfAddresses == CY_SCB_EZI2C_TWO_ADDRESSES) ? 1ul : 0ul);
        temp_CTRL.stcField.u1EC_AM_MODE  = (config->enableWakeFromSleep ? 1ul : 0ul);
        temp_CTRL.stcField.u2MEM_WIDTH   = 0ul;
        temp_CTRL.stcField.u1EZ_MODE     = 1ul;
        base->unCTRL.u32Register         = temp_CTRL.u32Register;

        temp_I2C_CTRL.stcField.u1S_READY_ADDR_ACK = 1ul;
        temp_I2C_CTRL.stcField.u1S_READY_DATA_ACK = 1ul;
        temp_I2C_CTRL.stcField.u1S_GENERAL_IGNORE = 1ul;
        temp_I2C_CTRL.stcField.u1SLAVE_MODE       = 1ul;
        base->unI2C_CTRL.u32Register              = temp_I2C_CTRL.u32Register;

        /* Configure the RX direction */
        temp_RX_CTRL.stcField.u5DATA_WIDTH = CY_SCB_I2C_DATA_WIDTH;
        temp_RX_CTRL.stcField.u1MSB_FIRST  = 1ul;
        base->unRX_CTRL.u32Register        = temp_RX_CTRL.u32Register;
        base->unRX_FIFO_CTRL.u32Register   = 0ul;

        /* Set the default address and mask */
        if (config->numberOfAddresses == CY_SCB_EZI2C_ONE_ADDRESS)
        {
            context->address2 = 0u;
            Cy_SCB_EZI2C_SetAddress1(base, config->slaveAddress1, context);
        }
        else
        {
            Cy_SCB_EZI2C_SetAddress1(base, config->slaveAddress1, context);
            Cy_SCB_EZI2C_SetAddress2(base, config->slaveAddress2, context);
        }

        /* Configure the TX direction */
        temp_TX_CTRL.stcField.u5DATA_WIDTH = CY_SCB_I2C_DATA_WIDTH;
        temp_TX_CTRL.stcField.u1MSB_FIRST  = 1ul;
        temp_TX_CTRL.stcField.u1OPEN_DRAIN = 1ul;
        base->unTX_CTRL.u32Register        = temp_TX_CTRL.u32Register;
        base->unTX_FIFO_CTRL.u32Register = CY_SCB_EZI2C_HALF_FIFO_SIZE;

        /* Configure the interrupt sources */
        base->unINTR_SPI_EC_MASK.u32Register = 0ul;
        base->unINTR_I2C_EC_MASK.u32Register = 0ul;
        base->unINTR_RX_MASK.u32Register     = 0ul;
        base->unINTR_TX_MASK.u32Register     = 0ul;
        base->unINTR_M_MASK.u32Register      = 0ul;
        base->unINTR_S_MASK.u32Register      = CY_SCB_EZI2C_SLAVE_INTR;

        /* Initialize the context */
        context->status = 0ul;
        context->state  = CY_SCB_EZI2C_STATE_IDLE;

        context->subAddrSize = config->subAddressSize;

        context->buf1Size      = 0ul;
        context->buf1rwBondary = 0ul;
        context->baseAddr1     = 0ul;

        context->buf1Size      = 0ul;
        context->buf1rwBondary = 0ul;
        context->baseAddr2     = 0ul;

        retStatus = CY_SCB_EZI2C_SUCCESS;
    }

    return (retStatus);
}


/*******************************************************************************
*  Function Name: Cy_SCB_EZI2C_DeInit
****************************************************************************//**
*
* De-initializes the SCB block, returns the register values to default.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \note
* Ensure that the SCB block is disabled before calling this function.
*
*******************************************************************************/
void Cy_SCB_EZI2C_DeInit(volatile stc_SCB_t *base)
{
    /* Returns the block registers into the default state */
    base->unCTRL.u32Register     = CY_SCB_CTRL_DEF_VAL;
    base->unI2C_CTRL.u32Register = CY_SCB_I2C_CTRL_DEF_VAL;

    base->unRX_CTRL.u32Register      = CY_SCB_RX_CTRL_DEF_VAL;
    base->unRX_FIFO_CTRL.u32Register = 0ul;
    base->unRX_MATCH.u32Register     = 0ul;

    base->unTX_CTRL.u32Register      = CY_SCB_TX_CTRL_DEF_VAL;
    base->unTX_FIFO_CTRL.u32Register = 0ul;

    base->unINTR_SPI_EC_MASK.u32Register = 0ul;
    base->unINTR_I2C_EC_MASK.u32Register = 0ul;
    base->unINTR_RX_MASK.u32Register     = 0ul;
    base->unINTR_TX_MASK.u32Register     = 0ul;
    base->unINTR_M_MASK.u32Register      = 0ul;
    base->unINTR_S_MASK.u32Register      = 0ul;
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Disable
****************************************************************************//**
*
* Disables the SCB block and clears the context statuses.
* Note that after the block is disabled, the TX and RX FIFOs and hardware
* statuses are cleared. Also, the hardware stops driving the output and
* ignores the input.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \note
* Calling this function when EZI2C is busy (the slave was addressed and is
* communicating with the master), may cause transaction corruption because
* the hardware stops driving the output and ignores the input. Ensure that
* the EZI2C slave is not busy before calling this function.
*
*******************************************************************************/
void Cy_SCB_EZI2C_Disable(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context)
{
    base->unCTRL.stcField.u1ENABLED = 0ul;

    /* Set the state to default and clear the statuses */
    context->status = 0ul;
    context->state  = CY_SCB_EZI2C_STATE_IDLE;
}


/*******************************************************************************
* Function Name: Cy_EZI2C_DeepSleepCallback
****************************************************************************//**
*
* This function handles the transition of the EZI2C SCB into and out of 
* Deep-Sleep mode. It prevents the device from entering Deep-Sleep mode 
* if the EZI2C slave is actively communicating. 
* The following behavior of the EZI2C depends on if the SCB block is 
* wakeup-capable or not:
* * The SCB wakeup-capable: on the incoming EZI2C slave address, the slave  
*   receives address and stretches the clock until the device is awoken from 
*   Deep-Sleep mode. If the slave address occurs before the device enters 
*   Deep-Sleep mode, the device will not enter Deep-Sleep mode.
* * The SCB is not wakeup-capable: the EZI2C is disabled. It is enabled when 
*   the device failed to enter Deep-Sleep mode or it is awaken from Deep-Sleep 
*   mode. During the EZI2C is disabled, it stops driving the outputs and 
*   ignores the input lines. The slave NACKs all incoming addresses.
* 
* This function can be called during execution of \ref Cy_SysPm_DeepSleep,
* to do it, register this function as a callback before calling 
* \ref Cy_SysPm_DeepSleep : specify \ref CY_SYSPM_DEEPSLEEP as the callback 
* type and call \ref Cy_SysPm_RegisterCallback.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
* \param mode
* \ref cy_en_syspm_callback_mode_t
*
* \return
* \ref cy_en_syspm_status_t
*
* \warning
* For proper operation, when the EZI2C slave is configured to be a wakeup source 
* from Deep-Sleep mode, this function must be copied and modified by the user. 
* The EZI2C clock disable code must be inserted in the 
* \ref CY_SYSPM_BEFORE_TRANSITION and enable code in the \ref CY_SYSPM_AFTER_TRANSITION 
* mode processing.
*
*******************************************************************************/
cy_en_syspm_status_t Cy_EZI2C_DeepSleepCallback(void *base, void *context, cy_en_syspm_callback_mode_t mode)
{
    cy_en_syspm_status_t retStatus = CY_SYSPM_SUCCESS;

    volatile stc_SCB_t *locBase = (volatile stc_SCB_t *) base;
    cy_stc_scb_ezi2c_context_t *locContext = (cy_stc_scb_ezi2c_context_t *) context;

    switch (mode)
    {
    case CY_SYSPM_CHECK_READY:
    {
        /* Disable the slave interrupt sources to protect the state */
        Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_CLEAR_ALL_INTR_SRC);

        /* If the EZI2C is in the IDLE state, it is ready for Deep-Sleep
        *  mode, otherwise return fail and restore the slave interrupt 
        * sources.
        */
        if(CY_SCB_EZI2C_STATE_IDLE == locContext->state)
        {
            if(locBase->unCTRL.stcField.u1EC_AM_MODE)
            {
                /* The SCB is wakeup-capable: do not restore the address 
                * match interrupt source. The next transaction intended 
                * for the slave will be paused (SCL is stretched) before 
                * the address is ACKed because the corresponding interrupt 
                * source is disabled.
                */
                Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_EZI2C_SLAVE_INTR_NO_ADDR);
            }
            else
            {
                /* The SCB is NOT wakeup-capable: disable the EZI2C. 
                * The slave stops responding to the master until the 
                * EZI2C is enabled. This happens when the device is failed 
                * to enter Deep-Sleep mode or it is awaken from Deep-Sleep 
                * mode.
                */
                Cy_SCB_EZI2C_Disable(locBase, locContext);
                Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_EZI2C_SLAVE_INTR);
            }
        }
        else
        {
            Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_EZI2C_SLAVE_INTR);
            retStatus = CY_SYSPM_FAIL;
        }
    }
        break;

    case CY_SYSPM_CHECK_FAIL:
    {
        /* The other driver is not ready for Deep-Sleep mode. Restore 
        * Active mode configuration.
        */

        if(locBase->unCTRL.stcField.u1EC_AM_MODE)
        {
            /* The SCB is wakeup-capable: restore the slave interrupt 
            * sources 
            */
            Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_EZI2C_SLAVE_INTR);
        }
        else
        {
            /* The SCB is NOT wakeup-capable: enable the slave to operate */
            Cy_SCB_EZI2C_Enable(locBase);
        }
    }
        break;

    case CY_SYSPM_BEFORE_ENTER:
    {
        /* This code executes inside the critical section and enabling the 
        * active interrupt source makes the interrupt pending in the NVIC.
        * However, the interrupt processing is delayed until the code exists 
        * the critical section. The pending interrupt force WFI instruction 
        * does nothing and the device remains in Active mode.
        */

        if(locBase->unCTRL.stcField.u1EC_AM_MODE)
        {
            /* The SCB is wakeup-capable: enable the I2C wakeup interrupt 
            * source. If any transaction was paused the the EZI2C interrupt
            * becomes pending and prevents entering Deep-Sleep mode.  
            * The transaction continues as soon as the global interrupts 
            * are enabled.
            */
            Cy_SCB_SetI2CInterruptMask(locBase, CY_SCB_I2C_INTR_WAKEUP);

            /* IMPORTANT (insert code below): for proper entering Deep
            * Sleep the EZI2C clock must be disabled. This code must be 
            * inserted by the user because the driver does not have access 
            * to the clock.
            */
        }
    }
        break;

    case CY_SYSPM_AFTER_EXIT:
    {
        if(locBase->unCTRL.stcField.u1EC_AM_MODE)
        {
            /* IMPORTANT (insert code below): for proper exiting Deep  
            * Sleep, the EZI2C clock must be enabled. This code must be 
            * inserted by the user because the driver does not have access 
            * to the clock.
            */

            /* The SCB is wakeup-capable: disable the I2C wakeup interrupt 
            * source and restore slave interrupt sources.
            */
            Cy_SCB_SetI2CInterruptMask(locBase, CY_SCB_CLEAR_ALL_INTR_SRC);
            Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_EZI2C_SLAVE_INTR);
        }
        else
        {
            /* The SCB is NOT wakeup-capable: enable the slave to operate */
            Cy_SCB_EZI2C_Enable(locBase);
        }
    }
        break;

    default:
        retStatus = CY_SYSPM_FAIL;
        break;
    }

    return (retStatus);
}


/*******************************************************************************
* Function Name: Cy_EZI2C_HibernateCallback
****************************************************************************//**
*
* This function handles the transition of the EZI2C SCB block into Hibernate 
* mode. It prevents the device from entering Hibernate mode if the EZI2C slave 
* is actively communicating.  
* If the EZI2C is ready to enter Hibernate mode, it is disabled. If the device  
* failed to enter Hibernate mode, the EZI2C is enabled. During the EZI2C  
* is disabled, it does stops driving the output and ignores the inputs. 
* The slave NACKs all incoming addresses.
* 
* This function can be called during execution of \ref Cy_SysPm_Hibernate,
* to do it, register this function as a callback before calling 
* \ref Cy_SysPm_Hibernate : specify \ref CY_SYSPM_HIBERNATE as the callback 
* type and call \ref Cy_SysPm_RegisterCallback.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
* \param mode
* \ref cy_en_syspm_callback_mode_t
*
* \return
* \ref cy_en_syspm_status_t
*
*******************************************************************************/
cy_en_syspm_status_t Cy_EZI2C_HibernateCallback(void *base, void *context, cy_en_syspm_callback_mode_t mode)
{
    cy_en_syspm_status_t retStatus = CY_SYSPM_SUCCESS;

    volatile stc_SCB_t *locBase = (volatile stc_SCB_t *) base;
    cy_stc_scb_ezi2c_context_t *locContext = (cy_stc_scb_ezi2c_context_t *) context;

    switch (mode)
    {
    case CY_SYSPM_CHECK_READY:
    {
        /* Disable the slave interrupt sources to protect the state */
        Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_CLEAR_ALL_INTR_SRC);

        /* If the EZI2C is in the IDLE state, it is ready for Hibernate mode,
        * otherwise return fail and restore the slave interrupt sources.
        */
        if(CY_SCB_EZI2C_STATE_IDLE == locContext->state)
        {
            /* Disable the EZI2C. It stops responding to the master until 
            * the EZI2C is enabled. This happens if the device failed to 
            * enter Hibernate mode.
            */
            Cy_SCB_EZI2C_Disable(locBase, locContext);
            Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_EZI2C_SLAVE_INTR);
        }
        else
        {
            Cy_SCB_SetSlaveInterruptMask(locBase, CY_SCB_EZI2C_SLAVE_INTR);
            retStatus = CY_SYSPM_FAIL;
        }
    }
        break;

    case CY_SYSPM_CHECK_FAIL:
    {
        /* The other driver is not ready for Hibernate mode. Restore the 
        * Active mode configuration.
        */

        /* Enable the slave to operate */
        Cy_SCB_EZI2C_Enable(locBase);
    }
        break;

    case CY_SYSPM_BEFORE_ENTER:
    case CY_SYSPM_AFTER_EXIT:
        /* The SCB is not capable of waking up from Hibernate mode: do nothing */
        break;

    default:
        retStatus = CY_SYSPM_FAIL;
        break;
    }

    return (retStatus);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_GetActivity
****************************************************************************//**
*
* Returns a non-zero value if an I2C Read or Write cycle has occurred since the
* last time this function was called. All flags are reset to zero at the end of
* this function call, except the \ref CY_SCB_EZI2C_STATUS_BUSY.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \return
* \ref group_scb_ezi2c_macro_get_activity.
*
*******************************************************************************/
uint32_t Cy_SCB_EZI2C_GetActivity(volatile stc_SCB_t const *base, cy_stc_scb_ezi2c_context_t *context)
{
    uint32_t intrState;
    uint32_t retStatus;

    /* Suppress a compiler warning about unused variables */
    (void) base;

    intrState = Cy_SysLib_EnterCriticalSection();

    retStatus = context->status;
    context->status &= CY_SCB_EZI2C_STATUS_BUSY;

    Cy_SysLib_ExitCriticalSection(intrState);

    return (retStatus);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetAddress1
****************************************************************************//**
*
* Sets the primary EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param addr
* The 7-bit right justified slave address.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetAddress1(volatile stc_SCB_t *base, uint8_t addr, cy_stc_scb_ezi2c_context_t *context)
{
    context->address1 = addr;

    // get-clear-modify-write
    base->unRX_MATCH.stcField.u8ADDR = 0ul;
    base->unRX_MATCH.stcField.u8ADDR = (uint32_t)((uint32_t) addr << 1ul);

    UpdateAddressMask(base, context);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_GetAddress1
****************************************************************************//**
*
* Returns the primary the EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* * \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \return
* The 7-bit right justified slave address.
*
*******************************************************************************/
uint32_t Cy_SCB_EZI2C_GetAddress1(volatile stc_SCB_t const *base, cy_stc_scb_ezi2c_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return ((uint32_t) context->address1);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetBuffer1
****************************************************************************//**
*
* Sets up the data buffer to be exposed to the I2C master on the primary slave
* address request.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param buf
* The pointer to the data buffer.
*
* \param bufSize
* The size of the buffer in bytes.
*
* \param rwBoundary
* The number of data bytes starting from the beginning of the buffer with Read and
* Write access. The data bytes located at rwBoundary or greater are read only.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \note
* * This fucntion is not interrupt-protected and to prevent a race condition,
*   it must be protected from the EZI2C interruption in the place where it
*   is called.
* * Calling this function in the middle of a transaction intended for the
*   secondary slave address leads to unexpected behavior.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetBuffer1(volatile stc_SCB_t const *base, uint8_t *buf, uint32_t bufSize, uint32_t rwBoundary,
                             cy_stc_scb_ezi2c_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    CY_ASSERT( ((NULL == buf) && (0u == bufSize)) || (NULL != buf) );

    context->buf1          = buf;
    context->buf1Size      = bufSize;
    context->buf1rwBondary = rwBoundary;
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetAddress2
****************************************************************************//**
*
* Sets the secondary EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param addr
* The 7-bit right justified slave address.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \note
* Calling this function when the EZI2C slave is configured for one-address
* operation leads to unexpected behavior because it updates the address mask.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetAddress2(volatile stc_SCB_t *base, uint8_t addr, cy_stc_scb_ezi2c_context_t *context)
{
    context->address2 = addr;

    UpdateAddressMask(base, context);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_GetAddress2
****************************************************************************//**
*
* Returns the secondary EZI2C slave address.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \return
* The 7-bit right justified slave address.
*
*******************************************************************************/
uint32_t Cy_SCB_EZI2C_GetAddress2(volatile stc_SCB_t const *base, cy_stc_scb_ezi2c_context_t const *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return ((uint32_t) context->address2);
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_SetBuffer2
****************************************************************************//**
*
* Sets up the data buffer to be exposed to the I2C master on the secondary
* slave address request.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param buf
* The pointer to the data buffer.
*
* \param bufSize
* The size of the buffer in bytes.
*
* \param rwBoundary
* The number of data bytes starting from the beginning of the buffer with Read and
* Write access. The data bytes located at rwBoundary or greater are read only.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t
* allocated by the user. The structure is used during the EZI2C operation for
* internal configuration and data keeping. The user must not modify anything
* in this structure.
*
* \note
* * This fucntion is not interrupt-protected and to prevent a race condition,
*   it must be protected from the EZI2C interruption in the place where it
*   is called.
* * Calling this function in the middle of a transaction intended for the
*   secondary slave address leads to unexpected behavior.
*
*******************************************************************************/
void Cy_SCB_EZI2C_SetBuffer2(volatile stc_SCB_t const *base, uint8_t *buf, uint32_t bufSize, uint32_t rwBoundary,
                             cy_stc_scb_ezi2c_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    CY_ASSERT( ((NULL == buf) && (0u == bufSize)) || (NULL != buf) );

    context->buf2          = buf;
    context->buf2Size      = bufSize;
    context->buf2rwBondary = rwBoundary;
}


/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Interrupt
****************************************************************************//**
*
* This is the interrupt function for the SCB configured in the EZI2C mode.
* This function must be called inside the user-defined interrupt service
* routine to make the EZI2C slave to work.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
void Cy_SCB_EZI2C_Interrupt(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context)
{
    uint32_t slaveIntrStatus;

    /* Handle an I2C wake-up event */
    if (0ul != (CY_SCB_I2C_INTR_WAKEUP & Cy_SCB_GetI2CInterruptStatusMasked(base)))
    {
        Cy_SCB_ClearI2CInterrupt(base, CY_SCB_I2C_INTR_WAKEUP);
    }

    /* Get the slave interrupt sources */
    slaveIntrStatus = Cy_SCB_GetSlaveInterruptStatusMasked(base);

    /* Handle the error conditions */
    if (0ul != (CY_SCB_EZI2C_SLAVE_INTR_ERROR & slaveIntrStatus))
    {
        HandleErrors(base, context);

        Cy_SCB_ClearSlaveInterrupt(base, CY_SCB_EZI2C_SLAVE_INTR_ERROR);

        /* Trigger the stop handling to complete the transaction */
        slaveIntrStatus |= CY_SCB_SLAVE_INTR_I2C_STOP;
    }
    else
    {
        if ((CY_SCB_EZI2C_STATE_RX_DATA1 == context->state) &&
            (0ul != (CY_SCB_SLAVE_INTR_I2C_STOP & slaveIntrStatus)))
        {
            /* Get data from the RX FIFO after Stop is generated */
            Cy_SCB_SetRxInterrupt    (base, CY_SCB_RX_INTR_LEVEL);
            Cy_SCB_SetRxInterruptMask(base, CY_SCB_RX_INTR_LEVEL);
        }
    }

    /* Handle the receive direction (master writes data) */
    if (0ul != (CY_SCB_RX_INTR_LEVEL & Cy_SCB_GetRxInterruptStatusMasked(base)))
    {
        HandleDataReceive(base, context);

        Cy_SCB_ClearRxInterrupt(base, CY_SCB_RX_INTR_LEVEL);
    }

    /* Handle the transaction completion */
    if (0ul != (CY_SCB_SLAVE_INTR_I2C_STOP & slaveIntrStatus))
    {
        HandleStop(base, context);

        Cy_SCB_ClearSlaveInterrupt(base, CY_SCB_SLAVE_INTR_I2C_STOP);

        /* Update the slave interrupt status */
        slaveIntrStatus = Cy_SCB_GetSlaveInterruptStatusMasked(base);
    }

    /* Handle the address byte */
    if (0ul != (CY_SCB_SLAVE_INTR_I2C_ADDR_MATCH & slaveIntrStatus))
    {
        HandleAddress(base, context);

        Cy_SCB_ClearI2CInterrupt  (base, CY_SCB_I2C_INTR_WAKEUP);
        Cy_SCB_ClearSlaveInterrupt(base, CY_SCB_SLAVE_INTR_I2C_ADDR_MATCH);
    }

    /* Handle the transmit direction (master reads data) */
    if (0ul != (CY_SCB_TX_INTR_LEVEL & Cy_SCB_GetTxInterruptStatusMasked(base)))
    {
        HandleDataTransmit(base, context);

        Cy_SCB_ClearTxInterrupt(base, CY_SCB_TX_INTR_LEVEL);
    }
}



/*******************************************************************************
* Function Name: HandleErrors
****************************************************************************//**
*
* Handles an error conditions.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleErrors(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context)
{
    context->status |= CY_SCB_EZI2C_STATUS_ERR;

    /* Drop any data available in the RX FIFO */
    Cy_SCB_ClearRxFifo(base);

    /* Stop the TX and RX processing */
    Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    Cy_SCB_SetTxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
}


/*******************************************************************************
* Function Name: HandleAddress
****************************************************************************//**
*
* Prepares the EZI2C slave for the following read or write transfer after the
* matched address was received.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleAddress(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context)
{
    /* Default actions: ACK address 1 */
    uint32_t cmd = SCB_I2C_S_CMD_S_ACK_Msk;
    context->addr1Active = true;

    if (0u != context->address2)
    {
        /* Get an address from the RX FIFO and make it a 7-bit address */
        uint32_t address = (Cy_SCB_ReadRxFifo(base) >> 1ul);
        Cy_SCB_ClearRxInterrupt(base, CY_SCB_RX_INTR_LEVEL);

        /* Decide if the address matches */
        if ((address == context->address1) || (address == context->address2))
        {
            /* ACK the address */
            if (address == context->address2)
            {
                context->addr1Active = false;
            }

            /* Clear and enable the stop interrupt source */
            Cy_SCB_ClearSlaveInterrupt  (base, CY_SCB_SLAVE_INTR_I2C_STOP);
            Cy_SCB_SetSlaveInterruptMask(base, CY_SCB_EZI2C_SLAVE_INTR);
        }
        else
        {
            /* NACK the address */
            cmd = SCB_I2C_S_CMD_S_NACK_Msk;

            /* Disable the stop interrupt source */
            Cy_SCB_SetI2CInterruptMask(base, CY_SCB_EZI2C_SLAVE_INTR_NO_STOP);
        }
    }

    /* Clear the TX FIFO before continue the transaction */
    Cy_SCB_ClearTxFifo(base);

    /* Set the command to an ACK or NACK address */
    base->unI2C_S_CMD.u32Register = cmd;

    if (cmd == SCB_I2C_S_CMD_S_ACK_Msk)
    {
        context->status |= CY_SCB_EZI2C_STATUS_BUSY;

        /* Prepare for a transaction */
        if (base->unI2C_STATUS.stcField.u1S_READ)
        {
            /* The master reads data from  the slave */
            context->state = CY_SCB_EZI2C_STATE_TX_DATA;

            /* Prepare the buffer for transmit */
            if (context->addr1Active)
            {
                context->curBuf  = &context->buf1[context->baseAddr1];
                context->bufSize = context->buf1Size - context->baseAddr1;
            }
            else
            {
                context->curBuf  = &context->buf2[context->baseAddr2];
                context->bufSize = context->buf2Size - context->baseAddr2;
            }

            Cy_SCB_SetTxInterruptMask(base, CY_SCB_TX_INTR_LEVEL);
        }
        else
        {
            /* The master writes data into the slave */
            context->state = CY_SCB_EZI2C_STATE_RX_OFFSET_MSB;

            context->bufSize = ((context->addr1Active) ? context->buf1Size : context->buf2Size);

            Cy_SCB_SetRxFifoLevel    (base, 0ul);
            Cy_SCB_SetRxInterruptMask(base, CY_SCB_RX_INTR_LEVEL);
        }
    }
}


/*******************************************************************************
* Function Name: HandleDataReceive
****************************************************************************//**
*
* Updates RX FIFO level to trigger the next read from it. It also manages
* the auto-data NACK feature.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param bufSize
* The size of the buffer in bytes.
*
*******************************************************************************/
static void UpdateRxFifoLevel(volatile stc_SCB_t *base, uint32_t bufSize)
{
    uint32_t level;

    if (bufSize > CY_SCB_EZI2C_FIFO_SIZE)
    {
        /* Continue the transaction: there is space in the buffer */
        level = (bufSize - CY_SCB_EZI2C_FIFO_SIZE);
        level = ((level > CY_SCB_EZI2C_FIFO_SIZE) ? CY_SCB_EZI2C_HALF_FIFO_SIZE : level) - 1ul;
    }
    else
    {
        /* Prepare to end the transaction: after the FIFO becomes full, NACK the next byte.
        * The NACKed byte is dropped by the hardware.
        */
        base->unI2C_CTRL.stcField.u1S_NOT_READY_DATA_NACK = 1ul;

        level = ((bufSize == 0ul) ? (0ul) : (bufSize - 1ul));
        Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }

    Cy_SCB_SetRxFifoLevel(base, level);
}


/*******************************************************************************
* Function Name: HandleDataReceive
****************************************************************************//**
*
* Handles the data read from the RX FIFO.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleDataReceive(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context)
{
    switch(context->state)
    {
    case CY_SCB_EZI2C_STATE_RX_OFFSET_MSB:
    case CY_SCB_EZI2C_STATE_RX_OFFSET_LSB:
    {
        /* Default actions: compare the base address and ACK it */
        bool     checkBaseAddr = true;

        /* Get the base address from the RX FIFO */
        uint32_t baseAddr      = Cy_SCB_ReadRxFifo(base);

        if(context->subAddrSize == CY_SCB_EZI2C_SUB_ADDR16_BITS)
        {
            if(context->state == CY_SCB_EZI2C_STATE_RX_OFFSET_MSB)
            {
                /* ACK base address MSB */
                base->unI2C_S_CMD.u32Register = SCB_I2C_S_CMD_S_ACK_Msk;

                /* Temporary store base address MSB */
                context->idx = (uint32_t)(baseAddr << 8ul);

                /* Do not compare until 16 bits are received */
                checkBaseAddr = false;
                context->state = CY_SCB_EZI2C_STATE_RX_OFFSET_LSB;
            }
            else
            {
                /* Get the base address (MSB | LSB) */
                baseAddr |= context->idx;
            }
        }

        /* Check if the received base address is valid */
        if(checkBaseAddr)
        {
            uint32_t cmd = SCB_I2C_S_CMD_S_ACK_Msk;

            /* Decide if the base address within the buffer range */
            if(baseAddr < context->bufSize)
            {
                /* Accept the new base address */
                if(context->addr1Active)
                {
                    context->baseAddr1 = baseAddr;
                }
                else
                {
                    context->baseAddr2 = baseAddr;
                }

                /* Store the base address to use it later */
                context->idx = baseAddr;
            }
            else
            {
                /* Restore the valid base address */
                context->idx = ((context->addr1Active) ? context->baseAddr1 : context->baseAddr2);

                /* The base address is out of range - NACK it */
                cmd = SCB_I2C_S_CMD_S_NACK_Msk;
            }

            /* Set the command to an ACK or NACK address */
            base->unI2C_S_CMD.u32Register = cmd;

            if(cmd == SCB_I2C_S_CMD_S_ACK_Msk)
            {
                /* Prepare the buffer for a write */
                if(context->addr1Active)
                {
                    context->curBuf  = &context->buf1[context->baseAddr1];
                    context->bufSize = ((context->baseAddr1 < context->buf1rwBondary) ?
                                        (context->buf1rwBondary - context->baseAddr1) : (0ul));
                }
                else
                {
                    context->curBuf  = &context->buf2[context->baseAddr2];
                    context->bufSize = ((context->baseAddr2 < context->buf2rwBondary) ?
                                        (context->buf2rwBondary - context->baseAddr2) : (0ul));
                }

                /* Choice receive scheme  */
                if((0u != context->address2) || (context->bufSize < CY_SCB_EZI2C_FIFO_SIZE))
                {
                    /* Handle each byte separately */
                    context->state = CY_SCB_EZI2C_STATE_RX_DATA0;
                }
                else
                {
                    /* Use the RX FIFO and the auto-ACK/NACK features */
                    base->unI2C_CTRL.stcField.u1S_READY_DATA_ACK = 1ul;
                    UpdateRxFifoLevel(base, context->bufSize);

                    context->state = CY_SCB_EZI2C_STATE_RX_DATA1;
                }
            }
        }
    }
        break;

    case CY_SCB_EZI2C_STATE_RX_DATA0:
    {
        uint32_t byte = Cy_SCB_ReadRxFifo(base);

        /* Check if there is space to store the byte */
        if(context->bufSize > 0ul)
        {
            /* Continue the transfer: send an ACK */
            base->unI2C_S_CMD.u32Register = SCB_I2C_S_CMD_S_ACK_Msk;

            /* Store the byte in the buffer */
            context->curBuf[0ul] = (uint8_t)byte;
            context->bufSize--;
            context->curBuf++;

            /* Update the base address to notice that the buffer is modified */
            context->idx++;
        }
        else
        {
            /* Finish the transfer: send a NACK. Drop the received byte */
            base->unI2C_S_CMD.u32Register = SCB_I2C_S_CMD_S_NACK_Msk;
            Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
        }
    }
        break;

    case CY_SCB_EZI2C_STATE_RX_DATA1:
    {
        /* Get the number of bytes to read from the RX FIFO */
        uint32_t numToCopy = Cy_SCB_GetRxFifoLevel(base) + 1ul;

        /* Get data from the RX FIFO */
        numToCopy = Cy_SCB_ReadArray(base, context->curBuf, numToCopy);
        context->bufSize -= numToCopy;
        context->curBuf  += numToCopy;

        /* Configure the next RX FIFO read event */
        UpdateRxFifoLevel(base, context->bufSize);

        /* Update the base address to notice that the buffer is modified */
        context->idx++;
    }
        break;

    default:
        break;
    }
}


/*******************************************************************************
* Function Name: HandleDataTransmit
****************************************************************************//**
*
* Loads the TX FIFO with data from the buffer.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleDataTransmit(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context)
{
    if (context->bufSize > 0ul)
    {
        /* Write data into the TX FIFO from the buffer */
        uint32_t numToCopy = Cy_SCB_WriteArray(base, context->curBuf, context->bufSize);
        context->bufSize  -= numToCopy;
        context->curBuf   += numToCopy;
    }

    if (0ul == context->bufSize)
    {
        /* Write the default bytes into the TX FIFO */
        (void) Cy_SCB_WriteDefaultArray(base, CY_SCB_EZI2C_DEFAULT_TX, CY_SCB_EZI2C_FIFO_SIZE);
    }
}


/*******************************************************************************
* Function Name: HandleStop
****************************************************************************//**
*
* Handles the transfer completion.
* It is triggered by a Stop or Restart condition on the bus.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void HandleStop(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context)
{
    /* Check for errors */
    if (0ul != (CY_SCB_EZI2C_STATUS_ERR & context->status))
    {
        /* Re-enable the SCB to recover from errors */
        Cy_SCB_FwBlockReset(base);
    }

    /* Clean up the hardware to be ready for the next transaction */
    if (CY_SCB_EZI2C_STATE_TX_DATA == context->state)
    {
        Cy_SCB_SetTxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);
    }
    else
    {
        Cy_SCB_SetRxInterruptMask(base, CY_SCB_CLEAR_ALL_INTR_SRC);

        base->unI2C_CTRL.stcField.u1S_READY_DATA_ACK      = 0ul;
        base->unI2C_CTRL.stcField.u1S_NOT_READY_DATA_NACK = 0ul;
    }

    /* Update the statuses */
    context->status &= (uint32_t) ~CY_SCB_EZI2C_STATUS_BUSY;

    if (context->addr1Active)
    {
        context->status |= ((CY_SCB_EZI2C_STATE_TX_DATA == context->state) ? CY_SCB_EZI2C_STATUS_READ1 :
                             ((context->baseAddr1 != context->idx) ?  CY_SCB_EZI2C_STATUS_WRITE1 : 0ul));
    }
    else
    {
        context->status |= ((CY_SCB_EZI2C_STATE_TX_DATA == context->state) ? CY_SCB_EZI2C_STATUS_READ2 :
                             ((context->baseAddr2 != context->idx) ? CY_SCB_EZI2C_STATUS_WRITE2 : 0ul));
    }

    /* Back to the idle state */
    context->state = CY_SCB_EZI2C_STATE_IDLE;
}


/*******************************************************************************
* Function Name: UpdateAddressMask
****************************************************************************//**
*
* Updates the slave address mask to enable the SCB hardware to receive matching
* slave addresses.
*
* \param base
* The pointer to the EZI2C SCB instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_scb_ezi2c_context_t allocated
* by the user. The structure is used during the EZI2C operation for internal
* configuration and data keeping. The user must not modify anything
* in this structure.
*
*******************************************************************************/
static void UpdateAddressMask(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t const *context)
{
    uint32_t addrMask;

    /* Check how many addresses are used: */
    if (0u != context->address2)
    {
        /* If (addr1 and addr2) bit matches - mask bit equals 1, otherwise 0 */
        addrMask  = (uint32_t) ~((uint32_t) context->address1 ^ (uint32_t) context->address2);
    }
    else
    {
        addrMask = CY_SCB_EZI2C_ONE_ADDRESS_MASK;
    }

    /* Update the hardware address match */
    base->unRX_MATCH.stcField.u8MASK = 0ul;
    base->unRX_MATCH.stcField.u8MASK = ((uint32_t) addrMask << 1ul);
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

