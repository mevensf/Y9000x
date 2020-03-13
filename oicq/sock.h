#ifndef _SOCK_H_
#define _SOCK_H_

#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"list.h"
#include"file.h"

#define REG 1
#define LOG 2
#define NEWS 3
#define HEART 4
//#define CHAT 5

typedef struct pack
{
	int type;
	int size;
	char data[0];
}Pack;

typedef struct regist//注册包
{
	unsigned int id;
	char name[32];
	char passwd[32];
	char ip[64];
}Reg;

typedef struct login//登录包
{
	unsigned int id;
	char passwd[32];
}Log;

typedef struct news//信息接收者端口包
{
	int id;
}News;
/*
   typedef struct heart//心跳包
   {
   int buff;
   }Heart;


   typedef struct //服务端发信息包
   {
   char buff[32];
   }Chat;
 */

void my_perror(int ret,char *a);

void read_close(Info *temp,Info *head);

void read_perror(char *a);

int sock_inet();

struct sockaddr_in addr_inet();//ip 与 端口号

int sock_listen(int sockfd,struct sockaddr_in seraddr);

Pack* packet_reg(Pack *pa,int type,Reg ch);

Pack* packet_log(Pack *pa,int type,Log ch);

Pack* packet_news(Pack *pa,int type,News ch);

Pack* packet_heart(Pack *pa,int type);

//Pack* packet_chat(Pack *pa,int type,Chat ch);

//Pack* packet_list(Pack *pa,int type,Node ch);

#endif
