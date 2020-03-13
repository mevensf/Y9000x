#include"cliaddr.h"

int main()
{
	signal(SIGPIPE,fun);

	int sockfd = sock_inet();
	struct sockaddr_in cliaddr,seraddr = addr_inet();

	int ret = connect(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	my_perror(ret,"connect");

	cliaddr.sin_family = AF_INET;
	inet_pton(AF_INET,"192.168.1.107",&cliaddr.sin_addr.s_addr);

	while(1)
	{
		printf(MENU);//进入主菜单（cliaddr.h）
		scanf("%d",&ret);
		my_char();//

		if(ret ==0)
		{
			break;
		}

		switch(ret)
		{
			case 1:
				regist(sockfd);
				break;
			case 2:
				login(sockfd,cliaddr);
				break;
			default:
				printf("对不起，请重新输入!\n");
		}
	}

	return 0;
}





