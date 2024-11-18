/*******************************************************************************
* \file cy_trigmux.h
* \version 1.0
*
*  This file provides constants and parameter values for the Trigger multiplexer driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \defgroup group_trigmux Trigger multiplexer (TriggerMux)
* \{
* The trigger multiplexer provides access to the multiplexer that selects a set 
* of trigger output signals from different peripheral blocks to route them to the 
* specific trigger input of another peripheral block.
*
* The TrigMux driver is based on the trigger multiplexer's hardware block. 
* The Trigger multiplexer block consists of multiple trigger multiplexers. 
* These trigger multiplexers are grouped in trigger groups. All the trigger 
* multiplexers in the trigger group share similar input options. The trigger 
* multiplexer groups are either reduction multiplexers or distribution multiplexers. 
* The reduction multiplexer groups have input options that are the trigger outputs 
* coming from the different peripheral blocks and the reduction multiplexer groups 
* route them to intermediate signals. The distribution multiplexer groups have input 
* options from these intermediate signals and route them back to multiple peripheral 
* blocks as their trigger inputs.
*
* The trigger architecture of the PSoC device is explained in the technical reference 
* manual (TRM). Refer to the TRM to better understand the trigger multiplexer routing
* architecture available.
* 
* \section group_trigmux_section_Configuration_Considerations Configuration Considerations
*
*
* To route a trigger signal from one peripheral in the PSoC 
* to another, the user must configure a reduction multiplexer and a distribution 
* multiplexer. The Cy_TrigMux_connect() is used to configure a trigger multiplexer connection.
* The user will need two calls of this API, one for the reduction multiplexer and another
* for the distribution multiplexer, to achieve the trigger connection from a source
* peripheral to a destination peripheral. The Cy_TrigMux_connect() function has two main 
* parameters, inTrig and outTrig that refer to the input and output trigger signals 
* connected using the multiplexer.
*
* In addition, the Cy_TrigMux_connect() function also has an invert and trigger type parameter.
* Refer to the API reference for a detailed description of this parameter. 
* All the constants associated with the different trigger signals in the system 
* (input and output) are defined as constants in the device configuration header file. 
* The constants for TrigMux in the device configuration header file are divided into four 
* types based on the signal being input/output and being part of a reduction/distribution 
* trigger multiplexer.
*
* The four types of the input/output parameters are: 
* 1) The parameters for the reduction multiplexer's inputs (input signals of TrigMux); 
* 2) The parameters for the reduction multiplexer's outputs (intermediate signals); 
* 3) The parameters for the distribution multiplexer's inputs (intermediate signals); 
* 4) The parameters for the distribution multiplexer's outputs (output signals of TrigMux). 
* Refer to the TRM for a more detailed description of this architecture and different options.
*
* The steps to connect one peripheral block to the other:
*
* Step 1. Find the trigger group number in the Trigger Group Inputs section of the device 
* configuration header file that corresponds to the output of the first peripheral block. 
* For example, TRIG10_IN_CPUSS_DW0_TR_OUT4 input of the reduction multiplexers belongs 
* to Trigger Group 10.
*
* Step 2. Find the trigger group number in the Trigger Group Outputs section of the device
* configuration header file that corresponds to the input of the second peripheral block. 
* For example, TRIG3_OUT_TCPWM1_TR_IN0 output of the distribution multiplexer belongs to 
* Trigger Group 3.
*
* Step 3. Find the same trigger group number in the Trigger Group Inputs section of the 
* device configuration header file that corresponds to the trigger group number found in 
* Step 1. Select the reduction multiplexer output that can be connected to the trigger group 
* found in Step 2. For example, TRIG3_IN_TR_GROUP10_OUTPUT5 means that Reduction Multiplexer
* Output 5 of Trigger Group 10 can be connected to Trigger Group 3.
*
* Step 4. Find the same trigger group number in the Trigger Group Outputs section of the 
* device configuration header file that corresponds to the trigger group number found in Step 2.
* Select the distribution multiplexer input that can be connected to the trigger group found
* in Step 1. For example, TRIG10_OUT_TR_GROUP3_INPUT1 means that the Distribution Multiplexer
* Input 1 of Trigger Group 3 can be connected to the output of the reduction multiplexer 
* in Trigger Group 10 found in Step 3.
*
* Step 5. Call Cy_TrigMux_Connect() API twice: the first call - with the constants for the 
* inTrig and outTrig parameters found in Steps 1 and Step 4, the second call - with the 
* constants for the inTrig and outTrig parameters found in Steps 2 and Step 3. 
* For example, 
* Cy_TrigMux_Connect(TRIG10_IN_CPUSS_DW0_TR_OUT4, TRIG10_OUT_TR_GROUP3_INPUT1, 
* TR_MUX_TR_INV_DISABLE, TRIGGER_TYPE_LEVEL); 
* Cy_TrigMux_Connect(TRIG3_IN_TR_GROUP10_OUTPUT5, TRIG3_OUT_TCPWM1_TR_IN0, 
* TR_MUX_TR_INV_DISABLE, TRIGGER_TYPE_LEVEL);
*
* \section group_trigmux_more_information More Information
* For more information on the TrigMux peripheral, refer to the technical reference manual (TRM).
*
* \defgroup group_trigmux_macro Macro
* \defgroup group_trigmux_functions Functions
* \defgroup group_trigmux_enums Enumerated Types
*/

#if !defined(CY_TRIGMUX_H)
#define CY_TRIGMUX_H

#include "cy_device_headers.h"
#include "syslib/cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_trigmux_macro
* \{
*/

/** The driver major version */
#define CY_TRIGMUX_DRV_VERSION_MAJOR       1

/** The driver minor version */
#define CY_TRIGMUX_DRV_VERSION_MINOR       0

/**< TRIGMUX PDL ID */
#define CY_TRIGMUX_ID                       CY_PDL_DRV_ID(0x33u) /**< The trigger multiplexer driver identifier */

#define CY_TR_MUX_TR_INV_ENABLE            (0x01u)               /**< Enable trigger invert  */
#define CY_TR_MUX_TR_INV_DISABLE           (0x00u)               /**< Disable trigger invert */

/** \cond */

#define CY_TR_MASK                         (0x000000FFu)    // 7:0 TR_SEL
#define CY_TR_SHIFT                        (0x0)
  
#define CY_TR_GROUP_MASK                   (0x00000F00u)    // 11:8 GROUP_SEL
#define CY_TR_GROUP_SHIFT                  (0x08u)

#define CY_TR_OUT_MASK                     (0x40000000u) //New: Mask Trigger Output

#define CY_TR_TYPE_MASK                    (0x00001000u) //New: Mask 1-to-1 Trigger Input and Output

/** \endcond */

/** \} group_trigmux_macro */


/**
* \addtogroup group_trigmux_enums
* \{
*/

/******************************************************************************
 * Enumerations
 *****************************************************************************/

/** The TRIGMUX error codes. */
typedef enum 
{
    CY_TRIGMUX_SUCCESS = 0x00u,                                             /**< Successful */
    CY_TRIGMUX_BAD_PARAM = CY_TRIGMUX_ID | CY_PDL_STATUS_ERROR | 0x01u,     /**< One or more invalid parameters */
    CY_TRIGMUX_INVALID_STATE = CY_TRIGMUX_ID | CY_PDL_STATUS_ERROR | 0x02u  /**< Operation not setup or is in an improper state */
} cy_en_trigmux_status_t;

/** \} group_trigmux_enums */

/**
* \addtogroup group_trigmux_functions
* \{
*/

cy_en_trigmux_status_t Cy_TrigMux_Connect(uint32_t inTrig, uint32_t outTrig, uint32_t invert, en_trig_type_t trigType, uint32_t dbg_frz_en);
cy_en_trigmux_status_t Cy_TrigMux_Connect1To1(uint32_t outTrig, uint32_t invert, en_trig_type_t trigType, uint32_t dbg_frz_en); //New
cy_en_trigmux_status_t Cy_TrigMux_SwTrigger(uint32_t trigLine, en_trig_type_t trigType, uint32_t outSel);


/** \} group_trigmux_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_TRIGMUX_H */

/** \} group_lpcomp */

/* [] END OF FILE */
