#ifndef _MYSQL_H_
#define _MYSQL_H_

#include"sock.h"
#include<mysql/mysql.h>
#include"file.h"

void my_connect(MYSQL *conn);//链接数据库


void my_query(MYSQL *conn,char *sql);//发送信息(运行库)

//void creat_database(MYSQL *conn,char *sql);//创建tb_per表

void add_per(MYSQL *conn,char *sql,Reg ch);//用户注册

void find_per(MYSQL *conn,char *sql,Log ch);

void find_max(MYSQL *conn,char *sql,Reg *ch);

#endif
