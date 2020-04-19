#进阶2：条件查询
/*
语法
select
	查询列表
from
	表名
where
	筛选条件;

分类：
	一、按条件表达式筛选
		条件运算符：> < = 不等于两种皆可，推荐后边的!= <> >= <=
	二、按逻辑表达式筛选
		逻辑运算符 && || ！，推荐使用 and or not
	三、模糊查询
		like
		between and
		in
		in null
*/
#一、按条件表达式筛选

#案例1：查询工资>12000的员工信息
SELECT * FROM employees WHERE salary>12000;

#案例2：部门编号不等于90号的员工名和部门编号
SELECT CONCAT(first_name,'-',last_name) AS 姓名,department_id FROM employees WHERE department_id <> 90 

#二、按逻辑表达式筛选
#案例1 查询工资在1w到2w之间的员工名，工资，以及奖金
SELECT 
	CONCAT(first_name,' ',last_name) AS 姓名,salary,commission_pct
FROM
	employees
WHERE
	salary>10000 AND salary<20000;
#案例2 查询部门编号不是在90到110之间,或者工资高于1w5的员工信息
SELECT
	*
FROM
	employees
WHERE
	department_id<90 OR department_id>110 OR salary>15000
# where not(department_id>=90 and department_id<=110) or salary>15000;

#三、模糊查询
/*
like
特点：
1 一般和通配符搭配使用
% 任意多个字符，包含0个
_任意单个字符

between and
in
is null    is not null
*/

#案例1：查询员工名中包含字符a的员工信息
SELECT
	*
FROM
	employees
WHERE
	last_name LIKE '%a%';
# %是通配符
#案例2：查询名中第三个字符为n，第五个为l的员工名和工资
SELECT
	last_name,
	salary
FROM
	employees
WHERE
	last_name LIKE '__n_l%';
#案例3：查询员工名中第二个字符为_的员工
SELECT
	last_name
FROM
	employees
WHERE
	last_name LIKE '_\_%'
	#last_name like '_$_%' escape '$'  $字符作为转义字符
	
#2.between and
/*
1.可以提高语句简洁度
2.左右两端包含，相当于>= <=
3.临界值不能调换顺序

*/
#案例1：查询员工编号在100-120之间
SELECT
	*
FROM
	employees
WHERE
	employee_id BETWEEN 100 AND 120;
	
	
#3.in
/*
含义：类似Python中的 in
特点：
简单，简洁
列表中的值类型必须一致或兼容
无法做模糊查询，因为in 等同于 =，模糊查询是like

*/
#案例：查询员工的编号是IT_PROG,AD_VP,AD_PRES中的员工
SELECT
	last_name,
	job_id
FROM
	employees
WHERE
	job_id IN ('IT_PROG','AD_VP','AD_PRES')
	

#4.is null
/*
使用 = 或者 <> 不能判断 null
*/
#案例1：查询没有奖金的员工名和奖金率
SELECT
	last_name,
	commission_pct
FROM
	employees
WHERE
	commission_pct IS NULL
	#错误写法：salary is 12000
	
#安全等于 <=>
/*
可读性差
即可以判断null，又可以判断普通数值
*/
SELECT
	last_name,
	commission_pct
FROM
	employees
WHERE
	commission_pct <=> NULL
	
#案例2：查询工资12000的员工信息
SELECT
	last_name,
	salary
FROM
	employees
WHERE
	salary <=> 12000
	
/* 
select * from employees;
和
select *from employees where commission_pct like '%%' and last_name like '%%';
一样吗
不一样！，因为有null，如果没有null就一样
*/