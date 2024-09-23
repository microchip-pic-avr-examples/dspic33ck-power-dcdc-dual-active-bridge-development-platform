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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USEFUL_MACROS_H
#define	USEFUL_MACROS_H

#include <xc.h> // include processor files - each processor file is guarded.  

/******************************************************************************
 * @ingroup useful-macros
 * @{
 ******************************************************************************/
#define _rnd(a)    ((int16_t)((a)+((a)<0?-0.5:0.5)))
#define _min(a,b)  (((a)>(b)) ? (b):(a))
#define _max(a,b)  (((a)>(b)) ? (a):(b))

#define _rnd_int32_t(a)    ((int32_t)((a)+((a)<0?-0.5:0.5)))

// following 2 macros used to get high byte and low byte of a word
#define WordLowByte(word)   ((uint8_t) (word & 0x00FF))
#define WordHighByte(word)  ((uint8_t) (word >> 8))

// to convert number to QFORMAT (positive) with max resolution, multiply by 2^QFORMAT_SHIFT
// this gives max possibly resolution with 15 bits available
#define QFORMAT_SHIFT(value)  ((value) < ((float)(1<<0)) ? (15) : \
                              ((value) < ((float)(1<<1)) ? (14) : \
                              ((value) < ((float)(1<<2)) ? (13) : \
                              ((value) < ((float)(1<<3)) ? (12) : \
                              ((value) < ((float)(1<<4)) ? (11) : \
                              ((value) < ((float)(1<<5)) ? (10) : \
                              ((value) < ((float)(1<<6)) ? (9) : \
                              ((value) < ((float)(1<<7)) ? (8) : \
                              ((value) < ((float)(1<<8)) ? (7) : \
                              ((value) < ((float)(1<<9)) ? (6) : \
                              ((value) < ((float)(1<<10)) ? (5) : \
                              ((value) < ((float)(1<<11)) ? (4) : \
                              ((value) < ((float)(1<<13)) ? (3) : \
                              ((value) < ((float)(1<<13)) ? (2) : \
                              ((value) < ((float)(1<<14)) ? (1) : (0))))))))))))))))

// TIME_TO_TICKS: convert time in seconds to counter ticks
#define TIME_TO_TICKS(time, tick_period)    (_rnd(time/tick_period))

// RMS_TO_PEAK: sqrt(2): rms to peak-peak is 2*sqrt(2), so rms to peak is sqrt(2)
#define RMS_TO_PEAK           (1.414213562)

// AVG_TO_RMS: PI / (2* sqrt(2)) = 1.11
#define AVG_TO_RMS            (1.11)

#define RMS_TO_AVG            (1.0/AVG_TO_RMS)

// convert frequency in Hz to integer setting that can be loaded into the PWMxPER register
// PWM_HR_EDGE_ALIGNED_PGxPER: high resolution, edge aligned mode
// assuming a PWM clock frequency of 4GHz
#define PWM_HR_EDGE_ALIGNED_PGxPER(freq_hz)     ((uint16_t)(_rnd_int32_t((8.0*500.0e+6)/(float)freq_hz)-8.0))

// convert duty cycle in percent to an integer that can be loaded into the PGxDC register
// duty_percent should be between 0 and 100
#define PWM_HR_EDGE_ALIGNED_PGxDC(PGxPER, duty_percent)      ((uint16_t)(_rnd_int32_t((PGxPER+8)*(duty_percent/100.0))))

// convert trigger offset in seconds to an integer that can be loaded into PGxTRIGy register
#define PWM_HR_PGxTRIGy(trigger_offset)        ((uint16_t)(_rnd_int32_t(8.0*500.0e+6*trigger_offset)))

// converter dead times in seconds to integer that can be loaded into PGxDTy register
#define PWM_HR_PGxDTy(dead_time)                ((uint16_t)(_rnd_int32_t(8.0*500.0e+6*dead_time)))

// convert a threshold in engineering units (volts,amps etc,) or amps to ADC threshold
// formula is:
// integer threshold = (volts or amps threshold)*gain + offset
#define UNITS_FROM_ENG_TO_ADC(threshold, gain, offset)       (_rnd((((float)threshold*(float)gain) + (float)offset)/3.3*4095.0))

#define UNITS_FROM_ADC_TO_ENG(AdcValue, Gain)       (_rnd((((float)AdcValue /(float)Gain))/4095.0 * 3.3))

/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

#endif	/* USEFUL_MACROS_H */

