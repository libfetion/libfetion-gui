/**
* \file event.h
* \author dengdd <dedodong@163.com>
* \date 2009/2/11
* 
* This file includes fetion event definitions
* by libfetion.
*
\verbatim

Copyright (C) 2008 dengdd.

All rights reserved by dengdd.

This file is part of libfetion, a cross-operating-system library to operate China mobile's fetion IM.

\endverbatim
*/

#ifndef _LIBFETION_EVENT_H_
#define _LIBFETION_EVENT_H_

#ifdef __cplusplus 
extern "C" { 
#endif 

/**
 * \defgroup events LibFetion Event Define
 * @{
 * \sa	cb_func
 */


/**
* \def FX_EVENT_BASE
* \brief the fetion event base value
* 
*/
#define FX_EVENT_BASE   0x0000

/**
* \def FX_EVENT_SPARE
* \brief the spare fetion event base value
* 
*/
#define FX_EVENT_SPARE   0x00F0


	
/**
 * \defgroup login_event Login Event
 * @{
 */

/**
* \def FX_LOGIN_CONNECTING
* \brief login state is connecting service. 
* 
*/
#define FX_LOGIN_CONNECTING      FX_EVENT_BASE + 0x0001

/**
* \def FX_LOGIN_WAIT_AUTH
* \brief login state is waiting service auth the password and id.  
* 
*/
#define FX_LOGIN_WAIT_AUTH       FX_EVENT_BASE + 0x0002//等待认证

/**
* \def FX_LOGIN_AUTH_OK
* \brief login state is service auth ok.  
* 
*/
#define FX_LOGIN_AUTH_OK         FX_EVENT_BASE + 0x0003//认证成功

/**
* \def FX_LOGIN_FAIL
* \brief login state is login fail, password is invalidity.  
* 
*/
#define FX_LOGIN_FAIL            FX_EVENT_BASE + 0x0004//登陆失败

/**
* \def FX_LOGIN_NETWORK_ERROR
* \brief login state is login fail of network error.  
* 
*/
#define FX_LOGIN_NETWORK_ERROR   FX_EVENT_BASE + 0x0005//网络错误

/**
* \def FX_LOGIN_UNKOWN_ERROR
* \brief login state is login fail of unkown error.  
* 
*/
#define FX_LOGIN_UNKOWN_ERROR    FX_EVENT_BASE + 0x0006//未知错误

/**
* \def FX_LOGIN_UNKOWN_USR
* \brief login state is login fail of the user id is invalidity.  
* 
*/
#define FX_LOGIN_UNKOWN_USR      FX_EVENT_BASE + 0x0007//用户不存在  

/**
* \def FX_LOGIN_GCL_GETTING
* \brief login state is getting the connection list.  
* 
*/
#define FX_LOGIN_GCL_GETTING     FX_EVENT_BASE + 0x0008//正在获取联系人列表

/**
* \def FX_LOGIN_GCL_OK
* \brief login state is get the connection list ok .  
* 
*/
#define FX_LOGIN_GCL_OK          FX_EVENT_BASE + 0x0009//获取联系人列表成功

/**
* \def FX_LOGIN_GCL_FAIL
* \brief login state is login fail of  getting the connection list fail.  
* 
*/
#define FX_LOGIN_GCL_FAIL        FX_EVENT_BASE + 0x000A//获取联系人列表失败

/**
* \def FX_LOGIN_GP_GETTING
* \brief login state is getting the presence info.  
* 
*/
#define FX_LOGIN_GP_GETTING      FX_EVENT_BASE + 0x000B//正在获取联系人状态

/**
* \def FX_LOGIN_GP_OK
* \brief login state is getting the presence info ok .  
* 
*/
#define FX_LOGIN_GP_OK           FX_EVENT_BASE + 0x000C//获取联系人状态成功

/**
* \def FX_LOGIN_GP_FAIL
* \brief login state is login fail of getting the presence info fail.  
* 
*/
#define FX_LOGIN_GP_FAIL         FX_EVENT_BASE + 0x000D//获取联系人状态失败

/**
* \def FX_LOGIN_OK 
* \brief login state is login ok.  
* 
*/
#define FX_LOGIN_OK              FX_EVENT_BASE + 0x000E//登陆成功

/**
* \def FX_LOGIN_TIMEOUT
* \brief login state is login fail of login timeout.  
* 
*/
#define FX_LOGIN_TIMEOUT         FX_EVENT_BASE + 0x000F//登陆超时 ? need it.. have a UNKOWN_ERROR

/**
* \def FX_LOGIN_URI_ERROR
* \brief login state is login fail of get the user's uri fail.  
* 
*/
#define FX_LOGIN_URI_ERROR       FX_EVENT_SPARE  + 0x0001

/**
* \def FX_LOGIN_SERVER_ERROR
* \brief login state is login fail of your login error fetion server.  
* 
*/
#define FX_LOGIN_SERVER_ERROR       FX_EVENT_SPARE  + 0x0002

/**
* \def FX_LOGIN_SERVER_REFUSE
* \brief login state is fetion server refuse, you should relogin again.  
* 
*/
#define FX_LOGIN_SERVER_REFUSE       FX_EVENT_SPARE  + 0x0003

/**
* \def FX_LOGIN_GSML_OK
* \brief login state is fetion get schedule sms list ok  
* 
*/
#define FX_LOGIN_GSML_OK       FX_EVENT_SPARE  + 0x0004

/**
* \def FX_LOGIN_GSML_FAIL
* \brief login state is fetion get schedule sms list fail  
* 
*/
#define FX_LOGIN_GSML_FAIL       FX_EVENT_SPARE  + 0x0005

/** @} end of login_event */

	
/**
 * \defgroup recv_msg_event Receive Message Event
 * @{
 */


/**
* \def FX_NEW_MESSAGE
* \brief a new message received
*
* This message is posted when the client receive a new message..
 *\code
 * case FX_NEW_MESSAGE:
 * long who = (long)lParam;
 *
 * printf("the usr [%ld] send a message to you \n, ", who );
 *
 * \endcode
 *
 * \param lParam The message sender.
 * 
 * \sa fx_get_msg Fetion_MSG
*/
#define FX_NEW_MESSAGE            FX_EVENT_BASE + 0x0010

/**
* \def FX_NEW_QUN_MESSAGE
* \brief a new qun message received
*
* This message is posted when the client receive a new message..
 *\code
 * case FX_NEW_QUN_MESSAGE:
 * long qun_id = (long)lParam;
 *
 * printf("the qun [%ld] have a new message \n, ", qun_id );
 *
 * \endcode
 *
 * \param lParam The message sender.
* 
  * \sa fx_get_msg Fetion_MSG
*/
#define FX_NEW_QUN_MESSAGE            FX_EVENT_BASE + 0x0011

/**
* \def FX_SYS_MESSAGE
* \brief a system message received
*
* This message is posted when the client receive a new message..
 *\code
 * case FX_SYS_MESSAGE:
 * long sys_id = (long)lParam;
 *
 * \endcode
 *
 * \param lParam The message sender.
* 
  * \sa fx_get_msg Fetion_MSG
*/
#define FX_SYS_MESSAGE            FX_EVENT_BASE + 0x0012

/**
* \def FX_NUDGE_MESSAGE
* \brief a nudge message received
*
* This message is posted when the client receive a nudge message..
 *
 * \param lParam The message sender.
* 
*/
#define FX_NUDGE_MESSAGE           FX_EVENT_BASE + 0x0013
/** @} end of recv_msg_event */


/**
 * \defgroup sendsms_event Send SMS Event
 * @{
 */
/**
* \def FX_SMS_OK
* \brief send sms state is ok.  
* 
*/
#define FX_SMS_OK                FX_EVENT_BASE + 0x0020//发送短消息成功

/**
* \def FX_SMS_FAIL
* \brief send sms state is fail.  
* 
*/
#define FX_SMS_FAIL              FX_EVENT_BASE + 0x0021//发送短消息失败（服务器）

/**
* \def FX_SMS_TIMEOUT
* \brief send sms state is fail of time out.  
* 
*/
#define FX_SMS_TIMEOUT           FX_EVENT_BASE + 0x0022//发送短消息超时

/**
* \def FX_SMS_UNKOWN_ERROR
* \brief send sms state is fail of unkown error.  
* 
*/
#define FX_SMS_UNKOWN_ERROR      FX_EVENT_BASE + 0x0023//发送短消息未知错误. this message may be noever happened.

/**
* \def FX_SMS_FAIL_LIMIT
* \brief send sms state is fail of unkown error.  
* 
*/
#define  FX_SMS_FAIL_LIMIT      FX_EVENT_BASE + 0x0024//sms-daily-limit
/** @} end of sendsms_event */

/**
 * \defgroup dialog_event Dialog Event
 * @{
 */

/**
* \def FX_DIA_BG_TRYING
* \brief a dialog begin ok 
*
*/
#define FX_DIA_BG_TRYING         FX_EVENT_BASE + 0x0030//begin dialog trying.... 

/**
* \def FX_DIA_BG_OK
* \brief a dialog begin ok 
*
*/
#define FX_DIA_BG_OK             FX_EVENT_BASE + 0x0031//begin dialog ok

/**
* \def FX_DIA_BG_FAIL
* \brief a dialog begin fail 
*
*/
#define FX_DIA_BG_FAIL           FX_EVENT_BASE + 0x0032//begin dialog fail

/**
* \def FX_DIA_BG_TIMEOUT
* \brief a dialog begin timeout 
*
*/
#define FX_DIA_BG_TIMEOUT        FX_EVENT_BASE + 0x0033//begin dialog timeout


/**
* \def FX_DIA_SEND_OK
* \brief a dialog send message ok
*
*/
#define FX_DIA_SEND_OK           FX_EVENT_BASE + 0x0034//发送消息成功

/**
* \def FX_DIA_SEND_FAIL
* \brief a dialog send message fail
*
*/
#define FX_DIA_SEND_FAIL         FX_EVENT_BASE + 0x0035//发送消息失败


/**
* \def FX_DIA_SEND_TIMEOUT
* \brief a dialog send message timeout
*
*/
#define FX_DIA_SEND_TIMEOUT      FX_EVENT_BASE + 0x0036//发送消息失败


/**
* \def FX_DIA_UNKOWN_ERROR
* \brief a dialog unkown error message
*
*/
#define FX_DIA_UNKOWN_ERROR      FX_EVENT_BASE + 0x0037//发送消息未知错误. this message may be noever happened.

/** @} end of dialog_event */


/**
 * \defgroup online_event USER STATUS Event
 * @{
 */

/**
* \def FX_STATUS_UNSET
* \brief user status is unset.
*
*/
#define FX_STATUS_UNSET           FX_EVENT_BASE + 0x0040   //未设置

//#define FX_STATUS_OFFLINE         FX_EVENT_BASE + 0x0041   //离线

/**
* \def FX_STATUS_ONLINE
* \brief user status is online.
*
*/
#define FX_STATUS_ONLINE          FX_EVENT_BASE + 0x0042   //在线

/**
* \def FX_STATUS_BUSY
* \brief user status is busy.
*
*/
#define FX_STATUS_BUSY            FX_EVENT_BASE + 0x0043   //忙碌

/**
* \def FX_STATUS_OFFLINE
* \brief user status is offline.
*
*/
#define FX_STATUS_OFFLINE        FX_EVENT_BASE + 0x0044    //隐身  

/**
* \def FX_STATUS_AWAY
* \brief user status is away.
*
*/
#define FX_STATUS_AWAY            FX_EVENT_BASE + 0x0045   //马上回来

/**
* \def FX_STATUS_MEETING
* \brief user status is meeting.
*
*/
#define FX_STATUS_MEETING         FX_EVENT_BASE + 0x0046   //会议中

/**
* \def FX_STATUS_PHONE
* \brief user status is phone.
*
*/
#define FX_STATUS_PHONE           FX_EVENT_BASE + 0x0047   //电话中

/**
* \def FX_STATUS_DINNER
* \brief user status is dinner.
*
*/
#define FX_STATUS_DINNER          FX_EVENT_BASE + 0x0048   //外出用餐

/**
* \def FX_STATUS_EXTENDED_AWAY 
* \brief user status is extended away.
*
*/
#define FX_STATUS_EXTENDED_AWAY   FX_EVENT_BASE + 0x0049   //离开 

/**
* \def FX_STATUS_NUM_PRIMITIVES
* \brief user status is primitives num.
*
*/
#define FX_STATUS_NUM_PRIMITIVES  FX_EVENT_BASE + 0x004A   //自定义

/**
* \def FX_STATUS_WAITING_AUTH
* \brief waitting user to add you.
*
*/
#define FX_STATUS_WAITING_AUTH  FX_EVENT_BASE + 0x004B   

/**
* \def FX_STATUS_REFUSE
* \brief user status refuse to add you.
*
*/
#define FX_STATUS_REFUSE  FX_EVENT_BASE + 0x004C   

/**
* \def FX_STATUS_BLACK
* \brief this user is in Blacklist of you.
*
*/
#define FX_STATUS_BLACK  FX_EVENT_BASE + 0x004D   

/**
* \def FX_STATUS_MOBILE
* \brief it is mobile user.
*
*/
#define FX_STATUS_MOBILE  FX_EVENT_BASE + 0x004E

/**
* \def FX_STATUS_SMSEXTENED
* \brief the user of sms extend is chagend.
*
*/
#define FX_STATUS_SMSEXTENED  FX_EVENT_BASE + 0x004F
/** @} end of online_event */


/**
 * \defgroup system_misc_event Libfetion System Event
 * @{
 */
/**
* \def FX_ACCOUNT_UPDATA_OK
* \brief the updata account opt ok
*
* This message is posted when the updata account opt is ok..
 *\code
 * case FX_ACCOUNT_UPDATA_OK:
 * long who = (long)lParam;
 *
 * printf("the id of [%ld] user updata its info ok, you can the new info of it \n, ", who );
 *
 * \endcode
 *
 * \param lParam The updata info account 's uid.
*/
#define FX_ACCOUNT_UPDATA_OK        FX_EVENT_BASE + 0x0050   //更新用户信息OK

/**
* \def FX_ADD_GROUP_OK
* \brief fetion add group ok.
*
*/
#define FX_ADD_GROUP_OK             FX_EVENT_BASE + 0x0051   //增加用户组信息OK

/**
* \def FX_ADD_GROUP_FAIL
* \brief fetion add group fail.
*
*/
#define FX_ADD_GROUP_FAIL           FX_EVENT_BASE + 0x0052   //增加用户组信息FAIL

/**
* \def FX_ADD_GROUP_TIMEOUT
* \brief fetion add group fail of timeout.
*
*/
#define FX_ADD_GROUP_TIMEOUT        FX_EVENT_BASE + 0x0053   //增加用户组信息timeout

/**
* \def FX_DEL_GROUP_OK
* \brief fetion delete group ok.
*
*/
#define FX_DEL_GROUP_OK             FX_EVENT_BASE + 0x0054   //删除用户组信息OK

/**
* \def FX_DEL_GROUP_FAIL
* \brief fetion delete group fail.
*
*/
#define FX_DEL_GROUP_FAIL           FX_EVENT_BASE + 0x0055   //删除用户组信息FAIL

/**
* \def FX_DEL_GROUP_TIMEOUT
* \brief fetion delete group fail of timeout.
*
*/
#define FX_DEL_GROUP_TIMEOUT        FX_EVENT_BASE + 0x0056   //删除用户组信息timeout

/**
* \def FX_RENAME_GROUP_OK
* \brief fetion rename group ok.
*
*/
#define FX_RENAME_GROUP_OK          FX_EVENT_BASE + 0x0057   //重命名用户组OK

/**
* \def FX_RENAME_GROUP_FAIL
* \brief fetion rename group fail.
*
*/
#define FX_RENAME_GROUP_FAIL        FX_EVENT_BASE + 0x0058   //重命名用户组FAIL

/**
* \def FX_RENAME_GROUP_TIMEOUT
* \brief fetion rename group fail of timeout.
*
*/
#define FX_RENAME_GROUP_TIMEOUT     FX_EVENT_BASE + 0x0059   //重命名用户组timeout

/**
* \def FX_SET_BUDDY_INFO_OK
* \brief fetion set account loacl name ok.
*
*/
#define FX_SET_BUDDY_INFO_OK        FX_EVENT_BASE + 0x005A   //设置用户本地名称OK

/**
* \def FX_SET_BUDDY_INFO_FAIL
* \brief fetion set account loacl name fail.
*
*/
#define FX_SET_BUDDY_INFO_FAIL      FX_EVENT_BASE + 0x005B   //设置用户本地名称FAIL

/**
* \def FX_SET_BUDDY_INFO_TIMEOUT
* \brief fetion set account loacl name fail of timeout.
*
*/
#define FX_SET_BUDDY_INFO_TIMEOUT   FX_EVENT_BASE + 0x005C   //设置用户本地名称timeout

/**
* \def FX_ADDACCOUNT_APP
* \brief fetion have a addaccount request.
*
*/
#define FX_ADDACCOUNT_APP   FX_EVENT_BASE + 0x005D   //

/**
* \def FX_ADD_BUDDY_OK
* \brief fetion add account ok.
*
*/
#define FX_ADD_BUDDY_OK   FX_EVENT_BASE + 0x005E   //
	

/**
* \def FX_SYS_ERR_NETWORK
* \brief fetion syetem error of network, when usr receive this message, should quit fetion.
*
*/
#define FX_SYS_ERR_NETWORK             FX_EVENT_BASE + 0x0061   

/**
* \def FX_SYS_DEREGISTERED
* \brief fetion user had login in other pc, 
* and the server shutdown the connect, so we should relogin of this fetion when 
* receive this message.
*
*/
#define FX_SYS_DEREGISTERED      FX_EVENT_BASE + 0x0062   

/**
* \def FX_SET_STATE_OK
* \brief fetion set user state ok.
*
*/
#define FX_SET_STATE_OK   FX_EVENT_BASE + 0x0063  
/**
* \def FX_SET_STATE_FAIL
* \brief fetion set user state fail.
*
*/
#define FX_SET_STATE_FAIL   FX_EVENT_BASE + 0x0064  
/**
* \def FX_SET_STATE_TIMEOUT
* \brief fetion set user state timeout.
*
*/
#define FX_SET_STATE_TIMEOUT  FX_EVENT_BASE + 0x0065  

/**
* \def FX_SET_IMPRESA_OK
* \brief fetion set user impresa ok.
*
*/
#define FX_SET_IMPRESA_OK  FX_EVENT_BASE + 0x0066  

/**
* \def FX_SET_IMPRESA_FAIL
* \brief fetion set user impresa fail.
*
*/
#define FX_SET_IMPRESA_FAIL  FX_EVENT_BASE + 0x0067  

/**
* \def FX_SET_NICKNAME_OK
* \brief fetion set user nickname ok
*
*/
#define FX_SET_NICKNAME_OK  FX_EVENT_BASE + 0x0068  

/**
* \def FX_SET_NICKNAME_FAIL
* \brief fetion set user nickname fail
*
*/
#define FX_SET_NICKNAME_FAIL  FX_EVENT_BASE + 0x0069  

/**
* \def FX_SET_REFUSE_SMS_DAY_OK
* \brief fetion set user refuse sms day ok
*
*/
#define FX_SET_REFUSE_SMS_DAY_OK  FX_EVENT_BASE + 0x006A  

/**
* \def FX_SET_REFUSE_SMS_DAY_FAIL
* \brief fetion set user refuse sms day fail 
*
*/
#define FX_SET_REFUSE_SMS_DAY_FAIL  FX_EVENT_BASE + 0x006B  

/**
* \def FX_ADD_BLACKLIST_OK
* \brief fetion add user blacklist ok. 
*
* lParam is uid
*
*/
#define FX_ADD_BLACKLIST_OK  FX_EVENT_BASE + 0x0071  

/**
* \def FX_ADD_BLACKLIST_FAIL
* \brief fetion add user blacklist fail. 
*
* lParam is uid
*
*/
#define FX_ADD_BLACKLIST_FAIL  FX_EVENT_BASE + 0x0072  

/**
* \def FX_ADD_BLACKLIST_TIMEOUT
* \brief fetion add user blacklist timeout. 
*
*/
#define FX_ADD_BLACKLIST_TIMEOUT  FX_EVENT_BASE + 0x0073 

/**
* \def FX_REMOVE_BLACKLIST_OK
* \brief fetion remove user blacklist ok. 
*
* lParam is uid
*
*/
#define FX_REMOVE_BLACKLIST_OK  FX_EVENT_BASE + 0x0074  

/**
* \def FX_REMOVE_BLACKLIST_FAIL
* \brief fetion remove user blacklist fail. 
*
* lParam is uid
*
*/
#define FX_REMOVE_BLACKLIST_FAIL  FX_EVENT_BASE + 0x0075

/**
* \def FX_REMOVE_BLACKLIST_TIMEOUT
* \brief fetion remove user blacklist timeout. 
*
*/
#define FX_REMOVE_BLACKLIST_TIMEOUT  FX_EVENT_BASE + 0x0076 

/**
* \def FX_SET_SCH_SMS_FAIL
* \brief fetion set schedule sms fail. 
*
*/
#define	FX_SET_SCH_SMS_FAIL  FX_EVENT_BASE + 0x0077


/**
* \def FX_SET_SCH_SMS_OK
* \brief fetion set schedule sms ok. 
*
*/
#define	FX_SET_SCH_SMS_OK  FX_EVENT_BASE + 0x0078 

/**
* \def FX_SET_SCH_SMS_TIMEOUT
* \brief fetion set schedule sms timeout. 
*
*/
#define FX_SET_SCH_SMS_TIMEOUT  FX_EVENT_BASE + 0x0079

/**
* \def FX_DEL_SCH_SMS_FAIL
* \brief fetion delete schedule sms fail. 
*
*/
#define FX_DEL_SCH_SMS_FAIL  FX_EVENT_BASE + 0x007A

/**
* \def FX_DEL_SCH_SMS_OK
* \brief fetion delete schedule sms ok. 
*
*/
#define FX_DEL_SCH_SMS_OK  FX_EVENT_BASE + 0x007B

/**
* \def FX_DEL_SCH_SMS_TIMEOUT
* \brief fetion delete schedule sms timeout. 
*
*/
#define FX_DEL_SCH_SMS_TIMEOUT  FX_EVENT_BASE + 0x007C

/**
* \def FX_DEL_BUDDY_OK
* \brief fetion delete friend ok. 
*
* lParam is uid
*
*/
#define FX_DEL_BUDDY_OK FX_EVENT_BASE + 0x0081  

/**
* \def FX_DEL_BUDDY_FAIL
* \brief fetion delete friend fail. 
*
* lParam is uid
*
*/
#define FX_DEL_BUDDY_FAIL  FX_EVENT_BASE + 0x0082

/**
* \def FX_DEL_BUDDY_TIMEOUT
* \brief fetion delete friend timeout. 
*
*/
#define FX_DEL_BUDDY_TIMEOUT  FX_EVENT_BASE + 0x0083 

/**
* \def FX_MOVE_GROUP_OK
* \brief fetion move the account 's group opt ok. 
*
*/
#define FX_MOVE_GROUP_OK  FX_EVENT_BASE + 0x0084 

/**
* \def FX_MOVE_GROUP_FAIL
* \brief fetion move the account 's group opt fail. 
*
*/
#define FX_MOVE_GROUP_FAIL  FX_EVENT_BASE + 0x0085 

/**
* \def FX_RECEIVE_FILE
* \brief fetion receive a file message. 
*
* wParam is F_FILE_INFO 
* lParam is uid
*/
#define FX_RECEIVE_FILE  FX_EVENT_BASE + 0x0087 

/**
* \def FX_CURRENT_VERSION
* \brief return the current version of libfetion(this message is just using in GUI Application) . 
*
* \param wParam The current version value.
*/ 
#define FX_CURRENT_VERSION  FX_EVENT_BASE + 0x0088 


/** @} end of system_misc_event */

/**
 * \defgroup qun_event Libfetion Qun Event
 * @{
 */
/**
* \def FX_GET_QUNLIST_OK
* \brief fetion get qun list ok. 
*
*/
#define FX_GET_QUNLIST_OK  FX_EVENT_BASE + 0x0091

/**
* \def FX_GET_QUNLIST_FAIL
* \brief fetion get qun list fail. 
*
*/
#define FX_GET_QUNLIST_FAIL  FX_EVENT_BASE + 0x0092

/**
* \def FX_QUN_GP_OK
* \brief fetion sub qun ok. 
*
*/
#define FX_QUN_GP_OK  FX_EVENT_BASE + 0x0093

/**
* \def FX_QUN_SEND_OK
* \brief fetion send qun im message ok. 
*
*/
#define FX_QUN_SEND_OK  FX_EVENT_BASE + 0x0094

/**
* \def FX_QUN_SEND_TIMEOUT
* \brief fetion send qun im message timeout. 
*
*/
#define FX_QUN_SEND_TIMEOUT  FX_EVENT_BASE + 0x0095

/**
* \def FX_QUN_SEND_FAIL
* \brief fetion send qun im message fail. 
*
*/
#define FX_QUN_SEND_FAIL  FX_EVENT_BASE + 0x0096

/**
* \def FX_QUN_SMS_OK
* \brief fetion send qun sms message ok. 
*
*/
#define FX_QUN_SMS_OK  FX_EVENT_BASE + 0x0097

/**
* \def FX_QUN_SMS_TIMEOUT
* \brief fetion send qun sms message timeout. 
*
*/
#define FX_QUN_SMS_TIMEOUT  FX_EVENT_BASE + 0x0098

/**
* \def FX_QUN_SMS_FAIL
* \brief fetion send qun sms message fail. 
*
*/
#define FX_QUN_SMS_FAIL  FX_EVENT_BASE + 0x0099

/**
* \def FX_QUN_SMS_FAIL_LIMIT
* \brief fetion send qun sms message fail by daily limit . 
*
*/
#define FX_QUN_SMS_FAIL_LIMIT  FX_EVENT_BASE + 0x009A
/** @} end of qun_event */


/**
 * \defgroup net_test_event Libfetion Qun Event
 * @{
 */
/**
* \def FX_GET_QUNLIST_OK
* \brief fetion get qun list ok. 
*
*/
#define FX_NET_OK  FX_EVENT_BASE + 0x00A1

/** @} end of net_test_event */

/** @} end of events */

#ifdef __cplusplus 
} 
#endif 

#endif /* _LIBFETION_EVENT_H_ */
