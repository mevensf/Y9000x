#ifndef _FILE_H_
#define _FILE_H_

#include<time.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void set_time(char *buff);//获取时间

int  open_file();//打开日志文件

void write_file(char *buff);//写日志文件

int  open_file();//打开配置文件

void write_file(char *buff);//写配置文件

#endif
