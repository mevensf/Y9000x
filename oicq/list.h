#ifndef _LIST_H_
#define _LIST_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex;

typedef struct info
{
		int time;
		int id;
		int clifd;
		char ip[64];
		int  port;
		struct info *next;
}Info;
/*
typedef struct node
{
		int port;
		struct node* next;
}Node;
*/
void add_list(Info **head,int clifd,char *ip,int port);

void del_list(Info **head,int clifd);

int len_list(Info *head);

void show_list(Info *head);
#endif


