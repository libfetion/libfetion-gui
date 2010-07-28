/**
 * \file libfetion.h
 * \author dengdd <dedodong@163.com>
 * \date 2009/2/11
 * 
 * This file includes necessary interfaces of libfetion.
 *
 \verbatim

    Copyright (C) 2008 dengdd.

    All rights reserved by dengdd.

    This file is part of libfetion, a cross-operating-system library to operate China mobile's fetion IM.

 \endverbatim
 */

#ifndef _LIB_FETION_H
#define _LIB_FETION_H

#ifdef __LIBFETION_LIB__
#include "common.h"
#include "event.h"
#else
#include "libfetion/common.h"
#include "libfetion/event.h"
#include "libfetion/datastruct.h"
#endif

#ifdef __cplusplus 
extern "C" {
#endif

/**
 * \defgroup libfetion  LibFetion Functions
 * @{
 */

	/**
	 * \defgroup init_libfetion  LibFetion Init
	 * @{
	 */
	/** @} end of init_libfetion */

	/**
	 * \defgroup fetion_login  Login and Out
	 * @{
	 */
		/**
		 * \defgroup  simple_fetion_login  Simple Login function
		 * @{
		*/
		/** @} end of simple_fetion_login*/
		/**
		* \defgroup  generic_fetion_login_functions  Generic Login function
		* @{
		*/
		/** @} end of generic_fetion_login_functions */
	/** @} end of fetion_login */

	/**
	 * \defgroup  system_msg_cb  Set System Message CallBack Functions
	 * @{
	 */
	/** @} end of system_msg_cb */

	/**
	 * \defgroup fetion_receive  Receive message Functions
	 * @{
	 */
	/** @} end of fetion_receive */

	/**
	 * \defgroup fetion_send_sms  Send SMS Functions
	 * @{
	 */
		/**
		* \defgroup simple_send_sms_functions  Simple SendSms functions
		* @{
		*/
		/** @} end of simple_send_sms_functions */

		/**
		 * \defgroup generic_send_sms_functions  Generic SendSMS functions
		 * @{
		*/
		/** @} end of generic_send_sms_functions */
	/** @} end of fetion_send_sms */

	/**
	 * \defgroup send_dialog_fetion_functions  Dialog Message 
	 * @{
	 */

		/**
		 * \defgroup simple_send_dialog_fetion_functions  Simple Dialog Message 
		 * @{
		*/
		/** @} end of simple_send_dialog_fetion_functions */

		/**
		* \defgroup generic_send_dialog_fetion_functions  Generic Dialog Message 
		* @{
		*/
		/** @} end of generic_send_dialog_fetion_functions */
	/** @} end of send_dialog_fetion_functions */

	/**
	 * \defgroup schedule_sms_fetion_functions Schedule SMS Function 
	 * @{
	 */
	/** @} end of schedule_sms_fetion_functions */

	/**
	 * \defgroup get_or_set_user_info  Get Or Set User Info
	 * @{
	 */
	/** @} end of get_or_set_user_info */

	/**
	 * \defgroup get_contact_info  Get Contacts info
	 * @{
	 */
	/** @} end of get_contact_info */

	/**
	 * \defgroup fetion_group_opt  Group Opt Functions
	 * @{
	 */

		/**
		 * \defgroup simple_fetion_group_opt  Simple Group Opt Functions
		 * @{
		 */
		/** @} end of simple_fetion_group_opt */

		/**
		 * \defgroup generic_fetion_group_opt  Generic Group Opt Functions
		 * @{
		 */
		/** @} end of generic_fetion_group_opt */

	/** @} end of fetion_group_opt */

	/**
	 * \defgroup fetion_buddy_opt  Buddy Opt Functions
	 * @{
	 */

		/**
		 * \defgroup simple_fetion_buddy_opt  Simple Buddy Opt Functions
		 * @{
		 */
		/** @} end of simple_fetion_buddy_opt */

		/**
		* \defgroup generic_fetion_buddy_opt  Generic Buddy Opt Functions
		* @{
		*/
		/** @} end of generic_fetion_buddy_opt */

	/** @} end of fetion_buddy_opt */

	/**
	 * \defgroup fetion_ext  Extend Functions
	 * @{
	 */
	/** @} end of fetion_ext */


	/**
	 * \defgroup fetion_misc  Misc Functions
	 * @{
	 */
	/** @} end of fetion_misc */


/** @} end of libfetion */

/**
 * \addtogroup init_libfetion
 * @{
 */
/**
  * \fn BOOL fx_init(void)
  * \brief if you want to use the libfetion, you must invoke this functions at first.
  *
  * This function will init some resources which the libfetion needed.
  *
  * \return TRUE if init successfully, otherwise return FALSE.
  *
  * \sa fx_shutdown
*/
FX_EXPORT BOOL fx_init(void);

/**
  * \fn void fx_terminate(void)
  * \brief shutdown the libfetion, when libfetion isn't used any more.
  *
  * This function will release the resources which the libfetion used.
  *
  * \sa fx_init
*/
FX_EXPORT void fx_terminate(void);

/**
  * \fn BOOL fx_close_network()
  * \brief shutdown the connect to fetion's servers.
  *
  * this function will close the fetion's socket from fetion's servers, if you will not use fetion anymore
  * \return TRUE if init successfully, otherwise return FALSE.
*/
FX_EXPORT BOOL fx_close_network();

/** @} end of init_libfetion */



/**
 * \addtogroup fetion_login  
 * @{
 */

/**
 * \addtogroup  generic_fetion_login_functions  
 * @{
 */

/**
  * \fn void fx_set_login_status(int longin_status)
  * \brief set the user login status.
  *
  * This function will set the user login status. 
  *
  * \param login_status The login status of you want to set.
  */
FX_EXPORT void fx_set_login_status(int longin_status);

/**
  * \fn void fx_cancel_login()
  * \brief cancel login fetion.
  *
  * This function will cancel login, don't use it after logined fetion!!!
  * because it's not tested for that case.
  *
  *
  * \sa fx_login
  */
FX_EXPORT void fx_cancel_login();


/**
  * \fn void fx_login(const char *uid, const char *password, EventListener func, void *args)
  * \brief login fetion.
  *
  * This function contain all the fetion login operates. 
  * and without callback function, when it return show login is finished.  
  * this function implement login,  get contact list and get presence operates.
  *
  * \param uid The login info of the usr id or mobile_no.
  * \param password The login info of the usr password.
  * \param status_login The place which store the error info.
  *
  * \sa EventListener 
  *
  */
FX_EXPORT int fx_login(const char *uid, const char *password, EventListener func, void *args);

/**
 * \fn void fx_relogin()
 * \brief relogin fetion when fetion have break from server..
  *
*/
FX_EXPORT int fx_relogin(EventListener func, void *args, const char* uid, const char* passwd);

/**
  * \fn void fx_loginout()
  * \brief loginout fetion.
  *
  * This function will done all loginout fetion operates, when you want to loginout fetion. 
  *
  * \sa fx_login
*/
FX_EXPORT void fx_loginout();

/**
  * \fn Fetion_Verfy *fx_get_auth_code()
  * \brief get the verfy info.
  *
  * This function will get all the verfy info. 
  *
  * \param guid The guid of the wanted Fetion_Verfy.
  *
  * \sa Fetion_Verfy
  * \sa fx_set_auth_code
*/
FX_EXPORT Fetion_Verfy *fx_get_auth_code(char* guid);

/**
  * \fn int fx_set_auth_code(const char* authcode)
  * \brief set the auth code to libfetion.
  *
  * This function will set the auth code to libfetion. 
  * \param authcode The auth code valut of setting.
  * \param verfy The opted Fetion_Verfy.
  *
  * \sa Fetion_Verfy
  * \sa fx_set_auth_code
*/
FX_EXPORT int fx_set_auth_code(const char* authcode, Fetion_Verfy *verfy);


#if 0
/**
  * \fn int fs_keep_alive()
  * \brief send a keep alive package. 
  *
  * This function will send a keep alive package to fetion server to keep alive int the expires time.
  * ****this function will be removed in later version****, because it can done by libfetion.
  *
  * \sa fetion_data_get_expirestime
  *
  * \return non zero on successfully, otherwise return zero.
*/
FX_EXPORT int fs_keep_alive();
#endif
/** @} end of generic_fetion_login_functions */

/** @} end of fetion_login */

/**
 * \addtogroup  system_msg_cb  
 * @{
 */

/**
  * \fn void fx_set_system_msg_cb(EventListener func, void* args)
  * \brief set the system message callback funtion. 
  *
  * This function will set the fuction address which is be sended  all fetion system messages to usr.
  * like ....
  *
  * \sa ...
*/
FX_EXPORT void fx_set_system_msg_cb(EventListener func, void* args);

/** @} end of system_msg_cb */

/**
* \addtogroup fetion_receive  
* @{
*/

/**
  * \fn Fetion_MSG *fx_get_msg(long uid)
  * \brief get the uid 's message. 
  *
  * \param uid The message 's sender. 
  *
  * you should usr the fx_destroy_msg function to destroy the msg which you get
  *
  * \return NULL if the uid no send message, or Fetion_MSG a struct which store message info.
  *
  *
  * \sa FX_NEW_MESSAGE fx_destroy_msg
*/
FX_EXPORT Fetion_MSG *fx_get_msg(long uid);

/**
  * \fn void fx_destory_msg(Fetion_MSG *msg)
  * \brief destroy the fetion message, it will free msg's memory, and set msg = NULL. 
  *
  * \param msg The  fetion message will be destroy. 
  *
  *
  * \sa fx_get_msg
*/
FX_EXPORT void fx_destroy_msg(Fetion_MSG *msg);

/** @} end of fetion_receive */

/**
 * \addtogroup fetion_send_sms  
 * @{
 */

/**
 * \addtogroup simple_send_sms_functions  
 * @{
 */

/**
  * \fn BOOL fs_send_sms(long who, const char *message)
  * \brief simple send smsfunctions
  *
  * \param who The user uid  of you want to send.
  * \param message The content of you want to send.
  *
  * \return TRUE if init successfully, otherwise return FALSE..
  *
  * \sa fx_send_sms
*/
FX_EXPORT BOOL fs_send_sms(const long who, const char *message);


/**
  * \fn BOOL BOOL fs_send_sms_by_mobile_no(const char *mobile_no, const char *message)
  * \brief simple send smsfunctions
  *
  * \param mobile_no The mobile number of you want to send.
  * \param message The content which you want to send.
  *
  * \return TRUE if init successfully, otherwise return FALSE..
  *
  * \sa fx_send_sms_by_mobile_no
*/
FX_EXPORT BOOL fs_send_sms_by_mobile_no(const char *mobile_no, const char *message);


/**
  * \fn BOOL fs_send_sms_to_self(const char *message)
  * \brief send a shortmessage to user self 
  *
  * \param message The content which you want to send.
  *
  * \return TRUE if init successfully, otherwise return FALSE..
  *
  * \sa fx_send_sms_to_self
*/
FX_EXPORT BOOL fs_send_sms_to_self(const char *message);

/** @} end of simple_send_sms_functions */

/**
 * \addtogroup generic_send_sms_functions  
 * @{
 */

/**
  * \fn int fx_send_sms(const long who, const char *message, EventListener func,void *args)
  * \brief send sms. 
  *
  * \param who The user uid  which you want to send.
  *
  * \param message The content which you want to send.
  *
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \sa EventListener
  *
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if send sms fail immediately, or can get the result from to callback_send_sms func.
  *
  *
  * \sa fs_send_sms
*/
FX_EXPORT int fx_send_sms(const long who, const char *message, EventListener func,void *args);


/**
  * \fn int fx_send_sms_to_self(const char *message, EventListener func,void *args)
  * \brief send sms to self.
  *
  * \param message The content which you want to send.
  *
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \sa EventListener
  *
  * \param args The send_sms operate's callback function's args. 
  * 
  * \return 0 if send sms fail immediately, or can get the result from to callback_send_sms func.
  *
  *
  * \sa fs_send_sms_to_self
*/
FX_EXPORT int fx_send_sms_to_self(const char *message, EventListener func,void *args);


/**
  * \fn int fx_send_sms_by_mobile_no(const char *mobile_no, const char *message, EventListener func, void *args)
  * \brief send sms by mobile numbers. 
  *
  * \param mobile_no The mobile number which you want to send.
  * \param message The content which you want to send.
  *
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \sa EventListener
  *
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if send sms fail immediately, or can get the result from to callback_send_sms func.
  *
  *
  * \sa fs_send_sms_by_mobile_no
*/
FX_EXPORT int fx_send_sms_by_mobile_no(const char *mobile_no, const char *message, EventListener func, void *args);

/**
  * \fn void fx_set_longsms(BOOL bl)
  * \brief set the send sms mode, it maybe longsms or sms normally. 
  *
  *  longsms mode: it will send 180 characters in one short message,
  *  sms normally mode: it will send 70 characters in one short message,
  *
  * \param bl The TRUE is set longsms mode, FALSE set normally mode .
*/
FX_EXPORT void fx_set_longsms(BOOL bl);

/** @} end of generic_send_sms_functions */

/** @} end of fetion_send_sms */

/**
 * \addtogroup send_dialog_fetion_functions  
 * @{
 */

/**
 * \addtogroup simple_send_dialog_fetion_functions  
 * @{
 */

/**
  * \fn BOOL fs_begin_dialog(long who)
  * \brief begin the dialog function
  *
  * if you want to have a dialog with somebody. invoke this function first,
  * of course, if you not invoke this function to send a dialog, it will invoked by itself.
  * if you will not have a dialog with somebody, you should invoke fetion_end_dialog
  * advise: invoke this function before send a dialog to somebody,
  * \sa fetion_end_dialog
  *
  * \param who The somebody who you want to have a dialog
  *
  * \return non zero if init successfully, otherwise if fail return 0.( this function will not return 0)
  *
*/
FX_EXPORT BOOL fs_begin_dialog(long who);


/**
  * \fn BOOL fs_dialog_send(long who, const char *message)
  * \brief send a message to somebody in a dialog 
  *
  * \param who The somebody who you want to send
  * \param message The content which you want to send.
  *
  * \return TRUE if init successfully, otherwise return FALSE..
*/
FX_EXPORT BOOL fs_dialog_send(long who, const char *message);

/**
  * \fn void fs_end_dialog(long who)
  * \brief end a dialog with somebody
  *
  * if you will not have a dialog with somebody, you should invoke this function
  *
  * \param who The somebody who you want to end a dialog
  *
*/
void fs_end_dialog(long who);

/** @} end of simple_send_dialog_fetion_functions */

/**
 * \addtogroup generic_send_dialog_fetion_functions 
 * @{
 */
/**
  * \fn int fx_begin_dialog(long who, EventListener func,void *args)
  * \brief begin the dialog function
  *
  * if you want to have a dialog with somebody. invoke this function first,
  * of course, if you not invoke this function to send a dialog, it will invoked by itself.
  * if you will not have a dialog with somebody, you should invoke fetion_end_dialog
  * advise: invoke this function before send a dialog to somebody,
  * \sa fetion_end_dialog
  *
  * \param who The somebody who you want to have a dialog
  *
  * \return non zero if init successfully, otherwise if fail return 0.( this function will not return 0)
  *
*/
FX_EXPORT int fx_begin_dialog(long who, EventListener func,void *args);

/**
  * \fn int fx_dialog_send(long who, const char *message, EventListener func,void *args)
  * \brief send a message to somebody in a dialog 
  *
  * \param who The somebody who you want to send
  * \param message The content which you want to send.
  *
  * \return ..
*/
FX_EXPORT int fx_dialog_send(long who, const char *message, EventListener func,void *args);


/**
  * \fn void fx_end_dialog(long who)
  * \brief end a dialog with somebody
  *
  * if you will not have a dialog with somebody, you should invoke this function
  *
  * \param who The somebody who you want to end a dialog
  *
*/
FX_EXPORT void fx_end_dialog(long who);


/** @} end of generic_send_dialog_fetion_functions */


/** @} end of send_dialog_fetion_functions */

/**
* \addtogroup schedule_sms_fetion_functions  
* @{
*/
/**
  * \fn  const DList *fx_data_get_smlist(void)
  * \brief get all the fetion's schedule sms info
  *
  * this function will return a list which store the schedule sms info
  *
  * \return a dlist value if successfully, otherwise return NULL..
  *
  *
  *
  ** A template to get all schedule sms info from the dlis struct
  * \code
  * 
  * Fetion_Schedule_SMS *sch_sms = NULL;
  *	DList *tmp = (DList *)fx_get_smlist();
  *
  *	while(tmp)
  *	{
  * 	sch_sms = (Fetion_Schedule_SMS *) tmp->data;
  *		if(sch_sms) {
  *         // do something to access this structure...	
  *		}
  *		tmp = d_list_next(tmp);
  *	}
  * \endcode
  *
  *
  * \sa Fetion_Schedule_SMS DList 
  */
FX_EXPORT const DList *fx_data_get_smlist(void);

/**
  * \fn const Fetion_Schedule_SMS* fx_get_schedulesms_by_id(int id)
  * \brief get an schedule sms structure by schedue_sms_id
  *
  * this function will return an schedule sms structure by schedue_sms_id
  *
  * \return a vaild value if successfully, otherwise return NULL..
  *
  *
  * the id you can got it by fx_data_get_smlist,
  * or the paramter of fx_set_schedule_sms's message
  *
  *
  * \sa Fetion_Schedule_SMS fx_data_get_smlist 
  */
FX_EXPORT const Fetion_Schedule_SMS* fx_get_schedulesms_by_id(int id);

/**
  * \fn int fx_set_schedule_sms(const DList* receiver, const char *msg, const char *send_time, EventListener func, void *args)
  * \brief set an schedule sms 
  *
  * \param receiver a dlist of the receivers who you want to send sms.
  * \param msg The content which you want to send.
  * \param send_time The time when which you want to send. format: "2009-04-22 16:15:00"
  *
  * \return ..
  *
  */
FX_EXPORT int fx_set_schedule_sms(const DList* receiver, const char *msg, const char *send_time, EventListener func, void *args);

/**
  * \fn int fx_delete_schedule_sms(int sms_id, EventListener func, void *args)
  * \brief delete an schedule sms 
  *
  * \param sms_id The id which you want to delete the schedule sms.
  *
  * \return ..
  *
  */
FX_EXPORT int fx_delete_schedule_sms(int sms_id, EventListener func, void *args);

/**
  * \fn char *fx_covert_schedule_receiver_to_string(DList * receiver)
  * \brief covert schedule receiver to string from DList
  *
  * \param receiver The DList point of the schedule sms.
  *
  * \return a string of receiver name..
  *
  */
FX_EXPORT char *fx_covert_schedule_receiver_to_string(DList * receiver);
/** @} end of schedule_sms_fetion_functions */

/**
* \addtogroup get_or_set_user_info  
* @{
*/

/**
  * \fn const char *fx_get_usr_uid(void)
  * \brief get a current user is uid
  *
  * \sa Fetion_Data
  *
  * \return non NULL value if successfully, otherwise return NULL..
*/
FX_EXPORT const char *fx_get_usr_uid(void);

/**
  * \fn const char *fx_get_usr_passwd(void)
  * \brief get a current user is passwd 
  *
  * \sa Fetion_Data
  *
  * \return non NULL value if successfully, otherwise return NULL..
*/
FX_EXPORT const char *fx_get_usr_passwd(void);

/**
  * \fn const char *fx_get_usr_show_name(void)
  * \brief get a current user is name
  *
  * \sa Fetion_Data
  *
  * \return non NULL value if successfully, otherwise return NULL..
*/
FX_EXPORT const char *fx_get_usr_show_name(void);

/**
  * \fn const char *fx_get_usr_uri(void)
  * \brief get a current user is uir
  *
  * \sa Fetion_Data
  *
  * \return non NULL value if successfully, otherwise return NULL..
*/
FX_EXPORT const char *fx_get_usr_uri(void);

/**
  * \fn const char *fx_get_usr_mobilenum(void)
  * \brief get a current user is mobile number
  *
  * \sa Fetion_Data
  *
  * \return non NULL value if successfully, otherwise return NULL..
*/
FX_EXPORT const char *fx_get_usr_mobilenum(void);

/**
  * \fn int fx_get_usr_score(void)
  * \brief get the user is score
  *
  * \return the value of score..
*/
FX_EXPORT int fx_get_usr_score(void);

/**
  * \fn int fx_set_user_state(int state, char *desc, EventListener func, void *args)
  * \brief set the user state.
  *
  * This function set the usr fetion state. 
  *
  * \param state The state which you want to set.
  * \param desc If you set the state is by your self define, set the connect of desc.
  *
  *
  * \return non zero on successfully, otherwise return zero.
*/
FX_EXPORT int fx_set_user_state(int state, char *desc, EventListener func, void *args);

/**
  * \fn int fx_get_user_state(void)
  * \brief get the user state.
  *
  * This function get the usr fetion state. 
  * \return the current usr state.
*/
FX_EXPORT int fx_get_user_state(void);


/**
  * \fn int fx_set_user_impresa(const char *impresa, EventListener func, void *args)
  * \brief set the user impresa. 
  *
  * \param impresa The impresa which you want to set.
  *
  * \return non zero on successfully, otherwise return zero.
*/
FX_EXPORT int fx_set_user_impresa(const char *impresa, EventListener func, void *args);

/**
  * \fn int fx_set_user_nickname(const char *nickname, EventListener func, void *args)
  * \brief set the user nickname. 
  *
  * \param nickname The nickname which you want to set.
  *
  * \return non zero on successfully, otherwise return zero.
*/
FX_EXPORT int fx_set_user_nickname(const char *nickname, EventListener func, void *args);

/**
  * \fn int fx_get_user_refuse_sms_day(void)
  * \brief get the user how long day to receive the sms message. 
  *
  * This function get the usr fetion day. 
  * \return the day.
*/
FX_EXPORT int fx_get_user_refuse_sms_day(void);

/**
  * \fn int fx_set_user_refuse_sms_day(int day, EventListener func, void *args)
  * \brief set the user how long day to receive the sms message. 
  *
  * \param day The day which you want to set, if day = 0, show will 
  * receive sms, or it is the refuse sms days. .
  *
  * \return non zero on successfully, otherwise return zero.
*/
FX_EXPORT int fx_set_user_refuse_sms_day(int day, EventListener func, void *args);


/**
  * \fn int fx_get_expirestime(void)
  * \brief get the expires time of the fetion.
  *
  * you should once the expires time to invoke the fetion_keep_alive functions to keep your fetion alive.
  *
  * \sa fetion_keep_alive
  *
  * \return the expirestime. if the expires time equal zero, you should quit fetion.
*/
FX_EXPORT int  fx_get_expirestime(void);

/**
  * \fn const Fetion_Personal *fx_data_get_PersonalInfo(void)
  * \brief get a current user is personal info
  *
  * \sa Fetion_Personal
  *
  * \return the Fetion_Personal info struct of the user
*/
FX_EXPORT const Fetion_Personal *fx_data_get_PersonalInfo(void);

/**
  * \fn Ftion_Status_Primitive fetion_data_get_status(void)
  * \brief get a current user is status info
  *
  * \sa Ftion_Status_Primitive
  *
  * \return status info of the user
*/
//Ftion_Status_Primitive fetion_data_get_status(void);

/** @} end of get_or_set_user_info */


/**
* \addtogroup get_contact_info  
* @{
*/

/**
  * \fn const DList *fx_get_group()
  * \brief get the fetion's group info
  *
  * this function will return a list which store the group info
  *
  * \return a dlist value if successfully, otherwise return NULL..
  *
  *
  *
  ** A template to get all group info from the dlis struct
  * \code
  * 
  * Fetion_Group *group = NULL;
  *	DList *tmp = (DList *)fx_get_group();
  *
  *	while(tmp)
  *	{
  * 	group = (Fetion_Group *) tmp->data;
  *		if(group) {
  *         // do something to access this group...	
  *		}
  *		tmp = d_list_next(tmp);
  *	}
  * \endcode
  *
  *
  * \sa Fetion_Group DList 
  */

FX_EXPORT const DList *fx_get_group();


/**
  * \fn const Fetion_Account *fx_get_first_account()
  * \brief get the first account of fetion's accounts list
  *
  * this function will return the first account of account info list
  *
  * \return a No_NULL value if successfully, otherwise return NULL..
  *
  *
  *
  *	const Fetion_Account *account = fx_get_first_account();
  *	while(account)
  *	{
  *  //do some things...
  *
  *  account = fx_get_next_account(account);
  * }
  * \endcode
  *
  * \sa Fetion_Account  fx_get_next_account
*/
FX_EXPORT const Fetion_Account *fx_get_first_account();

/**
  * \fn const Fetion_Account *fx_get_next_account(const Fetion_Account *account)
  * \brief get the next  account of the given account
  *
  * this function will return a account that was next postion of given account
  *
  * \return a No_NULL value if successfully, otherwise return NULL..
  *
  *	const Fetion_Account *account = fx_get_first_account();
  *	while(account)
  *	{
  *  //do some things...
  *
  *  account = fx_get_next_account(account);
  * }
  * \endcode
  *
  * \sa Fetion_Account  fx_get_first_account
*/
FX_EXPORT const Fetion_Account *fx_get_next_account(const Fetion_Account *account);

/**
  * \fn const DList *fx_get_blacklist()
  * \brief get the fetion's all black's accounts info
  *
  * this function will return a list which store all black's account info
  *
  * \return a dlist value if successfully, otherwise return NULL..
  *
  *
  *
  * A template to get all blacks account info from the dlis struct
  * \code
  * 
  * Fetion_Black *black = NULL;
  *	DList *tmp = (DList *)fx_get_blacklist();
  *	while(tmp) 
  *	{
  *		if(	black =(Fetion_Black *)tmp->data ) {
  *         // do something to access this account...
  *        }
  *
  *		tmp = d_list_next(tmp);
  *	}
  * \endcode
  *
  *
  * \sa Fetion_Black DList 
*/
FX_EXPORT const DList *fx_get_blacklist();

/**
  * \fn const DList *fx_get_qun()
  * \brief get the fetion's qun info
  *
  * this function will return a list which store the qun info
  *
  * \return a dlist value if successfully, otherwise return NULL..
  *
  *
  *
  ** A template to get all qun info from the dlis struct
  * \code
  * 
  * Fetion_Qun *qun = NULL;
  *	DList *tmp = (DList *)fx_get_qun();
  *
  *	while(tmp)
  *	{
  * 	qun = (Fetion_Qun *) tmp->data;
  *		if(qun) {
  *         // do something to access this group...	
  *		}
  *		tmp = d_list_next(tmp);
  *	}
  * \endcode
  *
  *
  * \sa Fetion_Qun DList 
  */

FX_EXPORT const DList *fx_get_qun();


/**
  * \fn const Fetion_Account *fx_get_account_by_id(long id)
  * \brief get a account which uid 's the gived id
  *
  * \sa Fetion_Account
  *
  * \return a account value if successfully, otherwise return NULL..
*/
FX_EXPORT const Fetion_Account *fx_get_account_by_id(long id);

/**
  * \fn const Fetion_Qun *fx_get_qun_by_id(long id)
  * \brief get  qun which uid 's the gived id
  *
  * \sa Fetion_Qun
  *
  * \return a qun value if successfully, otherwise return NULL..
*/
FX_EXPORT const Fetion_Qun *fx_get_qun_by_id(long id);


/**
  * \fn BOOL fx_is_pc_user_by_id(long id)
  * \brief judge this id is PC USER or not
  *
  * \return TRUE if id is PC USER, otherwise return FALSE..
*/
FX_EXPORT BOOL fx_is_pc_user_by_id(long id);

/**
  * \fn BOOL fx_is_qun_by_id(long id)
  * \brief judge this id is qun or not
  *
  * \return TRUE if id is qun id, otherwise return FALSE..
*/
FX_EXPORT BOOL fx_is_qun_by_id(long id);

/**
  * \fn BOOL fx_is_pc_user_by_account(const Fetion_Account *account)
  * \brief judge this account is PC USER or not
  *
  * \return TRUE if id is PC USER, otherwise return FALSE..
*/
FX_EXPORT BOOL fx_is_pc_user_by_account(const Fetion_Account *account);


/**
  * \fn int fx_is_authed_by_id(long id)
  * \brief judge this id is auth to you add it
  *
  * \return AUTH_OK if id is auth,  AUTH_REFUS if the id auth refus, 
  * AUTH_WAIT is the id is not decide.
*/
FX_EXPORT int fx_is_authed_by_id(long id);

/**
  * \fn int fx_is_authed_by_account(Fetion_Account *account)
  * \brief judge this account is auth to you add it
  *
  * if id is not correct, it will return AUTH_WAIT
  *
  * \return AUTH_OK if account is auth,  AUTH_REFUS if it auth refus, 
  * AUTH_WAIT if it not decide.
*/
FX_EXPORT int fx_is_authed_by_account(const Fetion_Account *account);

/**
  * \fn BOOL fx_is_InBlacklist_by_id(const long id)
  * \brief judge this account is in the blacklist
  *
  * if id is not correct, it will return FALSE 
  *
  * \return TRUE if account is in blacklist,  FALSE if not in, 
*/
FX_EXPORT BOOL fx_is_InBlacklist_by_id(const long id);

/**
  * \fn BOOL fx_is_InBlacklist_by_account(const Fetion_Account * account)
  * \brief judge this account is in the blacklist
  *
  * if id is not correct, it will return FALSE 
  *
  * \return TRUE if account is in blacklist,  FALSE if not in, 
*/
FX_EXPORT BOOL fx_is_InBlacklist_by_account(const Fetion_Account * account);


/**
 * \fn int fx_move_group_buddy_by_id(long uid, int group_id, EventListener func, void *args)
 * \brief modify the group which account was belonged. 
 *
 * \param uid The account's id which you want to move.
 * \param group_id The group is id.
 * \param func The operate's callback function's address, and the operate result will pass to this function.
 * \param args The operate's callback function's args. 
 *
 * \return 0 if fail immediately, or can get the result from func.
 */
FX_EXPORT int fx_move_group_buddy_by_id(long uid, int group_id, EventListener func, void *args);

/**
 * \fn int fx_move_group_buddy(const Fetion_Account * account, int group_id, EventListener func, void *args)
 * \brief modify the group which account was belonged. 
 *
 * \param account The account which you want to move.
 * \param group_id The group is id.
 * \param func The operate's callback function's address, and the operate result will pass to this function.
 * \param args The operate's callback function's args. 
 *
 * \return 0 if fail immediately, or can get the result from func.
 */
FX_EXPORT int fx_move_group_buddy(const Fetion_Account *account, int group_id, EventListener func, void *args);

/**
  * \fn BOOL fx_is_on_line_by_id(long id)
  * \brief judge this id is OnLine or not
  *
  * \return TRUE if id is OnLine, otherwise return FALSE..
*/
FX_EXPORT BOOL fx_is_on_line_by_id(long id);

/**
  * \fn BOOL fx_is_on_line_by_account(const Fetion_Account *account)
  * \brief judge this account is OnLine or not
  *
  * \return TRUE if id is OnLine, otherwise return FALSE..
*/
FX_EXPORT BOOL fx_is_on_line_by_account(const Fetion_Account *account);

/**
  * \fn int fx_get_online_status_by_id(const long uid)
  * \brief return fetion uid 's online status.  
  *
  * \return -1 if the fetion uid is mobile user, or return the states of fetion .
*/
FX_EXPORT int fx_get_online_status_by_id(const long uid);

/**
  * \fn BOOL fx_is_online_status(int online_status)
  * \brief judge the status is online or not.  
  *
  * \return TRUE when it is online status, or return FALSE.
*/
FX_EXPORT BOOL fx_is_online_status(int online_status);

/**
  * \fn BOOL fx_is_receive_sms_by_account(const Fetion_Account *account)
  * \brief judge this account could receive SMS or not
  *
  * \return FALSE if this account couldn't sms or return TRUE.
*/
FX_EXPORT BOOL fx_is_receive_sms_by_account(const Fetion_Account *account);

/**
  * \fn BOOL fx_is_auth_chat_by_account(const Fetion_Account *account)
  * \brief judge this account could char or not
  *
  * \return FALSE if this account couldn't char or return TRUE.
*/
FX_EXPORT BOOL fx_is_auth_chat_by_account(const Fetion_Account *account);




/**
  * \fn int fx_get_online_status_by_account(const Fetion_Account * account)
  * \brief return fetion account's online status.  
  *
  * \return -1 if the fetion uid is mobile user, or return the states of fetion .
  *
*/
FX_EXPORT int fx_get_online_status_by_account(const Fetion_Account * account);

/**
  * \fn int fx_islogin_by_mobile(const Fetion_Account * account)
  * \brief return fetion account's login by mobile or not.  
  *
  * \return 1 if the fetion is login by mobile, or return 0.
  *
*/
FX_EXPORT int fx_islogin_by_mobile(const Fetion_Account * account);
/**
  * \fn int fx_get_refuse_sms_day(const Fetion_Account *account)
  * \brief return fetion account's refuse_sms_day.  
  *
  * \return the numbers of refuse_sms_day.  
  *
*/
FX_EXPORT int fx_get_refuse_sms_day(const Fetion_Account *account);

/**
  * \fn fx_update_account_info_by_id(long id)
  * \brief update the account info which uid 's the gived id
  *
  * \sa 
  *
  * \after invoke this function, if update OK, you will receice a message of ..
*/
FX_EXPORT void fx_update_account_info_by_id(long id);


/**
  * \fn char *fx_get_account_show_name(const Fetion_Account *account, BOOL needImpresa)
  * \brief get the show name of the account
  *
  * note: this function maybe modify at late version....
  *
  * \param account The Fetion_Account which you want to get showname. 
  * \param needImpresa Is need the impresa in showname 
  *
  * \sa Fetion_Account
  * \sa fx_get_account_show_name_with_state
  *
  * \return the show name of account
*/
FX_EXPORT char *fx_get_account_show_name(const Fetion_Account *account, BOOL needImpresa);

/**
  * \fn char *fx_get_account_show_name_with_state(const Fetion_Account *account, BOOL needImpresa, BOOL needSState)
  * \brief get the show name of the account
  *
  * note: this function maybe modify at late version....
  *
  * \param account The Fetion_Account which you want to get showname. 
  * \param needImpresa Is need the impresa in showname 
  * \param needAccountStatue Is need the AccountStatue in showname 
  *
  * \sa Fetion_Account
  * \sa fx_get_account_show_name
  *
  * \return the show name of account
*/
FX_EXPORT char *fx_get_account_show_name_with_state(const Fetion_Account *account, BOOL needImpresa, BOOL needSState);

/**
  * \fn char *fx_get_qun_show_name(Fetion_Qun *qun)
  * \brief get the show name of the qun 
  *
  * note: this function maybe modify at late version....
  *
  * \param qun The Fetion_Qun which you want to getinfo
  *
  * \sa Fetion_Qun
  *
  * \return the show name of qun
*/
FX_EXPORT char *fx_get_qun_show_name(Fetion_Qun *qun);

/**
  * \fn int fx_get_account_group_id(const Fetion_Account *account)
  * \brief get the group id of the account
  *
  * note: group_id is started by 0,
  *
  * \param account The Fetion_Account which you want to getinfo
  *
  * \sa Fetion_Account
  *
  * \return the id of account's group if successfully, otherwise return -1..
*/
FX_EXPORT int fx_get_account_group_id(const Fetion_Account *account);


/** @} end of get_contact_info */


/**
 * \addtogroup simple_fetion_group_opt 
 * @{
 */

/**
 * \fn BOOL fs_rename_buddylist(int id, const char* name)
 * \brief rename the group. 
 *
 * \param uid The group which you want to rename.
 * \param name The name which you want to set.
 *
 * \return TRUE if success, or will return FALSE.
 */
FX_EXPORT BOOL fs_rename_buddylist(int id, const char* name);

/**
 * \fn BOOL fs_delete_buddylist(int id)
 * \brief delete a group. 
 *
 * \param id The group which you want to delete.
 *
 * \return TRUE if success, or will return FALSE.
 */
FX_EXPORT BOOL fs_delete_buddylist(int id);

/** @} end of simple_fetion_group_opt */

/**
 * \addtogroup generic_fetion_group_opt  
 * @{
 */
/**
 * \fn int fx_rename_buddylist(int id, const char* name, EventListener func, void *args)
 * \brief rename the group. 
 *
 * \param uid The group which you want to rename.
 * \param name The name which you want to set.
 * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
 * \param args The send_sms operate's callback function's args. 
 *
 * \return 0 if fail immediately, or can get the result from func.
 */
FX_EXPORT int fx_rename_buddylist(int id, const char* name, EventListener func, void *args);

/**
 * \fn int fx_add_buddylist(const char* name, EventListener func, void *args)
 * \brief rename the group. 
 *
 * \param name The group name which you want to add.
 * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
 * \param args The send_sms operate's callback function's args. 
 *
 * \return 0 if fail immediately, or can get the result from func.
 */
FX_EXPORT int fx_add_buddylist(const char* name, EventListener func, void *args);

/**
 * \fn int fx_add_buddy_by_uid(const char* uid, const char* local_name, int groupid, const char* desc, EventListener func, void *args)
 * \brief add a account by fetion id. 
 *
 * \param uid The uid of account which you want to add.
 * \param loacl_name The name of account which you want to add.
 * \param groupid The groupid of account which you want to add.
 * \param desc The name of you send to the user see.
 * \param func The operate's callback function's address, and the operate result will pass to this function.
 * \param args The operate's callback function's args. 
 *
 * \return 0 if fail immediately, or can get the result from func.
 */
FX_EXPORT int fx_add_buddy_by_uid(const char* uid, const char* local_name, int groupid, const char* desc, EventListener func, void *args);


/**
 * \fn int fx_add_buddy_by_mobile(const char* mobile, const char* local_name, int groupid, const char* desc, EventListener func, void *args)
 * \brief add a account by mobile number. 
 *
 * \param uid The uid of account which you want to add.
 * \param loacl_name The name of account which you want to add.
 * \param groupid The groupid of account which you want to add.
 * \param desc The name of you send to the user see.
 * \param func The operate's callback function's address, and the operate result will pass to this function.
 * \param args The operate's callback function's args. 
 *
 * \return 0 if fail immediately, or can get the result from func.
 */
FX_EXPORT int fx_add_buddy_by_mobile(const char* mobile, const char* local_name, int groupid, const char* desc, EventListener func, void *args);


/**
 * \fn int fx_handleContactRequest(const char *uri, int isPassed, int group, const char* local_name)
 *
 * \brief handle a request of addaccount. 
 *
 * \param uri The uri of account which want to add with you.
 * \param isPassed  
 * \param group The groupid of account which you want to add.
 * \param local_name The name of you send to the user see.
 * \return 0 if fail immediately.
 */
FX_EXPORT int fx_handleContactRequest(const char *uri, int isPassed, int group, const char* local_name);

/**
 * \fn int fx_delete_buddylist(int id, EventListener func, void *args)
 * \brief delete a  group. 
 *
 * \param id The group id which you want to delete.
 * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
 * \param args The send_sms operate's callback function's args. 
 *
 * \return 0 if fail immediately, or can get the result from func.
 */
FX_EXPORT int fx_delete_buddylist(int id, EventListener func, void *args);


/** @} end of generic_fetion_group_opt */

/**
* \addtogroup simple_fetion_buddy_opt  
* @{
*/

/**
  * \fn BOOL fs_set_buddyinfo(long uid, const char* localname)
  * \brief set the local name of the account. 
  *
  * \param uid The account which you want to set.
  * \param localname The local name which you want to set.
  * \return TRUE if success, or will return FALSE.
*/
FX_EXPORT BOOL fs_set_buddyinfo(long uid, const char* localname);


/** @} end of simple_fetion_buddy_opt */

/**
* \addtogroup generic_fetion_buddy_opt  
* @{
*/

/**
  * \fn int fx_set_buddyinfo(long uid, const char* localname, EventListener func, void *args)
  * \brief set the local name of the account. 
  *
  * \param uid The account which you want to set.
  * \param localname The local name which you want to set.
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if set buddyinfo fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_set_buddyinfo(long uid, const char* localname, EventListener func, void *args);

/**
  * \fn int fx_delete_buddy_by_id(long uid, EventListener func, void *args)
  * \brief delete the account. 
  *
  * \param uid The account which you want to delete.
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_delete_buddy_by_id(long uid, EventListener func, void *args);

/**
  * \fn int fx_delete_buddy_by_account(Fetion_Account *account, EventListener func, void *args)
  * \brief delete the account. 
  *
  * \param account The account which you want to delete.
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_delete_buddy_by_account(Fetion_Account *account, EventListener func, void *args);

/**
  * \fn int fx_addto_blacklist_by_id(const long uid, EventListener func, void *args)
  * \brief add the user to blacklist 
  *
  * \param uid The id which you want to add to blacklist.
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_addto_blacklist_by_id(const long uid, EventListener func, void *args);

/**
  * \fn int fx_addto_blacklist_by_account(Fetion_Account *account, EventListener func, void *args)
  * \brief add the uri user to blacklist 
  *
  * \param account The account which you want to add to blacklist.
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_addto_blacklist_by_account(Fetion_Account *account, EventListener func, void *args);

/**
  * \fn int fx_addto_blacklist_by_uri(const char* uri, EventListener func, void *args)
  * \brief add the uri user to blacklist 
  *
  * \param uri The account's uri which you want to add to blacklist.
  * \param func The send sms operate's callback function's address, and the operate result will pass to this function.
  * \param args The send_sms operate's callback function's args. 
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_addto_blacklist_by_uri(const char* uri, EventListener func, void *args);


/**
  * \fn int fx_removefrom_blacklist_by_id(const long uid, EventListener func, void *args)
  * \brief remove the uri user frome blacklist 
  *
  * \param uid The id of account which you want to remove.
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_removefrom_blacklist_by_id(const long uid, EventListener func, void *args);
/**
  * \fn int fx_removefrom_blacklist_by_account(Fetion_Account *account, EventListener func, void *args)
  * \brief remove the uri user frome blacklist 
  *
  * \param account The account which you want to remove.
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_removefrom_blacklist_by_account(Fetion_Account *account, EventListener func, void *args);
/**
  * \fn int fx_removefrom_blacklist_by_uri(const char* uri, EventListener func, void *args)
  * \brief remove the uri user frome blacklist 
  *
  * \param uri The uri of account which you want to remove.
  *
  * \return 0 if fail immediately, or can get the result from func.
*/
FX_EXPORT int fx_removefrom_blacklist_by_uri(const char* uri, EventListener func, void *args);

/** @} end of generic_fetion_buddy_opt */

/**
* \addtogroup fetion_ext  
* @{
*/

/**
  * \fn BOOL fx_send_nudge(long who)
  * \brief send a nudge 
  *
  * 
  * this function should send by 10 second pre time . or it will return FALSE, and the receicer must be online.
  *
  * \param who which one you want to send.
  *
  * \return TRUE when success, FALSE when send fail .
*/
FX_EXPORT BOOL fx_send_nudge(long who);

/**
  * \fn BOOL fx_set_serve_address(const char *serve_address)
  * \brief set the fetion proxy ip and port, the proxy format is  ip:port(192.168.0.1:8080). 
  *
  *
  * This function should used before invoke login function
  *
  * \param proxy The fetion proxy string.
  *
  * \return TRUE when set success, FALSE when set fail of the proxy format is wrong.
*/
FX_EXPORT BOOL fx_set_serve_address(const char *serve_address);

/**
  * \fn void fx_set_unknow_serve_address(void)
  * \brief let the fetion get the serve_address ip and port by it self. 
  *
  *
  * This function tell libfetion to get serve_address ip and port by it self. 
  * ****this function will be removed in later version****.
  *
  *
*/
FX_EXPORT void fx_set_unknow_serve_address(void);

/**
  * \fn char* fx_get_serve_address(void)
  * \brief get the current fetion id 's proxy sever ip. the ip format is  ip:port(192.168.0.1:8080).
  *
  * This function can use just after login success. 
  *
  * \return the the current fetion id 's proxy sever ip, if no will return NULL.
*/
FX_EXPORT char* fx_get_serve_address(void);

/**
 * \fn BOOL fx_set_proxy(PROXY_ITEM *item)
 * \brief Set the proxy when you use proxy to connnect the network.
 *
 * \param item The proxy information.
 * \return TRUE on success, FALSE on error.
 *
 * \sa fetion_get_proxy
 */
FX_EXPORT BOOL fx_set_proxy(PROXY_ITEM *item);

/**
 * \fn const PROXY_ITEM * fx_get_proxy()
 * \brief Get the proxy information.
 *
 * \return no NULL if have proxy info, or return NULL.
 *
 * \sa fetion_set_proxy
 */
FX_EXPORT const PROXY_ITEM * fx_get_proxy();

/**
 * \fn void fetion_setProxyEnabled(BOOL status)
 * \brief Set the proxy isEnabled.
 *
 * \param status Active proxy.
 *
 * \sa fetion_proxyEnabled
 */
FX_EXPORT void fx_setProxyEnabled(BOOL status);

/**
 * \fn BOOL fetion_proxyEnabled()
 * \brief Get the proxy isEnabled.
 *
 * \return TRUE on Enabled, or return FALSE .
 *
 * \sa fetion_set_proxy
 */
FX_EXPORT BOOL fx_proxyEnabled();


/**
 * \fn int fx_test_network(PROXY_ITEM *item, EventListener func, void *args)
 * \brief Get the fetion network is working of proxy_item.
 *
 * if item is NULL, will test direct connect to net is working.
 *
  * \return 0 if fail immediately, or can get the result from func.
 *
 * \sa fetion_set_proxy
 */
FX_EXPORT int fx_test_network(PROXY_ITEM *item, EventListener func, void *args);
/** @} end of fetion_ext */

/**
* \addtogroup fetion_misc  
* @{
*/

/**
  * \fn char* fx_msg_no_format(const char * msg)
  * \brief simple paser the msg, remove the <Font></Font> tags. 
  *
  * \param msg The original msg.
  *
  * \return the no format message when it is a correct msg,
  * or return original msg.
*/
FX_EXPORT char* fx_msg_no_format(const char * msg);

/**
  * \fn char* fx_msg_qt_format(const char * msg)
  * \brief transform msg format to qt support. 
  *
  * \param msg The original msg.
  *
  * \return the msg which 's format was supported by QT GUI when it is 
  * a correct msg, or return original msg  if the msg is no format.
  * 
*/
FX_EXPORT char* fx_msg_qt_format(const char * msg);

/**
  * \fn char* fx_get_original_ID(long id)
  * \brief get the original id of the user.
  *
  * if the user is pc user, it will return the fetion id. 
  * if the user is the mobile user, it will return the user's mobile. 
  *
  * \param msg The original msg.
  *
  * \return the no format message, return NULL if the msg is wrong format.
*/
FX_EXPORT char* fx_get_original_ID(long id);

/**
  * \fn void fx_set_buddy_nickname_ex(long uid, const char *nickname)
  * \brief set the nickname for account.
  *
  * just used by relax, this change will not be pulled to server of fetion. 
  *
  * \param uid The id which you want to change.
  * \param nickname The nickname which you want to set.
*/
FX_EXPORT void fx_set_buddy_nickname_ex(long uid, const char *nickname);

/**
  * \fn fx_set_buddy_impresa_ex(long uid, const char *impresa)
  * \brief get the original id of the user.
  *
  * just used by relax, this change will not be pulled to server of fetion. 
  *
  * \param uid The id which you want to change.
  * \param impresa The impresa which you want to set.
  *
*/
FX_EXPORT void fx_set_buddy_impresa_ex(long uid, const char *impresa);
/** @} end of fetion_misc */

#ifdef __cplusplus 
}
#endif

#endif
