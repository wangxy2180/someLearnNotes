/*
DML语言
数据操作语言
数据
插入，insert
修改，update
删除，delete
*/

-- 一 插入语句
-- 语法
经典插入语句：
-- insert INTO 表名(列名，。。。)
-- VALUES(值1，值2，。。。和列一一对应)


注意1：插入值得类型与列的类型一致
INSERT INTO beauty(id,Name,sex,borndate,phone,photo,boyfriend_id)
VALUES(13,'唐艺昕','女','1990-4-23',1898888,NULL,2)

注意2：不可以为null的列必须插入值，可以为null的列如何插入值？
方式一：
INSERT INTO beauty(id,Name,sex,borndate,phone,photo,boyfriend_id)
VALUES(13,'唐艺昕','女','1990-4-23',1898888,NULL,2)
方式二：
INSERT INTO beauty(id,Name,phone)
VALUES(14,'娜扎',12323898888)



注意3：列的顺序可以调换，但是要一一对应

注意4：列数和值的个数必须一致

注意5：可以省略列名，默认是所有列，列的顺序和表中顺序一致
INSERT INTO beauty
VALUES(15,'张飞','男','1990-2-28',1123123,NULL,9)


-- 
-- 插入语句方式二：
-- insert INTO 表名
-- set 列名=值，列名2=值2。。。
-- 
INSERT INTO beauty
SET id = 19,NAME='刘涛',sex = '女',phone = 909090 


两种方式比较：
方式一支持一次插入多行，方式二不支持
insert into beauty
VALUES(13,'唐艺昕','女','1990-4-23',1898888,NULL,2),
(14,'张艺昕','女','1990-4-23',1898888,NULL,2),
(15,'王艺昕','女','1990-4-23',1898888,NULL,2),
(16,'李艺昕','女','1990-4-23',1898888,NULL,2)

方式一支持子查询方式二不支持
insert into beauty
SELECT id,boyName,'118118118'
FROM boys where id < 3;


-- -----------------------
-----------------------
-- 二 修改数据
-- 修改单表的记录（掌握）
-- UPDATE 表名
-- set 列 = 新值，列=新值。。。
-- where 筛选条件;

案例1：修改beauty中姓唐的女神电话为1234567
UPDATE beauty set phone = 1234567
where name LIKE '唐%'

案例2：修改boy表id号2的名称为张飞，魅力值10
UPDATE boys set boyName = '张飞',userCP = 10
where id = 2


-- 修改多表的记录（补充）
-- 语法
-- sql92
-- UPDATE 表一 别名，表二 别名
-- set 列= 值，。。。
-- where 连接条件
-- and 筛选条件；
-- 只支持内联
-- 
-- 
-- sql99
-- update 表1 别名
-- inner|left|right join 表2 别名
-- on 连接条件
-- set 列= 值，。。。
-- where 筛选条件

案例1：修改张无忌的女朋友的手机号为114
update boys bo
inner join beauty b ON bo.id = b.boyfriend_id
set b.phone = 333
where bo.boyName = '张无忌'



案例2，修改没有男朋友的女神的男朋友编号都为张飞
update beauty b
left join boys bo on b.boyfriend_id = bo.id
set b.boyfriend_id = 2
where b.boyfriend_id is null

-- 
-- 删除语句
-- 方式一：delete
-- 语法
-- 1.单表★
-- delete from 表名 where 筛选条件
案例1：手机编号最后一位是9
DELETE from beauty where phone LIKE '%9'


2.多表删除
案例：删除张无忌的女朋友的信息
-- sql99
delete b 
FROM beauty b
INNER JOIN boys bo on b.boyfriend_id = bo.id
where bo.boyName = '张无忌';

案例2：删除黄晓明的信息以及他女朋友的信息
DELETE bo,b
from beauty b
inner JOIN boys bo on b.boyfriend_id = bo.id
WHERE bo.boyName = '黄晓明'

方式二：TRUNCATE
TRUNCATE 不允许有where，用于清空数据
案例：将魅力值>100的男神信息删除
TRUNCATE TABLE boys 


delete 和 TRUNCATE 区别
1.delete可以加条件，TRUNCATE不可以加
2.truncate效率高一丢丢
3.假如我们要删除的表中有自增长列，delete删除后再插入数据，自增长列的值从断点开始
TRUNCATE删除后再插入数据，自增长列从1开始
4.TRUNCATE删除没有返回值，delete有返回值
5.TRUNCATE删除后不能回滚，delete删除可以回滚

