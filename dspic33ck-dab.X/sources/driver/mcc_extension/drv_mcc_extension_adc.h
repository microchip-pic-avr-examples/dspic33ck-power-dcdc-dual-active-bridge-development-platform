/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
 * @file    drv_mcc_extension_adc.h     
 * @ingroup driver-layer-adc   
 * @brief   Contains public functions and defines for the MCC code extension for ADC.  
 */

#ifndef DRV_MCC_EXTENSION_ADC_H
#define	DRV_MCC_EXTENSION_ADC_H

#include <xc.h>
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

/*********************************************************************************
 * @ingroup driver-layer-adc   
 * @brief   ADC Filter Modes of operation
 **********************************************************************************/
enum ADC_FILTER_MODE_e
{
    ADC_FILTER_MODE_OVERSAMPLING = 0,
    ADC_FILTER_MODE_AVERAGING = 3
};

typedef enum ADC_FILTER_MODE_e ADC_FILTER_MODE_t;

/*********************************************************************************
 * @ingroup driver-layer-adc  
 * @brief   ADC Filter Oversampling Ratio when in averaging mode
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
 * @ingroup driver-layer-adc  
 * @brief   Enables the ADC filter feature of the ADC
 * @param   filterIndex ADC filter index (four indexes available for ADC filter)
 * @param   enable       ADC filter enable
 * @return  void
 * @details This function enables the ADC feature of the ADC instance.
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
 * @ingroup driver-layer-adc  
 * @brief   Sets the ADC instance filter mode
 * @param   filterIndex ADC filter index (four indexes available for ADC filter)
 * @param   mode Pointer to a power control data object of type ADC_FILTER_MODE_t
 * @return  void
 * @details This function set the ADC instance filter mode either OverSampling
 *  mode or Averaging mode. 
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
 * @ingroup driver-layer-adc  
 * @brief   Sets the ADC instance number of samples to average
 * @param   filterIndex ADC filter index (four indexes available for ADC filter)
 * @param   averagingRatio Pointer to a power control data object of type ADC_FILTER_OVRSAM_AVG_t
 * @return  void
 * @details This function sets the number of samples to average in the particular
 *  ADC instance.
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
 * @ingroup driver-layer-adc  
 * @brief   Sets the ADC instance to be filtered
 * @param   filterIndex ADC filter index (four indexes available for ADC filter)
 * @param   channelIndex ADC channel
 * @return  void
 * @details This function sets up the ADC filter for the specific ADC instance.
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
 * @ingroup driver-layer-adc  
 * @brief   Gets the ADC filter result
 * @param   filterIndex ADC filter index (four indexes available for ADC filter)
 * @return  filterResult result of the filtered ADC
 * @details This function gives the value of the ADC filter output. 
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

