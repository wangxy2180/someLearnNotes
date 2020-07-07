# 进阶7 子查询
/*
含义：出现在其他语句中的select语句，成为子查询或内查询
外部的查询语句，称为主查询或外查询

分类：
按子查询出现的位置：
		select后边：
					支持标量子查询
					
		from后边：
					支持表子查询
					
		where或者having后边【重要】
					支持标量子查询（单行）【重要】
					列子查询（多行）【重要】
					行子查询（用的较少）
					
		exists后边（相关子查询）
					表子查询
		

按功能/结果集的行列数不同
		标量子查询（结果集只有一行一列）
		列子查询（结果集只有一列多行）
		行子查询（结果集只有一行多列）
		表子查询（结果集一般为多行多列）
		

*/

-- 一，where having后边
/*
1。标量子查询（单行子查询）
2。列子查询（多行子查询）

3.行子查询（多列多行）

特点：
①子查询放在小括号内
②一般放在条件的右侧
③标量子查询一般大配置单行操作符使用
< > <= >= = <>

列子查询，一般搭配着多行操作符使用
IN，ANY/SOME,ALL

④子查询的执行优先于主查询执行，主查询执行用到了子查询结果
*/


-- 1.标量子查询
-- 案例1：谁的工资比Abel高？
-- 查询名字叫Abel的工资
-- 查询员工信息，满足salary>上一步

SELECT *
FROM employees
WHERE salary>(
	SELECT salary
	FROM employees
	WHERE last_name = 'Abel'

)


#案例2:题目:返回job_ id与141号员工相同，salary比143号员工多的员工姓名，job_ id和工资
SELECT last_name,job_id,salary
FROM employees
WHERE salary > (
	SELECT salary
	from employees
	WHERE employee_id = 143
)
AND job_id = (
	SELECT job_id
	FROM employees
	WHERE employee_id = 141
)

#案例3:返回公司工资最少的员工的last_ name, job_ id和salary|
SELECT last_name,job_id,salary
from employees
WHERE salary = (
	SELECT MIN(salary)
	FROM employees
)


#案例4:查询最低工资大于50号部门最低工资的部门id和其最低工资
SELECT department_id,MIN(salary)
FROM employees
GROUP BY department_id
HAVING MIN(salary) > (
	SELECT MIN(salary)
	FROM employees
	WHERE department_id = 50
)

-- 非法使用标量子查询
SELECT department_id,MIN(salary)
FROM employees
GROUP BY department_id
HAVING MIN(salary) > (
	SELECT salary  #这里有错误，应该是标量子查询
	FROM employees
	WHERE department_id = 50
)

-- 2.列子查询（多行子查询）
-- 多行操作符：IN/NOT IN,ANY SOME ALL
-- ANY/SOME 用得少，有其他方式代替，且可读性更高
-- a > ANY(10,20,30),a大于任意一个就可以比如a=15
-- 可以换成
-- a > MIN(10,20,30)
-- 
-- a > ALL(10,20,30),a大于所有的
-- 可以替换为
-- a > MAX(10,20,30)
-- 

#案例1:返回location_ id是1400或1700的部门中的所有员工姓名
SELECT last_name
FROM employees
WHERE department_id IN (
	SELECT DISTINCT department_id
	FROM departments
	WHERE location_id IN (1400,1700)
)
这里的 IN 可以用 =ANY 代替
NOT IN 可以用 <> ALL
#案例2:返回其它工种中比job_ id为'IT_ _PROG'部门任一工资低的员工的员工号、姓名、job_ id以及salary
SELECT employee_id,last_name,job_id,salary
FROM employees
WHERE salary < ANY(
	SELECT DISTINCT	salary
	from employees
	WHERE job_id = 'IT_PROG'
)
AND job_id<>'IT_PROG'

#案例3:返回其它部门中比job_ id为'IT_ PROG' 部门所有工资都低的员工的员工号、姓名、job_ id以及salary .
SELECT employee_id,last_name,job_id,salary
FROM employees
WHERE salary < ALL(
	SELECT DISTINCT	salary
	from employees
	WHERE job_id = 'IT_PROG'
)
AND job_id<>'IT_PROG'



-- 3.行子查询（结果集一行多列或者多行多列）
#案例:查询员工编号最小并且工资最高的员工信息
SELECT *
from employees
WHERE (employee_id,salary)=(
	SELECT MIN(employee_id),MAX(salary)
	FROM employees
)
-- 所有条件都用一样的操作符=<>时才能用，将这两个当做一个虚拟字段的去用
-- 一般可以分别查，在用AND

# 二 放在select后边
-- 仅仅支持标量子查询
-- 查询部门的员工个数
SELECT d.* , (
	SELECT COUNT(*)
	from employees e
	WHERE e.department_id = d.department_id
)
from departments d;


-- 案例2：查询员工号 = 102的部门名
SELECT (
	SELECT department_name 
	FROM departments d
	INNER JOIN employees e
	ON d.department_id = e.department_id
	WHERE employee_id = 102
)
-- 看似外边那一层select是有问题的，其实这就让他只能返回一行一列，
-- 你可以去掉where跑一下里边的那句，
-- 

-- 三。from后边
-- 将子查询结果充当一张表，要求必须起别名
-- 案例：每个部门的平均工资的工资等级
SELECT ag_dep.*,g.grade_level
FROM(
	SELECT AVG(salary) ag,department_id
	from employees
	GROUP BY department_id
) ag_dep
INNER JOIN job_grades g
ON ag_dep.ag BETWEEN lowest_sal AND highest_sal



-- 四 exist后边（相关子查询）
-- 只关心是否存在，布尔类型

-- 案例1：查询有员工的部门名
SELECT department_name
from departments
where department_id in(
	SELECT department_id
	FROM employees
)

-- 或者

SELECT department_name
from departments
WHERE EXISTS(
	SELECT*
	FROM employees
	WHERE departments.department_id = employees.department_id
	)


-- 大量案例
-- 1.查询和Zlotkey相同部门的员工姓名和工资
select last_name,salary
FROM employees
WHERE department_id = (
	SELECT department_id
	FROM employees
	WHERE last_name = 'Zlotkey'
)

-- 2.查询工资比公司平均工资高的员工的员工号，姓名和工资.
SELECT employee_id,last_name,salary
from employees
where salary > (
	SELECT AVG(salary)
	from employees
)

-- 3.查询各部门中工资比本部门平均工资高的员工的员工号，姓名和工资
SELECT employee_id,last_name,salary
FROM employees e
INNER JOIN (
	select AVG(salary) ag,department_id
	from employees
	GROUP BY department_id
)ag_dep
on e.department_id = ag_dep.department_id
where salary>ag_dep.ag


-- 4.查询和姓名中包含字母u的员工在相同部门的员工的员工号和姓名


SELECT last_name,employee_id
from employees
where department_id IN(
	SELECT DISTINCT department_id
	from employees
	WHERE last_name LIKE '%u%'
)
-- 5.查询在部门的location_ id为1700的部门工作的员工的员工号
select employee_id,department_id
from employees
where department_id in (
	SELECT department_id
	from departments
	where location_id = 1700
)


-- 6.查询管理者是King的员工姓名和工资
SELECT last_name,salary
from employees
WHERE manager_id in (
	SELECT employee_id
	from employees
	where last_name = 'K_ing'
)



-- 7.查询工资最高的员工的姓名，要求first_ name和last_ name显示为一-列，列名为姓.名
select CONCAT(first_name,' ',last_name) '姓名'
FROM employees
where salary = (
	SELECT MAX(salary)
	FROM employees
	)

