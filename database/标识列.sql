标识列
又称为自增长列
含义：可以不用手动插入值，系统提供默认的序列值

特点：
1 标识列必须合主键搭配吗？不一定，但要求是一个key 比如unique
2 至多有一个自增长
3 标识列类型只能是数值型，一般是int
4 可以通过set auto_increment_increment=3
					set auto_increment_offset=3
					设置步长和起始值

一 创建表时设置标识列
drop table id EXISTS tab_id
create table tab_id(
	id INT PRIMARY KEY auto_increment,
	Name VARCHAR(20)
)

insert into tab_id(id,Name) values(null,'join') 
insert into tab_id(Name) values('lucy') 

查看增长量和起始
MySQL可以设置起始,可以设置步长
show VARIABLES LIKE '%auto_increment%'

set auto_increment_increment=3
set auto_increment_offset=3

二 修改表时设置标识列
alter table tab_id MODIFY COLUMN id int PRIMARY KEY auto_increment

三 修改表时删除标识列
alter table tab_id MODIFY COLUMN id int;








