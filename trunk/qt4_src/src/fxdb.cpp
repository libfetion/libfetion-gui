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
#include "fxdb.h"
#include "fxEncrypt.h"
//#include "appconfig.h"

#include <QTextStream>
#include <QFile>
#include "fxResource.h"

#define AUTOLG   "autologin021"
#define SERVERADD   "cacheserveradd"
#define PROXY   "proxy"
#define ACINFO   "acinfo"

#define SQL_MAXLEN 1024*3
#define MAX_PATH 512


/********************************
 *
 * table  AUTOLG
 *
 *	flag
 *		1  auto login
 *		0  no
 *  id
 *		the fetion id
 *  len_id
 *		the len of fetion's id
 *  pwd
 *      the password
 *  len_pwd
 *      the len of the password
 *
 * login_state
 *     login state..(add by iptton)
 *
 *
 * table  SERVERADD
 *
 *	id
 *		the id of fetion
 *	proxy
 *		the proxy of the fetion server
 *
 *
 * table fx****usr
 * uid id of opter
 * msg message connect , it is base64, and XOR
 * msg_len msg 's len
 * date_time  date of message
 *
 *
 * table PROXY
 * type
 * host
 * port
 * name
 * pwd
 *
 * table ACINFO	
 * id
 *		the id of fetion
 * mobilenm
 *      the mobile number of
 * local_name
 *      the account's local_name
 * nickname
 *      the account's nickname
 * impresa
 *      the account's impresa
 * showname
 *      the account's showname
 * ******************************/

sqlite3 *pdb = NULL;

//global share sql variable
char sql[SQL_MAXLEN] =
{
    0
};

bool init_db()
{
    char **result;
    char *perrmsg;
    int nrow, ncol;

    if (pdb)
    {
        return true;
    }

    //open db
    if (sqlite3_open(chatDBFile().toUtf8().data(), &pdb) != SQLITE_OK)
    {
        pdb = NULL;
        return false;
    }

    // Are there DBTABLE ? if not, create it.
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "select * from %s limit 1", AUTOLG);
    if (sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg) !=
        SQLITE_OK)
    {
        //create the table
        memset(sql, 0, SQL_MAXLEN);
        sprintf(sql,
                "create table %s(flag, id, len_id, pwd, len_pwd, login_state)",
                AUTOLG);
        sqlite3_exec(pdb, sql, 0, 0, &perrmsg);
    }
    sqlite3_free_table(result);

    // Are there DBTABLE ? if not, create it.
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "select * from %s limit 1", SERVERADD);
    if (sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg) !=
        SQLITE_OK)
    {
        //create the table
        memset(sql, 0, SQL_MAXLEN);
        sprintf(sql, "create table %s(id,proxy)", SERVERADD);
        sqlite3_exec(pdb, sql, 0, 0, &perrmsg);
    }

    sqlite3_free_table(result);

    // Are there PROXY ? if not, create it.
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "select * from %s limit 1", PROXY);
    if (sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg) !=
        SQLITE_OK)
    {
        //create the table
        memset(sql, 0, SQL_MAXLEN);
        sprintf(sql, "create table %s(type, host, port, name, pwd)", PROXY);
        sqlite3_exec(pdb, sql, 0, 0, &perrmsg);
    }
    sqlite3_free_table(result);

    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void destoy_db()
{
    if (pdb)
    {
        sqlite3_close(pdb);
    }
    pdb = NULL;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool isAutoLogin(char **id, char **pwd, int *loginState)
{
    if (!init_db())
    {
        return false;
    }

    char **result;
    char *perrmsg;
    int nrow, ncol;
    int ret;

    // select url from databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql,
            "select id, pwd, len_id, len_pwd, login_state from %s where flag=\"1\" ", AUTOLG);
    ret = sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg);
    if (ret != SQLITE_OK)
    {
        return false;
    }

    bool res = false;

    if (nrow)
    {
        res = true;

        if (id && pwd)
        {
            *id = encryPWD(result[5], strlen(result[5]), false);
            *pwd = encryPWD(result[6], strlen(result[6]), false);
            int len_id = atoi(result[7]);
            int len_pwd = atoi(result[8]);
            (*id)[len_id] = '\0';
            (*pwd)[len_pwd] = '\0';

            if (loginState)
            {
                if (result[9])
                {
                    *loginState = atoi(result[9]);
                }
                else
                {
                    *loginState = 0;
                }
            }
        }
    }

    sqlite3_free_table(result);
    return res;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void disAutoLogin()
{
    if (!init_db())
    {
        return ;
    }

    // select url from databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "delete from %s", AUTOLG);

    //exec the delete table sql
    sqlite3_exec(pdb, sql, 0, 0, 0);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool setAutoLogin(const char *id, const char *pwd, int login_state)
{
    if (!id || !pwd)
    {
        return false;
    }

    disAutoLogin();

    char *en_id = encryPWD(id, strlen(id), true);
    char *en_pwd = encryPWD(pwd, strlen(pwd), true);

    if (!en_id || !en_pwd)
    {
        return false;
    }

    // insert data to databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql,
            "insert into %s values(\"1\", \"%s\", \"%d\", \"%s\", \"%d\", \"%d\")", AUTOLG, en_id, strlen(id), en_pwd, strlen(pwd), login_state);
    sqlite3_exec(pdb, sql, 0, 0, 0);

    if (en_id)
    {
        free(en_id);
    }
    if (en_pwd)
    {
        free(en_pwd);
    }
    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

char *getCacheServerAdd(const char *id)
{
    if (!init_db())
    {
        return false;
    }

    char *proxy = NULL;

    char **result;
    char *perrmsg;
    int nrow, ncol;
    int ret;

    // select url from databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "select proxy from %s where id=\"%s\" ", SERVERADD, id);
    ret = sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg);
    if (ret != SQLITE_OK)
    {
        return false;
    }

    if (nrow)
    {
        proxy = (char*)malloc(strlen(result[1]) + 1);
        memset(proxy, 0, strlen(result[1]) + 1);
        strcpy(proxy, result[1]);
    }

    sqlite3_free_table(result);
    return proxy;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void disProxy()
{
    if (!init_db())
    {
        return ;
    }

    // select url from databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "delete from %s", PROXY);

    //exec the delete table sql
    sqlite3_exec(pdb, sql, 0, 0, 0);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool isSetProxy(PROXY_ITEM *proxy_item)
{
    if (!init_db())
    {
        return false;
    }

    char **result;
    char *perrmsg;
    int nrow, ncol;
    int ret;

    // select url from databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "select type, host, port, name, pwd from %s", PROXY);
    ret = sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg);
    if (ret != SQLITE_OK)
    {
        return false;
    }

    bool res = false;

    if (nrow)
    {
        res = true;

        if (proxy_item)
        {
            proxy_item->host = strdup(result[6]);
            proxy_item->port = strdup(result[7]);
            //bad coding... fuck!!!!
            if (atoi(result[8]) == 10 && atoi(result[9]) == 10)
            {
                proxy_item->name = NULL;
                proxy_item->pwd = NULL;
            }
            else
            {
                proxy_item->name = strdup(result[8]);
                proxy_item->pwd = strdup(result[9]);
            }

            switch (atoi(result[5]))
            {
                case 0:
                    proxy_item->type = PROXY_DIRECT;
                    break;
                case 1:
                    proxy_item->type = PROXY_HTTP;
                    break;
                case 2:
                    proxy_item->type = PROXY_SOCKS4;
                    break;
                case 3:
                    proxy_item->type = PROXY_SOCKS5;
                    break;
                case 4:
                    proxy_item->type = PROXY_MAX;
                    break;
            }
        }
    }
    sqlite3_free_table(result);
    return res;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool setProxy(PROXY_ITEM *proxy_item)
{
    if (!proxy_item || !init_db())
    {
        return false;
    }

    disProxy();
    // insert data to databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "insert into %s values(\"%d\", \"%s\", \"%s\", \"%s\", \"%s\")",
            PROXY, proxy_item->type,
            proxy_item->host, proxy_item->port,
            proxy_item->name ? proxy_item->name: "10",
            proxy_item->pwd ? proxy_item->pwd: "10");
    sqlite3_exec(pdb, sql, 0, 0, 0);

    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/



bool setCacheServerAdd(const char *id, const char *prxy)
{
    if (!init_db())
    {
        return false;
    }

    // select url from databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "delete from %s where id =\"%s\"", SERVERADD, id);
    //exec the delete table sql
    sqlite3_exec(pdb, sql, 0, 0, 0);

    // insert data to databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "insert into %s values(\"%s\", \"%s\")", SERVERADD, id, prxy);
    sqlite3_exec(pdb, sql, 0, 0, 0);

    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void cleanCacheServerAdd()
{
    if (!init_db())
    {
        return ;
    }

    // select url from databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "delete from %s ", SERVERADD);
    //exec the delete table sql
    sqlite3_exec(pdb, sql, 0, 0, 0);

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString create_result(char **result, int nrow, int ncol);

BOOL init_histroy_db(long usr)
{
    static BOOL have_init = FALSE;

    if (!init_db())
    {
        return FALSE;
    }

    if (have_init)
    {
        return TRUE;
    }
    char **result;
    char *perrmsg;
    int nrow, ncol;
    // Are there DBTABLE ? if not, create it.
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "select * from fx%ldusr limit 1", usr);

    if (sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg) !=
        SQLITE_OK)
    {

        //create the table
        memset(sql, 0, SQL_MAXLEN);
        sprintf(sql,
                "create table fx%ldusr(uid,msg text, msg_len, date_time NOT NULL DEFAULT CURRENT_TIMESTAMP)", usr);

        //exec the create table sql
        if (sqlite3_exec(pdb, sql, 0, 0, &perrmsg) != SQLITE_OK)
        {
            printf("exel create table %s\n", sql);
            return FALSE;
        }
        sqlite3_free_table(result);
        //creat index
        memset(sql, 0, SQL_MAXLEN);
        sprintf(sql, "create index uid_index on fx%ldusr(uid)", usr);

        if (sqlite3_exec(pdb, sql, 0, 0, &perrmsg) != SQLITE_OK)
        {
            printf("exel create index %s\n", sql);
            return FALSE;
        }
        else
        {
            sqlite3_free_table(result);
        }

    }
    have_init = TRUE;
    return TRUE;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void saveHistroyMsg(long usr, long uid, const char *msg, const char *date)
{
    if (!init_histroy_db(usr))
    {
        printf("init_histrou fial \n");
        return ;
    }
    if (!msg)
    {
        return ;
    }

    int msg_len = strlen(msg);
    char *base64_msg = encryptXOR(msg, msg_len, KEY);
    if (!base64_msg)
    {
        return ;
    }

    //memset(sql, 0, sql_maxlen);
    int len = strlen(base64_msg) + 100;
    char *sql = (char*)malloc(sizeof(char) *len);
    memset(sql, 0, len);

    if (!date)
    {
        sprintf(sql,"insert into fx%ldusr values(\"%ld\",\"%s\",\"%d\", datetime(CURRENT_TIMESTAMP,'localtime'))", 
                usr, uid, base64_msg, msg_len);
    }
    else
    {
        sprintf(sql, "insert into fx%ldusr values(\"%ld\",\"%s\",\"%d\",\"%s\")",
                usr, uid, base64_msg, msg_len, date);
    }

    if (sqlite3_exec(pdb, sql, 0, 0, 0) != SQLITE_OK)
    {
        printf("it is error %s\n", sql);
    }
    if (sql)
    {
        free(sql);
    }
    if (base64_msg)
    {
        free(base64_msg);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

int export_history(long uid, const char *filename)
{
    char **result;
    char *perrmsg;
    int nrow, ncol;

    QString htm_name(filename);
    QFile htm(htm_name);
    if (!htm.open(QFile::WriteOnly | QFile::Truncate))
    {
        printf("can not open file \n");
        return 2;
    }

    if (!init_histroy_db(uid))
    {
        printf("can not find the user \n");
        return 1;
    }

    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "select * from fx%ldusr", uid);
    if (SQLITE_OK != sqlite3_get_table(pdb, sql, &result, &nrow, &ncol,
        &perrmsg))
    {
        printf("slecet flag  table error!, the sql is...%s\n", sql);
        return  - 1;
    }

    QString res = create_result(result, nrow, ncol);
    sqlite3_free_table(result);

    QTextStream out(&htm);
    out <<
        "<html><head><meta http-equiv=Content-Type content=\"text/html;charset=gb2312\"><title>LibFx chatlog</title></head><body>" << res << "</body></html>";

    return 0;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool clean_history(long uid)
{
    char **result;
    char *perrmsg;
    int nrow, ncol;

    if (!init_db())
    {
        return false;
    }
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "delete from fx%ldusr", uid);
    if (SQLITE_OK != sqlite3_get_table(pdb, sql, &result, &nrow, &ncol,
        &perrmsg))
    {
        printf("slecet flag  table error!, the sql is...%s\n", sql);
        return false;
    }
    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


#if 0
    int type = 1  //today
    int type = 2  //one week
    int type = 3  //two week
    int type = 4  //one month
    int type = 5  //all
#endif

char *create_sql_for_histroy(long usr, long uid, HISTORY_DATE type)
{
    char *sql = (char*)malloc(sizeof(char) *1024);
    memset(sql, 0, 1024);
    switch (type)
    {
        case TODAY_HISTORY:
            sprintf(sql,
                    "select * from fx%ldusr where julianday(date(datetime(CURRENT_TIMESTAMP,'localtime'))) - julianday(date(date_time)) == 0 and uid == \"%ld\" order by date_time asc", usr, uid);
            break;
        case ONE_WEEK_HISTORY:
            sprintf(sql,
                    "select * from fx%ldusr where julianday(date(datetime(CURRENT_TIMESTAMP,'localtime'))) - julianday(date(date_time)) <= 7 and uid == \"%ld\"  order by date_time asc", usr, uid);
            break;
        case TWO_WEEK_HISTORY:
            sprintf(sql,
                    "select * from fx%ldusr where julianday(date(datetime(CURRENT_TIMESTAMP,'localtime'))) - julianday(date(date_time)) <= 14  and uid == \"%ld\"  order by date_time asc", usr, uid);
            break;
        case ONE_MONTH_HISTORY:
            sprintf(sql,
                    "select * from fx%ldusr where julianday(date(datetime(CURRENT_TIMESTAMP,'localtime'))) - julianday(date(date_time)) <= 30  and uid == \"%ld\"  order by date_time asc", usr, uid);
            break;
        case ALL_DATE_HISTORY:
            sprintf(sql,
                    "select * from fx%ldusr where uid == \"%ld\" order by date_time asc", usr, uid);
            break;
        default:
            return NULL;
    }
    return sql;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString selectHistroyMsg(long usr, long uid, HISTORY_DATE type)
{
    QString res;
    if (!init_histroy_db(usr))
    {
        return NULL;
    }

    int ret, nrow, ncol;
    char *perrmsg;
    char **result;

    char *sql = create_sql_for_histroy(usr, uid, type);
    if (!sql)
    {
        return NULL;
    }

    ret = sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg);
    if (ret != SQLITE_OK)
    {
        printf("slecet flag  table error!, the sql is...%s\n", sql);
        return NULL;
    }

    res = create_result(result, nrow, ncol);

    sqlite3_free_table(result);
    if (sql)
    {
        free(sql);
    }

    return res;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString create_result(char **result, int nrow, int ncol)
{
    /*
     * uid result[index]
     * msg result[index+1]
     * msg_len result[index+2]
     * date_time result[index+3]
     */
    QString res;
    int index = ncol;
    for (int i = 0; i < nrow; i++)
    {
        char *msg,  *date;
        int msg_len = atoi(result[index + 2]);
        msg = decryptXOR(result[index + 1], msg_len, KEY);
        date = result[index + 4];

        /*printf("get the  result %s,msg_len is %d msg is %s \n", result [index+3], msg_len, msg);
        char *tmp = (char*)malloc(sizeof(char)* (msg_len+1));
        memcpy(tmp, msg, msg_len);
        tmp[msg_len] = '\0';
        printf("the tmp msg is %s \n", tmp);
        if (tmp)
        free(tmp);
         */
        res += QString::fromUtf8(msg) + "<br>";


        if (msg)
        {
            free(msg);
        }

        index = index + ncol;
    }
    return res;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool selectSystemMsg(long usr, long uid, const char *msg)
{
    if (!msg || !init_histroy_db(usr))
    {
        return false;
    }

    bool ret = false;
    int nrow, ncol;
    char *perrmsg;
    char **result;
    long len_msg = strlen(msg);

    //fixme: here is simple to compile of the msg's len...
    //maybe it is not very good, but for system message(not important) it is enough..
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql,
            "select * from fx%ldusr where msg_len == \"%ld\" and uid == \"%ld\" ", usr, len_msg, uid);

    if (sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg) !=
        SQLITE_OK)
    {
        printf("slecet system table error!, the sql is...%s\n", sql);
        return false;
    }
    sqlite3_free_table(result);
    if (nrow)
    {
        ret = true;
    }

    // when system msg over 20, we will clean system msg
    {
        memset(sql, 0, SQL_MAXLEN);
        sprintf(sql, "select * from fx%ldusr where uid == \"%ld\" ", usr, uid);
        if (sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg) !=
            SQLITE_OK)
        {
            return ret;
        }
        sqlite3_free_table(result);
        if (nrow >= 20)
        {
            printf("haha , i will delete all system msg \n");
            memset(sql, 0, SQL_MAXLEN);
            sprintf(sql, "delete from fx%ldusr where uid == \"%ld\" ", usr, uid)
                    ;
            //exec the delete table sql
            sqlite3_exec(pdb, sql, 0, 0, 0);
        }

    }

    return ret;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

BOOL init_storeAccountInfo_db(long usr)
{
    static BOOL have_init = FALSE;
    if (!init_db())
    {
        return FALSE;
    }
    if (have_init)
    {
        return TRUE;
    }
    char *perrmsg;

    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "DROP TABLE IF EXISTS fxACINFO%ldusr ", usr);
    sqlite3_exec(pdb, sql, 0, 0, &perrmsg);

    //create the table
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql,
            "create table fxACINFO%ldusr(uid, online_state, mobilenm, local_name, nickname, impresa, showname)", usr);
    //exec the create table sql
    if (sqlite3_exec(pdb, sql, 0, 0, &perrmsg) != SQLITE_OK)
    {
        printf("exel create table %s\n", sql);
        return FALSE;
    }

    //creat index
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql, "create index uid_index on fxACINFO%ldusr(uid)", usr);
    sqlite3_exec(pdb, sql, 0, 0, &perrmsg);

    have_init = TRUE;
    return TRUE;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void saveAccountToDB(const Fetion_Account *account, long usr)
{
    if (!account || account->id == usr)
    {
        return ;
    }

    Fetion_Personal *personal = account->personal;
    long uid = account->id;
    int online_state;
    char *local_name;
    char *mobilenm;
    char *nickname;
    char *impresa;
    char *showname;

    online_state = fx_get_online_status_by_account(account);
    if (account->local_name)
    {
        local_name = account->local_name;
    }
    else
    {
        local_name = "";
    }

    if (personal && personal->mobile_no)
    {
        mobilenm = personal->mobile_no;
    }
    else
    {
        mobilenm = "";
    }

    if (personal && personal->nickname)
    {
        nickname = personal->nickname;
    }
    else
    {
        nickname = "";
    }

    if (personal && personal->impresa)
    {
        impresa = personal->impresa;
    }
    else
    {
        impresa = "";
    }

    showname = fx_get_account_show_name(account, TRUE);
    // insert data to databse
    memset(sql, 0, SQL_MAXLEN);
    sprintf(sql,
            "insert into fxACINFO%ldusr values(\"%ld\", \"%d\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")", usr, uid, online_state, mobilenm, local_name, nickname, impresa, showname);

    sqlite3_exec(pdb, sql, 0, 0, 0);

    if (showname)
    {
        free(showname);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void saveAccountInfo()
{
    char *perrmsg;
    long usr = (qlonglong)strtol(fx_get_usr_uid(), NULL, 10);

    if (!init_db() || !init_storeAccountInfo_db(usr))
    {
        return ;
    }

    sqlite3_exec(pdb, "begin transaction", 0, 0, &perrmsg);

    const Fetion_Account *account = fx_get_first_account();
    while (account)
    {
        saveAccountToDB(account, usr);
        account = fx_get_next_account(account);
    }

    sqlite3_exec(pdb, "commit transaction", 0, 0, &perrmsg);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void create_search_result(QList < QTreeWidgetItem * >  * items, char **result,
                          int nrow, int ncol);
QList < QTreeWidgetItem * >  * searchAccountInfo(char *keyword)
{
    QList < QTreeWidgetItem * >  * items = new QList < QTreeWidgetItem * > ;
    if (!init_db())
    {
        return items;
    }
    long usr = (qlonglong)strtol(fx_get_usr_uid(), NULL, 10);


    int ret, nrow, ncol;
    char *perrmsg;
    char **result;

    sprintf(sql,
            "select * from fxACINFO%ldusr where uid like \'%s%%\' or mobilenm like \'%s%%\' or local_name like \'%%%s%%\' or  nickname like \'%%%s%%\' or impresa like \'%%%s%%\'", usr, keyword, keyword, keyword, keyword, keyword);

    ret = sqlite3_get_table(pdb, sql, &result, &nrow, &ncol, &perrmsg);
    if (ret != SQLITE_OK)
    {
        printf("slecet flag  table error!, the sql is...%s\n", sql);
        return items;
    }

    create_search_result(items, result, nrow, ncol);
    sqlite3_free_table(result);

    return items;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void create_search_result(QList < QTreeWidgetItem * >  * items, char **result,
                          int nrow, int ncol)
{
    //	QList<QTreeWidgetItem *> items;
    /*
    long uid;
    int online_state;
    char *local_name;
    char *mobilenm;
    char *nickname;
    char *impresa;
    char *showname;
     */
    //uid, online_state, mobilenm, local_name, nickname, impresa
    /*
     * uid result[index]
     * online_state result[index+1]
     * mobilenm result[index+2]
     * local_name result[index+3]
     * nickname result[index+4]
     * impresa result[index+5]
     * showname result[index+6]
     */
    QString res;
    int index = ncol;
    for (int i = 0; i < nrow; i++)
    {
        Account_Info *ac_info = new Account_Info;
        ac_info->accountID = atol(result[index]);
        ac_info->onlinestate = atoi(result[index + 1]);
        ac_info->accountName = QString::fromUtf8(result[index + 6]);

        QTreeWidgetItem *accountItem = new QTreeWidgetItem;
        accountItem->setText(0, ac_info->accountName);

        #if MS_VC6
            QVariant Var((uint)ac_info);
        #else
            QVariant Var;
            Var.setValue(ac_info); //vc7 up
        #endif
        accountItem->setData(0, Qt::UserRole, Var);

        accountItem->setIcon(0, getOnlineStatusIcon(ac_info->onlinestate));


        QString tips;
        QString tmp;
        QString info;

        if (fx_is_pc_user_by_id(ac_info->accountID))
        {
            info = QObject::tr("fetion_no:");
            info += "<b style=\"color:red; \">" + QString("%1").arg(ac_info
                ->accountID) + "</b>";
            tips += info + "<br>";
        }

        tmp = QString::fromUtf8(result[index + 2]);
        if (!tmp.isEmpty())
        {
            info = QObject::tr("mobile no:");
            info += "<b style=\"color:red; \">" + tmp + "</b>";
            tips += info + "<br>";
        }
        tmp = QString::fromUtf8(result[index + 3]);
        if (!tmp.isEmpty())
        {
            info = QObject::tr("local_name:");
            info += "<b style=\"color:red; \">" + tmp + "</b>";
            tips += info + "<br>";
        }
        tmp = QString::fromUtf8(result[index + 4]);
        if (!tmp.isEmpty())
        {
            info = QObject::tr("nickname:");
            info += "<b style=\"color:red; \">" + tmp + "</b>";
            tips += info + "<br>";
        }

        tmp = QString::fromUtf8(result[index + 5]);
        if (!tmp.isEmpty())
        {
            info = QObject::tr("impresa:");
            info += "<b style=\"color:red; \">" + tmp + "</b>";
            tips += info + "<br>";
        }

        //remov the last "<br>"
        tips = tips.remove(tips.size() - strlen("<br>"), strlen("<br>"));
        accountItem->setToolTip(0, tips);

        index = index + ncol;
        items->append(accountItem);
    }
    //	printf("the nrow is %d ncol is %d \n", nrow, ncol);
    //return items;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


#if DEBUG_GUI
    char buf_sd[1024 *10];

    void qt_debug(char *fmt, ...)
    {
        int tmp;
        va_list va;
        va_start(va, fmt);
        #ifdef WIN32
            tmp = _vsnprintf(buf_sd, sizeof(buf_sd) - 1, fmt, va);
        #else
            tmp = vsnprintf(buf_sd, sizeof(buf_sd) - 1, fmt, va);
        #endif
        buf_sd[tmp] = '\0';
        va_end(va);

        #ifdef WIN32
            static int isInit = 0;
            if (!isInit)
            {
                //	open_log();
                AllocConsole();
                isInit = 1;
            }

            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD n, m;
            WriteConsoleA(h, buf_sd, strlen(buf_sd), &n, &m); //call non-unicode
            // logit(buf_sd);

            #if _MSC_VER > 1000
                //	::OutputDebugString(buf);
            #endif
        #endif

        printf("%s", buf_sd);
        //	printf("--[%s,%s,%d]--%s", __FUNCTION__, __FILE__, __LINE__, buf);	
    }
#endif
