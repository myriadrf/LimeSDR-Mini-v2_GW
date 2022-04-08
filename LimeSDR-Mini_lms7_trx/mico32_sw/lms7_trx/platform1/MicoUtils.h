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
 *----------------------------------------------------------------------------
 */

#ifndef MICO32_MICOUTILS_HEADER_FILE
#define MICO32_MICOUTILS_HEADER_FILE

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* case-sensitive string-compare */
int MicoStrCompare(const char* s1, const char *s2);
int nMicoStrCompare(const char *s1, const char *s2, int n);


/* converts unsigned int to string (atleast 9 charspaces) */
void ltostr(unsigned int uiValue, char *pString);

/* computes length of a null-terminated string */
int cStrLen(const char*str);

/* "sleep" CPU */



/*
 * Implements a tight-loop to achieve a "sleep" of the
 * desired time specified in microseconds.
 *
 * Note: the time spent in the tight loop is only an
 * approximation and is not very precise.
 *
 * If the instruction cache is disabled, this function
 * is highly dependent on memory lattency and will be
 * grossly off than expected.
 *
 */
void MicoSleepMicroSecs(unsigned int timeInMicroSecs);


/*
 * Implements a tight-loop to achieve a "sleep" of the
 * desired time specified in milliseconds.
 *
 * Note: the time spent in the tight loop is only an
 * approximation and is not very precise.
 * 
 * If the instruction cache is disabled, this function
 * is highly dependent on memory lattency and will be
 * grossly off than expected.
 */
void MicoSleepMilliSecs(unsigned int timeInMilliSecs);



#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /*MICO32_MICOUTILS_HEADER_FILE*/

