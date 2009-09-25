/***************************************************************************
 *   Copyright (C) 2008 by DDD                                             *
 *   dedodong@163.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FXDEBUG_H_
#define FXDEBUG_H_

#include "appconfig.h"

/*------------------------------------------------------------------
 * LOG4QT DECLARE HERE
 *-----------------------------------------------------------------*/
#ifdef HAVE_LOG4QT_DEBUG_ENABLED
    /* DEBUG Logger */
    #include "log4qt/logger.h"

    #define FX_LOGGER   logger()
    #define FX_DEBUG(x) do{logger()->debug(x);}while(0);
    #define FX_INFO(x) do{logger()->info(x);}while(0);
    #define FX_WARN(x) do{logger()->warn(x);}while(0);
    #define FX_ERROR(x) do{logger()->error(x);}while(0);
    #define FX_FATAL(x) do{logger()->fatal(x);}while(0);

    #ifdef WIN32
        #define FX_FUNCTION FX_DEBUG(__FUNCTION__)
    #else
        #define FX_FUNCTION FX_DEBUG(__PRETTY_FUNCTION__)
    #endif
#else
    /* unmask log4qt private macro */
    #define LOG4QT_DECLARE_QCLASS_LOGGER
    #define FX_LOGGER
    #define FX_DEBUG(x)
    #define FX_INFO(x)
    #define FX_WARN(x)
    #define FX_ERROR(x)
    #define FX_FATAL(x)
    #define FX_FUNCTION
#endif/* HAVE_LOG4QT_DEBUG_ENABLED */

/*------------------------------------------------------------------
 * ASSERT SERIES DEBUG MACRO
 *-----------------------------------------------------------------*/
#define FX_RETURN_IF_FAILED(x) do{ if (!x) return; }while(0);
#define FX_RETURN_WITH_VALUE_IF_FAILED(x, v) do{ if (!x) return v; }while(0);

#endif /* FXDEBUG_H_ */
