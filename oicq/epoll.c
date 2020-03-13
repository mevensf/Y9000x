#include"epoll.h"

void epoll_inet(int epfd,int fd)//将新的fd 添加到epfd
{
	struct epoll_event ev;

	ev.events =EPOLLIN;
	ev.data.fd = fd;

	epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);

}

void read_choose(Info *temp,Info *head,MYSQL *conn,int epfd,int type)//操作选择
{
	switch(type)
	{
		case REG:
			read_reg(temp,head,conn,epfd);//注册
			break;
		case LOG:
			read_log(temp,head,conn);//登录
			break;
		case NEWS:
			read_news(temp,head);//读信息
			break;
		case HEART:
			printf("heartbeat...\n");//心跳
			temp->time = 10;
	}
}


void write_news(Info *temp,News ch)//
{
	Pack *wpa = NULL;
	wpa = packet_news(wpa,NEWS,ch);

	int ret = write(temp->clifd,wpa,sizeof(Pack)+sizeof(News));
	my_perror(ret,"write");
}

void write_list(Info *temp,Info *head,News ch)//信息存入链表
{
	Info *cli = head;
	show_list(cli);//遍历
	ch.id = len_list(cli);
	write_news(temp,ch);//

	while(cli !=NULL)
	{
		if(cli->port == temp->port)
		{
			cli = cli->next;
			continue;
		}
		ch.id = cli->port;
		printf("客户的port = %d\t客户的id = %d\n",cli->port,ch.id);

		write_news(temp,ch);//
		printf("qun_write\n");

		cli = cli->next;
	}
	printf("list_end\n");
}

void read_reg(Info *temp,Info *head,MYSQL *conn,int epfd)//注册账户，将信息填入数据库
{
	char sql[128];
	Reg ch;
	int ret =read(temp->clifd,&ch,sizeof(Reg));
	printf("注册 reg=%d\n",ret);
//	printf("read_reg ret %d ,%d\n",ret,__LINE__);
	if(ret <0)
	{
		read_perror("read_reg");
	}
	else if(ret ==0)
	{
		read_close(temp,head);
		epoll_ctl(epfd,EPOLL_CTL_DEL,temp->clifd,NULL);
	}
	else
	{
		//printf("add_per\n");

		add_per(conn,sql,ch);//增加客户

		printf("注册账户 :  %s\n",ch.name);

		char buff[128];
		memset(buff,0x00,sizeof(buff));
		sprintf(buff,"%s注册账户",ch.name);
		write_file(buff);//写日志文件
		//epoll_ctl(epfd,EPOLL_CTL_DEL,temp->clifd,NULL);

	}
}



void read_log(Info *temp,Info *head,MYSQL *conn)//登录账户，比较密码
{
	Pack *wpa = NULL;
	char sql[128];
	Log ch;
	int ret =read(temp->clifd,&ch,sizeof(Log));
	printf("登录   ret= %d\n",ret);
//	printf("read_log ret = %d,line %d \n",ret,__LINE__);

	if(ret <0)
	{
		read_perror("read_reg");
	}
	else if(ret ==0)
	{
		read_close(temp,head);
	}
	else
	{
		temp->id = ch.id;
		find_per(conn,sql,ch);//寻找匹配对象
		strcpy(ch.passwd,sql);
		wpa = packet_log(wpa,LOG,ch);
		ret = write(temp->clifd,wpa,sizeof(Pack)+sizeof(Log));
		my_perror(ret,"log_write");

		char buff[128];
		memset(buff,0x00,sizeof(buff));

		printf("登录账户：%d\n",ch.id);

		sprintf(buff,"\t%d登录账户\t",ch.id);
		write_file(buff);//写入日志文件
	}
}


int find_port(Info *head,News ch)//1对1发信息找端口
{
	char buff[128];
	memset(buff,0x00,sizeof(buff));

	Info *temp =head;
	while(temp !=NULL)
	{
		if(temp->id == ch.id)
		{
			return temp->port;
		}

		temp =temp->next;
	}
	sprintf(buff,"\t没有找到用户\t");//???
	write_file(buff);
	return 0;

}

void read_news(Info *temp,Info *head)//发信息所需的端口号
{
	News ch;

	int ret =read(temp->clifd,&ch,sizeof(News));
	printf("信息  ret= %d\n",ret);
//	printf("read_news  ret = %d\t%d\n",ret,__LINE__);

	if(ret <0)
	{
		read_perror("read_reg");
	}
	else if(ret ==0)
	{
		read_close(temp,head);
	}
	else
	{
		char buff[128];
		memset(buff,0x00,sizeof(buff));
		if(ch.id == 0)
		{
			printf("[%d   群发信息...]\n",temp->id);
			write_list(temp,head,ch);
			sprintf(buff,"%d 群发信息\t",temp->id);
			write_file(buff);
			return ;
		}
		else
		{
			//printf("temp->id = %d\tch.id = %d\n",temp->id,ch.id);
			if(ch.id ==temp->id)
			{
				printf("[%d   寻找自己端口...]\n",ch.id);
				sprintf(buff,"\t%d寻找自己端口\t",ch.id);
			}
			else
			{
				printf("[%d   发信息给:  %d]\n",temp->id,ch.id);
				sprintf(buff,"\t%d发信息给%d\t",temp->id,ch.id);
			}

			write_file(buff);
			ch.id = find_port(head,ch);//寻找端口
			write_news(temp,ch);//发送信息

			//printf("%d\n",__LINE__);
		}
	}
}
/*

 */
void heart(Info *temp,Info *head)
{
	Info *head2 = head;
	while(1)
	{
		while(head2 !=NULL)
		{
			(head->time)--;
			head = head->next;
		}
		/*
		   if(read_heart(heart.temp,heart.head) ==1)
		   {
		   heart.temp->time = 10;
		   }
		 */
		if(temp->time == 0)
		{
			read_close(temp,head);
		}
		sleep(1);
	}
}





