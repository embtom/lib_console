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

/* *******************************************************************
 * includes
 * ******************************************************************/
/* frame */
#include <lib_list.h>
#include <lib_convention__mem.h>
#include <lib_convention__errno.h>
#include <lib_convention__macro.h>

/* project */
#include <lib_console_types_internal.h>
#include <lib_console.h>

/* ****************************************************************************
 * static function declarations
 * ***************************************************************************/
static inline void lib_console_factory__register(const console_hdl_t _consoleHdl);
static inline int lib_console_factory__unregister(const console_hdl_t _consoleHdl);

/* *******************************************************************
 * (static) variables declarations
  ******************************************************************/
static unsigned int s_initializedList = 0;
static struct queue_attr s_consoleList;

/* ************************************************************************//**
 * \brief	Creation of a new console handle
 * \param	_serialDev [IN]	:	serial device handle used 
 * \return	console_hdl_t if successfully, NULL if not successful
 * ****************************************************************************/
console_hdl_t lib_console_factory__getInstance(const lib_serial_hdl _serialDev)
{
	console_hdl_t consoleHdl;
	if(_serialDev == NULL) {
		return NULL;
	}

	consoleHdl = (console_hdl_t)alloc_memory(1, sizeof(struct console_hdl_handle));
	if (consoleHdl == NULL ) {
		return NULL;
	}

	consoleHdl->serialDev = _serialDev;
    lib_console_factory__register(consoleHdl);
	return consoleHdl;
}

/* ************************************************************************//**
 * \brief	Destroys the console handle, it is the counter-part of 
 * 			"lib_console_factory__getInstance"
 * \param	_hdl [IN|OUT]	:	console handle
 * \return	EOK, if successful, ret< EOK if not successful
 * ****************************************************************************/
int ib_console_factory__destroy(console_hdl_t *_hdl)
{	
    int ret;
	/* hdl param check */
	if ((_hdl == NULL) || (*_hdl == NULL)) {
		return -ESTD_INVAL;
	}

    ret = lib_console_factory__unregister(*_hdl);
    if (ret < EOK) {
        return ret;
    }

	if((*_hdl)->initialized == M_LIB_CONSOLE__OPENED) {
		lib_console__close(*_hdl);
	}

	/* free allocated space and declare handle as invalid */
	free_memory(*_hdl);
	*_hdl = NULL;
	return EOK;
}

/* ************************************************************************//**
 * \brief	Returns the number of registered consoles 
 * \return	void
 * ****************************************************************************/
unsigned int lib_console_factory__instances(void)
{
    int ret;
    ret = lib_list__count(&s_consoleList,0,NULL);
    if (ret < 0) {
        ret = 0;
    }
    return ret;
}

/* ************************************************************************//**
 * \brief Request idx corresponding console 
 * \return	console_hdl_t
 * ****************************************************************************/
console_hdl_t lib_console_factory__instance(unsigned int _idx)
{
    struct list_node *itr;
    console_hdl_t consoleHdl;

    unsigned int iter;
    int ret;

    ret = lib_list__get_begin(&s_consoleList, &itr, 0, NULL);
    if (ret < EOK) {
        return NULL;
    }

    for (iter = 0; iter < _idx; iter++) {
        lib_list__get_next(&s_consoleList, &itr, 0, NULL);
    }
    consoleHdl = (console_hdl_t)GET_CONTAINER_OF(itr, struct console_hdl_handle, node);
    return consoleHdl;
}

/* *******************************************************************
 * static function definitions
 * ******************************************************************/

/* ************************************************************************//**
 * \brief	Register a the console device list 
 * \param	_consoleHdl[IN]	    :	serial device handle used 
 * \return	void
 * ****************************************************************************/
static void lib_console_factory__register(const console_hdl_t _consoleHdl)
{
    struct consoleNode *newConsole;

    if (!s_initializedList) {
        lib_list__init(&s_consoleList,NULL);
        s_initializedList = 1;
    }
    lib_list__enqueue(&s_consoleList, &_consoleHdl->node, 0, NULL);
}

/* ************************************************************************//**
 * \brief	Unregister a the console device list 
 * \param	_consoleHdl[IN]	    :	serial device handle used 
 * \return	void
 * ****************************************************************************/
static int lib_console_factory__unregister(const console_hdl_t _consoleHdl)
{
    int ret;

    ret = lib_list__contains(&s_consoleList, &_consoleHdl->node, 0, NULL);
    if (ret < 0) {
        return -ESTD_FAULT;
    }
    else if (ret == 0) {
        return -ESTD_NOENT;
    }

    lib_list__delete(&s_consoleList,  &_consoleHdl->node, 0, NULL);
    return EOK;
}
