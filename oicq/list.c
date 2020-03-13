#include"list.h"


void add_list(Info **head,int clifd,char *ip,int port)
{
		pthread_mutex_lock(&mutex);
		
		Info *pnew = (Info *)malloc(sizeof(Info));

		//printf("port = %d\n",port);
		pnew->time = 10;
		pnew->clifd = clifd;
		strcpy(pnew->ip,ip);
		pnew->port = port;

		pnew->next = *head;
		*head = pnew;

		pthread_mutex_unlock(&mutex);
}

void show_list(Info *head)
{
		Info *temp = head;
		while(temp !=NULL)
		{

				printf("[%d|%s |%p]\n",temp->port,temp->ip,temp->next);
				
				temp = temp->next;
		}
}

void del_list(Info **head,int clifd)
{
		//pthread_mutex_t mutex;
		pthread_mutex_lock(&mutex);
		
		Info *temp = *head;
		Info *del =NULL;
		if(*head ==NULL)
		{
				return ;
		}
		if((*head)->clifd == clifd)
		{
				*head =(*head)->next;
				free(temp);
		}
		else
		{
				while(temp !=NULL)
				{
						if(temp->next->clifd ==clifd)
						{
								del =temp->next;
								temp->next = del->next;
								free(del);
								break;
						}
						temp =temp->next;
				}
		}
		pthread_mutex_unlock(&mutex);
}
				

int len_list(Info *head)
{
		int num=0;
		Info *temp = head; 
		while(temp !=NULL)
		{
				num++;
				temp = temp->next;
		}
		return num;
}

