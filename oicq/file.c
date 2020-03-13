#include"file.h"

pthread_mutex_t mutex;
void set_time(char *buff)//获取时间
{
	char a[128];
	time_t ti = time(NULL);
	struct tm *t = localtime(&ti);
	sprintf(a,"%d-%d-%d %d:%d:%d\n",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);;
	strcat(buff,a); 
}

int  open_time_file()//打开日志文件
{
	int fd;
	fd = open("./time.txt",O_WRONLY | O_CREAT | O_APPEND,0777);
	if(fd <0)
	{
		exit(-1);
	}
	return fd;
}

int  open_config_file()//打开配置文件
{
	int fd;
	fd = open("./config.txt",O_WRONLY | O_CREAT | O_APPEND,0777);
	if(fd <0)
	{
		exit(-1);
	}
	return fd;
}

void write_file(char *buff)//写日志文件
{
	set_time(buff);
	int fd = open_time_file();

	int ret = write(fd,buff,strlen(buff));
	//printf("%d\t%s\n",__LINE__,__FILE__);//???
	if(ret < 0)
	{
		sprintf(buff,"write_time_error");
		write_file(buff);
	}
	close(fd);
}

void write_config(char *buff)
{
	//pthread_mutex_t mutex;
	pthread_mutex_lock(&mutex);
	int fd = open_config_file();

	int ret = write(fd,buff,strlen(buff));
	if(ret < 0)
	{
		sprintf(buff,"write_config_error");
		write_file(buff);
	}

	close(fd);
	pthread_mutex_unlock(&mutex);
}

