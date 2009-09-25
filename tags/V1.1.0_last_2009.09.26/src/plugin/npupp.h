/*
 *  npupp.h $Revision: 3.25 $
 *  function call mecahnics needed by platform specific glue code.
 */

#ifndef _NPUPP_H_
#define _NPUPP_H_

#ifndef GENERATINGCFM
#define GENERATINGCFM 0
#endif

#ifndef _NPAPI_H_
#include "npapi.h"
#endif

/******************************************************************************************
   plug-in function table macros
 	        for each function in and out of the plugin API we define
                    typedef NPP_FooUPP
					#define NewNPP_FooProc
					#define CallNPP_FooProc
 *******************************************************************************************/


/* NPP_Initialize */
typedef void (* NP_LOADDS NPP_InitializeUPP)(void);
#define NewNPP_InitializeProc(FUNC)		\
		((NPP_InitializeUPP) (FUNC))
#define CallNPP_InitializeProc(FUNC)		\
		(*(FUNC))()

/* NPP_Shutdown */
typedef void (* NP_LOADDS NPP_ShutdownUPP)(void);
#define NewNPP_ShutdownProc(FUNC)		\
		((NPP_ShutdownUPP) (FUNC))
#define CallNPP_ShutdownProc(FUNC)		\
		(*(FUNC))()

/* NPP_HandleEvent */
typedef int16 (* NP_LOADDS NPP_HandleEventUPP)(NPEvent* event);
#define NewNPP_HandleEventProc(FUNC)		\
		((NPP_HandleEventUPP) (FUNC))
#define CallNPP_HandleEventProc(FUNC, voidPtr)		\
		(*(FUNC))(voidPtr)

/* NPP_GetPluginName */
typedef char* (* NP_LOADDS NPP_GetPluginNameUPP)(void);
#define NewNPP_GetPluginNameProc(FUNC)		\
		((NPP_GetPluginNameUPP) (FUNC))
#define CallNPP_GetPluginNameProc(FUNC)		\
		(*(FUNC))()

/* NPP_GetPluginDescription */
typedef char* (* NP_LOADDS NPP_GetPluginDescriptionUPP)(void);
#define NewNPP_GetPluginDescriptionProc(FUNC)		\
		((NPP_GetPluginDescriptionUPP) (FUNC))
#define CallNPP_GetPluginDescriptionProc(FUNC)		\
		(*(FUNC))()

/* NPP_GetPluginDescription */
typedef void (* NP_LOADDS NPP_PluginThreadAsyncExtendCallUPP)(void);
#define NewNPP_PluginThreadAsyncExtendCallProc(FUNC)		\
		((NPP_PluginThreadAsyncExtendCallUPP) (FUNC))
#define CallNPP_PluginThreadAsyncExtendCallProc(FUNC)		\
		(*(FUNC))()

/*
 *  Netscape entry points
 */

/* NPN_GetValue */
typedef NPError	(* NP_LOADDS NPN_GetValueUPP)(NPNVariable variable, void *ret_alue);
#define NewNPN_GetValueProc(FUNC)		\
		((NPN_GetValueUPP) (FUNC))
#define CallNPN_GetValueProc(FUNC, ARG1, ARG2)		\
		(*(FUNC))((ARG1), (ARG2))

/* NPN_PluginThreadAsyncCall */
typedef void (* NP_LOADDS NPN_PluginThreadAsyncCallUPP)(void (*func) (void *),
														void *userData);
#define NewNPN_PluginThreadAsyncCallProc(FUNC)		\
		((NPN_PluginThreadAsyncCallUPP) (FUNC))
#define CallNPN_PluginThreadAsyncCallProc(FUNC, ARG1, ARG2)		\
		(*(FUNC))((ARG1), (ARG2))

/* NPN_GetUIFuncsTable */
typedef void* (* NP_LOADDS NPN_GetUIFuncsTableUPP)(void);
#define NewNPN_GetUIFuncsTableProc(FUNC)		\
		((NPN_GetUIFuncsTableUPP) (FUNC))
#define CallNPN_GetUIFuncsTableProc(FUNC)		\
		(*(FUNC))()	

/* NPN_GetLibFetionFuncsTable */
typedef void* (* NP_LOADDS NPN_GetLibFetionFuncsTableUPP)(void);
#define NewNPN_GetLibFetionFuncsTableProc(FUNC)		\
		((NPN_GetLibFetionFuncsTableUPP) (FUNC))
#define CallNPN_GetLibFetionFuncsTableProc(FUNC)		\
		(*(FUNC))()	

/******************************************************************************************
 * The actual plugin function table definitions
 *******************************************************************************************/

typedef struct _NPPluginFuncs {
    uint16 size;
    uint16 version;
	NPP_InitializeUPP init;
	NPP_ShutdownUPP shut;
	NPP_GetPluginNameUPP name;
	NPP_GetPluginDescriptionUPP desc;
    NPP_HandleEventUPP event;
    NPP_PluginThreadAsyncExtendCallUPP excall;
} NPPluginFuncs;

typedef struct _NPNetscapeFuncs {
    uint16 size;
    uint16 version;
    NPN_GetValueUPP getvalue;
	NPN_PluginThreadAsyncCallUPP asyncCall;
	NPN_GetUIFuncsTableUPP uitable;
	NPN_GetLibFetionFuncsTableUPP libfxtable;
} NPNetscapeFuncs;


#if defined(XP_UNIX)
/* GCC 3.3 and later support the visibility attribute. */
#if defined(__GNUC__) && \
    ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define NP_VISIBILITY_DEFAULT __attribute__((visibility("default")))
//#define NP_VISIBILITY_DEFAULT __attribute__((visibility("hidden")))
#else
#define NP_VISIBILITY_DEFAULT
#endif

#define NP_EXPORT(__type) NP_VISIBILITY_DEFAULT __type
#endif

#if defined(XP_WIN)
	#ifdef BUILDING_DLL
		 #define NP_EXPORT(__type) __declspec(dllexport) __type
	#else
		// Temporarily commented because of VC++ compiler problems 
		 #define NP_EXPORT(__type) /* __declspec(dllimport) */ __type
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* plugin meta member functions */
NP_EXPORT(NPError)	Plugin_EntryPoints(NPNetscapeFuncs* NPN, NPPluginFuncs* NPP);

#ifdef __cplusplus
}
#endif

#endif /* _NPUPP_H_ */
