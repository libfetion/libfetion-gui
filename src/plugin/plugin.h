#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#ifdef __cplusplus
extern "C" {
#endif
NPError	NPP_Initialize(void);
void	NPP_Shutdown(void);
int16	NPP_HandleEvent(NPEvent* event);
void    NPP_PluginThreadAsyncExtendCall(void);
#ifdef __cplusplus
}
#endif

#endif // end of __PLUGIN_H__

