#include"deamon.h"

void init_deamon()
{
		int pid;
		int i;
		if(signal(SIGCHLD,SIG_IGN) == SIG_ERR)
		{
				printf("cant signal in init_deamon.");
				exit(1);
		}
		if((pid = fork()) >0)
		{
				exit(0);
		}
		else if(pid < 0)
		{
				perror("fail to fork1");
				exit(1);
		}

		setsid();
		
		if((pid = fork()) >0)
		{
				exit(0);
		}
		else if(pid <0)
		{
				exit(1);
		}

		for(i=0;i<getdtablesize();i++)
		{
				close(i);
		}

		chdir("/tmp");

		umask(0);

		return ;
}
