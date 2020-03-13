#include"cliaddr.h"

void fun()//信号接收函数
{
	printf("服务端断开!\n");
	exit(-1);
}

void my_char()
{
	while(getchar() !='\n');
}

void my_scanf(char *buff)
{
	printf("请输入要发送的信息:\n");
	scanf("%s",buff);
	my_char();
}

void cli_scanf(char *name,char *passwd)
{	
	printf("请输入名字:\n");
	scanf("%s",name);
	my_char();

	printf("请输入密码:\n");
	scanf("%s",passwd);
	my_char();
}

void regist(int sockfd)//注册账户
{
	Pack *wpa =NULL;
	Reg ch;
	memset(ch.name,0x00,sizeof(ch.name));
	memset(ch.passwd,0x00,sizeof(ch.passwd));
	memset(ch.ip,0x00,sizeof(ch.ip));

	strcpy(ch.ip,"192.168.1.107");
	cli_scanf(ch.name,ch.passwd);//输入完成注册

	wpa = packet_reg(wpa,REG,ch);//封包

	int ret = write(sockfd,wpa,sizeof(Pack)+sizeof(Reg));
	my_perror(ret,"write");
	printf("注册成功!\n");
}

//****************************************udp协议
void my_sendto(int udpfd,struct sockaddr_in cliaddr,char *buff)//发信息
{
	printf("port = %d\n",ntohs(cliaddr.sin_port));
	int ret =sendto(udpfd,buff,strlen(buff),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	my_perror(ret,"sendto");

	printf("sendto_write ret %d\n",ret);
}

void my_recvfrom(void *read)//读信息
{
	while(1)
	{
		Read cli = *(Read *)read;
		char buff[128];

		memset(buff,0x00,sizeof(buff));

		socklen_t addrlen = sizeof(cli.readaddr);

		printf("recvfrom...\n");
		int ret = recvfrom(cli.readfd,buff,sizeof(buff),0,(struct sockaddr *)&cli.readaddr,&addrlen);
		if(ret <0)
		{
			read_perror("recvfrom");
		}
		else if(ret ==0)
		{
			printf("接收失败!\n");
			return ;
		}
		else 
		{
			printf("buff = %s\n",buff);
		}
	}
}
//×××××××××××××××××××××××××××××××××××××××××××××××××××

void cli_write_heart(void *arg)//发心跳包
{
	int sockfd  = *(int *)arg;
	Pack *wpa = NULL;//
	while(1)
	{
		wpa = packet_heart(wpa,HEART);

		int ret = write(sockfd,wpa,sizeof(Pack));
		//printf("write_ret = %d\n",ret);
		my_perror(ret,"write");
		sleep(3);
	}
}

//*********************************************聊天
void cli_write_news(int sockfd,News ch)//获取对方端口号
{
	Pack *wpa = NULL;
	wpa = packet_news(wpa,NEWS,ch);

	int ret = write(sockfd,wpa,sizeof(Pack)+sizeof(News));
	my_perror(ret,"write");
}

void cli_mynews(int sockfd,Log log)//获取自己端口号
{
	Pack *wpa =NULL;
	News ch;

	ch.id = log.id;//登录id赋值于信息接收者id

	wpa = packet_news(wpa,NEWS,ch);//信息包

	int ret = write(sockfd,wpa,sizeof(Pack)+sizeof(News));
	my_perror(ret,"write");
}

int  cli_read_news(int sockfd,struct sockaddr_in *cliaddr)
{
	Pack rpa;
	News ch;
	int ret = read(sockfd,&rpa,sizeof(Pack));//读包
	printf("Pack_ret = %d\n",ret);
	if(ret <0)
	{
		read_perror("cli_pa_read");
	}
	else if(ret ==0)
	{
		printf("接收失败!\n");
		return 0;
	}
	else
	{
		if(rpa.type ==NEWS)
		{
			ret = read(sockfd,&ch,sizeof(News));//读包里的信息
			printf("news_ret = %d\n",ret);
			if(ret <0)
			{
				read_perror("cli_ch_read");
			}
			else if(ret == 0)
			{

				printf("接收失败!\n");
				return 0;
			}
			else
			{
				if(ch.id ==0)
				{
					return 0;
				}
				cliaddr->sin_port = htons(ch.id);
			}
		}		
	}
	return 1;
}
//×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××

void  per_fun(int sockfd,struct sockaddr_in cliaddr)
{
	int udpfd = socket(AF_INET,SOCK_DGRAM,0);
	char buff[128];
	int ret;
	News ch;
	while(1)
	{
		printf(FUN);
		scanf("%d",&ret);
		my_char();
		if(ret ==0)
		{
			break;
		}

		switch(ret)
		{
			case 1:
				{
					printf("请输入接受者的id(如果id = 0,则为群发)\n");
					scanf("%d",&ch.id);
					my_char();

					memset(buff,0x00,sizeof(buff));
					cli_write_news(sockfd,ch);//获取对方端口
					ret = cli_read_news(sockfd,&cliaddr);//读取信息
					//ret =bind(udpfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
					if(ret ==1)
					{
						my_scanf(buff);//输入需发送信息
						my_sendto(udpfd,cliaddr,buff);
					}
					else 
					{
						printf("没有找到用户\n");
					}
				}
				break;
			case 2:
				{
					ch.id = 0;
					memset(buff,0x00,sizeof(buff));
					my_scanf(buff);
					cli_write_news(sockfd,ch);

					cli_read_news(sockfd,&cliaddr);
					int n = ntohs(cliaddr.sin_port);
					while(n>1)
					{
						ret =cli_read_news(sockfd,&cliaddr);
						printf("群收%d\n",ret);
						if(ret ==0)
						{
							printf("out\n");
							break;
						}
						else
						{
							my_sendto(udpfd,cliaddr,buff);
							printf("群发信息\n");
						}
						n--;
					}
				}

				break;
			default:
				printf("输入错误，请重新输入!\n");
				break;
		}
	}

}




void login(int sockfd,struct sockaddr_in cliaddr)//登录账户
{
	Pack *wpa = NULL,rpa;
	Read read;
	read.readfd = socket(AF_INET,SOCK_DGRAM,0);
	read.readaddr = cliaddr;


	Log ch,rch;
	memset(ch.passwd,0x00,sizeof(ch.passwd));

	printf("请输入id:\n");
	scanf("%d",&ch.id);
	my_char();

	printf("请输入密码:\n");
	scanf("%s",ch.passwd);
	my_char();

	wpa = packet_log(wpa,LOG,ch);//登录包

	int ret = send(sockfd,wpa,sizeof(Pack)+sizeof(Log),0);
	printf("%d %s",__LINE__,__FILE__);
	my_perror(ret,"reg_write");

	ret = recv(sockfd,&rpa,sizeof(Pack),0);
	printf("read_ret = %d\n",ret);

	if(ret <0)
	{
		read_perror("reg_read");
	}
	else if(ret ==0)
	{
		printf("服务端断开!\n");
		exit(-1);
	}
	else
	{
		if(rpa.type ==LOG)
		{
			ret = recv(sockfd,&rch,sizeof(Log),0);
			if(ret <0)
			{
				read_perror("reg_ch_read");
			}
			else if(ret ==0)
			{
				printf("服务端断开!\n");
				exit(-1);
			}
			else
			{
				if(strcmp(ch.passwd,rch.passwd) == 0)
				{
					printf("登录成功!\n");
					pthread_t pid;

					ret = pthread_create(&pid,NULL,(void *)cli_write_heart,(void *)&sockfd);//线程1：发心跳包
					my_perror(ret,"pthread");

					cli_mynews(sockfd,ch);//获取自己的端口号			
					ret = cli_read_news(sockfd,&read.readaddr);
					//printf("%d\n",__LINE__);
					ret =bind(read.readfd,(struct sockaddr *)&read.readaddr,sizeof(read.readaddr));//绑定udp接收端端口号
					my_perror(ret,"bind");

					ret = pthread_create(&pid,NULL,(void *)my_recvfrom,(void *)&read);//线程2：接收udp信息
					my_perror(ret,"pthread");

					per_fun(sockfd,cliaddr);//发信息				
				}
				else
				{
					printf("账户或密码错误，请重新输入!\n");
				}
			}	
		}
	}
}








