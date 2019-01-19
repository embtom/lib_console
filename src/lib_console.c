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


/* ****************************************************************************
 * includes
 * ****************************************************************************/

/* c-runtime */
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* frame */
#include <lib_convention__errno.h>
#include <lib_convention__macro.h>
#include <lib_convention__mem.h>
#include <lib_thread.h>
#include <mini-printf.h>
#include <lib_serial.h>

/* project */
#include <lib_console_types_internal.h>
#include "lib_console.h"


/* ****************************************************************************
 * static function declarations
 * ***************************************************************************/
static void lib_console__send (console_hdl_t _hdl, uint8_t *_data_send, unsigned int _length);

/* ****************************************************************************
 * Global Functions
 * ****************************************************************************/

/* ************************************************************************//**
 * \brief	Opens the console port with a specific baud rate and format 
 * \param	_hdl [IN]	:	console handle used for communication
 * \parm    _baudrate	:	baudrate to be used
 * \parm    _format		:   serial message format
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__open(console_hdl_t _hdl, enum baudrate _baudrate, enum data_format _format)
{
	int ret;
	if (_hdl == NULL) {
		return -ESTD_INVAL;
	}	

	ret = lib_serial_open(_hdl->serialDev, _baudrate, _format);
	if (ret < EOK) {
		goto ERR_SERIAL_OPEN;
		return -ESTD_IO;
	}

	memset(&_hdl->txBuffer[0],0, sizeof(_hdl->txBuffer));

	ret = lib_thread__mutex_init(&_hdl->txMtx);
 	if (ret < EOK) {
 		goto ERR_TX_MTX;
 	}
	_hdl->initialized = M_LIB_CONSOLE__OPENED;
	return EOK;

	ERR_TX_MTX:
	lib_serial_close(_hdl->serialDev);

	ERR_SERIAL_OPEN:
	return ret;
}

/* ************************************************************************//**
 * \brief	Close of the console, it is the counter-part function of lib_console__open
 * \param	_hdl [IN]	:	console handle used for communication
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__close(console_hdl_t _hdl)
{
	int ret;
	if (_hdl == NULL) {
		return -ESTD_INVAL;
	}

	if (_hdl->initialized != M_LIB_CONSOLE__OPENED) {
		return -EEXEC_NOINIT;	
	}

	lib_thread__mutex_destroy(&_hdl->txMtx);
	ret = lib_serial_close(_hdl->serialDev);
	_hdl->initialized = 0;
	return ret;
}

/* ************************************************************************//**
 * \brief Printout a message on the serial console
 * \param	_hdl [IN]	:	console handle used for communication
 * \param   _format 	:	"printf" style formatted string argument
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__print_debug_message(console_hdl_t _hdl, const char * const _format, ...)
{
	int ret;
	va_list ap;

	if (_hdl == NULL) {
		return -ESTD_INVAL;
	}

	if (_hdl->initialized != M_LIB_CONSOLE__OPENED) {
		return -EEXEC_NOINIT;
	}

	va_start(ap,_format);
	ret = lib_console__vprint_debug_message(_hdl, _format, ap);
	va_end(ap);

	return ret;
}

/* ************************************************************************//**
 * \brief	Printout a variable argument list message on the serial console
 * \param	_hdl [IN]	:	console handle used for communication
 * \param   _format 	:	"printf" style formatted string argument
 * \param	_ap		    :	variable argument list
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__vprint_debug_message(console_hdl_t _hdl, const char * const _format, va_list _ap)
{
	int len, ret;

	if (_hdl == NULL) {
		return -ESTD_INVAL;
	}

	if (_hdl->initialized != M_LIB_CONSOLE__OPENED) {
		return -EEXEC_NOINIT;
	}

	lib_thread__mutex_lock(_hdl->txMtx);
	memset(&_hdl->txBuffer[0],0,M_LIB_CONSOLE__TX_BUFFER_SIZE);

	len = mini_vsnprintf(&_hdl->txBuffer[0],M_LIB_CONSOLE__TX_BUFFER_SIZE, (const char*) _format,_ap);
	if(len > M_LIB_CONSOLE__TX_BUFFER_SIZE - 1) {
	 	lib_thread__mutex_unlock(_hdl->txMtx);
	 	return -ESTD_NOMEM;
	}

	// append "\r" in case of new line
	if(_hdl->txBuffer[len - 1] == '\n'){
	 	_hdl->txBuffer[len] = '\r';
	 	len++;
	}

	ret = lib_serial_write(_hdl->serialDev, &_hdl->txBuffer[0], len);
	if (ret < EOK) {
		lib_thread__mutex_unlock(_hdl->txMtx);
		return ret;
	}
	lib_thread__mutex_unlock(_hdl->txMtx);
	return ret;
}

/* ************************************************************************//**
 * \brief Printout a character on the serial console
 * \param	_hdl [IN]	:	console handle used for communication
 * \param   _c 		    : 	Character to print
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__putchar(console_hdl_t _hdl, char _c)
{
	int ret;

	if (_hdl == NULL) {
		return -ESTD_INVAL;
	}
	
	if (_hdl->initialized != M_LIB_CONSOLE__OPENED) {
		return -EEXEC_NOINIT;
	}

	lib_thread__mutex_lock(_hdl->txMtx);
	ret = lib_serial_write(_hdl->serialDev, &_c, 1);
	lib_thread__mutex_unlock(_hdl->txMtx);
	return ret;
}

/* ************************************************************************//**
 * \brief	Read of a full console log until the newline is reached
 * \param	_hdl [IN]	 :	console handle used for communication
 * \param   _lineptr[OUT]:	pointer to storage location
 * \param	_n[IN|OU]	 :	pointer to buffer length
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__getline(console_hdl_t _hdl, char *_lineptr, size_t *_n)
{
	return lib_console__getdelim(_hdl, _lineptr,_n,'\n');
}

/* ************************************************************************//**
 * \brief	 Read string until the delimitaion character is found
 * \param	_hdl [IN]	 :	console handle used for communication
 * \param   _lineptr[OUT]:	pointer to storage location
 * \param	_n[IN|OU]	 :	pointer to buffer length
 * \param	_delimiter	 :	delimiter character to read line
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int lib_console__getdelim(console_hdl_t _hdl, char *_lineptr, size_t *_n, char _delimiter)
{
	int ret, i;
	unsigned int loopExit = 0;
	size_t max_lineptr_size, readSize = 0;
	char *readPos;

	if ((_hdl == NULL) || (_lineptr == NULL) || (_n == NULL)) {
		return -ESTD_INVAL;
	}

	if (_hdl->initialized != M_LIB_CONSOLE__OPENED) {
		return -EEXEC_NOINIT;
	}

	max_lineptr_size = *_n;
	readPos = _lineptr;
	while ((readSize < max_lineptr_size) && (!loopExit))
	{
		ret = lib_serial_read(_hdl->serialDev, (uint8_t*)readPos, max_lineptr_size - readSize, M_LIB_CONSOLE__INTER_FRAME_TIMEOUT);
		if (ret < EOK) {
			return ret;
		}

		for(i = 0; i < ret; i++) {
			if (readPos[i] == '\r') {
				readPos[i] = '\n';
			}

			if (readPos[i] == 0x7f) {
				lib_console__putchar(_hdl, '\b');
				lib_console__putchar(_hdl, ' ');
				lib_console__putchar(_hdl, '\b');
				readPos[i] = 0;
				readPos[i-1] = 0;
				readPos -=1;
				readSize -=1;
				ret=0;
			}

			else if (readPos[i] == _delimiter) {
				loopExit = 1;	
			}
		}
		lib_console__send(_hdl, (uint8_t*)readPos, ret);
		readPos += ret;
		readSize += ret;
	}
	*_n = readSize;
	return EOK;
}

/* *******************************************************************
 * static function definitions
 * ******************************************************************/
static void lib_console__send (console_hdl_t _hdl, uint8_t *_data_send, unsigned int _length)
{
	int ret;
	char characterReturn = '\r';

	if (_length == 0) {
		return;
	}

	lib_thread__mutex_lock(_hdl->txMtx);
	lib_serial_write(_hdl->serialDev, (uint8_t*)_data_send, _length);
	// append "\r" in case of new line
	if(_data_send[_length - 1] == '\n'){
		lib_serial_write(_hdl->serialDev, (uint8_t*)&characterReturn, 1);
	}
	lib_thread__mutex_unlock(_hdl->txMtx);
}


