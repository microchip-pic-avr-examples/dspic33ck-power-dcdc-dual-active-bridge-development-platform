/*
Copyright (c) [2012-2020] Microchip Technology Inc.  
    All rights reserved.
    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
*/

/**
 * This file implements the X2C Lin protocol connection with MCC Peripheral drivers.
 */
#include "config/comms_config.h"

#ifdef X2CDEBUG_ENABLED 
#include <xc.h>
#include "X2CScopeComm.h"
#include "../mcc_generated_files/uart/uart1.h"
/** 
  @brief
    Puts the data to the hardware layer. (UART)
   @param[in] serial Serial interface object. (Not used)
   @param[in] data Data to send 
 */
void sendSerial(uint8_t data)
{
//#error "Implement your send byte function, then delete this line."
/* Example:
 * U1TXREG = data;   
 */
    
    U1TXREG = data;  
//    UART1_Write(data);
}

/** 
  @brief
   Get serial data from hardware. Reset the hardware in case of error. (UART2)
  @param[in] serial Serial interface object. (Not used)
  @return
    Return with the received data
 */
uint8_t receiveSerial()
{
//#error "Implement your receive byte function, then delete this line."
/* Example:
 * 
    if (U1STA & 0x0E) {
        U1STAbits.OERR = 0; // reset error
        return (0);
    }
    return U1RXREG; 
*/
    
//        if (U1STA & 0x0E) {
//        U1STAbits.OERR = 0; // reset error
//        return (0);
//    }
    return U1RXREG; 
//    return UART1_Read();
}

/** 
  @brief  Check data availability (UART).
  @param[in] serial Serial interface object. (Not used)
  @return
    True -> Serial data ready to read.
    False -> No data.
 */
uint8_t isReceiveDataAvailable()
{
//error "Implement your receive byte available function, then delete this line."
/*Example:
    return (U1STAbits.URXDA == 1); //Status RX data available
 */
    //return (U1STAbits.URXDA == 1); //Status RX data available
    return (U1STAHbits.URXBE == 0);    
}

/** 
  @brief
   Check output buffer. (UART)
  @param[in] serial Serial interface object. (Not used)
  @return    
    True -> Transmit buffer is not full, at least one more character can be written.
    False -> Transmit buffer is full.
 */
uint8_t isSendReady()
{
//#error "Implement your send byte possible function, then delete this line."
/*Example:
    return (U1STAbits.UTXBF == 0); //Status TX buffer full
*/
    return (U1STAHbits.UTXBF == 0); //Status TX buffer full    
}
/* *****************************************************************************
 End of File
 */
#endif