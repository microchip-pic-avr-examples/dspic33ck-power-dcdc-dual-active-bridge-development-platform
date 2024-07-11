/**
 * CRC Generated Driver Types Header File
 * 
 * @file      crc_types.h
 *            
 * @ingroup   crcdriver
 *            
 * @brief     This is the generated driver types header file for the CRC driver
 *            
 * @skipline @version   Firmware Driver Version 1.2.2
 *
 * @skipline @version   PLIB Version 1.3.0
 *            
 * @skipline  Device : dsPIC33CK256MP506
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef CRC_TYPES_H
#define CRC_TYPES_H

// Section: Enum Declarations

/**
 @ingroup  crcdriver
 @enum     CRC_STATE
 @brief    Defines the CRC calculation states
*/
enum CRC_STATE
{
    CRC_STATE_CALCULATE,   /**< CRC calculation start */
    CRC_STATE_FLUSH,       /**< CRC calculation data flush */
    CRC_STATE_CLEANUP,     /**< CRC calculation reset */
    CRC_STATE_DONE         /**< CRC calculation complete */
};

/**
 @ingroup  crcdriver
 @enum     CRC_SEED_METHOD
 @brief    Defines the CRC calculation seed method \ref CRC_SeedSet. 
           The direct method refers to the seed being placed before the shifters with 
           the result being shifted through the polynomial. The indirect method refers 
           to the seed being placed after the shifters with the result not being shifted 
           through the polynomial.
*/
enum CRC_SEED_METHOD
{
    CRC_SEED_METHOD_DIRECT,     /**< CRC seed in direct method */
    CRC_SEED_METHOD_INDIRECT    /**< CRC seed in indirect method */
};

/**
 @ingroup  crcdriver
 @enum     CRC_SEED_DIRECTION
 @brief    Defines the CRC calculation seed direction in direct method \ref CRC_SeedSet
*/
enum CRC_SEED_DIRECTION
{
    CRC_SEED_DIRECTION_Msb,     /**< CRC seed in Big Endian */
    CRC_SEED_DIRECTION_Lsb,     /**< CRC seed in Little Endian */
};

#endif  //CRC_TYPES_H
/**
 End of File
*/
