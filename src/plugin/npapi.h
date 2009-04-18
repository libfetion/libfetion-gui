#ifndef _NPAPI_H_
#define _NPAPI_H_

#ifdef __OS2__
#pragma pack(1)
#endif

#if   defined(unix)        \
   || defined(__unix)      \
   || defined(__unix__)    
#	  ifndef XP_UNIX
#     define XP_UNIX 1
#	  endif
#endif

#if defined (__OS2__ ) || defined (OS2)
#	ifndef XP_OS2
#		define XP_OS2 1
#	endif /* XP_OS2 */
#endif /* __OS2__ */

#ifdef _WINDOWS
#	include <windef.h>
#	ifndef XP_WIN
#		define XP_WIN 1
#	endif /* XP_WIN */
#endif /* _WINDOWS */

#ifdef __MWERKS__
#	define _declspec __declspec
#	ifdef __INTEL__
#		undef NULL
#		ifndef XP_WIN
#			define XP_WIN 1
#		endif /* XP_WIN */
#	endif /* __INTEL__ */
#endif /* __MWERKS__ */

#ifdef XP_MACOSX
#include <Carbon/Carbon.h>
#endif

#if defined(XP_UNIX) 
#	include <stdio.h>
#	if defined(MOZ_X11)
#		include <X11/Xlib.h>
#		include <X11/Xutil.h>
#	endif
#endif

/*----------------------------------------------------------------------*/
/*             Plugin Version Constants                                 */
/*----------------------------------------------------------------------*/
#define NP_VERSION_MAJOR 0
#define NP_VERSION_MINOR 30

/*----------------------------------------------------------------------*/
/*                       Definition of Basic Types                      */
/*----------------------------------------------------------------------*/

#ifndef _UINT16
typedef unsigned short uint16;
#endif

#ifndef _UINT32
#    if defined(__alpha) || defined(__amd64__) || defined(__x86_64__)
typedef unsigned int uint32;
#    else  /* __alpha */
typedef unsigned long uint32;
#    endif /* __alpha */
#endif

/*
 * AIX defines these in sys/inttypes.h included from sys/types.h
 */
#ifndef AIX
#ifndef _INT16
typedef short int16;
#endif

#ifndef _INT32
#    if defined(__alpha) || defined(__amd64__) || defined(__x86_64__)
typedef int int32;
#    else  /* __alpha */
typedef long int32;
#    endif /* __alpha */
#endif
#endif

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (1)
#endif
#ifndef NULL
#define NULL (0L)
#endif

typedef unsigned char	NPBool;
typedef int16			NPError;
typedef int16			NPReason;



/*----------------------------------------------------------------------*/
/*                       Structures and definitions                     */
/*----------------------------------------------------------------------*/

/*
 *  NPP is a plug-in's opaque instance handle
 */
typedef struct _NPP
{
  void*	pdata;      /* plug-in private data */
  void*	ndata;      /* netscape private data */
} NPP_t;

typedef NPP_t*  NPP;

/*
 * List of variable names for which NPP_GetValue shall be implemented
 */
typedef enum {
  NPPVpluginNameString = 1,
  NPPVpluginDescriptionString,
  NPPVpluginTimerInterval,

} NPPVariable;

/*
 * List of variable names for which NPN_GetValue is implemented by LIBFETION_GUI 
 */
typedef enum {
  NPNVersion = 1,
  NPNVxtAppContext,
  NPNVnetscapeWindow,

  /* Get the NPObject wrapper for the browser window. */
  NPNVWindowNPObject = 15,
} NPNVariable;


typedef struct _NPEvent
{
  uint32 event;
  uint32 wParam;
  uint32 lParam;
} NPEvent;


/*----------------------------------------------------------------------*/
/*		     Error and Reason Code definitions			*/
/*----------------------------------------------------------------------*/

/*
 * Values of type NPError:
 */
#define NPERR_BASE                         0
#define NPERR_NO_ERROR                    (NPERR_BASE + 0)
#define NPERR_GENERIC_ERROR               (NPERR_BASE + 1)
#define NPERR_INVALID_INSTANCE_ERROR      (NPERR_BASE + 2)
#define NPERR_INVALID_FUNCTABLE_ERROR     (NPERR_BASE + 3)
#define NPERR_MODULE_LOAD_FAILED_ERROR    (NPERR_BASE + 4)
#define NPERR_OUT_OF_MEMORY_ERROR         (NPERR_BASE + 5)
#define NPERR_INVALID_PLUGIN_ERROR        (NPERR_BASE + 6)
#define NPERR_INVALID_PLUGIN_DIR_ERROR    (NPERR_BASE + 7)
#define NPERR_INCOMPATIBLE_VERSION_ERROR  (NPERR_BASE + 8)


/*----------------------------------------------------------------------*/
/*                        Function Prototypes                           */
/*----------------------------------------------------------------------*/

#if defined(_WINDOWS) && !defined(WIN32)
#define NP_LOADDS  _loadds
#else
#if defined(__OS2__)
#define NP_LOADDS _System
#else
#define NP_LOADDS
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

	
/*
 * NPP_* functions are provided by the plugin and called by the navigator.
 */

NPError NP_LOADDS NPP_Initialize(void);
void    NP_LOADDS NPP_Shutdown(void);
char*   NP_LOADDS NPP_GetPluginName(void);
char*   NP_LOADDS NPP_GetPluginDescription(void);
int16   NP_LOADDS NPP_HandleEvent(NPEvent* event);
void    NP_LOADDS NPP_PluginThreadAsyncExtendCall();


/*
 * NPN_* functions are provided by the navigator and called by the plugin.
 */
void    NP_LOADDS NPN_Version(int* plugin_major, int* plugin_minor,
                              int* netscape_major, int* netscape_minor);
NPError NP_LOADDS NPN_GetValue(NPNVariable variable, void *value);
void    NP_LOADDS NPN_PluginThreadAsyncCall(void (*func) (void *),
                                            void *userData);
void*  NP_LOADDS NPN_GetUIFuncsTable(void);
void*  NP_LOADDS NPN_GetLibFetionFuncsTable(void);

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#ifdef __OS2__
#pragma pack()
#endif

#endif /* _NPAPI_H_ */
