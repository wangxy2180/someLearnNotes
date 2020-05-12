-- 进阶3 排序查询
/*
引入

SELECT * FROM employees

顺序与原表一致，不方便

语法

SELECT 查询列表
FROM 表
[where xxx]
order by 排序列表 [asc|desc]

特点：1.ASC升序，DESC降序，默认升序
			2.ORDER BY可以放多个字段，表达式，函数，别名
			3.ORDER BY执行顺序在条件查询的基础上进行的，一般放在查询语句的最后面，但是limit子句除外
*/

-- 案例1：查询员工信息，按工资从高到低排序
-- ASC可以省略
SELECT * FROM employees ORDER BY salary DESC;
SELECT * FROM employees ORDER BY salary ;

-- 案例2：查询部门编号>=90的员工信息，按入职时间先后进行排序(添加筛选条件)
SELECT * FROM employees WHERE department_id >= 90 ORDER BY hiredate ASC;

-- 案例3：按年薪高低显示员工信息和年薪（按表达式排序）
-- 表中并不存在年薪这一项
SELECT * ,salary*12*(1+IFNULL(commission_pct,0)) 年薪
FROM employees ORDER BY salary*12*(1+IFNULL(commission_pct,0)) DESC;

-- 案例4：按年薪高低显示员工信息和年薪（按别名排序）
SELECT * ,salary*12*(1+IFNULL(commission_pct,0)) 年薪
FROM employees ORDER BY 年薪 DESC;

-- 案例5：按姓名长度显示姓名和工资[按函数排序]
SELECT LENGTH(last_name) 字节长度, last_name, salary
FROM employees ORDER BY LENGTH(last_name);

-- 案例6：查询员工信息，要求先按工资排序，再按员工编号排序[按多个字段排序]
-- 先按谁排，谁就放在前边,主关键字和次要关键字的区别
SELECT * FROM employees ORDER BY salary ASC , employee_id DESC;

-- 
-- 
-- 练习
#1.查询员工的姓名和部门号和年薪，按年薪降序按姓名升序
SELECT last_name,department_id,salary*12*(1+IFNULL(commission_pct,0)) 年薪 FROM employees ORDER BY 年薪 ASC

-- 2.选择工资不在8000到17000的员工的姓名和工资，按工资降序
SELECT last_name,salary FROM employees WHERE salary>17000 OR salary<8000 ORDER BY salary DESC;
SELECT last_name,salary FROM employees WHERE salary NOT BETWEEN 8000 AND 17000 ORDER BY salary DESC;
-- 3.查询|邮箱中包含e的员工信息，并先按邮箱的字节数降序，再按部门号升序
SELECT *,LENGTH(email) FROM employees WHERE email LIKE '%e%'
ORDER BY LENGTH(email)DESC, department_id ASC;
