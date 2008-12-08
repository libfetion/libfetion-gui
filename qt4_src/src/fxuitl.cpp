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


/***************************************************************************
 *
 *
 * The faces resources and implement by iptton（iptton@gmail.com). thanks iptton.
 *
 *
 ***************************************************************************/
#include "fxuitl.h"
/* 
 * Copyright (C) 2008
 * 
 * An instance check class used in libfetion for Mac/Linux/Windows.
 *
 * Author: YongLi(liyong03@gmail.com)
 */

#ifndef WIN32 
	#include <errno.h>
	#include <signal.h>
	#include <stdio.h>
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <sys/stat.h>
#else
	#include <iostream>
	using namespace std;
#include <windows.h>
#define hotkey_id 1
static int QtModToWinMod(Qt::KeyboardModifiers keyMod)
{
	int Modifiy = 0;
	bool isHaveModifie = false;

	if (keyMod& Qt::ControlModifier)
	{
		Modifiy = Modifiy | MOD_CONTROL;
		isHaveModifie = true;
	}
	if (keyMod & Qt::ShiftModifier)
	{
		Modifiy = Modifiy | MOD_SHIFT;
		isHaveModifie = true;
	}
	if (keyMod & Qt::AltModifier)
	{
		Modifiy = Modifiy | MOD_ALT;
		isHaveModifie = true;
	}
	if (!isHaveModifie)
		Modifiy = MOD_CONTROL | MOD_ALT;
	return Modifiy;
}
#endif

bool RegistHotkey(QWidget *window, QChar keyValue, Qt::KeyboardModifiers keyMod)
{
#if WIN32
	WId w_handle = window?window->winId():0;
	int modifiy = QtModToWinMod(keyMod);
	return RegisterHotKey(w_handle, hotkey_id, modifiy, VkKeyScan(keyValue.toAscii()));
#else //liunx or mac os are not implement...
	return false;
#endif

}

bool UnRegistHotkey(QWidget *window, QChar keyValue, Qt::KeyboardModifiers keyMod)
{
#if WIN32
	WId w_handle = window?window->winId():0;
	return UnregisterHotKey(w_handle, hotkey_id);
#else
	return false;
#endif
}

//fixme: this function should move to fxResource..
QString FxFacePath()
{
#ifdef WIN32
	return "./faces_image";
#else //linux
	static QString path;
	static bool init = false;
	FILE* fp;

	if(init)
		return path;

	  if ((fp = fopen("./faces_image/1.gif", "r")))
	  {
		  fclose (fp);
		  path = "./faces_image";
	  }	
	  else
		  path = "/usr/share/libfetion/faces_image";

	init = true;
	return path;
#endif
}

QString fxgui_handle_newMsg(Fetion_MSG *fxMsg)
{
	//note: there are two formats of the fetion msg: text/plain and text/html-fragment.
	//text/plain  is sended from mobile client.
	//text/html-fragment is sended from pc client, and it is the xml format.
	if (!fxMsg)
		return "";
	QString newmsg;

	if (fxMsg->msgformat && strstr (fxMsg->msgformat, "plain")) //message from mobile  text/plain
	{
		newmsg = newmsg.fromUtf8(fxMsg->message);
		newmsg.replace(QString("<"), QString("&lt;"));
		newmsg.replace(QString(">"), QString("&gt;"));
	} else { //message from pc  text/html-fragment

		//here imple the font type and size.
		char *msg = fx_simple_paser_msg(fxMsg->message); 
		newmsg = newmsg.fromUtf8(msg);
		if(msg)
			free(msg);
	}
	newmsg.replace(QString("\n"), QString("<br>"));
	return fxgui_to_faces(newmsg);
}

QString fxgui_to_faces(QString newmsg)
{
	newmsg.replace("&gt;:)","<img src='"+FxFacePath()+"/50.gif'>");
	newmsg.replace("o:)","<img src='"+FxFacePath()+"/47.gif'>");
	newmsg.replace("*-:)","<img src='"+FxFacePath()+"/37.gif'>");

	newmsg.replace(":-d","<img src='"+FxFacePath()+"/2.gif'>");
	newmsg.replace(";)","<img src='"+FxFacePath()+"/3.gif'>");
	newmsg.replace(":-o","<img src='"+FxFacePath()+"/4.gif'>");
	newmsg.replace(":-p","<img src='"+FxFacePath()+"/5.gif'>");
	newmsg.replace("(h)","<img src='"+FxFacePath()+"/6.gif'>");
	newmsg.replace(":-@","<img src='"+FxFacePath()+"/7.gif'>");
	newmsg.replace(":(","<img src='"+FxFacePath()+"/8.gif'>");
	newmsg.replace(":'(","<img src='"+FxFacePath()+"/9.gif'>");
	newmsg.replace(":\"&gt;","<img src='"+FxFacePath()+"/10.gif'>");
	newmsg.replace("^o)","<img src='"+FxFacePath()+"/11.gif'>");

	//linux
	//newmsg.replace(":&amp;","<img src='"+FxFacePath()+"/12.gif'>");
	//官方
	newmsg.replace(":&","<img src='"+FxFacePath()+"/12.gif'>");

	newmsg.replace("8o|","<img src='"+FxFacePath()+"/13.gif'>");
	newmsg.replace("|-)","<img src='"+FxFacePath()+"/14.gif'>");
	newmsg.replace(":-#","<img src='"+FxFacePath()+"/15.gif'>");
	newmsg.replace("8-)","<img src='"+FxFacePath()+"/16.gif'>");
	newmsg.replace("(s)","<img src='"+FxFacePath()+"/17.gif'>");
	newmsg.replace("(st)","<img src='"+FxFacePath()+"/18.gif'>");
	newmsg.replace("(o)","<img src='"+FxFacePath()+"/19.gif'>");
	newmsg.replace("(l)","<img src='"+FxFacePath()+"/20.gif'>");
	newmsg.replace("(u)","<img src='"+FxFacePath()+"/21.gif'>");
	newmsg.replace("(@)","<img src='"+FxFacePath()+"/22.gif'>");

	//linux
	//newmsg.replace("(&amp;)","<img src='"+FxFacePath()+"/23.gif'>");
	//官方
	newmsg.replace("(&)","<img src='"+FxFacePath()+"/23.gif'>");

	newmsg.replace("(sn)","<img src='"+FxFacePath()+"/24.gif'>");
	newmsg.replace("(*)","<img src='"+FxFacePath()+"/25.gif'>");
	newmsg.replace("(#)","<img src='"+FxFacePath()+"/26.gif'>");
	newmsg.replace("(r)","<img src='"+FxFacePath()+"/27.gif'>");
	newmsg.replace("(})","<img src='"+FxFacePath()+"/28.gif'>");
	newmsg.replace("({)","<img src='"+FxFacePath()+"/29.gif'>");
	newmsg.replace("(k)","<img src='"+FxFacePath()+"/30.gif'>");
	newmsg.replace("(f)","<img src='"+FxFacePath()+"/31.gif'>");
	newmsg.replace("(w)","<img src='"+FxFacePath()+"/32.gif'>");
	newmsg.replace("(g)","<img src='"+FxFacePath()+"/33.gif'>");
	newmsg.replace("(^)","<img src='"+FxFacePath()+"/34.gif'>");
	newmsg.replace("-8","<img src='"+FxFacePath()+"/35.gif'>");
	newmsg.replace("(i)","<img src='"+FxFacePath()+"/36.gif'>");
	newmsg.replace("(c)","<img src='"+FxFacePath()+"/38.gif'>");
	newmsg.replace("(um)","<img src='"+FxFacePath()+"/39.gif'>");
	newmsg.replace("(mp)","<img src='"+FxFacePath()+"/40.gif'>");
	newmsg.replace("(co)","<img src='"+FxFacePath()+"/41.gif'>");
	newmsg.replace(":-|","<img src='"+FxFacePath()+"/42.gif'>");
	newmsg.replace(":-/","<img src='"+FxFacePath()+"/43.gif'>");
	newmsg.replace(":-s","<img src='"+FxFacePath()+"/44.gif'>");
	newmsg.replace(")-|","<img src='"+FxFacePath()+"/45.gif'>");
	newmsg.replace("(d)","<img src='"+FxFacePath()+"/46.gif'>");
	newmsg.replace(":-?","<img src='"+FxFacePath()+"/48.gif'>");
	newmsg.replace("(y)","<img src='"+FxFacePath()+"/49.gif'>");
	newmsg.replace(":-b","<img src='"+FxFacePath()+"/51.gif'>");
	newmsg.replace("b)","<img src='"+FxFacePath()+"/52.gif'>");

	newmsg.replace(":)","<img src='"+FxFacePath()+"/1.gif'>");

	return newmsg;
}

/* 
 * Copyright (C) 2008
 * 
 * An instance check class used in libfetion for Mac/Linux/Windows.
 *
 * Author: YongLi(liyong03@gmail.com)
 */
static int* shmPointer;

#ifndef WIN32 
	static int shid;
#else
	static HANDLE shHandle = NULL;
#endif

int initshared(const char* path,const char* keyString)
{

#ifndef WIN32 
	key_t 	key = ftok( path, 'S' );

	/* get attached memory, creating it if necessary */
	shid = shmget(key, sizeof(int), PERM | IPC_CREAT | IPC_EXCL);
	if ((shid == -1) && (errno != EEXIST))                    /* real error */
		return -1;
	if (shid == -1) 
	{              	/* already created, access and attach it */
		if (((shid = shmget(key, sizeof(int), PERM)) == -1) || ((shmPointer = (int *)shmat(shid, NULL, 0)) == (void *)-1) )
			return -1;
		//printf("already have a shared memory and the shmPointer = %d !\n", *shmPointer);
		addCount();
	}
	else {    		/* successfully created, must attach and initialize variables */
		//printf("create a shared memory!\n");
		shmPointer = (int *)shmat(shid, NULL, 0);
		if (shmPointer == (void *)-1)
			return -1;
		*shmPointer = 1;
	}
	//printf("There are %d instances!\n", *shmPointer);
	return 0;

#else
	
	/* Check the handler */
	if ( (shHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS,false,LPCWSTR(keyString) )) == NULL )
	{
		/* There is no file mapping, so create one */
		shHandle = CreateFileMapping( (HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1, LPCWSTR(keyString) );
		if ( shHandle == NULL )
			return -1;

		//printf("create a shared memory!\n");

		/* Get the access pointer */
		shmPointer = (int *)MapViewOfFile( shHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
		if ( shmPointer == NULL )
			return -1;
		(*shmPointer) = 1;
	}
	else
	{
		/* Get the access pointer */
		shmPointer = (int *)MapViewOfFile( shHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
		if ( shmPointer == NULL )
			return -1;

		//printf("already have a shared memory and the shmPointer = %d !\n", *shmPointer);
		
		addCount();
	}

	//printf("There are %d instances!\n", *shmPointer);

	return 0;

#endif
}

int addCount()
{
	(*shmPointer) = (*shmPointer) + 1;
	return 0;
}

int reducecount()
{
	//printf("reduce shmPointer!\n");
	*shmPointer = *shmPointer - 1;
	if( *shmPointer == 0 )
	{
		detachandremove(shmPointer);
		//printf("There is no instanceso, so release the shared memory!\n");
	}
	return 0;
}

int detachandremove( void *shmaddr) 
{

#ifndef WIN32
   
	int error = 0; 

	if (shmdt(shmaddr) == -1)
	  error = errno;
	if ((shmctl(shid, IPC_RMID, NULL) == -1) && !error)
	  error = errno;
	if (!error)
	  return 0;
	errno = error;

	return -1;

#else
	
	if ( UnmapViewOfFile(shmPointer) != 0 )
		return -1;
	
	if ( CloseHandle(shHandle) != 0 )
		return -1;

	return 0;
#endif

}

m_Instance::m_Instance()
{
	initshared(KEYPATH, STRKEY);
}

m_Instance::~m_Instance()
{
	reducecount();
}

int m_Instance::GetInstancesNum()
{
	return *shmPointer;
}

int m_Instance::isHaveInstance()
{
	static m_Instance instance;
	if ( instance.GetInstancesNum() > 1 )
		return 1;
	else
		return 0;
}
