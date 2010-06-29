#include <stdio.h>
#include "npapi.h"
#include "npupp.h"
//#include "plugin.h"

#define PLUGIN_NAME "your plugin's name"

#define PLUGIN_DESCRIPTION PLUGIN_NAME"plugin's description"

/*----------------------------------------------------------------------
 * PLUGIN DEVELOPERS:
 *    YOU WILL NOT NEED TO EDIT THIS REGION.
 *----------------------------------------------------------------------
 * ----------------------------------------------------------------------
*/
//extern NPNetscapeFuncs   gNetscapeFuncs; 

char* NPP_GetPluginName(void)
{
	return (char *)(PLUGIN_NAME); 
}

char* NPP_GetPluginDescription(void)
{
	return (char *)(PLUGIN_DESCRIPTION);
}
/* ---------------------------------------------------------------------
 * --------------- END OF NO EDIT REGION. ------------------------------
 * ---------------------------------------------------------------------
 */


/*----------------------------------------------------------------------
 * ----------------------------------------------------------------------
*/
int main()
{
	printf("hello plugin \n");
	printf("hello sdk \n");
	return 0;
}

NPError	NPP_Initialize(void)
{
	return NPERR_NO_ERROR;
}

void NPP_Shutdown(void)
{

}

int16 NPP_HandleEvent(NPEvent* event)
{
	return NPERR_NO_ERROR;
}

void NPP_PluginThreadAsyncExtendCall()
{

}
