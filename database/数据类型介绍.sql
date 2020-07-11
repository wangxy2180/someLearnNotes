-- 常见的数据类型
/*
数值型：
	整型
	小数：定点数
				浮点型

字符型：
	较短的文本 char VARCHAR
	较长的文本 text blob(较长的二进制数据)

日期：
	


*/

-- 一 整型
TINYINT				1字节
SMALLINT			2字节
MEDIUMINT			3字节
INT INTEGER		4字节
BIGINT				8字节

特点：
默认有符号，设置无符号要添加unsigned
如果不设置长度会有默认长度,
整形的长度代表显示结果的宽度，不够左填0，搭配zerofill，默认会变成无符号
t1 INT(7) ZEROFILL


案例1：如何设置无符号和有符号
drop table if EXISTS tab_int
create table tab_int(
	t1 INT,
	t2 int UNSIGNED
)

INSERT into tab_int VALUES(-123456)
INSERT into tab_int VALUES(-123456,-123456)



desc tab_int

二小数
浮点型
FLOAT(M,D)	4
DOUBLE(M,D)	8

精确度要求较高的使用
定点型
DEC(M,D)
DECIMAL(M,D)

特点
M	代表整数部位+小数部位总长度
D	为小数部分范围
M，D都可以省略
DECIMAL的(M,D)默认为(10,0)
float 和double会根据插入数值的精度来决定精度

定点型精确度较高

原则：
所选的类型越简单越好，能保存数值的类型越小越好


CREATE table tab_float(
	f1 FLOAT(5,2),
	f2 DOUBLE(5,2),
	f3 DECIMAL(5,2)
)

正常
insert into tab_float values(123.45,123.45,123.45)
会四舍五入
insert into tab_float values(123.456,123.456,123.456)
会补0
insert into tab_float values(123.4,123.4,123.4)
报错
insert into tab_float values(1231.4,1233.4,1233.4)


三：字符型
/*
较短的文本
M个 字符 数
char(M)				固定长度字符		比较耗费空间	性能略高 	M可省略，默认1
VARCHAR(M)		可变长度字符		比较节省空间	性能略低	不可省略

较长的文本
text
blob(较大的二进制)



*/
枚举只能从列表中选一个，集合可以选多个
不区分大小写，会自动做大小写转换


枚举 enum
create table tab_char(
	c1 enum('a','b','c')

)

insert into tab_char VALUES('a');
insert into tab_char VALUES('b');
insert into tab_char VALUES('c');
insert into tab_char VALUES('d');
insert into tab_char VALUES('A');

create table tab_set(
	s1 SET('a','b','c')

)

insert into tab_set VALUES('a');
insert into tab_set VALUES('a,b');
insert into tab_set VALUES('a,c,d');


四 日期型：
date				4字节		只保存日期
datetime		8字节		保存日期时间
TIMESTAMP		4字节		保存日期时间
time				3字节		只保存时间
YEAR				1字节		只保存年

datetime	1000 - 9999		不受时区影响
TIMESTAMP	1970 - 2038		受时区影响



create table tab_date(
		t1 datetime,
		t2 TIMESTAMP
)

insert into tab_date values(now(),NOW());

show VARIABLES like 'time_zone'
set time_zone = '+9:00'