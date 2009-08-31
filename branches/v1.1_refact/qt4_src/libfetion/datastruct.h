/**
 * \file datastruct.h
 * \author dengdd <dedodong@163.com>
 * \date 2009/4/24
 * 
 * This file includes necessary struct define of libfetion.
 *
 \verbatim

    Copyright (C) 2009 dengdd.

    All rights reserved by dengdd.

    This file is part of libfetion, a cross-operating-system library to operate China mobile's fetion IM.

 \endverbatim
 */
#ifndef _LIBFETION_DATASTRUCT_H_
#define _LIBFETION_DATASTRUCT_H_

#ifndef __LIBFETION_LIB__

#ifdef __cplusplus 
extern "C" { 
#endif 

/**
 * \defgroup fetion_dlist LibFetion DList Define
 * @{
 * \sa
 */

typedef int (*GCompareFunc) (const void* a, const void *b);
typedef int (*GCompareDataFunc)  (const void*  a,const void*  b, void* user_data);

typedef void (*GFunc)    (void *data,void* user_data);


/**
 * The DList structure.
 *
 * \sa 
 */
typedef struct _DList
{
	/** data.*/
	void* data;
	/** the next node of list.*/
	struct _DList *next;
}DList;

DList*  d_list_alloc (void);
void     d_list_free                    (DList           *list);
DList*  d_list_append                  (DList           *list,void*          data);
DList*  d_list_last                    (DList           *list);
DList*  d_list_prepend                 (DList           *list, void*          data) ;

DList*  d_list_insert                  (DList           *list,void*          data,int              position);

#if 0
void     d_list_free_1                  (DList           *list);
#define	 d_list_free1		         d_list_free_1
#endif
#if 1

DList*  d_list_insert_sorted           (DList           *list,void*          data,GCompareFunc      func)  ;
DList*  d_list_insert_sorted_with_data (DList           *list,void* data,GCompareDataFunc  func,void* user_data)  ;
DList*  d_list_insert_before           (DList           *slist,
					  DList           *sibling,
					  void*          data)  ;
DList*  d_list_concat                  (DList           *list1,
					  DList           *list2)  ;
DList*  d_list_remove                  (DList           *list,
					   const void*     data)  ;
DList*  d_list_remove_all              (DList           *list,
					   const void*     data)  ;
DList*  d_list_remove_link             (DList           *list,
					  DList           *link_)  ;
DList*  d_list_delete_link             (DList           *list,
					  DList           *link_)  ;
DList*  d_list_reverse                 (DList           *list)  ;
DList*  d_list_copy                    (DList           *list)  ;
DList*  d_list_nth                     (DList           *list,
					  unsigned int             n);
DList*  d_list_find                    (DList           *list,
					   const void*     data);
DList*  d_list_find_custom             (DList           *list,
					   const void*     data,
					  GCompareFunc      func);
int     d_list_position                (DList           *list,
					  DList           *llink);
int     d_list_index                   (DList           *list,
					   const void*     data);

unsigned int    d_list_length                  (DList           *list);
void     d_list_foreach                 (DList           *list,
					  GFunc             func,
					  void*          user_data);
DList*  d_list_sort                    (DList           *list,
					  GCompareFunc      compare_func)  ;
DList*  d_list_sort_with_data          (DList           *list,
					  GCompareDataFunc  compare_func,
					  void*          user_data)  ;
void* d_list_nth_data                (DList           *list,
					  unsigned int             n);

#define  d_list_next(list)	         ((list) ? (((DList *)(list))->next) : NULL)

#ifndef G_DISABLE_DEPRECATED
void     d_list_push_allocator          (void*	   dummy);
void     d_list_pop_allocator           (void);
#endif

#endif
/** @} end of fetion_dlist */

/**
 * The fetion msg structure.
 *
 * \sa fx_get_msg
 */
typedef struct _fetion_msg {
	/** The send account 's uid. */
	long uid;
	/** The message content. */
	char *message;
	/** The message format. */
	char *msgformat;
	/** Thu, 20 Dec 2007 13:29:23 GMT. */
	char *msgtime;
	/** Just using in qun, this id show sended of qun. */
	long ext_id;
}Fetion_MSG;

/**
 *  * The fetion schedule sms structure.
 *   *
 *    * \sa fx_get_schedule_sms
 *     */

typedef struct _fetion_schedule_sms {
	/** schedule_sms id. */
	int id;
	/** schedule_sms version. */
	int version;
	/** schedule_sms send_time. */
	char* send_time;
	/** schedule_sms contain. */
	char* message;
	/** schedule_sms receiver list.
	 *
	 * DList * tmp = receivers;
	 * while (tmp)
	 * {
	 *  if (tmp->data);
	 *		printf("%d \n", (int)tmp->data);
	 *    tmp = d_list_next(tmp);
	 * }
	 *
	 * */
	DList* receivers; 
}Fetion_Schedule_SMS;



/**
 * The proxy type.
 * \sa PROXY_ITEM, fetion_get_proxy
 */
typedef enum {
    /** DIRECT. */
    PROXY_DIRECT = 0,
    /** HTTP. */
    PROXY_HTTP,
    /** SOCKS4. */
    PROXY_SOCKS4,
    /** SOCKS5. */
    PROXY_SOCKS5,
    PROXY_MAX
}PROXY_TYPE;

/**
 * The proxy item information structure.
 *
 * \sa fetion_get_proxy, PROXY_INFO
 */
typedef struct _PROXY_ITEM{
    /** Proxy host. */
    char *host;
    /** Proxy port. */
    char *port;
    /** User name. */
    char *name;
    /** User password. */
    char *pwd;
    /** Proxy type, which can be HTTP, SOCKS4, or SOCKS5. */
    PROXY_TYPE  type; 
}PROXY_ITEM;

/**
 * \defgroup fetion_blist LibFetion Contact struct Define
 * @{
 * \sa
 */

typedef enum
{
	FTION_UTYPE_UNSET = 0,        //未设置 
    FTION_UTYPE_PC,               //PC用户，已经注册fetion 
	FTION_UTYPE_MOBLIE            //手机用户，未注册fetion 
} Ftion_USER_TYPE;

/**
 * The fetion account personal info structure.
 *
 * \sa 
 */
	typedef struct _fetion_personal {           //该结构信息可能需要分类来保存

		/** nickname . */
		char *nickname;                 //昵称
		/** impresa . */
		char *impresa;                  //心情 
		/** sexuality 1 is boy, 0 is girl . */
		int gender;  //1 boy, 0 girl.   //性别
		/** nation. */
		char *nation;                   //国家 
		/** province. */
		char *province;                 //洲
		/** city. */
		int city;                       //城市编号，需要测试
		int ivr_enabled;
		/** portrait address. */
		int portrait_crc;
		int provisioning;
		/** mobile number. */
		char *mobile_no;                 //手机号码
		/** real name. */
		char *name;                      //名称
		/** birth date. */
		char *birth_date;                //生日 
		int birthday_valid;
		/** lunar animal. */
		int lunar_animal;              //生肖
		/** horoscope. */
		int horoscope;                 //星座
		/** profile. */
		char *profile;                   //爱好吗？？？？*************
		/** blood_type. */
		int blood_type;                //血型 
		/** occupation. */
		char *occupation;                //职业
		/** hobby. */
		char *hobby;                     //业余爱好
		/** personal_email. */
		char *personal_email;            //私人邮箱
		/** work_email. */
		char *work_email;                //工作邮箱
		/** other_email. */
		char *other_email;               //其它邮箱
		/** primary_email. */
		int primary_email;               //主要邮箱
		/** job_title. */
		char *job_title;
		/** home_phone. */
		char *home_phone;
		/** work_phone. */
		char *work_phone;
		/** other_phone. */
		char *other_phone;
		/** company. */
		char *company;
		/** company_website. */
		char *company_website;
		/** match_enabled. */
		int match_enabled;
		/** 0 accept sms  other show refuse sms days... */
		int refuse_sms_day; 
	}Fetion_Personal;

/**
 * The fetion group structure.
 *
 * \sa fx_get_group
 */
	typedef struct _fetion_group {  //该结构存储组信息
    /** fetion group id. */
		int id;             //组ID 
    /** fetion group name. */
		char *name;         //组名称
	}Fetion_Group;

/**
 * The fetion account structure.
 *
 * \sa fx_get_account
 */
		typedef struct _fetion_account { 
    /** account id. */
		long id; //619369**
    /** account uri. */
		char *uri; //619369**@fetion.com.cn;p=2082
    /** account local_name. */
		char *local_name;
    /** account group id, it store as string, the format is *;*;. */
		char *buddy_lists;
		int group_id;
		int relation_status;
		int online_notify;
		int status_code; //200 or 400
		/** account online status 
		 * \sa online_event
		 */
		int online_status;   //sa event.h USER STATUS Event
		long portrait_crc;
		/** account personal info. 
		 * \sa Fetion_Personal 
		 */
		Fetion_Personal *personal;
		Ftion_USER_TYPE usr_type; //PC or moblie
        char *device_type; //device_type: "PC" or mobile
	}Fetion_Account;

/**
 * The fetion black buddies list structure.
 *
 * \sa fx_get_blacklist
 */
	typedef struct _fetion_black  {  
    /** account id. */
		long uid;             
    /** account uri. */
		char *uri;         
    /** account local_name. */
		char *local_name;
	}Fetion_Black;

	typedef struct _fetion_qun_member Fetion_QunMember;
	struct _fetion_qun_member {  
		char *uri;
		long id;
		char *nickname;
		char *iicnickname;
		int identity;
		int state;
		char *client_type;
	};

	typedef struct _fetion_qun_info Fetion_QunInfo;
	struct _fetion_qun_info {  
		char *uri;     
		int group_attributes_version;
		char* name;
		int category;
		char* introduce;
		char* bulletin;
		int	portrait_crc;
		int searchable;
		int current_member_count;
		int limit_member_count;
		char *group_activity;
		DList *QunMember;
	};

	typedef struct _fetion_qun Fetion_Qun;
	struct _fetion_qun {  
		long id;         
		char *uri;     
		int identity;
		Fetion_QunInfo *quninfo;
	};

/**
 * The fetion buddy list structure.
 *
 * \sa 
 */
typedef struct _fetion_blist {
	/** store group info.
	 *
	 * \sa Fetion_Group  DList  fx_get_group
	 */
	DList* group;
	/** store account info.
	 *
	 * \sa Fetion_Account  DList  fx_get_account
	 */
	DList* account;
	/** store blacked account infos.
	 *
	 * \sa Fetion_Black  DList  fx_get_blacklist
	 */
	DList* blacklist;

	/** store qun infos.
	 *
	 * \sa Fetion_Qun  DList  fx_get_qunlist
	 */
	DList* qun;
	/** amount accounts. */
	int account_count;
}Fetion_BList;

	void free_fetion_personal(Fetion_Personal *personal);
	void free_fetion_account(Fetion_Account *account);
	void free_fetion_group(Fetion_Group *group);

	Fetion_Personal *copy_fetion_personal(const Fetion_Personal *src);
	Fetion_Account *copy_fetion_account(const Fetion_Account *src);
	Fetion_Group *copy_fetion_group(const Fetion_Group *src);

//	Fetion_BList *get_fetion_blist();

	void free_fetion_blist(Fetion_BList *blist);

	void fetion_blist_add_group(Fetion_BList * blist,const Fetion_Group *group);
	void fetion_blist_add_account(Fetion_BList * blist,const Fetion_Account *account);

	DList *fetion_blist_get_group(Fetion_BList * blist);
	DList *fetion_blist_get_account(Fetion_BList * blist);

	Fetion_Account *fetion_blist_get_account_by_id(Fetion_BList * blist, long id);

	const char *fetion_blist_get_uri_by_id(Fetion_BList * blist, long id);

	//misc....
#define AUTH_WAIT  0
#define AUTH_OK    1
#define AUTH_REFUS 2

/** @} end of fetion_blist */
#ifdef __cplusplus 
} 
#endif 

#endif /* #ifndef __LIBFETION_LIB__ */

#endif /* _LIBFETION_DATASTRUCT_H_ */
