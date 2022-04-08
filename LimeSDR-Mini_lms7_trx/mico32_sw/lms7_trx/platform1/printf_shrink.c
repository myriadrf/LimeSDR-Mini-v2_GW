/****************************************************************************
**
**  Name: printf_shrink.c
**
**
**  Description:
**        implements shrunk printf, putc and puts functions that can
**        replace Newlib C library's printf functions for reduced
**        code-size
**
**
**  $Revision: $
**
** Disclaimer:
**
**   This source code is intended as a design reference which
**   illustrates how these types of functions can be implemented.  It
**   is the user's responsibility to verify their design for
**   consistency and functionality through the use of formal
**   verification methods.  Lattice Semiconductor provides no warranty
**   regarding the use or functionality of this code.
**
** --------------------------------------------------------------------
**
**                     Lattice Semiconductor Corporation
**                     5555 NE Moore Court
**                     Hillsboro, OR 97214
**                     U.S.A
**
**                     TEL: 1-800-Lattice (USA and Canada)
**                          (503)268-8001 (other locations)
**
**                     web:   http://www.latticesemi.com
**                     email: techsupport@latticesemi.com
**
** --------------------------------------------------------------------------
**
**  Change History (Latest changes on top)
**
**  Ver    Date        Description
** --------------------------------------------------------------------------
**
**  3.0   Mar-25-2008  Added Header
**
**---------------------------------------------------------------------------
*****************************************************************************/

#include "stdarg.h"
#include "string.h"

/* ------ START CONFIGURATION FOR THE FUNCTIONS IN THIS FILE --- */

/*
 *---------------------------------------------
 * map LSCC_PRINTF to printf
 * Define _USE_LSCC_PRINTF_ if LSCC_PRINTF
 * implementation's to become the default
 * printf implementation.
 *---------------------------------------------
 */
/*#define _USE_LSCC_PRINTF_ */


/*
 *---------------------------------------------
 * Define _SHRINK_LSCC_PRINTF_SPACE_FMTS_to
 * support precision/width specifier when 
 * performing printf.  Adds some code ~1kbyte.
 * Default is not defined to match smallC libc
 * output which also doesn't support such
 * precision/width specifiers
 *---------------------------------------------
 */
/*#define _SHRINK_LSCC_PRINTF_SPACE_FMTS_ */

/*
 *---------------------------------------------
 * Define _LSCC_PRINTF_UNBUFFERED_ if you want
 * unbuffered printf (slight code shrinkage).
 * Not a good idea if using LM32 JTAG for
 * standard output but a good idea if using the
 * UART.
 *---------------------------------------------
 */
/*#define _LSCC_PRINTF_UNBUFFERED_ */

/*
 * If using buffered printf, you can customize
 * the buffer size (characters) used by printf.
 * Minimum is 4 characters and is enforced in
 * the code in this file.  Default is 32.
 */
/*#define LSCC_PRINTF_BUFFER_SIZE (32)*/


/* ------ END CONFIGURATION FOR THE FUNCTIONS IN THIS FILE ------ */


/* ANALYZE CONFIGURATION SPECIFICATIONS: */
#ifdef _USE_LSCC_PRINTF_
#define LSCC_PRINTF     (printf)
#define LSCC_PUTS       (puts)
#define LSCC_PUTCHAR    (putchar)
#endif

/*
 * THIS IS USED ONLY FOR DEBUGGING THE FORMATTING
 * OPERATION.  SET IT TO (1) TO ENABLE OUTPUT
 * OF THE STATE-MACHINE.  NORMAL USE DOES NOT
 * REQUIRE THIS AND SO SET IT TO (0)
 */
#ifndef _DEBUG_PRINTF_IMPLEMENTATION
#define _DEBUG_PRINTF_IMPLEMENTATION    (0)
#endif


/*
 * IF DEBUGGING THE STATE-MACHINE, DEFINE
 * YOUR OWN DEBUG_PRINT(X); A SAMPLE IS SHOWN
 * BELOW:
 */
#if _DEBUG_PRINTF_IMPLEMENTATION
#if 0 /* SAMPLE IMPLEMENTATION ONLY */
#include <iostream>
using namespace std;
#define DEBUG_PRINT(X)  \
    cout << X;
#endif
#else
/* NULL DEFINITION i.e. NOT DEBUGGING THE S/M */
#define DEBUG_PRINT(X)
#endif /* _DEBUG_PRINTF_IMPLEMENTATION */


/* printf-states */
typedef enum {
	PROCESS_GENERIC_CHAR,           /* 0 */
    PROCESS_OPTIONAL_FORMAT_SPEC,   /* 1 */
    PROCESS_WIDTH_SPEC,             /* 2 */
	PROCESS_PRECISION_SPEC,         /* 3 */
    SKIP_PRECISION_SPEC,            /* 4 */
    PROCESS_TYPE_PREFIX,            /* 5 */
}printf_states_t;




/*
 * State-machine data: Allows this printf
 * routine to become re-entrant.
 */
typedef struct {
    int width;
    int precision;
    int chars_printed;
    printf_states_t state;
    char align_right;
    char prefix_sign;
    char zero_padding;
    char blank;
    char format_prefix;
    char type_prefix;
    va_list args;
}printf_sm_data;



#ifndef _LSCC_PRINTF_UNBUFFERED_
#ifdef __cplusplus
extern "C" {
#endif
extern int _write(int, char*, int);
#ifdef __cplusplus
}
#endif
/* if not specified, use buffered printf */
/* buffer minimum size is 4; default is 32 */
#ifndef LSCC_PRINTF_BUFFER_SIZE
#define LSCC_PRINTF_BUFFER_SIZE	(32)
#endif
#if(LSCC_PRINTF_BUFFER_SIZE < 4)
#define LSCC_PRINTF_BUFFER_SIZE	(4)
#endif

/* local array to contain characters to print */
static char LSCC_PRINTF_BUFFER[LSCC_PRINTF_BUFFER_SIZE];
static unsigned int lscc_printf_buffer_chars = 0;

void OUTPUT_CHAR(char C, printf_sm_data* DATA)
{
	LSCC_PRINTF_BUFFER[lscc_printf_buffer_chars] = C;
	(DATA)->chars_printed++;
	lscc_printf_buffer_chars++;
	if(	(lscc_printf_buffer_chars >= LSCC_PRINTF_BUFFER_SIZE) ||
		(C == '\n'))
	{ 	/* maximum characters in buffer, so flush it */
		_write(1, LSCC_PRINTF_BUFFER, lscc_printf_buffer_chars);
		lscc_printf_buffer_chars = 0;
	}
}
#else
#ifdef __cplusplus
extern "C" {
#endif
extern int _write(int, char*, int);
#ifdef __cplusplus
}
#endif
/* Definition for unbuffered printf */
void OUTPUT_CHAR(char C, printf_sm_data* DATA)
{
	_write(1, &C, 1);
    (DATA)->chars_printed++;
}
#endif


/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*                    FORMAT SPECIFICATION STATE-MACHINE                   */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
#define SET_NEXT_STATE(X,N,C)   \
    DEBUG_PRINT("\ndbg - char : " << C << "\n");    \
    DEBUG_PRINT("dbg - changing state to " << N << " from " << X->state); \
    DEBUG_PRINT("\n");  \
    X->state = N;


static void init_printf_sm_data(printf_sm_data* sm_data)
{
    sm_data->width = 0; /* minimal width */
    sm_data->precision = -1;    /* no precision specified */
    sm_data->state = PROCESS_GENERIC_CHAR;  /* process generic character */
    sm_data->align_right = 1;   /* default: right-align */
    sm_data->prefix_sign = 0;   /* sign appears only for negative values */
    sm_data->zero_padding = 0;   /* no padding */
    sm_data->blank = 0; /* no blank appears */
    sm_data->format_prefix = 0; /* no type-prefixes */
    sm_data->type_prefix = ' '; /* type-prefix is not defined */
    return;
}


/* print pad-characters */
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
static void add_pad_chars(printf_sm_data* sm_data, int spaces, char c)
{
    while(spaces-- > 0)
    {
        OUTPUT_CHAR(c, sm_data);
    }
}
#endif

/* print characters */
static void print_chars(printf_sm_data* sm_data, char *c, int chars)
{
    while(chars-- > 0)
    {
        OUTPUT_CHAR(*c++, sm_data);
    }
}


/* prints integer (and its mutants) */
static void print_integer(
	printf_sm_data *sm_data, /* contains format info          */
    unsigned int int_value,  /* positive value for the number */
    int is_negative,         /* whether the number's negative */
    int integer_base,        /* 8(oct), 10(dec), 16(hex)      */
    char c)                  /* format (d, i, x, X, o, O)     */
{
    unsigned int max_power = 1;
    unsigned int power_value = 1;
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
    int total_digits = 0;
    int non_digits = 0;
#endif
	int min_width;
    unsigned int temp = int_value;
    int precision;
    int right_justified;
    int prefix_zeros;
	int prefix_sign;
	int prefix_base;
	int prefix_blank;

	/* initialize some parameters */
	prefix_blank = sm_data->blank;
	prefix_base = sm_data->format_prefix;
	prefix_sign = sm_data->prefix_sign;
	prefix_zeros = sm_data->zero_padding;
	right_justified = sm_data->align_right;
    precision = sm_data->precision;
    min_width = (sm_data->width >= 0)?sm_data->width:0;


    /* calculate total digits required */
    while((temp = (temp/integer_base)) > 0)
    {
        /* another multiply means one more digit */
        power_value *= integer_base;
        max_power++;
    };

#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
    /* total digits is max of precision and max_digits computed */
    if((precision > 0) && ((unsigned int)precision > max_power))
    {
        total_digits=precision;
    }
    else
    {
        total_digits = max_power;
    }

    /* update total digits */
    if((c=='d')||(c=='i'))
    {
        if( (is_negative == 1) ||
            (prefix_sign == 1) ||
            (prefix_blank == 1))
        {
            non_digits = 1;
        }
    }
    else if(prefix_base)
    {
        if(c == 'o')
        {
            non_digits = 1;
        }
        else if((c == 'x') || (c == 'X'))
        {
            non_digits = 2;
        }
    }

    /* if right-justified output, then add padding here */
    if( (right_justified) &&
        (min_width > (total_digits + non_digits)) &&
        ((!prefix_zeros) || ((prefix_zeros==1) && (precision >=0))))
    {
        add_pad_chars(sm_data, min_width-total_digits-non_digits, ' ');
    }
#endif

    /* print sign for decimal or format specifier */
    if((c=='d')||(c=='i'))
    {
        if(is_negative == 1)
        {
            OUTPUT_CHAR('-', sm_data);
        }
        else if(prefix_sign == 1)
        {
            OUTPUT_CHAR('+', sm_data);
        }
        else if(prefix_blank == 1)
        {
            OUTPUT_CHAR(' ', sm_data);
        }
    }
    else if(prefix_base)
    {
        if(c == 'o')
        {
            OUTPUT_CHAR('0', sm_data);
        }
        else if((c == 'x') || (c == 'X'))
        {
            OUTPUT_CHAR('0', sm_data);
            OUTPUT_CHAR('x', sm_data);
        }
    }

#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
    /* add zeros between sign/format-char and
     * actual digits */
    if((precision > 0) && (prefix_zeros))
    {
        add_pad_chars(sm_data, total_digits-max_power, '0');
    }
    else if((right_justified) &&
            (min_width > (total_digits + non_digits)) &&
            prefix_zeros)
    {
        add_pad_chars(sm_data, min_width-non_digits-total_digits, '0');
    }
#endif

    /* print actual digits */
    do
    {
        unsigned int digit = int_value / power_value;
        int_value = int_value - (digit*power_value);
        if(digit < 10)
        {
            OUTPUT_CHAR((digit + '0'), sm_data);
        }
        else
        {
            OUTPUT_CHAR((digit-10+'a'), sm_data);
        }
        power_value /= integer_base;
        if(power_value == 0)
        {
            break;
        }
    }while(1);

#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
    /* provide padding if applicable */
    if((!right_justified) && (min_width > total_digits))
    {
        add_pad_chars(sm_data, min_width-total_digits-non_digits, ' ');
    }
#endif

	return;
}


/* returns 0 if processed, else 1 */
static int process_type_spec_char(printf_sm_data* sm_data, char c)
{
    switch(c)
    {
    case 'c':
        {
            char c;
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            int pad_chars;
#endif

            c = va_arg(sm_data->args, int);

#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            pad_chars = sm_data->width-1;
            if(sm_data->align_right == 0)
            {   /* left-justified output*/
                OUTPUT_CHAR(c, sm_data);
            }
            /* add padding if needed */
            (pad_chars > 0)?add_pad_chars(sm_data,pad_chars, ' '):
            	(pad_chars = 0);
            if(sm_data->align_right == 1)
            {   /* right-justified output*/
#endif
                OUTPUT_CHAR(c, sm_data);
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            }
#endif
            break;
        };
    case 's':
        {
            char *str;
            int str_len;
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            int padding;
#endif
            /* get pointer to character */
            str = va_arg(sm_data->args, char*);
            /* calculate string-length */
            if(str == (char*)0)
            {
            	str = "(NUL)";
            }
            str_len = (int)strlen(str);

#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            /* calculate characters to actually print */
            if((sm_data->precision >= 0) &&
                (sm_data->precision < str_len))
            {
                str_len = sm_data->precision;
            }
            /* calculate padding */
            if(sm_data->width > str_len)
            {
                padding = sm_data->width - str_len;
            }
            else
            {
                padding = 0;
            }
            /* if align-right, add padding if applicable */
            if(sm_data->align_right == 1)
            {
                add_pad_chars(sm_data, padding, ' ');
            }
#endif

            /* dump out characters */
            print_chars(sm_data, str, str_len);

#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            /* if align-left, add padding if applicable */
            if(sm_data->align_right == 0)
            {
                add_pad_chars(sm_data, padding, ' ');
            }
#endif
        }break;
    case 'u':   /* decimal integer, as positive      */
    case 'd':   /* decimal integer (can be negative) */
    case 'i':   /* decimal integer (can be negative) */
    case 'x':
    case 'X':
    case 'o':
        {   /* we don't support long int or long long */
            int i_value = va_arg(sm_data->args, int);
            int is_negative = (i_value < 0)?1:0;
            int base;
            /* if integer format and negative, convert
             * number to positive and indicate it's negative */
            if(is_negative && ((c=='d') ||(c=='i')))
            {
                i_value *= -1;
            }
            else
            {
                is_negative = 0;
            }

            switch(c)
            {
            case 'x':
            case 'X':
                base = 16;
                break;
            case 'o':
                base = 8;
                break;
            default:
                base = 10;
                break;
            }
			print_integer(  sm_data,
							i_value,
                            is_negative,
                            base,
                            c);
            /*all done! */
        }break;
    case 'p':
        {
            char *ptr;
            ptr = va_arg(sm_data->args,char*);
            sm_data->format_prefix = 1;
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            sm_data->prefix_sign = 0;
            sm_data->blank = 0;
#endif
			print_integer(  sm_data,
							(int)ptr,   /* value */
                            0,          /* is not negative */
                            16,         /* hex base */
                            'x');
        }break;
    case 'G':   /* not supported in small C library */
    case 'g':   /* not supported in small C library */
	case 'E':   /* not supported in small C library */
	case 'e':	/* not supported in small C library */
    case 'f':   /* not supported in small C library */
	    {
			va_arg(sm_data->args, double);
			OUTPUT_CHAR('%',sm_data);
			OUTPUT_CHAR('f',sm_data);
	    }break;
    case 'n':   /* not supported in small C library */
	    {
	    }break;
    case 'S':   /* not supported in small C library */
    case 'C':   /* not supported in small C library */
	    {
	    }break;
    default:
        {
            return(1);
        }
    }
    /* set next state */
    SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
    return(0);
}


/* returns 0 if processed, else non-zero */
static int process_type_spec_prefix_char(printf_sm_data* sm_data, char c)
{
    switch(c)
    {
    case 'h':
    case 'l':
    case 'L':
        {
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
            sm_data->type_prefix = c;
#endif
            SET_NEXT_STATE(sm_data, PROCESS_TYPE_PREFIX, c);
            return(0);
        }
    default:
        return(1);
    }
}


/* main state-machine */
static void printf_sm_engine(printf_sm_data* sm_data, char c)
{
    switch(sm_data->state)
    {
    case PROCESS_GENERIC_CHAR:
        {
            if(c == '%')
            {
                init_printf_sm_data(sm_data);
                SET_NEXT_STATE(sm_data, PROCESS_OPTIONAL_FORMAT_SPEC,c);
            }
            else
            {
                OUTPUT_CHAR(c, sm_data);
            }
        }break;
    case PROCESS_OPTIONAL_FORMAT_SPEC:
        {
            if(c == '%')
            {
                SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
                OUTPUT_CHAR(c, sm_data);
            }
            else
            {
                switch(c)
                {
                    /* optional flags specification */
                case '-':sm_data->align_right = 0;break;
                case '+':sm_data->prefix_sign = 1;break;
                case '0':sm_data->zero_padding = 1;break;
                case ' ':sm_data->blank = 1;break;
                case '#':sm_data->format_prefix=1;break;
                /* all others... */
                default:
                    {
                        /* valid inputs are:
                          1. '.': start of precision specification
                          2. numeric: start of (and also) width specification
                          3. type-specification character
                          4. type-specification prefix character */
                        if(c == '.')
                        {
                            SET_NEXT_STATE(sm_data, PROCESS_PRECISION_SPEC, c);
                        }
                        else if((c >= '0') && (c <= '9'))
                        {
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
                            /* it's a width specification */
                            sm_data->width = (c - '0');
#endif
                            SET_NEXT_STATE(sm_data, PROCESS_WIDTH_SPEC, c);
                        }
                        else if(process_type_spec_char(sm_data,c) == 0)
                        {
                            /*was processed as type-specification character so go away */
                        }
                        else if(process_type_spec_prefix_char(sm_data, c) == 0)
                        {
                            /* was processed as type-specification prefix so go away */
                        }
                        else
                        {
                            /* something unexpected! */
                            OUTPUT_CHAR('%', sm_data);
                            SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
                        }
                    }break;
                }
            }
        }break;
    case PROCESS_WIDTH_SPEC:
        {
            /* valid inputs:
               1. width integer
               2. '.': start of precision specification
               2. type-spec char: process
               3. type-spec prefix char: process */
            if((c >= '0') && (c <= '9'))
            {
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
                /* it's a width specification */
                sm_data->width = (sm_data->width * 10) + (c - '0');
#endif
            }
            else if(c == '.')
            {
                /* start of precision specification */
                SET_NEXT_STATE(sm_data, PROCESS_PRECISION_SPEC, c);
            }
            else if(process_type_spec_char(sm_data,c) == 0)
            {
                /*was processed as type-specification character so go away */
            }
            else if(process_type_spec_prefix_char(sm_data, c) == 0)
            {
                /* was processed as type-specification prefix so go away */
            }
            else
            {
                /* something unexpected! */
                OUTPUT_CHAR('%', sm_data);
                SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
            }
        }break;
    case PROCESS_PRECISION_SPEC:
        {
            /* valid inputs:
               1. precision specification integer (to ignore)
               2. type-spec char: process
               3. type-spec prefix char: process
               4. '-' sign: ignore precision spec but is a valid input */
            if(sm_data->precision < 0)sm_data->precision = 0;
            if((c >= '0') && (c <= '9'))
            {
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
                /* it's a width specification */
                sm_data->precision = (sm_data->precision * 10) + (c - '0');
#endif
            }
            else if(c == '-')
            {
#ifdef _SHRINK_LSCC_PRINTF_SPACE_FMTS_
                sm_data->precision = -1;
#endif
                SET_NEXT_STATE(sm_data, SKIP_PRECISION_SPEC, c);
            }
            else if(process_type_spec_char(sm_data,c) == 0)
            {
                /*was processed as type-specification character so go away */
            }
            else if(process_type_spec_prefix_char(sm_data, c) == 0)
            {
                /* was processed as type-specification prefix so go away */
            }
            else
            {
                /* something unexpected! */
                OUTPUT_CHAR('%',sm_data);
                SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
            }
        }break;
    case SKIP_PRECISION_SPEC:
        {
            /* valid inputs:
               1. precision specification integer (to ignore)
               2. type-spec char: process
               3. type-spec prefix char: process */
            if((c >= '0') && (c <= '9'))
            {
                /* do nothing and maintain state */
            }
            else if(process_type_spec_char(sm_data,c) == 0)
            {
                /*was processed as type-specification character so go away */
            }
            else if(process_type_spec_prefix_char(sm_data, c) == 0)
            {
                /* was processed as type-specification prefix so go away */
            }
            else
            {
                /* something unexpected! */
                OUTPUT_CHAR('%', sm_data);
                SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
            }
        }break;
    case PROCESS_TYPE_PREFIX:
        {
            /* here, the only valid input expected is type-specifier */
            if(process_type_spec_char(sm_data, c) != 0)
            {
                OUTPUT_CHAR('%', sm_data);
                SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
            }
        }break;
    default:
        {
            OUTPUT_CHAR('%', sm_data);
            SET_NEXT_STATE(sm_data, PROCESS_GENERIC_CHAR, c);
        }break;
    }
}


/* actual implementation */
static int my_printf_r(char* fmt, va_list args)
{
    printf_sm_data data;
    init_printf_sm_data(&data);
	data.chars_printed = 0;
    data.args = args;
    while(*fmt != '\0')
    {
        printf_sm_engine(&data, *fmt);
        fmt++;
    }
	return(data.chars_printed);
}


/* reduced printf */
int LSCC_PRINTF(char* fmt, ...)
{
	int ret_value;
	va_list args;

	va_start(args, fmt);
	ret_value = my_printf_r(fmt, args);
	va_end(args);

	return(ret_value);
}


/* reduced "puts" (standard out) */
int LSCC_PUTS(const char *s)
{
    printf_sm_data data;
    init_printf_sm_data(&data);
	data.chars_printed = 0;
    while(*s != '\0')
    {
        OUTPUT_CHAR(*s++, &data);
    }
	OUTPUT_CHAR('\n', &data);
    /* return total characters printed */
    return(data.chars_printed);
}

/* reduced PUTCHAR (standard out) */
int LSCC_PUTCHAR(int c)
{
	printf_sm_data data;
	OUTPUT_CHAR((char)c, &data);
	return(c);
}

