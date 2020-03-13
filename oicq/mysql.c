#include"mysql.h"

void my_connect(MYSQL *conn)//链接数据库
{
	char buff[128];
	memset(buff,0x00,sizeof(buff));//数组清零

	mysql_init(conn);//对mysql结构体做初始化
	if(mysql_real_connect(conn,"localhost","root","123456","per_db",0,NULL,0) ==NULL)//链接数据库
	{
		sprintf(buff,"链接失败：%s\t",mysql_error(conn));
		write_file(buff);
	}
	mysql_set_character_set(conn,"utf8");

	printf("数据库链接成功!\n");
	sprintf(buff,"数据库链接成功\t");

	//write_file(buff);//写日志文件
}


void my_query(MYSQL *conn,char *sql)//发送信息（运行）
{
	char buff[128];
	memset(buff,0x00,sizeof(buff));

	int ret = mysql_query(conn,sql);
	if(ret)
	{
		sprintf(buff,"发送失败%s\t",mysql_error(conn));
		write_file(buff);
	}
}
/*
   void creat_database(MYSQL *conn,char *sql)//创建tb_per表
   {
   sprintf(sql,"source mysql.sql");
   my_query(conn,sql);
   }
 */


void add_per(MYSQL *conn,char *sql,Reg ch)//用户注册
{
	find_max(conn,sql,&ch);
	//printf("find_max\n");

	sprintf(sql,"insert into tb_per values(%d,'%s','%s','%s')",ch.id,ch.name,ch.passwd,ch.ip);

	my_query(conn,sql);
	//printf("add_per\n");
}


void find_per(MYSQL *conn,char *sql,Log ch)//用户登录
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW row;

	sprintf(sql,"select passwd from tb_per where tb_per.id =%d",ch.id);
	my_query(conn,sql);

	res_ptr = mysql_store_result(conn);
	row =mysql_fetch_row(res_ptr);

	strcpy(sql,row[0]);

	mysql_free_result(res_ptr);

}

void find_max(MYSQL *conn,char *sql,Reg *ch)//-------
{	
	MYSQL_RES *res_ptr;
	MYSQL_ROW row;

	sprintf(sql,"select MAX(id) as max_id from tb_per ");
	my_query(conn,sql);

	res_ptr = mysql_store_result(conn);
	row =mysql_fetch_row(res_ptr);

	printf("row[0] = %s\n",row[0]);

	if(row[0] == NULL)//客户id自动分配
	{
		ch->id = 10001;
	}
	else
	{
		ch->id = atoi(row[0])+1;//将字符串转化为整型数
	}

	mysql_free_result(res_ptr);


}


