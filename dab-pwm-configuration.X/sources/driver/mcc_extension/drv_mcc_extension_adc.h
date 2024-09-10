/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

#ifndef DRV_MCC_EXTENSION_ADC_H
#define	DRV_MCC_EXTENSION_ADC_H

#include <xc.h>
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def     ADC_FILTER_MODE_e
 * @brief   ADC Filter Modes of operation
 * @details
 *  ADD_DESCRIPTION_HERE
 **********************************************************************************/
enum ADC_FILTER_MODE_e
{
    ADC_FILTER_MODE_OVERSAMPLING = 0,
    ADC_FILTER_MODE_AVERAGING = 3
};

typedef enum ADC_FILTER_MODE_e ADC_FILTER_MODE_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def     ADC_FILTER_OVRSAM_AVG_e
 * @brief   ADC Filter Oversampling Ratio when in averaging mode
 * @details
 *  ADD_DESCRIPTION_HERE
 **********************************************************************************/
enum ADC_FILTER_OVRSAM_AVG_e
{
    ADC_FILTER_OVRSAM_AVG_2x = 0,
    ADC_FILTER_OVRSAM_AVG_4x = 1,
    ADC_FILTER_OVRSAM_AVG_8x = 2,
    ADC_FILTER_OVRSAM_AVG_16x = 3,
    ADC_FILTER_OVRSAM_AVG_32x = 4,
    ADC_FILTER_OVRSAM_AVG_64x = 5,
    ADC_FILTER_OVRSAM_AVG_128x = 6,
    ADC_FILTER_OVRSAM_AVG_256x = 7,
};

typedef enum ADC_FILTER_OVRSAM_AVG_e ADC_FILTER_OVRSAM_AVG_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    inline static void ADC_Filter_Enable(uint16_t filterIndex, bool enable)
 * @brief  divide ADC trigger 1 frequency by an integer, from 1 to 32
 * @details 
 **********************************************************************************/
inline static void ADC_Filter_Enable(uint16_t filterIndex, bool enable)
{
    switch (filterIndex)
    {
        case 0:
            ADFL0CONbits.FLEN = (uint16_t)enable;
            break;
        case 1:
            ADFL1CONbits.FLEN = (uint16_t)enable;
            break;
        case 2:
            ADFL2CONbits.FLEN = (uint16_t)enable;
            break;
        case 3:
            ADFL3CONbits.FLEN = (uint16_t)enable;
            break;
        default:
            // index out of range, do nothing
            break;
    }
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    ADC_Filter_Mode(uint16_t filterIndex, ADC_FILTER_MODE_t mode)
 * @brief  set the filter mode (averaging or oversampling)
 * @details 
 **********************************************************************************/
inline static void ADC_Filter_Mode(uint16_t filterIndex, ADC_FILTER_MODE_t mode)
{
    switch (filterIndex)
    {
        case 0:
            ADFL0CONbits.MODE = (uint16_t)mode;
            break;
        case 1:
            ADFL1CONbits.MODE = (uint16_t)mode;
            break;
        case 2:
            ADFL2CONbits.MODE = (uint16_t)mode;
            break;
        case 3:
            ADFL3CONbits.MODE = (uint16_t)mode;
            break;            
        default:
            // do nothing
            break;
    }
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    ADC_Filter_Averaging(uint16_t filterIndex, ADC_FILTER_OVRSAM_AVG_t averagingRatio)
 * @brief  set the amount of filter averaging when in averaging mode
 * @details 
 **********************************************************************************/
inline static void ADC_Filter_Averaging(uint16_t filterIndex, ADC_FILTER_OVRSAM_AVG_t averagingRatio)
{
    switch (filterIndex)
    {
        case 0:
            ADFL0CONbits.OVRSAM = (uint16_t)averagingRatio;
            break;
        case 1:
            ADFL1CONbits.OVRSAM = (uint16_t)averagingRatio;
            break;
        case 2:
            ADFL2CONbits.OVRSAM = (uint16_t)averagingRatio;
            break;
        case 3:
            ADFL3CONbits.OVRSAM = (uint16_t)averagingRatio;
            break;            
        default:
            // do nothing
            break;        
    }
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    ADC_Filter_Averaging(uint16_t filterIndex, ADC_FILTER_OVRSAM_AVG_e averagingRatio)
 * @brief  set the amount of filter averaging when in averaging mode
 * @details 
 **********************************************************************************/
inline static void ADC_Filter_Input(uint16_t filterIndex, uint16_t channelIndex)
{
    if (channelIndex <= 25)
    {
        switch (filterIndex)
        {
            case 0:
                ADFL0CONbits.FLCHSEL = channelIndex;
                break;
            case 1:
                ADFL1CONbits.FLCHSEL = channelIndex;
                break;
            case 2:
                ADFL2CONbits.FLCHSEL = channelIndex;
                break;
            case 3:
                ADFL3CONbits.FLCHSEL = channelIndex;
                break;            
            default:
                // do nothing
                break;        
        }        
    }
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    ADC_Filter_GetResult(uint16_t filterIndex)
 * @brief  Read the Filter Result
 * @details 
 **********************************************************************************/
inline static uint16_t ADC_Filter_GetResult(uint16_t filterIndex)
{
    uint16_t filterResult = 0;
    switch (filterIndex)
    {
        case 0:  
            filterResult = ADFL0DAT;
            break;
        case 1:  
            filterResult = ADFL1DAT;
            break;
        case 2:  
            filterResult = ADFL2DAT;
            break;
        case 3:  
            filterResult = ADFL3DAT;
            break;
        default:
            break;            
    } 
    return (filterResult);
}

#endif	/* DRV_MC_EXTENSION_ADC_H */

