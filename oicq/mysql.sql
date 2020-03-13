drop database per_db;
create database per_db character set utf8;

use per_db;

create table tb_per
(
 	id int unsigned NOT NULL,
	name varchar(32) NOT NULL,
	passwd varchar(32) NOT NULL,
	ip varchar(64) NOT NULL
);
