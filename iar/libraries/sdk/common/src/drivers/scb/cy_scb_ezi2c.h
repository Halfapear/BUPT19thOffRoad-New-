/***************************************************************************//**
* \file cy_scb_ezi2c.h
* \version 1.0
*
* Provides EZI2C API declarations of the SCB driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \addtogroup group_scb_ezi2c
* \{
* Driver API for EZI2C slave Peripheral
*
* I2C - The Inter-Integrated Circuit (I2C) bus is an industry-standard.
* The two-wire hardware interface was developed by Philips Semiconductors
* (now NXP Semiconductors).
*
* The EZI2C slave peripheral driver provides an API to implement the I2C slave
* device based on the SCB hardware block. This slave device emulates a common
* I2C EEPROM interface that acts like dual-port memory between the external
* master and your code. I2C devices based on the SCB hardware are compatible
* with the I2C Standard mode, Fast mode, and Fast mode Plus specifications as
* defined in the I2C bus specification.
*
* Features:
* * An industry-standard I2C bus interface
* * Supports standard data rates of 100/400/1000 kbps
* * Emulates a common I2C EEPROM Interface
* * Acts like dual-port memory between the external master and your code
* * Supports Hardware Address Match
* * Supports two hardware addresses with separate buffers
* * Supports Wake from Deep Sleep on address match
* * Simple to set up and use, does not require calling EZI2C API
*   at run time.
*
* \section group_scb_ezi2c_configuration Configuration Considerations
*
* To set up the EZI2C slave driver, provide the configuration parameters in the
* \ref cy_stc_scb_ezi2c_config_t structure. For the slave, the primary slave
* address slaveAddress1 must be provided. The other parameters are optional
* for operation.
* To get EZI2C slave operate with desired data rate the SCB clock frequency
* must be configured. Use the SysClk driver API to do that. Refer to the
* technical reference manual (TRM) to get information how to select SCB
* frequency in I2C mode to support desired data rate.
* To initialize the driver, call the \ref Cy_SCB_EZI2C_Init function providing
* a pointer to the filled \ref cy_stc_scb_ezi2c_config_t structure and
* allocated \ref cy_stc_scb_ezi2c_context_t. The \ref Cy_SCB_EZI2C_Interrupt
* function must be called in the interrupt handler for the selected
* SCB instance and this interrupt must be enabled in the NVIC. Set up the
* EZI2C slave buffers before calling \ref Cy_SCB_EZI2C_Enable using
* \ref Cy_SCB_EZI2C_SetBuffer1 for the primary slave address and
* \ref Cy_SCB_EZI2C_SetBuffer2 for the secondary if it is enabled. Finally,
* enable the EZI2C slave operation calling \ref Cy_SCB_EZI2C_Enable.
*
* The following operation might not require calling any EZI2C slave function
* because the I2C master is able to access the slave buffer and the application
* can directly access it as well. Note that this is an application level task
* to ensure the buffer content integrity.
*
* The master can access the buffer:
* * Master Write operation starts with sending a base address which is 1
*   or 2 bytes depending on the sub-address size configuration. This base
*   address is retained and will be used for later read operations. Following
*   the base address, there is a sequence of bytes written into the buffer
*   starting from the base address location. The buffer index is incremented
*   for each written byte, but this does not affect the base address which is
*   retained. The length of a write operation is limited by the maximum buffer
*   read/write region size.\n
*   When a master attempts to write outside the read/write region or past the
*   end of the buffer, the last byte is NACKed.
* * Master Read operation always starts from the base address set by the most
*   recent Write operation. The buffer index is incremented for each read byte.
*   Two sequential read operations start from the same base address no matter
*   how many bytes were read. The length of a read operation is not limited by
*   the maximum size of the data buffer. The EZI2C slave returns 0xFF bytes
*   if the read operation passes the end of the buffer.\n
*   Typically, a read operation requires the base address to be updated before
*   starting the read. In this case, the write and read operations must be
*   combined together. The I2C master may use the ReStart or Stop/Start
*   conditions to combine the operations. The write operation sets only the
*   base address and the following read operation will start from the new base
*   address. In cases where the base address remains the same, there is no need
*   for a write operation.
*
* \section group_scb_ezi2c_more_information More Information
*
* For more information on the SCB peripheral, refer to the technical reference
* manual (TRM).
*
* \section group_scb_ezi2c_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_scb_ezi2c_macro Macro
* \defgroup group_scb_ezi2c_functions Functions
* \{
* \defgroup group_scb_ezi2c_general_functions General
* \defgroup group_scb_ezi2c_slave_functions Slave
* \defgroup group_scb_ezi2c_low_power_functions Low Power Callbacks
* \}
* \defgroup group_scb_ezi2c_data_structures Data Structures
* \defgroup group_scb_ezi2c_enums Enumerated Types
*/

#if !defined(CY_SCB_EZI2C_H)
#define CY_SCB_EZI2C_H

#include "cy_scb_common.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*       Enumerated Types
***************************************/

/**
* \addtogroup group_scb_ezi2c_enums
* \{
*/

/** EZI2C slave status codes */
typedef enum
{
    /** Operation completed successfully */
    CY_SCB_EZI2C_SUCCESS = 0u,

    /** One or more of input parameters are invalid */
    CY_SCB_EZI2C_BAD_PARAM = (CY_SCB_ID | CY_PDL_STATUS_ERROR | CY_SCB_EZI2C_ID | 1u),
} cy_en_scb_ezi2c_status_t;

/** \cond INTERNAL */
/** EZI2C slave FSM states */
typedef enum
{
    CY_SCB_EZI2C_STATE_IDLE,
    CY_SCB_EZI2C_STATE_RX_OFFSET_MSB,
    CY_SCB_EZI2C_STATE_RX_OFFSET_LSB,
    CY_SCB_EZI2C_STATE_RX_DATA0,
    CY_SCB_EZI2C_STATE_RX_DATA1,
    CY_SCB_EZI2C_STATE_TX_DATA
} cy_en_scb_ezi2c_state_t;
/** \endcond */
/** \} group_scb_ezi2c_enums */


/***************************************
*       Type Definitions
***************************************/

/**
* \addtogroup group_scb_ezi2c_data_structures
* \{
*/

/** EZI2C slave configuration structure */
typedef struct cy_stc_scb_ezi2c_config
{
    /**
    * The number of supported addresses either (See
    * \ref group_scb_ezi2c_macro_num_of_addr for the set of constants)
    */
    uint32_t numberOfAddresses;

    /** The 7-bit right justified primary slave address */
    uint8_t slaveAddress1;

    /** The 7-bit right justified secondary slave address */
    uint8_t slaveAddress2;

    /** The size of the sub-address, can either be 8 or 16 bits (See
    * \ref group_scb_ezi2c_macro_sub_addr_size for the set of constants)
    */
    uint32_t subAddressSize;

    /**
    * When set, the slave will wake the device from deep sleep on an address
    * match (The device datasheet must be consulted to determine which SCBs
    * support this mode)
    */
    bool enableWakeFromSleep;
} cy_stc_scb_ezi2c_config_t;

/** EZI2C slave context structure */
typedef struct cy_stc_scb_ezi2c_context
{
    cy_en_scb_ezi2c_state_t state;  /**< The driver state */
    uint32_t status;                /**< The slave status */

    uint8_t  address1;      /**< The primary slave address (7-bits right justified) */
    uint8_t  address2;      /**< The secondary slave address (7-bits right justified) */
    uint32_t subAddrSize;   /**< The sub-address size */

    uint32_t idx;       /**< The index within the buffer during operation */
    uint32_t baseAddr1; /**< The valid base address for the primary slave address */
    uint32_t baseAddr2; /**< The valid base address for the secondary slave address */

    bool addr1Active;   /**< Defines whether the request is intended for the primary slave address */
    uint8_t  *curBuf;   /**< The pointer to the current location in the buffer (while it is accessed) */
    uint32_t bufSize;   /**< Specifies how many bytes are left in the current buffer */

    uint8_t *buf1;          /**< The pointer to the buffer exposed  on the request intended for the primary slave address */
    uint32_t buf1Size;      /**< The buffer size assigned to the primary slave address */
    uint32_t buf1rwBondary; /**< The Read/Write boundary within the buffer assigned to the primary slave address */

    uint8_t *buf2;          /**< The pointer to the buffer exposed on the request intended for the secondary slave address */
    uint32_t buf2Size;      /**< The buffer size assigned to the secondary slave address */
    uint32_t buf2rwBondary; /**< The Read/Write boundary within the buffer assigned for the secondary slave address */

} cy_stc_scb_ezi2c_context_t;
/** \} group_scb_ezi2c_data_structures */


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_scb_ezi2c_general_functions
* \{
*/
cy_en_scb_ezi2c_status_t Cy_SCB_EZI2C_Init(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_config_t const *config,
                                           cy_stc_scb_ezi2c_context_t *context);
void     Cy_SCB_EZI2C_DeInit(volatile stc_SCB_t *base);
__STATIC_INLINE void Cy_SCB_EZI2C_Enable(volatile stc_SCB_t *base);
void     Cy_SCB_EZI2C_Disable(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context);

void     Cy_SCB_EZI2C_SetAddress1(volatile stc_SCB_t *base, uint8_t addr, cy_stc_scb_ezi2c_context_t *context);
uint32_t Cy_SCB_EZI2C_GetAddress1(volatile stc_SCB_t const *base, cy_stc_scb_ezi2c_context_t const *context);

void     Cy_SCB_EZI2C_SetAddress2(volatile stc_SCB_t *base, uint8_t addr, cy_stc_scb_ezi2c_context_t *context);
uint32_t Cy_SCB_EZI2C_GetAddress2(volatile stc_SCB_t const *base, cy_stc_scb_ezi2c_context_t const *context);
/** \} group_scb_ezi2c_general_functions */

/**
* \addtogroup group_scb_ezi2c_slave_functions
* \{
*/
void Cy_SCB_EZI2C_SetBuffer1(volatile stc_SCB_t const *base, uint8_t *buf, uint32_t bufSize, uint32_t rwBoundary,
                             cy_stc_scb_ezi2c_context_t *context);
void Cy_SCB_EZI2C_SetBuffer2(volatile stc_SCB_t const *base, uint8_t *buf, uint32_t bufSize, uint32_t rwBoundary,
                             cy_stc_scb_ezi2c_context_t *context);

uint32_t Cy_SCB_EZI2C_GetActivity(volatile stc_SCB_t const *base, cy_stc_scb_ezi2c_context_t *context);

void Cy_SCB_EZI2C_Interrupt(volatile stc_SCB_t *base, cy_stc_scb_ezi2c_context_t *context);
/** \} group_scb_ezi2c_slave_functions */

/**
* \addtogroup group_scb_ezi2c_low_power_functions
* \{
*/
cy_en_syspm_status_t Cy_EZI2C_DeepSleepCallback(void *base, void *context, cy_en_syspm_callback_mode_t mode);
cy_en_syspm_status_t Cy_EZI2C_HibernateCallback(void *base, void *context, cy_en_syspm_callback_mode_t mode);
/** \} group_scb_ezi2c_low_power_functions */


/***************************************
*            API Constants
***************************************/

/**
* \addtogroup group_scb_ezi2c_macro
* \{
*/

/**
* \defgroup group_scb_ezi2c_macro_num_of_addr Number of Addresses
* \{
*/
#define CY_SCB_EZI2C_ONE_ADDRESS    (0ul)   /**< Only one address */
#define CY_SCB_EZI2C_TWO_ADDRESSES  (1ul)   /**< Two addresses */
/** \} group_scb_ezi2c_macro_num_of_addr */

/**
* \defgroup group_scb_ezi2c_macro_sub_addr_size Size of Sub-Address
* \{
*/
#define CY_SCB_EZI2C_SUB_ADDR8_BITS     (0ul)   /**< Sub-address is 8 bits  */
#define CY_SCB_EZI2C_SUB_ADDR16_BITS    (1ul)   /**< Sub-address is 16 bits */
/** \} group_scb_ezi2c_macro_sub_addr_size */

/**
* \defgroup group_scb_ezi2c_macro_get_activity EZI2C Activity Status
* Each EZI2C slave status is encoded in a separate bit, therefore multiple bits
* may be set to indicate the current status.
* \{
*/

/**
* The Read transfer intended for the primary slave address is complete.
* The error condition status bit must be checked to ensure that the Read
* transfer was completed successfully.
*/
#define CY_SCB_EZI2C_STATUS_READ1       (0x01ul)

/**
* The Write transfer intended for the primary slave address is complete.
* The buffer content was modified.
* The error condition status bit must be checked to ensure that the Write
* transfer was completed successfully.
*/
#define CY_SCB_EZI2C_STATUS_WRITE1      (0x02ul)

/**
* The Read transfer intended for the secondary slave address is complete.
* The error condition status bit must be checked to ensure that the Read
* transfer was completed successfully.
*/
#define CY_SCB_EZI2C_STATUS_READ2       (0x04ul)

/**
* The Write transfer intended for the secondary slave address is complete.
* The buffer content was modified.
* The error condition status bit must be checked to ensure that the Write
* transfer was completed successfully.
*/
#define CY_SCB_EZI2C_STATUS_WRITE2       (0x08ul)

/**
* A transfer intended for the primary address or secondary address is in
* progress. The status bit is set after an address match and cleared
* on a Stop or ReStart condition.
*/
#define CY_SCB_EZI2C_STATUS_BUSY        (0x10ul)

/**
* An error occurred during a transfer intended for the primary or secondary
* slave address. The sources of the error are: a misplaced Start or Stop
* condition or lost arbitration while the slave drives SDA.
* When CY_SCB_EZI2C_STATUS_ERR is set, the slave buffer may contain an
* invalid byte. It is recommended to discard the buffer content in this case.
*/
#define CY_SCB_EZI2C_STATUS_ERR         (0x20ul)
/** \} group_scb_ezi2c_macro_get_activity */

/**
* This value is returned by the slave when the buffer is not configured or
* the master requests more bytes than available in the buffer.
*/
#define CY_SCB_EZI2C_DEFAULT_TX  (0xFFul)


/***************************************
*         Internal Constants
***************************************/

/** \cond INTERNAL */
/* Default registers values */
#define CY_SCB_EZI2C_I2C_CTRL   (SCB_I2C_CTRL_S_GENERAL_IGNORE_Msk | SCB_I2C_CTRL_SLAVE_MODE_Msk)
#define CY_SCB_EZI2C_RX_CTRL    (CY_SCB_I2C_RX_CTRL)
#define CY_SCB_EZI2C_TX_CTRL    (CY_SCB_I2C_TX_CTRL)

#define CY_SCB_EZI2C_SLAVE_INTR     (CY_SCB_SLAVE_INTR_I2C_ADDR_MATCH | CY_SCB_SLAVE_INTR_I2C_STOP | \
                                     CY_SCB_SLAVE_INTR_I2C_BUS_ERROR  | CY_SCB_SLAVE_INTR_I2C_ARB_LOST)
/* Error interrupt sources */
#define CY_SCB_EZI2C_SLAVE_INTR_ERROR   (CY_SCB_SLAVE_INTR_I2C_BUS_ERROR  | CY_SCB_SLAVE_INTR_I2C_ARB_LOST)

/* Disables Stop interrupt source */
#define CY_SCB_EZI2C_SLAVE_INTR_NO_STOP (CY_SCB_EZI2C_SLAVE_INTR & ((uint32_t) ~CY_SCB_SLAVE_INTR_I2C_STOP))

/* Disable Address interrupt source */
#define CY_SCB_EZI2C_SLAVE_INTR_NO_ADDR (CY_SCB_EZI2C_SLAVE_INTR & ((uint32_t) ~CY_SCB_SLAVE_INTR_I2C_ADDR_MATCH))

/* FIFO size */
#define CY_SCB_EZI2C_FIFO_SIZE         (CY_SCB_FIFO_SIZE)
#define CY_SCB_EZI2C_HALF_FIFO_SIZE    (CY_SCB_FIFO_SIZE / 2ul)

#define CY_SCB_EZI2C_ONE_ADDRESS_MASK (0xFFul)
/** \endcond */
/** \} group_scb_ezi2c_macro */


/***************************************
*    Inline Function Implementation
***************************************/

/**
* \addtogroup group_scb_ezi2c_general_functions
* \{
*/
/*******************************************************************************
* Function Name: Cy_SCB_EZI2C_Enable
****************************************************************************//**
*
* Enables the SCB block for the EZI2C operation
*
* \param base
* The pointer to the EZI2C SCB instance.
*
*******************************************************************************/
__STATIC_INLINE void Cy_SCB_EZI2C_Enable(volatile stc_SCB_t *base)
{
    base->unCTRL.stcField.u1ENABLED = 1ul;
}

/** \} group_scb_ezi2c_general_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_scb_ezi2c */

#endif /* (CY_SCB_EZI2C_H) */


/* [] END OF FILE */
