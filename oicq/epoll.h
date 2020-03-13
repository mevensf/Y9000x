#ifndef _EPOLL_H_
#define _EPOLL_H_

#include"sock.h"
#include<sys/epoll.h>
#include"mysql.h"

typedef struct
{
		Info *head;
		Info *temp;
}Arg;

void epoll_inet(int epfd,int fd);

void read_choose(Info *temp,Info *head,MYSQL *conn,int epfd,int type);

void write_news(Info*temp,News ch);

void write_list(Info*temp,Info *head,News ch);

void read_reg(Info *temp,Info *head,MYSQL *conn,int epfd);//注册账户，将信息填入数据库

void read_log(Info *temp,Info *head,MYSQL *conn);//登录账户，比较密码

int find_port(Info *head,News ch);//1对1发信息找端口

void read_news(Info *temp,Info *head);
		
void  read_heart(void *arg);

void heart(Info *temp,Info *head);

#endif
