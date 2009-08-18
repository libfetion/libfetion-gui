/***************************************************************************
 *   Copyright (C) 2009 by DDD                                             *
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
#ifndef FX_EventHandle_DATA
#define FX_EventHandle_DATA

void Sys_EventListener(int message, WPARAM wParam, LPARAM lParam, void
                       *args)
{
    if (!args)
    {
        return ;
    }
    FxMainWindow *mainWind = (FxMainWindow*)args;
    mainWind->handleFx_Sys_Event(message, wParam, lParam);
}

void FxMainWindow::handleFx_Sys_Event(int message, WPARAM wParam, LPARAM
    lParam)
{
    switch (message)
    {
        case FX_CURRENT_VERSION:
            emit signal_Current_Version((int)wParam);
            break;
        case FX_ADDACCOUNT_APP:
            emit signal_AddAccountApp((char*)(lParam), (char*)wParam);
            break;
        case FX_MOVE_GROUP_OK:
            emit signal_MoveGroup(qlonglong(lParam), (int)wParam);
            break;
        case FX_MOVE_GROUP_FAIL:
            break;
        case FX_SYS_MESSAGE:
            emit signal_NewSysMsg(qlonglong(lParam));
            break;
        case FX_NEW_MESSAGE:
            emit signal_NewMsg(qlonglong(lParam));
            break;
        case FX_NEW_QUN_MESSAGE:
            emit signal_NewQunMsg(qlonglong(lParam));
            break;
        case FX_SET_STATE_OK:
            emit signal_set_state((int)wParam);
            break;
        case FX_STATUS_OFFLINE:
            //²»ÔÚÏß
        case FX_STATUS_ONLINE:
            //ÔÚÏß
        case FX_STATUS_BUSY:
            //Ã¦Âµ
        case FX_STATUS_AWAY:
            //ÂíÉÏ»ØÀ´
        case FX_STATUS_MEETING:
            //»áÒéÖÐ
        case FX_STATUS_PHONE:
            //µç»°ÖÐ
        case FX_STATUS_DINNER:
            //Íâ³öÓÃ²Í
        case FX_STATUS_EXTENDED_AWAY:
            //Àë¿ª
        case FX_STATUS_NUM_PRIMITIVES:
            //×Ô¶¨Òå
        case FX_ACCOUNT_UPDATA_OK:
            emit signal_UpdateAcInfo(qlonglong(lParam));
            break;

        case FX_STATUS_SMSEXTENED:
            //printf("have receive the FX_STATUS_SMSEXTENED message of %d %ld\n", wParam, lParam);
            //emit signal_UpdateAcInfo(qlonglong(lParam));
            break;

        case FX_SET_REFUSE_SMS_DAY_OK:
            emit signal_UpdateSmsDay((wParam));
            break;

        case FX_REMOVE_BLACKLIST_OK:
        case FX_ADD_BLACKLIST_OK:
            emit signal_UpdateAcInfo(qlonglong(lParam));
            break;

        case FX_SYS_ERR_NETWORK:
            emit signal_SystemNetErr(wParam);
            break;

        case FX_SYS_DEREGISTERED:
            emit signal_DeRegistered();
            break;

        case FX_DIA_SEND_OK:
        case FX_DIA_SEND_FAIL:
        case FX_DIA_SEND_TIMEOUT:
        case FX_SMS_OK:
        case FX_SMS_FAIL:
        case FX_SMS_FAIL_LIMIT:
        case FX_SMS_TIMEOUT:

        case FX_QUN_SEND_OK:
        case FX_QUN_SEND_FAIL:
        case FX_QUN_SEND_TIMEOUT:
        case FX_QUN_SMS_OK:
        case FX_QUN_SMS_FAIL:
        case FX_QUN_SMS_FAIL_LIMIT:
        case FX_QUN_SMS_TIMEOUT:

            emit signal_SysDialogMsg(message, wParam, (qlonglong)lParam);
            break;

        case FX_ADD_BUDDY_OK:
            emit signal_add_buddy(message, wParam, (qlonglong)lParam);
            break;

        case FX_RENAME_GROUP_OK:
            emit signal_reName_group(message, wParam, (qlonglong)lParam);
            break;
        case FX_SET_BUDDY_INFO_OK:
            emit signal_reName_buddy(message, wParam, (qlonglong)lParam);
            break;
        case FX_ADD_GROUP_OK:
            emit signal_add_group(message, wParam, (qlonglong)lParam);
            break;
        case FX_DEL_GROUP_OK:
            emit signal_del_group(message, wParam, (qlonglong)lParam);
            break;
        case FX_DEL_BUDDY_OK:
            emit signal_del_buddy(message, wParam, (qlonglong)lParam);
            break;

        case FX_ADD_GROUP_FAIL:
        case FX_DEL_GROUP_FAIL:
        case FX_SET_BUDDY_INFO_FAIL:
        case FX_RENAME_GROUP_FAIL:
            //ignored this message
            if (wParam)
            {
                free((char*)(int)wParam);
            }
            break;

        case FX_SET_NICKNAME_OK:
            emit signal_set_nickname_ok();
            break;

        case FX_NUDGE_MESSAGE:
            emit signal_receive_nudge((qlonglong)lParam);
            break;


        case FX_SET_SCH_SMS_OK:
        case FX_DEL_SCH_SMS_OK:
        case FX_SET_SCH_SMS_FAIL:
        case FX_DEL_SCH_SMS_FAIL:
            //fix me: need to do some details handle for scheduleSms
            emit signal_update_scheduleList();
            break;

        default:
            break;

            #if 0 //follow message is ignored
            case FX_REMOVE_BLACKLIST_FAIL:
            case FX_ADD_BLACKLIST_FAIL:
            case FX_REMOVE_BLACKLIST_TIMEOUT:
            case FX_ADD_BLACKLIST_TIMEOUT:
            case FX_RENAME_GROUP_TIMEOUT:
            case FX_SET_BUDDY_INFO_TIMEOUT:
            case FX_SET_REFUSE_SMS_DAY_FAIL:
            #endif
    }

}

#endif
