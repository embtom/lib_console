/*
 * This file is part of the EMBTOM project
 * Copyright (c) 2018-2019 Thomas Willetal 
 * (https://github.com/tom3333)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef _LIB_CONSOLE_H_
#define _LIB_CONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************
 * includes
 * ****************************************************************************/

/* c-runtime */
#include <stdio.h>

/* system */
#include <stdarg.h>

/*frame*/
#include <lib_serial_types.h>
#include "lib_console_types.h"

/* ****************************************************************************
 * function declarations
 * ****************************************************************************/

/* ************************************************************************//**
 * \brief	Opens the console port with a specific baud rate and format 
 * \param	_hdl [IN]	:	console handle used for communication
 * \parm    _baudrate	:	baudrate to be used
 * \parm    _format		:   serial message format
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__open(console_hdl_t _hdl, enum baudrate _baudrate, enum data_format _format);

/* ************************************************************************//**
 * \brief	Close of the console, it is the counter-part function of lib_console__open
 * \param	_hdl [IN]	:	console handle used for communication
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__close(console_hdl_t _hdl);

/* ************************************************************************//**
 * \brief Printout a message on the serial console
 * \param	_hdl [IN]	:	console handle used for communication
 * \param   _format 	:	"printf" style formatted string argument
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__print_debug_message(console_hdl_t _hdl, const char * const _format, ...);

/* ************************************************************************//**
 * \brief	Printout a variable argument list message on the serial console
 * \param	_hdl [IN]	:	console handle used for communication
 * \param   _format 	:	"printf" style formatted string argument
 * \param	_ap		    :	variable argument list
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__vprint_debug_message(console_hdl_t _hdl, const char * const _format, va_list _ap);

/* ************************************************************************//**
 * \brief Printout a character on the serial console
 * \param	_hdl [IN]	:	console handle used for communication
 * \param   _c 		    : 	Character to print
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__putchar(console_hdl_t _hdl, char _c);

/* ************************************************************************//**
 * \brief	Read of a full console log until the newline is reached
 * \param	_hdl [IN]	 :	console handle used for communication
 * \param   _lineptr[OUT]:	pointer to storage location
 * \param	_n[IN|OU]	 :	pointer to buffer length
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__getline(console_hdl_t _hdl, char *_lineptr, size_t *_n);

/* ************************************************************************//**
 * \brief	 Read string until the delimitaion character is found
 * \param	_hdl [IN]	 :	console handle used for communication
 * \param   _lineptr[OUT]:	pointer to storage location
 * \param	_n[IN|OU]	 :	pointer to buffer length
 * \param	_delimiter	 :	delimiter character to read line
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__getdelim(console_hdl_t _hdl, char *_lineptr, size_t *_n, char _delimiter);

#ifdef __cplusplus
}
#endif

#endif /* _LIB_CONSOLE_H_ */
