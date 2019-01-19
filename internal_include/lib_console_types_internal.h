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


#ifndef _LIB_CONSOLE_TYPES_INTERNAL_H_
#define _LIB_CONSOLE_TYPES_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************
 * includes
 * ****************************************************************************/
/* c -runtime */
#include <stdint.h>
/* frame */
#include <lib_thread.h>
#include <lib_list_types.h>
/* project */
#include <lib_console_types.h>


/* ****************************************************************************
 * defines
 * ****************************************************************************/
#define M_LIB_CONSOLE__TX_BUFFER_SIZE 	200
#define M_LIB_CONSOLE__RX_BUFFER_SIZE	50
#define M_LIB_CONSOLE__OPENED			0xAAAAFFFF

/* ****************************************************************************
 * Configuration
 * ****************************************************************************/
#define M_LIB_CONSOLE__INTER_FRAME_TIMEOUT		100

/* ****************************************************************************
 * custom data types (e.g. enumerations, structures, unions)
 * ****************************************************************************/
struct console_hdl_handle {
	thread_hdl_t rxThd;
	lib_serial_hdl serialDev;
	char txBuffer[M_LIB_CONSOLE__TX_BUFFER_SIZE];
	mutex_hdl_t	txMtx;
	uint32_t initialized;
	struct list_node node;
};

#ifdef __cplusplus
}
#endif

#endif /* _LIB_CONSOLE_TYPES_INTERNAL_H_ */