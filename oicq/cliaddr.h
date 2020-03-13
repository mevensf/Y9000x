#ifndef _CLIADDR_H_
#define _CLIADDR_H_

#include"sock.h"
#include<signal.h>

#define MENU "****************welcome to OICQ****************\n"\
"---               1.注册账户                ---\n"\
"---               2.登录账户                ---\n"\
"---               0.退出账户                ---\n"\
"***********************************************\n"\
"请输入选项\t"

#define FUN "****************welcome to OICQ****************\n"\
"---               1.发送信息                ---\n"\
"---               2.群发信息                ---\n"\
"---               0.退出                    ---\n"\
"***********************************************\n"\
"请输入选项\t"


typedef struct 
{
	struct sockaddr_in readaddr;
	int readfd;
}Read;


void fun();

void my_char();

void my_scanf(char *buff);

void cli_scanf(char *name,char *passwd);

void regist(int sockfd);//注册账户

//****************************************udp协议
void my_sendto(int udpfd,struct sockaddr_in cliaddr,char *buff);//发信息

void my_recvfrom(void *read);//读信息
//×××××××××××××××××××××××××××××××××××××××××××××××××××

//*********************************************1对1聊天
void cli_write_news(int sockfd,News ch);

void cli_mynews(int sockfd,Log log);

int  cli_read_news(int sockfd,struct sockaddr_in *cliaddr);

//×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××

void  per_fun(int sockfd,struct sockaddr_in cliaddr);

void login(int sockfd,struct sockaddr_in cliaddr);//登录账户

#endif
