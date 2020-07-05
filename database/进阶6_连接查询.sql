进阶6连接查询
/*
多表查询
当查询的字段来自做个表时

笛卡尔积现象：
假如表1有m行，表2有n行，结果有m*n行，就是笛卡尔积现象，
发生原因：因为没有有效的连接条件
如何避免：添加有效的连接条件

分类：按年代分类
			sql92 仅支持内联
			sql99 推荐 支持内，外（左外右外），交叉
			
			按功能分类：
			内连接
				等值连接
				非等值连接
				自连接
			外连接
				左外连接
				右外连接
				全外连接
			交叉连接




*/

-- 一 等值连接

-- 案例1：查询女神男神对应名
SELECT name ,boyName from beauty,boys
where beauty.boyfriend_id = boys.id


-- 案例2：查询员工名和对应的部门名
select last_name, department_name
from employees,departments
where employees.department_id = departments.department_id

-- 2.为表起别名  AS可以省略
/*
提高了语句的简洁度
区分多个重名字段
起别名后原始表明不能用，只能使用别名
*/
-- 查询员工名，工种号，工种名
SELECT last_name,e.job_id,job_title
from employees AS e,jobs j
where e.job_id = j.job_id



-- 4. 可以加筛选
-- 案例1：查询有奖金的员工名，部门名
select last_name,department_name,commission_pct
from employees,departments
where employees.department_id = departments.department_id
AND employees.commission_pct is not null

-- 案例2：查询城市名中第二个字符为o的部门和城市名
SELECT departments.department_name , city
from departments,locations
where departments.location_id = locations.location_id
AND city LIKE "_o%"


-- 5.可以加分组
-- 案例1：查询每个城市的部门个数
select count(*),city
from departments,locations
where departments.location_id = locations.location_id
GROUP BY city


-- 案例2：查询有奖金的每个部门的部门名和部门的领导编号，以及该部门的最低工资
select department_name,d.manager_id,min(salary)
from departments d,employees AS e
where commission_pct is not null
AND d.department_id = e.department_id
GROUP BY d.department_id,manager_id


-- 6.可以加排序
-- 案例：查询每个工种的工种名，员工个数，按员工个数降序
SELECT job_title,count(*)
from jobs AS j,employees e
where j.job_id = e.job_id
GROUP BY j.job_id
ORDER BY count(*) DESC

-- 7.三表连接
查询员工名，部门名与所在的城市
select last_name,department_name,city
from employees,departments,locations
where employees.department_id = departments.department_id
AND departments.location_id = locations.location_id
AND city LIKE 's%'
ORDER BY department_name DESC


-- 二 非等值连接
-- 案例1：查询员工的工资和工资级别
SELECT salary,grade_level,last_name
from employees,job_grades
where salary BETWEEN lowest_sal and highest_sal
AND grade_level = 'A'


-- 三 自连接
-- 案例：查询员工名以及上级的名字
select e.last_name,e.employee_id,m.employee_id,m.last_name
from employees e,employees m
where e.manager_id = m.employee_id


-- 测试：

一、显示员工表的最大工资，工资平均值
SELECT max(salary),min(salary),avg(salary)
from employees

二、查询员工表的employee_ id, job_ id, last. name，按department_ id降序，sal ary升序
SELECT employee_id,job_id,last_name
FROM employees
ORDER BY department_id DESC,salary ASC

三、查询员工表的job_ id中包含 a和e的，并且a在e的前面
select job_id 
from employees
where job_id LIKE '%a%e%'

四、已知表student, 里面有id(学号)，
name,
gradeId (年级编号)
已知表grade,里面有id (年级编号)，name (年级名)
已知表result,里面有id, score, studentNo (学号)
要求查询姓名，年级名，成绩
select s.name,g.name,r.score
from student s,grade g,result r
where s.id = studentNo
and g.id - s.gradeid


五、显示当前日期，以及去前后空格，截取子字符串的函数
select now();
select trim('');
select trim('字符 from');去除指定字符
select substr(str,起始索引(从1开始))













CREATE TABLE job_grades
(grade_level VARCHAR(3),
 lowest_sal  int,
 highest_sal int);

INSERT INTO job_grades
VALUES ('A', 1000, 2999);

INSERT INTO job_grades
VALUES ('B', 3000, 5999);

INSERT INTO job_grades
VALUES('C', 6000, 9999);

INSERT INTO job_grades
VALUES('D', 10000, 14999);

INSERT INTO job_grades
VALUES('E', 15000, 24999);

INSERT INTO job_grades
VALUES('F', 25000, 40000);









