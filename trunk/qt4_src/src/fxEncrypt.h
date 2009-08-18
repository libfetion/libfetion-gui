/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
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
#ifndef FXENCRYPT_H
#define FXENCRYPT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    #ifndef KEY
    #define KEY "libfetion-dedodong@163.com"
    #endif

    char *encryPWD(const char * data, long datalen, bool isEncry);
    char* encryptXOR(const char *str, int str_len, const char* key);
    char* decryptXOR(const char *str, int str_len, const char* key);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
