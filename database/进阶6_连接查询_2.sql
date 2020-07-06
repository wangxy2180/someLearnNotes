sql99语法
/*
内连接 INNER
外连接
	左外	left 【OUTER】
	右外	right 【OUTER】
	全外	full 【outer】
交叉连接 CROSS

语法
	select 查询列表
	from 表1 别名【连接类型】
	join 表2 别名 on 连接条件
	where 筛选条件
	GROUP BY
	having
	ORDER BY


*/


-- 一 内连接
/*
语法：
SELECT 查询列表
FROM 表1 
inner join 表2
on 连接条件
*/

-- 1.等值连接
-- 案例1：查询员工名，部门名
SELECT last_name, department_name
from employees
inner JOIN departments
on employees.department_id = departments.department_id



案例2.查询名字中包含e的员工名和工种名(筛选)
SELECT last_name,job_title 
from employees
INNER JOIN jobs
on employees.job_id = jobs.job_id
where last_name LIKE '%e%'


案例3.查询部门个数>3的城市名名和部门个数， (分组+筛选)
SELECT count(*),city
from departments
INNER JOIN locations
on departments.location_id = locations.location_id
group by city
having count(*)>3


案例4.查询哪个部门的部门员工个数>3的部门名和员工个数，并按个数降序(排序)
SELECT count(*),department_name
from employees
INNER JOIN departments
on employees.department_id = departments.department_id
GROUP BY departments.department_name
having count(*)>3
ORDER BY count(*) DESC


案例5.查询员工名、部门名、工种名，并按部门名降序
select last_name,department_name,job_title
from employees
INNER JOIN jobs on employees.job_id = jobs.job_id
INNER JOIN departments on departments.department_id = employees.department_id
ORDER BY department_name DESC


二 非等值连接
查询员工的工资级别
SELECT grade_level,salary
from employees
INNER JOIN job_grades
on employees.salary BETWEEN job_grades.lowest_sal and job_grades.highest_sal


查询每个工资级别的个数>2 的个数，并按工资降序排序
SELECT salary,grade_level,count(*)
from employees
INNER JOIN job_grades
on employees.salary between job_grades.lowest_sal and job_grades.highest_sal
GROUP BY job_grades.grade_level
having count(*)>2
ORDER BY grade_level desc

三 自连接
查询员工的名字，上级的名字
SELECT e.last_name 员工名,m.last_name 上级名
from employees e
INNER JOIN employees m
on e.manager_id = m.employee_id



外连接
一般应用与查询一个表中，另一个表中没有的记录
分主从表
特点
	1.外连接的查询结果为主表中的所有记录
		如果从表中有匹配的，则显示匹配的值，反之显示null
		查询结果 = 内连接结果+主表有从表没有的记录
	2.左外连接、left左边的是主表
		右外连接、right 右边的是主表
	3.左外右外交换量表顺序可以实现同样结果
	你要查询的信息主要来自哪个表，哪个表就是主表
	
查询男朋友不在男神表的女神名
左外连接
SELECT beauty.`name`,boys.*
FROM beauty
LEFT JOIN boys
on beauty.boyfriend_id = boys.id
where boys.id is null

右外连接
SELECT beauty.`name`,boys.*
FROM boys
right JOIN beauty
on beauty.boyfriend_id = boys.id
where boys.id is null

案例1：查找哪个部门没有员工
左外
SELECT departments.*,employees.employee_id
from departments
LEFT JOIN employees
on departments.department_id = employees.department_id
where employees.employee_id is null


右外
SELECT departments.*,employees.employee_id
from employees
right JOIN departments
on departments.department_id = employees.department_id
where employees.employee_id is null



交叉连接
两表进行笛卡尔乘积
SELECT boys.*,beauty.*
from beauty
CROSS JOIN boys



练习
一、查询编号>3的女神的男朋友信息，如果有则列出详细，如果没有，用nul1填充
select beauty.id,beauty.`name`,boys.*
from beauty
left JOIN boys
on beauty.boyfriend_id = boys.id
where beauty.id>3


二、查询哪个城市没有部门
select city
from locations
LEFT JOIN departments
on locations.location_id = departments.location_id
where departments.department_id is null

三、查询部门名为SAL或IT的员工信息
select departments.department_name,employees.*
from employees
LEFT JOIN departments
on employees.department_id = departments.department_id
where departments.department_name IN('SAL','IT')















