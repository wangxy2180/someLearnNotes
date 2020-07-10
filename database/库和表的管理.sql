/*
DDL数据定义语言
设计库和表的管理
一：库的管理
创建，修改，删除

二表的管理
床就，修改，，删除

创建 create
修改 alter
删除 drop
*/

一：库的管理
1.库的创建
if not EXISTS 容错性处理
CREATE database 库名
案例：创建图书库：
CREATE DATABASE  if not EXISTS books

2.库的修改
RENAME DATABASE books TO 新库名;
上边这句已经不能用了，知道即可

更改库的字符集
alter DATABASE books CHARACTER SET utf8;


3.库的删除
drop DATABASE if EXISTS books;


二表的管理
1.表的创建[这个为可选]
create table 表名(
	列名 列的类型[(长度) 约束],
	列名 列的类型[(长度) 约束],
	列名 列的类型[(长度) 约束],
	列名 列的类型[(长度) 约束]
)
books里创建book表
create table book(
	id INT,#编号
	bName varchar(20),#图书名
	price double,#价格
	authorId INT,#作者
	publishDate datetime #出版日期
)
desc book

create table if not EXISTS author(
	id INT,
	au_name VARCHAR(20),
	nation VARCHAR(10)
)


2.表的修改
语法
alter table 表名 add/drop/modify/change COLUMN 列名 类型 约束

修改列名
alter table book change COLUMN publishDate pubDate datetime
column 可以省略

修改列的类型或约束
alter table book modify COLUMN pubDate TIMESTAMP;
添加新列
alter table author add column annual DOUBLE
删除列
alter table author drop COLUMN annual
修改表名
alter table author rename to book_author;


3.表的删除
drop table if EXISTS book_author 

show TABLES

通用的写法
DROP DATABASE if EXISTS 旧库名
create DATABASE 新库名	

DROP TABLE if EXISTS 旧表名
create TABLE 新表名	

4.表的复制
insert into author VALUES
(1,'村上春树','日本'),
(2,'莫言','中国'),
(3,'冯唐','中国'),
(4,'金庸','中国')

(1)仅仅复制表的结果
create table copy LIKE author

(2)复制表的结构和数据
create table copy2 
select * from author

(3)只复制部分数据
create table copy3
select id,au_name
from author
where nation = '中国';

(4)只复制某些字段
create table copy4
select id,au_name
from author
where 1=2
表中没有符合条件的数据，所以只是一个空表














