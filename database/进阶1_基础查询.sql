# 进阶1 基础查询
/*
select 查询的东西(列表，可以存在多个)
from 表名

特点：
1. 查询的东西可以是 字段，常量，表达式，函数
2. 查询的结果是一个虚拟的表格
*/
#**在这里，要想单独执行某句，选中后再执行

#0.先切换到制定的库
USE myemployees;

#1.查询表中单个字段
SELECT last_name FROM employees;

#2.查询表中多个字段
SELECT last_name,salary,email FROM employees;

#3.查询表中所有字段
#这个可以双击快速添加，workbench ctrl+b 格式化 sqlyog f12格式化
#`着重号`，自动添加的，可以去掉。目的是强调这是个字段，对于与关键字重复的字段名使用
 SELECT
  `employee_id`,
  `first_name`,
  `last_name`,
  `email`,
  `phone_number`,
  `job_id`,
  `salary`,
  `commission_pct`,
  `manager_id`,
  `department_id`,
  `hiredate`
FROM
  employees;
#或者可以
SELECT * FROM employees; 
#这种方式顺序不够灵活

#4.查询常量值 
SELECT 100;
SELECT 'john';

#5.查询表达式
SELECT 100*98;

#6.查询函数 调用该函数，得到返回值并显示
SELECT VERSION();

#7.起别名
/*
①便于理解
②如果要查询的字段有重名，使用别名可以区分开来

*/
SELECT 100*98 AS 结果;
SELECT last_name AS 姓,first_name AS 名 FROM employees;
#或者可以省略AS，空格就可以
SELECT last_name 姓,first_name 名 FROM employees;
#如果别名中有特殊符号，比如 # ，空格，关键字等，使用引号（单双都可以）包起来
SELECT salary AS 'out put' FROM employees;

#8.去重
#查询员工表中所涉及到的部门编号
SELECT DISTINCT department_id FROM employees;

#9.+号的作用
/*
mysql中+只有运算符的作用
select 100+90;
上边那句可以选中直接执行，因为选中的部分没有注释，选中谁执行谁
select '123'+90;其中一方为字符型，试图将字符型装换为数值型，转换成功继续运算
select 'john'+90;转换失败，将字符型数值转换为0
select null+10;只要其中一方为null，结果肯定为null
*/
#将姓名连接成一个字段
SELECT
	CONCAT(last_name,' ',first_name) AS 名字
FROM
	employees;
#注意，如果要拼接的字段中有null，那么也会全部变成null
SELECT
	IFNULL(commission_pct,0) AS 奖金率,
	commission_pct
FROM
	employees;
#ifnull(将要进行判断的表达式，如果是null返回的值)
SELECT
	CONCAT(`first_name`,`last_name`,IFNULL(commission_pct,0))
FROM employees;