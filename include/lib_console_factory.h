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


#ifndef _LIB_CONSOLE_FACTORY_H_
#define _LIB_CONSOLE_FACTORY_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************
 * includes
 * ****************************************************************************/

/* project */
#include "lib_console_types.h"

/* ****************************************************************************
 * custom data types (e.g. enumerations, structures, unions)
 * ****************************************************************************/
typedef struct lib_serial_handle *lib_serial_hdl;

/* ****************************************************************************
 * function declarations
 * ****************************************************************************/

/* ************************************************************************//**
 * \brief	Creation of a new console handle
 * \param	_serialDev [IN]	:	serial device handle used 
 * \return	console_hdl_t if successfully, NULL if not successful
 * ****************************************************************************/
console_hdl_t lib_console_factory__getInstance(const lib_serial_hdl _serialDev);

/* ************************************************************************//**
 * \brief	Destroys the console handle, it is the counter-part of "lib_console_factory__getInstance"
 * \param	_hdl [IN|OUT]	:	console handle
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console_factory__destroy(console_hdl_t *_hdl);

/* ************************************************************************//**
 * \brief	Returns the number of registered consoles 
 * \return	void
 * ****************************************************************************/
unsigned int lib_console_factory__instances(void);

/* ************************************************************************//**
 * \brief Request idx corresponding console 
 * \return	console_hdl_t
 * ****************************************************************************/
console_hdl_t lib_console_factory__instance(unsigned int _idx);

#ifdef __cplusplus
}
#endif

#endif /* _LIB_CONSOLE_FACTORY_H_ */
