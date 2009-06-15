/*
 * npunix.c
 *
 * Netscape Client Plugin API
 * - Wrapper function to interface with the minigui mdolphin
 *
 * dengdd <dengdd@minigui.org>
 *
 *----------------------------------------------------------------------
 * PLUGIN DEVELOPERS:
 *  YOU WILL NOT NEED TO EDIT THIS FILE.
 *----------------------------------------------------------------------
 */

#include <stdio.h>
#include "npapi.h"
#include "npupp.h"

/*
 * Define PLUGIN_TRACE to have the wrapper functions print
 * messages to stderr whenever they are called.
 */

#define PLUGIN_TRACE
#undef PLUGIN_TRACE

#ifdef PLUGIN_TRACE
#define PLUGINDEBUGSTR(msg) fprintf(stderr, "%s\n", msg)
#else
#define PLUGINDEBUGSTR(msg)
#endif


/***********************************************************************
 *
 * Globals
 *
 ***********************************************************************/

NPNetscapeFuncs   gNetscapeFuncs;    /* Netscape Function table */

/***********************************************************************
 *
 * Wrapper functions : plugin calling Netscape Navigator
 *
 * These functions let the plugin developer just call the APIs
 * as documented and defined in npapi.h, without needing to know
 * about the function table and call macros in npupp.h.
 *
 ***********************************************************************/

void
NPN_Version(int* plugin_major, int* plugin_minor,
         int* netscape_major, int* netscape_minor)
{
    *plugin_major = NP_VERSION_MAJOR;
    *plugin_minor = NP_VERSION_MINOR;

    /* Major version is in high byte */
    *netscape_major = gNetscapeFuncs.version >> 8;
    /* Minor version is in low byte */
    *netscape_minor = gNetscapeFuncs.version & 0xFF;
}

NPError
NPN_GetValue(NPNVariable variable, void *r_value)
{
    return CallNPN_GetValueProc(gNetscapeFuncs.getvalue,
								variable, r_value);
}

void
NPN_PluginThreadAsyncCall(void (*func) (void *), void *userData)
{
	CallNPN_PluginThreadAsyncCallProc(gNetscapeFuncs.asyncCall, func, userData);
}

void*
NPN_GetUIFuncsTable(void)
{
    return CallNPN_GetUIFuncsTableProc(gNetscapeFuncs.uitable);
}

void*
NPN_GetLibFetionFuncsTable(void)
{
	return CallNPN_GetLibFetionFuncsTableProc(gNetscapeFuncs.libfxtable);
}

/*********************************************************************** 
 *
 * These functions are located automagically by netscape.
 *
 ***********************************************************************/
/*
 * NP_Initialize [optional]
 *  - Netscape needs to know about this symbol.
 *  - It calls this function after looking up its symbol after
 *    the last object of this kind has been destroyed.
 *
 */
NPError
NP_Initialize(void)
{
	PLUGINDEBUGSTR("NP_Initialize");
	return NPP_Initialize();
}
/*
 * NP_Shutdown [optional]
 *  - Netscape needs to know about this symbol.
 *  - It calls this function after looking up its symbol after
 *    the last object of this kind has been destroyed.
 *
 */
NPError
NP_Shutdown(void)
{
    PLUGINDEBUGSTR("NP_Shutdown");
    NPP_Shutdown();
    return NPERR_NO_ERROR;
}

/*
 * NP_HandleEvent
 *  - Netscape needs to know about this symbol
 *  - Netscape uses the return value 
 *   
 */
int16
NP_HandleEvent(NPEvent* event)
{
	return NPP_HandleEvent(event);
}

/*
 * NP_GetPluginName
 *  - Netscape needs to know about this symbol
 *  - Netscape uses the return value to identify when an object instance
 *    of this plugin should be created.
 */
char *
NP_GetPluginName(void)
{
    return NPP_GetPluginName();
}

/*
 * NP_GetPluginDescription
 *  - Netscape needs to know about this symbol
 *  - Netscape uses the return value to introduce this plugin for the client user
 */
char *
NP_GetPluginDescription(void)
{
    return NPP_GetPluginDescription();
}

void
NP_PluginThreadAsyncExtendCall(void)
{
	return NPP_PluginThreadAsyncExtendCall(); 
}

/*
 * NP_SwapInterFaces
 *  - Netscape needs to know about this symbol.
 *  - It calls this function after looking up its symbol before it
 *    is about to create the first ever object of this kind.
 *
 * PARAMETERS
 *    nsTable   - The netscape function table. If developers just use these
 *        wrappers, they don't need to worry about all these function
 *        tables.
 * RETURN
 *    pluginFuncs
 *      - This functions needs to fill the plugin function table
 *        pluginFuncs and return it. Netscape Navigator plugin
 *        library will use this function table to call the plugin.
 *
 */
NPError
NP_SwapInterFaces(NPNetscapeFuncs* nsTable, NPPluginFuncs* pluginFuncs)
{
    NPError err = NPERR_NO_ERROR;

    PLUGINDEBUGSTR("NP_SwapInterFaces");
    
    /* validate input parameters */

    if ((nsTable == NULL) || (pluginFuncs == NULL))
        err = NPERR_INVALID_FUNCTABLE_ERROR;
    
    /*
     * Check the major version passed in Netscape's function table.
     * We won't load if the major version is newer than what we expect.
     * Also check that the function tables passed in are big enough for
     * all the functions we need (they could be bigger, if Netscape added
     * new APIs, but that's OK with us -- we'll just ignore them).
     *
     */
    if (err == NPERR_NO_ERROR) {
        if ((nsTable->version >> 8) > NP_VERSION_MAJOR)
            err = NPERR_INCOMPATIBLE_VERSION_ERROR;
        if (nsTable->size < ((void *)&nsTable->libfxtable - (void *)nsTable))
            err = NPERR_INVALID_FUNCTABLE_ERROR;
        if (pluginFuncs->size < sizeof(NPPluginFuncs))      
            err = NPERR_INVALID_FUNCTABLE_ERROR;
    }
    if (err == NPERR_NO_ERROR) {
        /*
         * Copy all the fields of Netscape function table into our
         * copy so we can call back into Netscape later.  Note that
         * we need to copy the fields one by one, rather than assigning
         * the whole structure, because the Netscape function table
         * could actually be bigger than what we expect.
         */
        gNetscapeFuncs.version       = nsTable->version;
        gNetscapeFuncs.size          = nsTable->size;
        gNetscapeFuncs.getvalue      = nsTable->getvalue;
        gNetscapeFuncs.asyncCall     = nsTable->asyncCall;
        gNetscapeFuncs.uitable		 = nsTable->uitable;
        gNetscapeFuncs.libfxtable    = nsTable->libfxtable;
        /*
         * Set up the plugin function table that Netscape will use to
         * call us.  Netscape needs to know about our version and size
         * and have a UniversalProcPointer for every function we
         * implement.
         */
        pluginFuncs->version = (NP_VERSION_MAJOR << 8) + NP_VERSION_MINOR;
        pluginFuncs->size    = sizeof(NPPluginFuncs);
        pluginFuncs->init    = NewNPP_InitializeProc(NP_Initialize);
        pluginFuncs->shut    = NewNPP_ShutdownProc(NP_Shutdown);
        pluginFuncs->name    = NewNPP_GetPluginNameProc(NP_GetPluginName);
        pluginFuncs->desc    = NewNPP_GetPluginDescriptionProc(NP_GetPluginDescription);
        pluginFuncs->event   = NewNPP_HandleEventProc(NP_HandleEvent);
        pluginFuncs->excall  = NewNPP_PluginThreadAsyncExtendCallProc(NP_PluginThreadAsyncExtendCall);
//        err = NPP_Initialize();
    }
    
    return err;
}

NPError	Plugin_EntryPoints(NPNetscapeFuncs* NPN, NPPluginFuncs* NPP)
{
	return NP_SwapInterFaces(NPN, NPP);
}
