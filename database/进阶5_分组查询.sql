-- 进阶5 分组查询
/*
select 分组函数，列（要求出现在group by的后边）
from 表名
where 筛选条件
GROUP BY 分组的列表
order by 子句

注意：查询列表比较特殊，要求是分组函数和group by 后出现的字段

特点：
1.筛选条件分为两类
	分组前筛选 WHERE      数据源是原始表             GROUP BY前边  where
	分组后筛选 having			数据源是分组后的结果集     GROUP BY后边  having 
	分组函数做条件(max min count)，肯定是放在having子句中
	尽量使用分组前筛选
2.group by 支持多个字段分组，用逗号隔开，没有顺序要求，也可以用表达式
*/


-- 引入 查询每个部门的平均工资


-- 案例1：查询每个工种的最高工资
-- 简单的分组查询
select max(salary),job_id
from employees
GROUP BY job_id;

-- 案例2：查询每个位置上的部门个数
select count(*),location_id
from departments
GROUP BY location_id;

-- 添加筛选条件
-- 案例1：查询邮箱中包含a字符的，每个部门的平均工资
select AVG(salary),department_id
from employees
where email like '%a%'
GROUP BY department_id;

-- 案例2：查询有奖金的每个领导手下员工的最高工资
select max(salary),manager_id
from employees
where commission_pct is not null
GROUP BY manager_id

-- 添加复杂的筛选条件
-- 案例1：查询哪个部门的员工个数大于2
-- 1 查询每个部门的员工个数
SELECT count(*),department_id
from employees
GROUP BY department_id

-- 2 根据1的结果进行筛选，查询哪个部门的员工个数大于2
-- 添加分组后的筛选
SELECT count(*),department_id
from employees
GROUP BY department_id
HAVING count(*)>2

案例2：查询每个工种有奖金的员工的最高工资>12000的工种编号和最高工资
1.查询每个工种有奖金的最高工资
select max(salary),job_id
from employees
WHERE commission_pct is not null
GROUP BY job_id
2.根据1的结果继续筛选最高工资>12000
select max(salary),job_id
from employees
WHERE commission_pct is not null
GROUP BY job_id
HAVING max(salary) > 12000;

-- 案例3：查询领导编号>102的每个领到手下的最低工资>5000的领导编号以及其最低工资
SELECT manager_id,MIN(salary)
from employees
where manager_id>102
GROUP BY manager_id
having MIN(salary)>5000;


-- 按表达式或者函数分组
-- 案例：按员工姓名的长度分组，查询每一组的员工个数，筛选员工个数>5的有哪些
select count(*) c,LENGTH(last_name) len_name
from employees
GROUP BY len_name
having c>5
-- MySQL支持别名，其他的数据库可能不支持


按多个字段分组
案例：查询每个部门每个工种的员工的平均工资
select AVG(salary),department_id,job_id
from employees
GROUP BY department_id,job_id
-- 只有两个都符合时才会被选中



-- 添加排序
-- 案例：查询每个部门每个工种的员工的平均工资，并按工资高低显示出来
select AVG(salary),department_id,job_id
from employees
where department_id is not null
GROUP BY department_id,job_id
having avg(salary)>10000
ORDER BY AVG(salary) DESC


-- 题目

#1.查询各job_ id的员工工资的最大值，最小值，平均值，总和，并按job_ id升序
SELECT max(salary),min(salary),avg(salary),sum(salary)
from employees
GROUP BY job_id
ORDER BY job_id asc

2.查询员工最高工资和最低工资的差距( DIFFERENCE)
SELECT MAX(salary)-min(salary) DIFFERENCE
from employees

3.查询各个管理者手下员工的最低工资，其中最低工资不能低于6000，没有管理者的员工不计算在内
SELECT min(salary),manager_id
from employees
where manager_id is not null
GROUP BY manager_id
having min(salary)>=6000

4.查询所有部门的编号，员工数量和工资平均值，并按平均工资降序
SELECT department_id, count(*),AVG(salary)
from employees
group by department_id
order by AVG(salary) ASC


5.选择具有各个job_ id的员工人数
SELECT count(*),job_id
from employees
where job_id is not null
GROUP BY job_id















