#include"sock.h"


void my_perror(int ret,char *a)
{
	if(ret <0)
	{
		perror(a);
		exit(-1);
	}
}

void read_close(Info *temp,Info *head)//客户断开
{
	char buff[128];
	memset(buff,0x00,sizeof(buff));

	printf("clifd %d [%s|%d]断开链接!\n",temp->clifd,temp->ip,temp->port);
	sprintf(buff,"clifd %d [%s|%d]断开链接\n",temp->clifd,temp->ip,temp->port);
	write_file(buff);

	del_list(&head,temp->clifd);//删除用户链表
	close(temp->clifd);//关闭套接字
}


void read_perror(char *a)
{
	perror(a);
	exit(-1);
}

int sock_inet()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	my_perror(sockfd,"socket");

	return sockfd;
}

struct sockaddr_in addr_inet()//
{
	struct sockaddr_in seraddr;

	seraddr.sin_family =AF_INET;
	seraddr.sin_port =htons(61000);
	inet_pton(AF_INET,"192.168.1.107",&seraddr.sin_addr.s_addr);

	return seraddr;
}

int sock_listen(int sockfd,struct sockaddr_in seraddr)//绑定
{
	int ret = bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
        if(ret<0)
        {
	   my_perror(ret,"bind");
        }

	ret = listen(sockfd,20);
        if(ret<0)
        {
	   my_perror(ret,"listen");
        }

	return sockfd;
}


Pack* packet_reg(Pack *pa,int type,Reg ch)//注册包
{
	pa =(Pack *) malloc(sizeof(Pack)+sizeof(Reg));
	pa->type =type;
	pa->size =sizeof(Reg);
	memcpy(pa->data,&ch,pa->size);
	return pa;
}

Pack* packet_log(Pack *pa,int type,Log ch)//登录包
{
	pa =(Pack *) malloc(sizeof(Pack)+sizeof(Log));
	pa->type =type;
	pa->size =sizeof(Log);
	memcpy(pa->data,&ch,pa->size);
	return pa;
}

Pack* packet_news(Pack *pa,int type,News ch)//信息包
{
	pa =(Pack *) malloc(sizeof(Pack)+sizeof(News));
	pa->type =type;
	pa->size =sizeof(News);
	memcpy(pa->data,&ch,pa->size);
	return pa;
}

Pack* packet_heart(Pack *pa,int type)//heartbag
{
	pa =(Pack *) malloc(sizeof(Pack));
	pa->type =type;
	return pa;
}


/*
   Pack* packet_chat(Pack *pa,int type,Chat ch)
   {
   pa =(Pack *) malloc(sizeof(Pack)+sizeof(Chat));
   pa->type =type;
   pa->size =sizeof(Chat);
   memcpy(pa->data,&ch,pa->size);
   return pa;
   }

*/




