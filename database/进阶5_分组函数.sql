-- 功能 ：用做统计使用，又称为聚合函数，或统计函数或组函数
/*
sum 求和
avg 平均值
max 最大值
min 最小值
count 计算个数
*/

-- 1 简单的使用
SELECT sum(salary) from employees;
SELECT AVG(salary) from employees;
SELECT MIN(salary) from employees;
SELECT MAX(salary) from employees;
SELECT count(salary) from employees;

-- 求和，平均，最大最小计数
SELECT SUM(salary) 和,avg(salary) 平均,MIN(salary) 最小,MAX(salary) 最大,COUNT(salary) 计数
FROM employees;
-- 平均值保留两位
SELECT SUM(salary) 和,ROUND(avg(salary),2) 平均,MIN(salary) 最小,MAX(salary) 最大,COUNT(salary) 计数
FROM employees;

-- 2 参数支持哪些类型

-- sum和avg只用于处理数值型

-- max和min会根据排序求取最大最小，其中 z是max
-- 日期同理，2016年>1999年

-- count 计算非空的值的个数

-- 3 忽略null值
-- 
-- null+任何值都得null
-- sum和avg中null不参与运算

SELECT SUM(commission_pct) , AVG(commission_pct) , SUM(commission_pct)/35, SUM(commission_pct)/107 FROM employees;



-- 4 可以和distince搭配实现去重
SELECT SUM(DISTINCT salary),SUM(salary) FROM employees;
select count(DISTINCT salary),count(salary) from employees;

-- 5 count的详细介绍
SELECT count(salary) from employees;
-- 查询表里的总行数
SELECT count(*) from employees;
-- 用来统计1的个数，相当于多了一列，每个元素都是1，写2，3都可以，相当于加了一列常量值
SELECT count(1) FROM employees;

效率
MYISAM存储引擎下 count(*)效率最高
INNODB存储引擎下 count(*)count(1)差不多
用count(*)比较多


-- 6 和分组函数一同查询的字段有限制，要求是group by后的字段
SELECT avg(salary),employee_id from employees;
-- 上边那句话没有任何意义，



-- 测试-------------------------
1.查询公司员工工资的最大值，最小值，平均值，总和
SELECT max(salary) 最大,min(salary) 最小,avg(salary) 平均,sum(salary) 总和 from employees;

2.查询员工表中的最大入职时间和最小入职时间的相差天数
select max(hiredate)-min(hiredate) difference from employees;
日期函数
SELECT DATEDIFF('2020-10-1','2020-10-7');
他会用前边日期减去后边日期
SELECT DATEDIFF(max(hiredate),min(hiredate)) from employees;
计算活了多少天
SELECT datediff(now(),'1997-8-12');

3.查询部门编号为90的员工个数
select count(*) from employees where department_id = 90;