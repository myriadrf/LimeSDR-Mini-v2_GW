/*
 * Disclaimer:
 *
 *   This source code is intended as a design reference
 *   which illustrates how these types of functions can be implemented.
 *   It is the user's responsibility to verify their design for
 *   consistency and functionality through the use of formal
 *   verification methods.  Lattice Semiconductor provides no warranty
 *   regarding the use or functionality of this code.
 *
 * --------------------------------------------------------------------
 *
 *                     Lattice Semiconductor Corporation
 *                     5555 NE Moore Court
 *                     Hillsboro, OR 97214
 *                     U.S.A
 *
 *                     TEL: 1-800-Lattice (USA and Canada)
 *                          408-826-6000 (other locations)
 *
 *                     web: http://www.latticesemi.com/
 *                     email: techsupport@latticesemi.com
 *
 * --------------------------------------------------------------------
 *
 *  Project:           LatticeMico32 
 *  File:              MicoUtils.c
 *  Description:       C Source code implementation for some utility functions
 *                     that don't require libc (C library) support:
 *
 *                          MicoStrCompare  : string compare (case-sensitive)
 *                          nMicoStrCompare : string compare upto n characters
 *                          cStrLen         : calculate null-terminated str len
 *                          ltostr          : convert unsigned int value to
 *                                            hex character string (needs
 *                                            9 characters array as arg.)
 *                          MicoSleepMicroSecs: LM32 loop for micro-sec delay
 *                          MicoSleepMilliSecs: LM32 loop for milli-secs delay
 *
 *----------------------------------------------------------------------------
 *  Change History (Since Mar 19, 2008)
 *
 * Ver  Date            Change
 * ---------------------------------------------------------------------------
 *
 * 3.0  Mar-25-2008:    Sync'd version to 3.0 for MSB 7.1 release
 *
 * 1.2  Mar-19-2008:    Changed the constant-factor used in millisec
 *                      constants from 5 to 11
 *
 * 1.1  Mar-19-2008:    Added file-header and description of macros used in
 *                      the MicoSleepMXXXSecs functions
 *
 *----------------------------------------------------------------------------
 */


#include "MicoUtils.h"
#include "MicoTypes.h"
#include "MicoMacros.h"


/*
 * This constant is used for timing a single millisecond loop (inner loop)
 * implemented in MicoSleepUtils.S file, called by MicoSleepMilliSecs function
 * implemented in this file.
 *
 * for Freq (CPU frequency), the number of loops required would be:
 *  loop = Freq/11000; since 
 * 11 * loop * (1/Freq) = 1msec.
 * Therefore, loop = (Freq * 1msec)/ 11 = Freq / 11000 with frequency
 * in hertz (e.g. 50MHz = 50,000,000)
 *
 * Same note applies to MicroSecConstantValue, except that is for microsec
 * loop
 */
#define DEFINED_MICO_SLEEP_MILLISEC_CONSTANT_VALUE  (MICO32_CPU_CLOCK_MHZ/11000)
#define DEFINED_MICO_SLEEP_MICROSEC_CONSTANT_VALUE  (MICO32_CPU_CLOCK_MHZ/11000000)



#ifdef __cplusplus
extern "C" void MicoSleepHelper(unsigned int, unsigned int);
#else
extern void MicoSleepHelper(unsigned int, unsigned int);
#endif /* __cplusplus */

/******************************************************************************
 * case-sensitive string-compare: returns 0 if strings match                  *
 ******************************************************************************/
int MicoStrCompare(const char* s1, const char* s2)
{
    if((s1 == 0) || (s2 == 0))
        return(s1 - s2);

    while(*s1 == *s2++){
        if(*s1++ == '\0')
            return(0);
    };
    return(*s1 - s2[-1]);
}



/*
 *******************************************************************************
 * case-sensitive string-compare upto n characters: returns 0 if strings match *
 *******************************************************************************
 */
int nMicoStrCompare(const char *s1, const char *s2, int n)
{
    if((s1 == 0) || (s2 == 0))
        return(s1 - s2);

    while(*s1 == *s2++){
        if((*s1++ == '\0') || (--n <= 0))
            return(0);
    };
    return(*s1 - s2[-1]);
}


/******************************************************************************
 * converts unsigned int value to a string (hex display); string needs        *
 * to have atleast 9 character-spaces.                                        *
 ******************************************************************************/
void ltostr(unsigned int uiValue, char *pString)
{
	int i;
	unsigned int value;
	pString[0] = '\0';
	pString[8] = '\0';
	for(i =0; i<8; i++){
		value = uiValue & 0xf;
		value += 48;
		if(value >= 58)
			value += 7;
		pString[7-i] = (char)value;
		uiValue = uiValue >> 4;
	}
	return;
}

/******************************************************************************
 * Computes length of a null-terminated string.                               *
 * NOTE: USER MUST ENSURE STRING IS APPROPRIATE.                              *
 ******************************************************************************/
int cStrLen(const char*str)
{
	int i = 0;
	if(str == 0)
		return(0);

	while(str[i] != '\0')i++;

	return(i);
}

/******************************************************************************
 * implements a cpu-sleep                                                     *
 ******************************************************************************/
void MicoSleepMicroSecs(unsigned int timeInMicroSecs)
{
    MicoSleepHelper(timeInMicroSecs, DEFINED_MICO_SLEEP_MICROSEC_CONSTANT_VALUE);
    return;
}


/******************************************************************************
 * implements a cpu-sleep                                                     *
 ******************************************************************************/
void MicoSleepMilliSecs(unsigned int timeInMilliSecs)
{
    MicoSleepHelper(timeInMilliSecs, DEFINED_MICO_SLEEP_MILLISEC_CONSTANT_VALUE);
    return;
}


