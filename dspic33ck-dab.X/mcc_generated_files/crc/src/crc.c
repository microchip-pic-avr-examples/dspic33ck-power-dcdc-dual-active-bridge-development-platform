/**
 * CRC Generated Driver Source File
 * 
 * @file      crc.h
 *            
 * @ingroup   crcdriver
 *            
 * @brief     This is the generated driver source file for the CRC driver
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

// Section: Included Files

#include <stddef.h>
#include "../crc.h"

// Section: File specific functions

static void (*CRC_EventHandler)(void) = NULL;
// Section: Driver Interface

const struct CRC_INTERFACE CRC = {
    .Initialize = &CRC_Initialize,
    .Deinitialize = &CRC_Deinitialize,
    .SeedSet = &CRC_SeedSet,
    .CalculateBufferStart = &CRC_CalculateBufferStart,
    .CalculateProgramStart = &CRC_CalculateProgramStart,
    .CalculationIsDone = &CRC_CalculationIsDone,
    .CalculationResultGet = &CRC_CalculationResultGet,
    .CalculationResultRawGet = &CRC_CalculationResultRawGet,
    .CalculationResultReverseGet = &CRC_CalculationResultReverseGet,
    .CalculationResultXORGet = &CRC_CalculationResultXORGet,
    .EventCallbackRegister = &CRC_EventCallbackRegister,
    .Tasks = &CRC_Tasks
};
// Section: Structure Declarations

struct CRC_OBJ
{
    uint8_t dataWidth;                              /**< Set the CRC data width */
    uint8_t polyWidth;                              /**< Set the CRC polynomial width */
    enum CRC_STATE state;       /**< Defines the CRC calculation state */
    bool program;                                   /**< Defines the Program space CRC start status */
    uint32_t remainingSize;                         /**< Defines the CRC calculation buffer remaining size */
    union
    {
        uint8_t *data8bit;                          /**< Set the CRC data 8bit pointer */
        uint16_t *data16bit;                        /**< Set the CRC data 16bit pointer */
        uint32_t *data32bit;                        /**< Set the CRC data 32bit pointer */
        void *data;                                 /**< Set the CRC data void pointer */
        uint32_t program;                           /**< Set the CRC program start address */
    } ptr;
};

// Section: Private Data
 
static struct CRC_OBJ crcObj;

void CRC_Initialize(void)
{
    // reset the module
    CRCCONL = 0;
    CRCCONH = 0;
    CRCXORH = 0;
    CRCXORL = 0;
    CRCWDATH = 0;
    CRCWDATL = 0;

    // initials the module - it will not be enabled until the end of the function
    // MOD Legacy; LENDIAN Start with MSb; CRCISEL Interrupt on shift complete and results ready; VWORD 0x0; SIDL disabled; CRCGO CRC is turned off; CRCMPT disabled; CRCEN disabled; CRCFUL disabled; 
    CRCCONL = 0x0;
    // PLEN 16; DWIDTH 16; 
    CRCCONH = 0xF0F;
    
    CRC_EventCallbackRegister(&CRC_EventCallback);
    
    // enable module
    CRCCONLbits.CRCEN = 1;

    // some polynomial
    CRCXORL = 0x8005;
    CRCXORH = 0x0;
    
    // some seed
    CRCWDATL = 0x0;
    CRCWDATH = 0x0;

    // set module state
    crcObj.remainingSize = 0;
    crcObj.state = CRC_STATE_CALCULATE;
	
	crcObj.polyWidth = CRCCONHbits.PLEN + 1;
}

void CRC_Deinitialize(void)
{
    
    CRCCONL = 0x40;
    CRCCONH = 0x0;
    CRCXORL = 0x0;
    CRCXORH = 0x0;
    CRCWDATL = 0x0;
    CRCWDATH = 0x0;
}

// Section: Private CRC Driver Functions
 
static bool CRC_ProgramTask(void)
{
    uint16_t size;
    uint16_t tempTbl;
    uint16_t lowWord;
    uint16_t highWord;
    bool status = false;

    size = (uint16_t)0xFFFE - (uint16_t)(crcObj.ptr.program & (uint16_t)0xFFFF);
    tempTbl = TBLPAG;

    CRCCONLbits.CRCGO = false;
    IFS3bits.CRCIF = false;

    TBLPAG = (uint16_t)(crcObj.ptr.program >> 16);

    while((!CRCCONLbits.CRCFUL) && (crcObj.remainingSize) && (size))
    {
        lowWord = __builtin_tblrdl((uint16_t)(crcObj.ptr.program & (uint16_t)0xFFFF));
        crcObj.ptr.program++;
        highWord  = __builtin_tblrdh((uint16_t)(crcObj.ptr.program & (uint16_t)0xFFFF));
        crcObj.ptr.program++;
        
        CRCDATL = lowWord;
        CRCDATH = highWord;

        crcObj.remainingSize -= (uint32_t)3;
        size -= (uint16_t)2;
    }

    TBLPAG = tempTbl;
    
    CRCCONLbits.CRCGO = true;
    
    if(crcObj.remainingSize == (uint32_t)0) 
    {
        status = true;

    }

    return status;
}

static bool CRC_BufferTask(void)
{
    bool status = false;

    if(CRCCONLbits.CRCFUL != 0U)
    {
        status = false;
    }
    else
    {
        IFS3bits.CRCIF = false;

        if(crcObj.dataWidth <= (uint8_t)8)
        {
            while((!CRCCONLbits.CRCFUL) && (crcObj.remainingSize))
            {
                *((uint8_t *)&CRCDATL) = *crcObj.ptr.data8bit;
                crcObj.ptr.data8bit++;
                crcObj.remainingSize--;
            }
        } 
        else if(crcObj.dataWidth <= (uint8_t)16)
        {
            while((!CRCCONLbits.CRCFUL) && (crcObj.remainingSize))
            {
                CRCDATL = *crcObj.ptr.data16bit;
                crcObj.ptr.data16bit++;
                crcObj.remainingSize -= (uint32_t)2;
            }
        } 
        else
        {
            while((!CRCCONLbits.CRCFUL) && (crcObj.remainingSize))
            {
                CRCDATL = *crcObj.ptr.data16bit;
                crcObj.ptr.data16bit++;
                CRCDATH = *crcObj.ptr.data16bit;
                crcObj.ptr.data16bit++;
                crcObj.remainingSize -= (uint32_t)4;
            }
        }

        CRCCONLbits.CRCGO = true;

        if(crcObj.remainingSize == (uint32_t)0)
        {
            status = true;
        }
    }

    return status;
}

static bool CRC_FlushTask(void)
{
    bool status = false;

    if(IFS3bits.CRCIF != 0U)
    {

        CRCCONLbits.CRCGO = false;
        IFS3bits.CRCIF = false;

        CRCCONHbits.DWIDTH = crcObj.polyWidth - (uint8_t)1;
        
        if(crcObj.polyWidth <= (uint8_t)8)
        {
            *((uint8_t *)&CRCDATL) = 0;
        }
        else if(crcObj.polyWidth <= (uint8_t)16)
        {
            CRCDATL = 0;
        }
        else
        {
            CRCDATL = 0;
            CRCDATH = 0;
        }

        CRCCONLbits.CRCGO = true;
        
        status = true;
    }

    return status;
}

static bool CRC_CleanUpTask(void)
{
    bool status = false;

    if(IFS3bits.CRCIF != 0U)
    {

        CRCCONHbits.DWIDTH = crcObj.dataWidth - (uint8_t)1;
        CRCCONLbits.CRCGO = false;
        IFS3bits.CRCIF = false;

        status = true;
    }
    
    return status;
}

static uint32_t CRC_ReverseValue(uint32_t crc)
{
    uint32_t mask;
    uint32_t reverse;
    uint32_t crctemp = crc;

    mask = 1;
    mask <<= (crcObj.polyWidth - (uint8_t)1);
    reverse = 0;

    while(crctemp  != 0U)
    {
        if((crctemp & (uint32_t)0x01) != 0U)
        {
            reverse |= mask;

        }
            
        mask >>= 1;
        crctemp >>= 1;
    }

    return reverse;
}

static uint32_t CRC_PolynomialMask(void)
{
    uint32_t mask = 0;
    uint32_t idx;

    for(idx = 0; idx < crcObj.polyWidth; idx++)
    {
        mask <<= 1;
        mask |= 1;
    }

    return mask;
}

// Section: CRC Module APIs

void CRC_SeedSet(uint32_t seed, enum CRC_SEED_METHOD seedMethod, enum CRC_SEED_DIRECTION seedDirection)
{
    uint8_t direction = CRCCONLbits.LENDIAN;
    uint8_t dataWidth = CRCCONHbits.DWIDTH;


    if(seedMethod == CRC_SEED_METHOD_INDIRECT)
    {
        CRCWDATL = (uint16_t)seed;
        CRCWDATH = (uint16_t)(seed >> 16);
    }
    else
    {

        CRCCONHbits.DWIDTH = CRCCONHbits.PLEN; 
        CRCCONLbits.LENDIAN = seedDirection;

        if(crcObj.polyWidth <= (uint8_t)8)
        {
            *((uint8_t *)&CRCDATL) = (uint8_t)seed;
        }
        else if(crcObj.polyWidth <= (uint8_t)16)
        {
            CRCDATL = (uint16_t)seed;
        }
        else
        {
            CRCDATL = (uint16_t)seed;
            CRCDATH = (uint16_t)(seed >> 16);
        }

        // Run the seed through the shift register
        IFS3bits.CRCIF = false;

        CRCCONLbits.CRCGO = true;
        while(1){
            if(IFS3bits.CRCIF == true){
                break;
            }

        }
        
        CRCCONLbits.CRCGO = false;

        CRCCONLbits.LENDIAN = direction;
        IFS3bits.CRCIF = false;
        CRCCONHbits.DWIDTH = dataWidth; 
    }
    
}

void CRC_CalculateBufferStart(void *buffer, uint32_t sizeBytes)
{
    crcObj.dataWidth = CRCCONHbits.DWIDTH + 1;
    crcObj.polyWidth = CRCCONHbits.PLEN + 1;
    crcObj.remainingSize = sizeBytes;
    crcObj.ptr.data = buffer;
    crcObj.state = CRC_STATE_CALCULATE;
    crcObj.program = false;

    CRCCONLbits.CRCGO = true;
}

void CRC_CalculateProgramStart(uint32_t startAddr, uint32_t sizeBytes)
{
    crcObj.dataWidth = CRCCONHbits.DWIDTH + 1;
    crcObj.polyWidth = CRCCONHbits.PLEN + 1;
    crcObj.remainingSize = sizeBytes;
	if(startAddr % 2 == 0) 
	{
		crcObj.ptr.program = startAddr;
	}
	else
	{
		crcObj.ptr.program = startAddr - 1;
	}
    crcObj.state = CRC_STATE_CALCULATE;
    crcObj.program = true;
    
    CRCCONHbits.DWIDTH = 32 - 1;

    CRCCONLbits.CRCGO = true;
}

void CRC_EventCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CRC_EventHandler = handler;
    }
}

void __attribute__ ((weak)) CRC_EventCallback ( void )
{ 

} 


bool CRC_CalculationIsDone(void)
{
    return crcObj.state == CRC_STATE_DONE;
}

uint32_t CRC_CalculationResultRawGet(void)
{
    uint32_t result;

    if(crcObj.polyWidth <= (uint8_t)8)
    {
        result = (uint32_t)CRCWDATL & (uint16_t)0xFF;
    }
    else if(crcObj.polyWidth <= (uint8_t)16)
    {
        result = (uint32_t)CRCWDATL;
    }
    else
    {
        result = (uint32_t)CRCWDATH;
        result <<= 16;
        result |= (uint32_t)CRCWDATL;
    }
    
    return result;
}

uint32_t CRC_CalculationResultReverseGet(void)
{
    uint32_t result;

    result = CRC_CalculationResultRawGet();

    return CRC_ReverseValue(result);
}

uint32_t CRC_CalculationResultXORGet(uint32_t xorValue)
{
    uint32_t result;

    result = CRC_CalculationResultRawGet();

    result ^= xorValue;

    return result & CRC_PolynomialMask();
}

uint32_t CRC_CalculationResultGet(bool reverse, uint32_t xorValue)
{
    uint32_t result;

    result = CRC_CalculationResultRawGet();

    if(reverse) 
    {
        result = CRC_CalculationResultReverseGet();
    }
    else
    {
        result = CRC_CalculationResultRawGet();
    }

    result ^= xorValue;

    return result & CRC_PolynomialMask();
}

void CRC_Tasks(void)
{
    switch(crcObj.state)
    {
        case CRC_STATE_CALCULATE:
            if(crcObj.program)
            {
                if(CRC_ProgramTask()) 
                {
                    crcObj.state = CRC_STATE_FLUSH;
                }
            }
            else
            {
                if(CRC_BufferTask())
                {
                    crcObj.state = CRC_STATE_FLUSH;
                }
            }    
            break;

        case CRC_STATE_FLUSH:
            if(CRC_FlushTask())
            {
                crcObj.state = CRC_STATE_CLEANUP;
            }
            break;

        case CRC_STATE_CLEANUP:
            if(CRC_CleanUpTask())
            {
                crcObj.state = CRC_STATE_DONE;
            }
            break;

        case CRC_STATE_DONE:
            break;
        
        default:
            CRCCONL = 0;
            CRCCONH = 0;
            break;
    }

    if(IFS3bits.CRCIF != 0U)
    {
        if(NULL != CRC_EventHandler)
        {
            (*CRC_EventHandler)();
        }
    }
}

/**
 End of File
*/