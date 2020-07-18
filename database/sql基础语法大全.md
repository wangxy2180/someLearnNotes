[toc]

# 查询

## 进阶1 基础查询

> SELECT 查询的东西(列表，可以存在多个)
> FROM 表名

特点：

1. 查询的东西可以是 字段，常量，表达式，函数

2. 查询的结果是一个虚拟的表格

步骤

1. 先切换到制定的库
```sql
USE myemployees;
```
2. 查询表中单个字段
```sql
SELECT last_name FROM employees;
```
3. 查询表中多个字段
```sql
SELECT last_name,salary,email FROM employees;
```

4. 查询表中所有字段
workbench ctrl+b 格式化 sqlyog f12格式化

着重号 \`，tab上的那个按键，自动添加的，可以去掉。目的是强调这是个字段，对于与关键字重复的字段名使用

```sql
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
```

或者可以

```
SELECT * FROM employees; 
```
但这种方式顺序不够灵活

5. 查询常量值

``` 
SELECT 100;
SELECT 'john';
```
6. 查询表达式

```
SELECT 100*98;
```
7. 查询函数 调用该函数，得到返回值并显示
```
SELECT VERSION();
```
8. 起别名
>①便于理解
>②如果要查询的字段有重名，使用别名可以区分开来
```
SELECT 100*98 AS 结果;
SELECT last_name AS 姓,first_name AS 名 FROM employees;
```
>或者可以省略AS，空格就可以
```
SELECT last_name 姓,first_name 名 FROM employees;
```
>如果别名中有特殊符号，比如 # ，空格，关键字等，使用引号（单双都可以）包起来
```
SELECT salary AS 'out put' FROM employees;
```

9. 去重
查询员工表中所涉及到的部门编号
```
SELECT DISTINCT department_id FROM employees;
```
10. +号的作用

mysql中+只有运算符的作用
```
SELECT 100+90;
```
上边那句可以选中直接执行，因为选中的部分没有注释，选中谁执行谁
```sql
SELECT '123'+90;	其中一方为字符型，试图将字符型装换为数值型，转换成功继续运算
SELECT 'john'+90;	转换失败，将字符型数值转换为0
SELECT null+10;		只要其中一方为null，结果肯定为null
```
将姓名连接成一个字段
```sql
SELECT
 CONCAT(last_name,' ',first_name) AS 名字
FROM
 employees;
```
注意，如果要拼接的字段中有null，那么也会全部变成null
```sql
SELECT
 IFNULL(commission_pct,0) AS 奖金率,
 commission_pct
FROM
 employees;
```
IFNULL(将要进行判断的表达式，如果是null返回的值)
```
SELECT
 CONCAT(`first_name`,`last_name`,IFNULL(commission_pct,0))
FROM employees;
```
## 进阶2：条件查询
语法
```sql
SELECT
  查询列表
FROM
  表名
WHERE
  筛选条件;
```

分类：

  一、按条件表达式筛选

​    条件运算符：> < = 不等于两种皆可，推荐后边的!= <> >= <=

  二、按逻辑表达式筛选

​    逻辑运算符 && || ！，推荐使用 and or not

  三、模糊查询

​    like

​    between and

​    in

​    in null

*/

一、按条件表达式筛选
案例1：查询工资>12000的员工信息
```
SELECT * FROM employees WHERE salary>12000;
```
案例2：部门编号不等于90号的员工名和部门编号
```
SELECT CONCAT(first_name,'-',last_name) AS 姓名,department_id FROM employees WHERE department_id <> 90 
```
二、按逻辑表达式筛选
案例1： 查询工资在1w到2w之间的员工名，工资，以及奖金
```
SELECT 
  CONCAT(first_name,' ',last_name) AS 姓名,salary,commission_pct
FROM
  employees
WHERE
  salary>10000 AND salary<20000;
```
案例2：查询部门编号不是在90到110之间,或者工资高于1w5的员工信息
```
SELECT
  *
FROM
  employees
WHERE
  department_id<90 OR department_id>110 OR salary>15000
  或者
WHERE
  not(department_id>=90 and department_id<=110) or salary>15000;
```
三、模糊查询
1. LIKE
特点：
一般和通配符搭配使用
% 任意多个字符，包含0个
\_任意单个字符
```
between and

in

is null  is not null
```
案例1：查询员工名中包含字符a的员工信息
```
SELECT
  *
FROM
  employees
WHERE
  last_name LIKE '%a%';
```
%是通配符
案例2：查询名中第三个字符为n，第五个为l的员工名和工资
```
SELECT
  last_name,
  salary
FROM
  employees
WHERE
  last_name LIKE '__n_l%';
```
案例3：查询员工名中第二个字符为\_的员工
```
SELECT
  last_name
FROM
  employees
WHERE
  last_name LIKE '_\_%'
  或者
  last_name LIKE '_$_%' escape '$' $字符作为转义字符
```
2. between and
- 可以提高语句简洁度
- 左右两端包含，相当于>= <=
- 临界值不能调换顺序
案例1：查询员工编号在100-120之间
```
SELECT
  *
FROM
  employees
WHERE
  employee_id BETWEEN 100 AND 120;
```
3. in
- 含义：类似Python中的 in
特点：
- 简单，简洁
- 列表中的值类型必须一致或兼容
- 无法做模糊查询，因为in 等同于 =，模糊查询是like
案例：查询员工的编号是IT_PROG,AD_VP,AD_PRES中的员工
```
SELECT
  last_name,
  job_id
FROM
  employees
WHERE
  job_id IN ('IT_PROG','AD_VP','AD_PRES')
```
4. is null
- 使用 = 或者 <> 不能判断 null
案例1：查询没有奖金的员工名和奖金率
```
SELECT
  last_name,
  commission_pct
FROM
  employees
WHERE
  commission_pct IS NULL
  \#错误写法：salary is 12000
```
- 安全等于 <=>
	-可读性差
	-即可以判断null，又可以判断普通数值
```
SELECT
  last_name,
  commission_pct
FROM
  employees
WHERE
  commission_pct <=> NULL
```
案例2：查询工资12000的员工信息
```
SELECT
  last_name,
  salary
FROM
  employees
WHERE
  salary <=> 12000
```

```
SELECT * FROM employees;
```
和
```
SELECT *FROM employees WHERE commission_pct like '%%' and last_name like '%%';
```
一样吗?
不一样！，因为有null，如果没有null就一样

## 进阶3 排序查询
```
SELECT * FROM employees
```
顺序与原表一致，不方便
语法
```
SELECT 查询列表
FROM 表
[WHERE xxx]
order by 排序列表 [asc|desc]
```
特点：
- ASC升序，DESC降序，默认升序
- ORDER BY可以放多个字段，表达式，函数，别名
- 3.ORDER BY执行顺序在条件查询的基础上进行的，一般放在查询语句的最后面，但是limit子句除外
案例1：查询员工信息，按工资从高到低排序
>ASC可以省略
```
SELECT * FROM employees ORDER BY salary DESC;
SELECT * FROM employees ORDER BY salary ;
```

案例2：查询部门编号>=90的员工信息，按入职时间先后进行排序(添加筛选条件)
```
SELECT * FROM employees WHERE department_id >= 90 ORDER BY hiredate ASC;
```
案例3：按年薪高低显示员工信息和年薪（按表达式排序）
>表中并不存在年薪这一项
```
SELECT * ,salary*12*(1+IFNULL(commission_pct,0)) 年薪
FROM employees 
ORDER BY salary*12*(1+IFNULL(commission_pct,0)) DESC;
```

案例4：按年薪高低显示员工信息和年薪（按别名排序）
```
SELECT 
* ,salary*12*(1+IFNULL(commission_pct,0)) 年薪
FROM employees 
ORDER BY 年薪 DESC;
```
案例5：按姓名长度显示姓名和工资[按函数排序]
```
SELECT 
LENGTH(last_name) 字节长度, last_name, salary
FROM employees 
ORDER BY LENGTH(last_name);
```
案例6：查询员工信息，要求先按工资排序，再按员工编号排序[按多个字段排序]
先按谁排，谁就放在前边,主关键字和次要关键字的区别
```
SELECT * FROM employees ORDER BY salary ASC , employee_id DESC;
```
**练习**

案例1： 查询员工的姓名和部门号和年薪，按年薪降序按姓名升序
```
SELECT 
last_name,department_id,salary*12*(1+IFNULL(commission_pct,0)) 年薪 
FROM employees 
ORDER BY 年薪 ASC
```
案例2：选择工资不在8000到17000的员工的姓名和工资，按工资降序
```
SELECT last_name,salary FROM employees WHERE salary>17000 OR salary<8000 ORDER BY salary DESC;

SELECT last_name,salary FROM employees WHERE salary NOT BETWEEN 8000 AND 17000 ORDER BY salary DESC;
```
案例3：查询|邮箱中包含e的员工信息，并先按邮箱的字节数降序，再按部门号升序
```
SELECT *,LENGTH(email) FROM employees WHERE email LIKE '%e%'
ORDER BY LENGTH(email)DESC, department_id ASC;
```
## 进阶4：常见函数

分类：
1. 单行函数 如CONCAT, LENGTH, IFNULL等
2. 分组函数 传入一组值，返回一个值，做统计用，又称作统计函数，聚合函数，组函数

一、字符函数
1. LENGTH(str)获取参数值的字节个数
```
SELECT LENGTH('join');
SELECT LENGTH('张三');
```
utf8字母占一个字节，中文占三个
```
SHOW VARIABLES LIKE '%CHAR%'
```
2. CONCAT(str1,str2,...)拼接字符串
```
SELECT CONCAT(last_name,'_',first_name) FROM employees;
```
3. upper,lower
```
SELECT UPPER('join')
SELECT LOWER('join')
```
示例：性变大写，名变小写，然后拼接
```
SELECT 
CONCAT(upper(last_name),'_',lower(first_name)) 
FROM employees;
```
4. 截取字符 substr substring
- 注意，索引从1开始
```
SELECT SUBSTR('李莫愁爱上了陆展元',7);#陆展元
SELECT SUBSTR('李莫愁爱上了陆展元',1,3);#李莫愁
```
案例：姓名中姓首字符大写，其他小写然后用\_拼接
```
SELECT 
CONCAT(upper(SUBSTR(first_name,1,1)),
SUBSTR(first_name,2),
'_',
LOWER(last_name)) output 
FROM employees;
```
5. instr 子串第一次下标查找，找不到返回0
```
SELECT INSTR('杨不悔爱上了殷六侠','殷六侠');
```
6. trim 去掉空格
```
SELECT LENGTH(TRIM('    张翠山      '));
SELECT TRIM('a' FROM 'aaaaaaaa张aa翠aa山aaaaaaaaaaa') output;# 只去前后
```
7. lpad，rpad指定字符实现左填充，右填充，超过长度会截断
```
SELECT LPAD('殷素素',10,'*') output;
SELECT LPAD('殷素素',2,'*') output;
SELECT RPAD('殷素素',10,'*') output;
SELECT RPAD('殷素素',2,'*') output;
```
8. replace替换
```
SELECT REPLACE('张无忌周芷若爱周芷若上了周芷若','周芷若','赵敏')
```
二、数学函数
1. ROUND(X)四舍五入
```
SELECT round(1.65);
SELECT round(1.65456,2);
```
2. 取整
- ceil()向上取整 返回>=该参数的最小整数
- FLOOR(X)向下取整 返回<=该参数的最小整数
- TRUNCATE()截断
```
SELECT CEIL(1.01)
SELECT CEIL(1.00)
SELECT FLOOR(-9.99)
SELECT TRUNCATE(1.6634,2)
```
3. mod取余
>mod(a,b) a-a/b*b
>mod(-10,-3) -10-(-10)/(-3)*(-3)=-1
```
SELECT MOD(10,3);
SELECT 10%3
```

三、日期函数
1. now系统日期
```
SELECT NOW();
```
curdate()返回当前系统日期/时间，不包含另一个
```
SELECT CURRENT_DATE
SELECT CURDATE()
SELECT CURRENT_TIME
SELECT CURTIME()
```
可以获取指定部分的年月日
```
SELECT year(NOW()) 年;
SELECT year(2020-1-1) 年;
SELECT YEAR(hiredate) FROM employees
SELECT MONTHNAME(now())
```
str\_to\_date
案例1：查询入职日期为1992-4-3的员工信息
```
SELECT 
* 
FROM
employees 
WHERE
hiredate = '1992-4-3'

SELECT 
* 
FROM 
employees 
WHERE 
hiredate = STR_TO_DATE('4-3 1992','%c-%d %Y')
```
将日期转换成字符
案例2：查询有奖金的员工名和入职日期(xx月/xx日 xx年)
```
SELECT 
last_name,
DATE_FORMAT(hiredate,'%m月/%d日 %y年') 入职日期 
FROM 
employees 
WHERE 
commission_pct is not null
```
四、其他函数
```
SELECT VERSION()
SELECT DATABASE
SELECT user
```
五、流程控制函数
1. if函数
```
SELECT if(10>5,'大于','小于')
SELECT 
last_name,commission_pct,
if(commission_pct is null,'没奖金','有奖金') 备注 
FROM 
employees 
```

2. case结构
- 使用一，类似switch case，处理等值判断
案例1：查询员工工资，部门号=30，工资1.1倍; =40，1.2倍; =50，1.3倍
```
SELECT salary 原始工资, department_id,
CASE department_id
  WHEN 30 THEN
​    salary*1.1
  when 40 THEN
​    salary*1.2
  when 50 THEN
​    salary*1.3
  ELSE
​    salary
END AS 新工资
FROM employees;
```
- 使用二 类似于多重if，处理区间判断
案例2：查询员工工资情况，工资大于20000，显示A级别，大于15000B，大于10000C，否则D
```
SELECT salary,
CASE 
  WHEN salary>20000 THEN 'A'
  when salary>15000 then 'B' 
  when salary>10000 then 'C' 
  ELSE
​    'D'
END 工资级别
FROM employees;
```
## 进阶5 分组函数
sql99语法
内连接 
```
INNER
```
外连接
```
  左外 left 【OUTER】
  右外 right 【OUTER】
  全外 full 【outer】
```
交叉连接
```
CROSS
```
语法
```
  SELECT 查询列表
  FROM 表1 别名【连接类型】
  join 表2 别名 on 连接条件
  WHERE 筛选条件
  GROUP BY
  having
  ORDER BY
```

一、 内连接
语法：
```
SELECT 查询列表
FROM 表1 
inner join 表2
on 连接条件
```
1. 等值连接
案例1：查询员工名，部门名
```
SELECT last_name, department_name
FROM employees
inner JOIN departments
on employees.department_id = departments.department_id
```
案例2.查询名字中包含e的员工名和工种名(筛选)
```
SELECT last_name,job_title 
FROM employees
INNER JOIN jobs
on employees.job_id = jobs.job_id
WHERE last_name LIKE '%e%'
```
案例3.查询部门个数>3的城市名名和部门个数， (分组+筛选)
```
SELECT count(*),city
FROM departments
INNER JOIN locations
on departments.location_id = locations.location_id
group by city
having count(*)>3
```
案例4.查询哪个部门的部门员工个数>3的部门名和员工个数，并按个数降序(排序)
```
SELECT count(*),department_name
FROM employees
INNER JOIN departments
on employees.department_id = departments.department_id
GROUP BY departments.department_name
having count(*)>3
ORDER BY count(*) DESC
```
案例5.查询员工名、部门名、工种名，并按部门名降序
```
SELECT last_name,department_name,job_title
FROM employees
INNER JOIN jobs on employees.job_id = jobs.job_id
INNER JOIN departments on departments.department_id = employees.department_id
ORDER BY department_name DESC
```
2. 非等值连接
案例1：查询员工的工资级别
```
SELECT grade_level,salary
FROM employees
INNER JOIN job_grades
on employees.salary BETWEEN job_grades.lowest_sal and job_grades.highest_sal
```
案例2：查询每个工资级别的个数>2 的个数，并按工资降序排序
```
SELECT salary,grade_level,count(*)
FROM employees
INNER JOIN job_grades
on employees.salary between job_grades.lowest_sal and job_grades.highest_sal
GROUP BY job_grades.grade_level
having count(*)>2
ORDER BY grade_level desc
```
3. 自连接
案例1：查询员工的名字，上级的名字
```
SELECT e.last_name 员工名,m.last_name 上级名
FROM employees e
INNER JOIN employees m
on e.manager_id = m.employee_id
```
二、 外连接
一般应用与查询一个表中，另一个表中没有的记录
分主从表
特点
1. 外连接的查询结果为主表中的所有记录
	- 如果从表中有匹配的，则显示匹配的值，反之显示null
	- 查询结果 = 内连接结果+主表有从表没有的记录
2. 左外连接、left左边的是主表
   右外连接、right 右边的是主表
3. 左外右外交换量表顺序可以实现同样结果
	- 你要查询的信息主要来自哪个表，哪个表就是主表

案例1：查询男朋友不在男神表的女神名
左外连接
```
SELECT beauty.`name`,boys.*
FROM beauty
LEFT JOIN boys
on beauty.boyfriend_id = boys.id
WHERE boys.id is null
```
右外连接
```
SELECT beauty.`name`,boys.*
FROM boys
right JOIN beauty
on beauty.boyfriend_id = boys.id
WHERE boys.id is null
```
案例2：查找哪个部门没有员工
左外
```
SELECT departments.*,employees.employee_id
FROM departments
LEFT JOIN employees
on departments.department_id = employees.department_id
WHERE employees.employee_id is null
```
右外
```
SELECT departments.*,employees.employee_id
FROM employees
right JOIN departments
on departments.department_id = employees.department_id
WHERE employees.employee_id is null
```

三、交叉连接
两表进行笛卡尔乘积
```
SELECT boys.*,beauty.*
FROM beauty
CROSS JOIN boys
```
**练习**
1. 查询编号>3的女神的男朋友信息，如果有则列出详细，如果没有，用nul1填充
```
SELECT beauty.id,beauty.`name`,boys.*
FROM beauty
left JOIN boys
on beauty.boyfriend_id = boys.id
WHERE beauty.id>3
```
2. 查询哪个城市没有部门
```
SELECT city
FROM locations
LEFT JOIN departments
on locations.location_id = departments.location_id
WHERE departments.department_id is null
```
3. 查询部门名为SAL或IT的员工信息
```
SELECT departments.department_name,employees.*
FROM employees
LEFT JOIN departments
on employees.department_id = departments.department_id
WHERE departments.department_name IN('SAL','IT')
```
## 进阶5分组查询
```
SELECT 分组函数，列（要求出现在group by的后边）
FROM 表名
WHERE 筛选条件
GROUP BY 分组的列表
order by 子句
```
注意：查询列表比较特殊，要求是分组函数和group by 后出现的字段

特点：

1. 筛选条件分为两类

	- 分组前筛选 WHERE   数据源是原始表       GROUP BY前边 WHERE
	- 分组后筛选 having      数据源是分组后的结果集   GROUP BY后边 having 
	- 分组函数做条件(max min count)，肯定是放在having子句中
	- 尽量使用分组前筛选
2. group by 支持多个字段分组，用逗号隔开，没有顺序要求，也可以用表达式
案例1：查询每个工种的最高工资
简单的分组查询
```
SELECT max(salary),job_id
FROM employees
GROUP BY job_id;
```
案例2：查询每个位置上的部门个数
```
SELECT count(*),location_id
FROM departments
GROUP BY location_id;
```
添加筛选条件
案例1：查询邮箱中包含a字符的，每个部门的平均工资
```
SELECT AVG(salary),department_id
FROM employees
WHERE email like '%a%'
GROUP BY department_id;
```
案例2：查询有奖金的每个领导手下员工的最高工资
```
SELECT max(salary),manager_id
FROM employees
WHERE commission_pct is not null
GROUP BY manager_id
```
添加复杂的筛选条件
案例1：查询哪个部门的员工个数大于2
step1 查询每个部门的员工个数
```
SELECT count(*),department_id
FROM employees
GROUP BY department_id
```
step2 根据1的结果进行筛选，查询哪个部门的员工个数大于2
添加分组后的筛选
```
SELECT count(*),department_id
FROM employees
GROUP BY department_id
HAVING count(*)>2
```
案例2：查询每个工种有奖金的员工的最高工资>12000的工种编号和最高工资
step1 查询每个工种有奖金的最高工资
```
SELECT max(salary),job_id
FROM employees
WHERE commission_pct is not null
GROUP BY job_id
```
step2 根据1的结果继续筛选最高工资>12000
```
SELECT max(salary),job_id
FROM employees
WHERE commission_pct is not null
GROUP BY job_id
HAVING max(salary) > 12000;
```
案例3：查询领导编号>102的每个领到手下的最低工资>5000的领导编号以及其最低工资
```
SELECT manager_id,MIN(salary)
FROM employees
WHERE manager_id>102
GROUP BY manager_id
having MIN(salary)>5000;
```
按表达式或者函数分组
案例：按员工姓名的长度分组，查询每一组的员工个数，筛选员工个数>5的有哪些
```
SELECT count(*) c,LENGTH(last_name) len_name
FROM employees
GROUP BY len_name
having c>5
```
MySQL支持别名，其他的数据库可能不支持
按多个字段分组
案例：查询每个部门每个工种的员工的平均工资
```
SELECT AVG(salary),department_id,job_id
FROM employees
GROUP BY department_id,job_id
```
只有两个都符合时才会被选中

添加排序
案例：查询每个部门每个工种的员工的平均工资，并按工资高低显示出来
```
SELECT AVG(salary),department_id,job_id
FROM employees
WHERE department_id is not null
GROUP BY department_id,job_id
having avg(salary)>10000
ORDER BY AVG(salary) DESC
```
**题目**

1. 查询各job\_ id的员工工资的最大值，最小值，平均值，总和，并按job\_id升序
```
SELECT max(salary),min(salary),avg(salary),sum(salary)
FROM employees
GROUP BY job_id
ORDER BY job_id asc
```
2. 查询员工最高工资和最低工资的差距( DIFFERENCE)
```
SELECT MAX(salary)-min(salary) DIFFERENCE
FROM employees
```
3. 查询各个管理者手下员工的最低工资，其中最低工资不能低于6000，没有管理者的员工不计算在内
```
SELECT min(salary),manager_id
FROM employees
WHERE manager_id is not null
GROUP BY manager_id
having min(salary)>=6000
```
4. 查询所有部门的编号，员工数量和工资平均值，并按平均工资降序
```
SELECT department_id, count(*),AVG(salary)
FROM employees
group by department_id
order by AVG(salary) ASC
```
5. 选择具有各个job_ id的员工人数
```
SELECT count(*),job_id
FROM employees
WHERE job_id is not null
GROUP BY job_id
```
## 进阶6 连接查询

多表查询
当查询的字段来自做个表时

笛卡尔积现象：
假如表1有m行，表2有n行，结果有m\*n行，就是笛卡尔积现象，
发生原因：因为没有有效的连接条件
如何避免：添加有效的连接条件

分类：
- 按年代分类
	- sql92 仅支持内联
	- sql99 推荐 支持内，外（左外右外），交叉
- 按功能分类：
	- 内连接
	- 等值连接
	- 非等值连接
	- 自连接
	- 外连接
	- 左外连接
	- 右外连接
	- 全外连接
	- 交叉连接

一、等值连接
案例1：查询女神男神对应名
```
SELECT name ,boyName FROM beauty,boys
WHERE beauty.boyfriend_id = boys.id
```
案例2：查询员工名和对应的部门名
```
SELECT last_name, department_name
FROM employees,departments
WHERE employees.department_id = departments.department_id
```
1. 为表起别名 AS可以省略
- 提高了语句的简洁度
- 区分多个重名字段
- 起别名后原始表明不能用，只能使用别名

案例：查询员工名，工种号，工种名
```
SELECT last_name,e.job_id,job_title
FROM employees AS e,jobs j
WHERE e.job_id = j.job_id
```
2. 可以加筛选
案例1：查询有奖金的员工名，部门名
```
SELECT last_name,department_name,commission_pct
FROM employees,departments
WHERE employees.department_id = departments.department_id
AND employees.commission_pct is not null
```
案例2：查询城市名中第二个字符为o的部门和城市名
```
SELECT departments.department_name , city
FROM departments,locations
WHERE departments.location_id = locations.location_id
AND city LIKE "_o%"
```
3. 可以加分组
-- 案例1：查询每个城市的部门个数
```
SELECT count(*),city
FROM departments,locations
WHERE departments.location_id = locations.location_id
GROUP BY city
```
案例2：查询有奖金的每个部门的部门名和部门的领导编号，以及该部门的最低工资
```
SELECT department_name,d.manager_id,min(salary)
FROM departments d,employees AS e
WHERE commission_pct is not null
AND d.department_id = e.department_id
GROUP BY d.department_id,manager_id
```
4. 可以加排序
案例：查询每个工种的工种名，员工个数，按员工个数降序
```
SELECT job_title,count(*)
FROM jobs AS j,employees e
WHERE j.job_id = e.job_id
GROUP BY j.job_id
ORDER BY count(*) DESC
```
5. 三表连接
查询员工名，部门名与所在的城市
```
SELECT last_name,department_name,city
FROM employees,departments,locations
WHERE employees.department_id = departments.department_id
AND departments.location_id = locations.location_id
AND city LIKE 's%'
ORDER BY department_name DESC
```
二、 非等值连接
案例1：查询员工的工资和工资级别
```
SELECT salary,grade_level,last_name
FROM employees,job_grades
WHERE salary BETWEEN lowest_sal and highest_sal
AND grade_level = 'A'
```
三、 自连接
案例：查询员工名以及上级的名字
```
SELECT e.last_name,e.employee_id,m.employee_id,m.last_name
FROM employees e,employees m
WHERE e.manager_id = m.employee_id
```
**测试：**
1. 显示员工表的最大工资，工资平均值
```
SELECT max(salary),min(salary),avg(salary)
FROM employees
```
2. 查询员工表的employee\_ id, job\_ id, last. name，按department\_ id降序，salary升序
```
SELECT employee_id,job_id,last_name
FROM employees
ORDER BY department_id DESC,salary ASC
```
3. 查询员工表的job\_ id中包含 a和e的，并且a在e的前面
```
SELECT job_id 
FROM employees
WHERE job_id LIKE '%a%e%'
```
4. 已知表student, 里面有id(学号)，
name,
gradeId (年级编号)
已知表grade,里面有id (年级编号)，name (年级名)
已知表result,里面有id, score, studentNo (学号)
要求查询姓名，年级名，成绩
```
SELECT s.name,g.name,r.score
FROM student s,grade g,result r
WHERE s.id = studentNo
and g.id - s.gradeid
```
5. 显示当前日期，以及去前后空格，截取子字符串的函数
```
SELECT now();
SELECT trim('');
SELECT trim('字符 FROM');去除指定字符
SELECT substr(str,起始索引(从1开始))
```
上边表的插入语法
```
CREATE TABLE job_grades
(grade_level VARCHAR(3),
 lowest_sal int,
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
```
sql99语法
- 内连接 INNER
- 外连接
	- 左外 left 【OUTER】
	- 右外 right 【OUTER】
	- 全外 full 【outer】
- 交叉连接 CROSS

语法
```
SELECT 查询列表
FROM 表1 别名【连接类型】
join 表2 别名 on 连接条件
WHERE 筛选条件
GROUP BY
having
ORDER BY
```
一、 内连接
语法：
```
SELECT 查询列表
FROM 表1 
inner join 表2
on 连接条件
```
1. 等值连接
案例1：查询员工名，部门名
```
SELECT last_name, department_name
FROM employees
inner JOIN departments
on employees.department_id = departments.department_id
```
案例2.查询名字中包含e的员工名和工种名(筛选)
```
SELECT last_name,job_title 
FROM employees
INNER JOIN jobs
on employees.job_id = jobs.job_id
WHERE last_name LIKE '%e%'
```
案例3.查询部门个数>3的城市名名和部门个数， (分组+筛选)
```
SELECT count(*),city
FROM departments
INNER JOIN locations
on departments.location_id = locations.location_id
group by city
having count(*)>3
```
案例4.查询哪个部门的部门员工个数>3的部门名和员工个数，并按个数降序(排序)
```
SELECT count(*),department_name
FROM employees
INNER JOIN departments
on employees.department_id = departments.department_id
GROUP BY departments.department_name
having count(*)>3
ORDER BY count(*) DESC
```
案例5.查询员工名、部门名、工种名，并按部门名降序
```
SELECT last_name,department_name,job_title
FROM employees
INNER JOIN jobs on employees.job_id = jobs.job_id
INNER JOIN departments on departments.department_id = employees.department_id
ORDER BY department_name DESC
```
二、 非等值连接

案例1：查询员工的工资级别
```
SELECT grade_level,salary
FROM employees
INNER JOIN job_grades
on employees.salary BETWEEN job_grades.lowest_sal and job_grades.highest_sal
```
案例2：查询每个工资级别的个数>2 的个数，并按工资降序排序
```
SELECT salary,grade_level,count(*)
FROM employees
INNER JOIN job_grades
on employees.salary between job_grades.lowest_sal and job_grades.highest_sal
GROUP BY job_grades.grade_level
having count(*)>2
ORDER BY grade_level desc
```
三、 自连接
查询员工的名字，上级的名字
```
SELECT e.last_name 员工名,m.last_name 上级名
FROM employees e
INNER JOIN employees m
on e.manager_id = m.employee_id
```
四、外连接
- 一般应用与查询一个表中，另一个表中没有的记录
- 分主从表
特点
1.外连接的查询结果为主表中的所有记录
	- 如果从表中有匹配的，则显示匹配的值，反之显示null
	- 查询结果 = 内连接结果+主表有从表没有的记录
2. 左外连接、left左边的是主表
   右外连接、right 右边的是主表
3. 左外右外交换量表顺序可以实现同样结果
   你要查询的信息主要来自哪个表，哪个表就是主表

案例1：查询男朋友不在男神表的女神名
左外连接
```
SELECT beauty.`name`,boys.*
FROM beauty
LEFT JOIN boys
on beauty.boyfriend_id = boys.id
WHERE boys.id is null
```
右外连接
```
SELECT beauty.`name`,boys.*
FROM boys
right JOIN beauty
on beauty.boyfriend_id = boys.id
WHERE boys.id is null
```
案例2：查找哪个部门没有员工
左外
```
SELECT departments.*,employees.employee_id
FROM departments
LEFT JOIN employees
on departments.department_id = employees.department_id
WHERE employees.employee_id is null
```
右外
```
SELECT departments.*,employees.employee_id
FROM employees
right JOIN departments
on departments.department_id = employees.department_id
WHERE employees.employee_id is null
```
五、交叉连接
两表进行笛卡尔乘积
```
SELECT boys.*,beauty.*
FROM beauty
CROSS JOIN boys
```

**练习**
1. 查询编号>3的女神的男朋友信息，如果有则列出详细，如果没有，用nul1填充
```
SELECT beauty.id,beauty.`name`,boys.*
FROM beauty
left JOIN boys
on beauty.boyfriend_id = boys.id
WHERE beauty.id>3
```
2. 查询哪个城市没有部门
```
SELECT city
FROM locations
LEFT JOIN departments
on locations.location_id = departments.location_id
WHERE departments.department_id is null
```
3. 查询部门名为SAL或IT的员工信息
```
SELECT departments.department_name,employees.*
FROM employees
LEFT JOIN departments
on employees.department_id = departments.department_id
WHERE departments.department_name IN('SAL','IT')
```
## 进阶7子查询

含义：
出现在其他语句中的SELECT语句，称为子查询或内查询
外部的查询语句，称为主查询或外查询

分类：
按子查询出现的位置：
- SELECT后边：
	- 支持标量子查询
- FROM后边：
	- 支持表子查询
- WHERE或者having后边【重要】
	- 支持标量子查询（单行）【重要】
	- 列子查询（多行）【重要】
	- 行子查询（用的较少）
- exists后边（相关子查询）
	- 表子查询
- 按功能/结果集的行列数不同
	- 标量子查询（结果集只有一行一列）
	- 列子查询（结果集只有一列多行）
	- 行子查询（结果集只有一行多列）
	- 表子查询（结果集一般为多行多列）
一、WHERE having后边
1. 标量子查询（单行子查询）
2. 列子查询（多行子查询）
3. 行子查询（多列多行）

特点：

①子查询放在小括号内

②一般放在条件的右侧

③标量子查询一般大配置单行操作符使用

< > <= >= = <>

列子查询，一般搭配着多行操作符使用
IN，ANY/SOME,ALL

④子查询的执行优先于主查询执行，主查询执行用到了子查询结果
1. 标量子查询
案例1：谁的工资比Abel高？
step1 查询名字叫Abel的工资
step2 查询员工信息，满足salary>上一步
```
SELECT *
FROM employees
WHERE salary>(
  SELECT salary
  FROM employees
  WHERE last_name = 'Abel'
)
```
案例2:题目:返回job\_ id与141号员工相同，salary比143号员工多的员工姓名，job\_ id和工资
```
SELECT last_name,job_id,salary
FROM employees
WHERE salary > (
  SELECT salary
  FROM employees
  WHERE employee_id = 143
)
AND job_id = (
  SELECT job_id
  FROM employees
  WHERE employee_id = 141
)
```
案例3:返回公司工资最少的员工的last\_ name, job\_ id和salary|
```
SELECT last_name,job_id,salary
FROM employees
WHERE salary = (
  SELECT MIN(salary)
  FROM employees
)
```
案例4:查询最低工资大于50号部门最低工资的部门id和其最低工资
```
SELECT department_id,MIN(salary)
FROM employees
GROUP BY department_id
HAVING MIN(salary) > (
  SELECT MIN(salary)
  FROM employees
  WHERE department_id = 50
)
```
非法使用标量子查询
```
SELECT department_id,MIN(salary)
FROM employees
GROUP BY department_id
HAVING MIN(salary) > (
  SELECT salary #这里有错误，应该是标量子查询
  FROM employees
  WHERE department_id = 50
)
```
2. 列子查询（多行子查询）
- 多行操作符：IN/NOT IN,ANY SOME ALL
- ANY/SOME 用得少，有其他方式代替，且可读性更高
- a > ANY(10,20,30),a大于任意一个就可以比如a=15
- 可以换成
- a > MIN(10,20,30)

- a > ALL(10,20,30),a大于所有的
- 可以替换为
- a > MAX(10,20,30)
案例1:返回location\_ id是1400或1700的部门中的所有员工姓名
```
SELECT last_name
FROM employees
WHERE department_id IN (
  SELECT DISTINCT department_id
  FROM departments
  WHERE location_id IN (1400,1700)
)
```
这里的 IN 可以用 =ANY 代替
NOT IN 可以用 <> ALL

案例2:返回其它工种中比job\_ id为'IT_PROG'部门任一工资低的员工的员工号、姓名、job\_ id以及salary
```
SELECT employee_id,last_name,job_id,salary
FROM employees
WHERE salary < ANY(
  SELECT DISTINCT salary
  FROM employees
  WHERE job_id = 'IT_PROG'
)
AND job_id<>'IT_PROG'
```
案例3:返回其它部门中比job\_ id为'IT_ PROG' 部门所有工资都低的员工的员工号、姓名、job_ id以及salary .
```
SELECT employee_id,last_name,job_id,salary
FROM employees
WHERE salary < ALL(
  SELECT DISTINCT salary
  FROM employees
  WHERE job_id = 'IT_PROG'
)
AND job_id<>'IT_PROG'
```
3. 行子查询（结果集一行多列或者多行多列）
案例:查询员工编号最小并且工资最高的员工信息
```
SELECT *
FROM employees
WHERE (employee_id,salary)=(
  SELECT MIN(employee_id),MAX(salary)
  FROM employees
)
```
- 所有条件都用一样的操作符=<>时才能用，将这两个当做一个虚拟字段的去用
- 一般可以分别查，在用AND

二、 放在SELECT后边
- 仅仅支持标量子查询
案例1：查询部门的员工个数
```
SELECT d.* , (
  SELECT COUNT(*)
  FROM employees e
  WHERE e.department_id = d.department_id
)
FROM departments d;
```
案例2：查询员工号 = 102的部门名
```
SELECT (
  SELECT department_name 
  FROM departments d
  INNER JOIN employees e
  ON d.department_id = e.department_id
  WHERE employee_id = 102
)
```
- 看似外边那一层SELECT是有问题的，其实这就让他只能返回一行一列，
- 你可以去掉WHERE跑一下里边的那句，

三、FROM后边
- 将子查询结果充当一张表，要求必须起别名
- 案例：每个部门的平均工资的工资等级
```
SELECT ag_dep.*,g.grade_level
FROM(
  SELECT AVG(salary) ag,department_id
  FROM employees
  GROUP BY department_id
) ag_dep
INNER JOIN job_grades g
ON ag_dep.ag BETWEEN lowest_sal AND highest_sal
```
四、 exist后边（相关子查询）
- 只关心是否存在，布尔类型
案例1：查询有员工的部门名
```
SELECT department_name
FROM departments
WHERE department_id in(
  SELECT department_id
  FROM employees
)
```
或者
```
SELECT department_name
FROM departments
WHERE EXISTS(
  SELECT*
  FROM employees
  WHERE departments.department_id = employees.department_id
  )
```
大量案例

1. 查询和Zlotkey相同部门的员工姓名和工资
```
SELECT last_name,salary
FROM employees
WHERE department_id = (
  SELECT department_id
  FROM employees
  WHERE last_name = 'Zlotkey'
)
```
2. 查询工资比公司平均工资高的员工的员工号，姓名和工资.
```
SELECT employee_id,last_name,salary
FROM employees
WHERE salary > (
  SELECT AVG(salary)
  FROM employees
)
```
3. 查询各部门中工资比本部门平均工资高的员工的员工号，姓名和工资
```
SELECT employee_id,last_name,salary
FROM employees e
INNER JOIN (
  SELECT AVG(salary) ag,department_id
  FROM employees
  GROUP BY department_id
)ag_dep
on e.department_id = ag_dep.department_id
WHERE salary>ag_dep.ag
```
4. 查询和姓名中包含字母u的员工在相同部门的员工的员工号和姓名
```
SELECT last_name,employee_id
FROM employees
WHERE department_id IN(
  SELECT DISTINCT department_id
  FROM employees
  WHERE last_name LIKE '%u%'
)
```
5. 查询在部门的location\_ id为1700的部门工作的员工的员工号
```
SELECT employee_id,department_id
FROM employees
WHERE department_id in (
  SELECT department_id
  FROM departments
  WHERE location_id = 1700
)
```
6. 查询管理者是King的员工姓名和工资
```
SELECT last_name,salary
FROM employees
WHERE manager_id in (
  SELECT employee_id
  FROM employees
  WHERE last_name = 'K_ing'
)
```
7. 查询工资最高的员工的姓名，要求first\_name和last\_name显示为一-列，列名为姓.名
```
SELECT CONCAT(first_name,' ',last_name) '姓名'
FROM employees
WHERE salary = (
  SELECT MAX(salary)
  FROM employees
  )
```

## 进阶8分页查询 重点！
应用场景:要显示的数据一页显示不全，分页提交请求
语法：
```
SELECT 查询列表
FROM 表
WHERE 筛选条件
limit offset, size;
limit 起始索引，从0开始；条目数
```
limit放在最后
公式：要显示的页数为page，每页条目为size
```
LIMIT (page-1)*size size
```
案例1：查询前五条员工信息
```
SELECT * FROM employees LIMIT 0,5;
SELECT * FROM employees LIMIT 5;
```
案例2：查询第11到第25条
```
SELECT * FROM employees LIMIT 10,15
```
案例3：有奖金的员工信息，并且工资较高的前10名
```
SELECT * FROM employees WHERE commission_pct is not null ORDER BY salary DESC LIMIT 10
```
# 数据增删改
DML语言
数据操作语言
数据
插入，insert
修改，update
删除，DELETE
一、 插入语句
语法
经典插入语句：
```
insert INTO 表名(列名，。。。)
VALUES(值1，值2，。。。和列一一对应)
```
- 注意1：插入值得类型与列的类型一致
```
INSERT INTO beauty(id,Name,sex,borndate,phone,photo,boyfriend_id)
VALUES(13,'唐艺昕','女','1990-4-23',1898888,NULL,2)
```
- 注意2：不可以为null的列必须插入值，可以为null的列如何插入值？
方式一：
```
INSERT INTO beauty(id,Name,sex,borndate,phone,photo,boyfriend_id)
VALUES(13,'唐艺昕','女','1990-4-23',1898888,NULL,2)
```
方式二：
```
INSERT INTO beauty(id,Name,phone)
VALUES(14,'娜扎',12323898888)
```
- 注意3：列的顺序可以调换，但是要一一对应
- 注意4：列数和值的个数必须一致
- 注意5：可以省略列名，默认是所有列，列的顺序和表中顺序一致
```
INSERT INTO beauty
VALUES(15,'张飞','男','1990-2-28',1123123,NULL,9)
```
插入语句方式二：
```
insert INTO 表名
set 列名=值，列名2=值2。。。
```
```
INSERT INTO beauty
SET id = 19,NAME='刘涛',sex = '女',phone = 909090 
```
两种方式比较：
方式一支持一次插入多行，方式二不支持
```
insert into beauty
VALUES(13,'唐艺昕','女','1990-4-23',1898888,NULL,2),
(14,'张艺昕','女','1990-4-23',1898888,NULL,2),
(15,'王艺昕','女','1990-4-23',1898888,NULL,2),
(16,'李艺昕','女','1990-4-23',1898888,NULL,2)
```
方式一支持子查询方式二不支持
```
insert into beauty
SELECT id,boyName,'118118118'
FROM boys WHERE id < 3;
```
二、 修改数据
- 修改单表的记录（掌握）
```
UPDATE 表名
set 列 = 新值，列=新值。。。
WHERE 筛选条件;
```
案例1：修改beauty中姓唐的女神电话为1234567
```
UPDATE beauty set phone = 1234567
WHERE name LIKE '唐%'
```
案例2：修改boy表id号2的名称为张飞，魅力值10
```
UPDATE boys set boyName = '张飞',userCP = 10
WHERE id = 2
```
- 修改多表的记录（补充）
- 语法
- sql92
```
UPDATE 表一 别名，表二 别名
set 列= 值，。。。
WHERE 连接条件
and 筛选条件；
只支持内联
```
- sql99
```
update 表1 别名
inner|left|right join 表2 别名
on 连接条件
set 列= 值，。。。
WHERE 筛选条件
```
案例1：修改张无忌的女朋友的手机号为114
```
update boys bo
inner join beauty b ON bo.id = b.boyfriend_id
set b.phone = 333
WHERE bo.boyName = '张无忌'
```
案例2：修改没有男朋友的女神的男朋友编号都为张飞
```
update beauty b
left join boys bo on b.boyfriend_id = bo.id
set b.boyfriend_id = 2
WHERE b.boyfriend_id is null
```
三、删除语句
- 方式一：DELETE
- 语法
1. 单表★
```
DELETE FROM 表名 WHERE 筛选条件
```
案例1：手机编号最后一位是9
```
DELETE FROM beauty WHERE phone LIKE '%9'
```
2. 多表删除
案例1：删除张无忌的女朋友的信息
- sql99
```
DELETE b 
FROM beauty b
INNER JOIN boys bo on b.boyfriend_id = bo.id
WHERE bo.boyName = '张无忌';
```
案例2：删除黄晓明的信息以及他女朋友的信息
```
DELETE bo,b
FROM beauty b
inner JOIN boys bo on b.boyfriend_id = bo.id
WHERE bo.boyName = '黄晓明'
```
方式二：TRUNCATE
TRUNCATE 不允许有WHERE，用于清空数据
案例：将魅力值>100的男神信息删除
```
TRUNCATE TABLE boys 
```
DELETE 和 TRUNCATE 区别

1. DELETE可以加条件，TRUNCATE不可以加

2. TRUNCATE效率高一丢丢

3. 假如我们要删除的表中有自增长列，DELETE删除后再插入数据，自增长列的值从断点开始

TRUNCATE删除后再插入数据，自增长列从1开始

4. TRUNCATE删除没有返回值，DELETE有返回值

5. TRUNCATE删除后不能回滚，DELETE删除可以回滚
# 表和库的关系
DDL数据定义语言
设计库和表的管理
一、库的管理
创建，修改，删除
二、表的管理
创建，修改，，删除

创建 create
修改 alter
删除 drop

一、库的管理
1. 库的创建
```
if not EXISTS 容错性处理
CREATE database 库名
```
案例：创建图书库：
```
CREATE DATABASE if not EXISTS books
```
2. 库的修改（下边这句已经不能用了，知道即可）
```
RENAME DATABASE books TO 新库名;
```
更改库的字符集
```
alter DATABASE books CHARACTER SET utf8;
```
3. 库的删除
```
drop DATABASE if EXISTS books;
```
二、表的管理
1. 表的创建[这个为可选]
```
create table 表名(
  列名 列的类型[(长度) 约束],
  列名 列的类型[(长度) 约束],
  列名 列的类型[(长度) 约束],
  列名 列的类型[(长度) 约束]
)
```
books里创建book表
```
create table book(
  id INT,#编号
  bName varchar(20),#图书名
  price double,#价格
  authorId INT,#作者
  publishDate datetime #出版日期
)
```
显示表的内容
```
desc book
```
```
create table if not EXISTS author(
  id INT,
  au_name VARCHAR(20),
  nation VARCHAR(10)
)
```
2. 表的修改
语法
```
alter table 表名 add/drop/modify/change COLUMN 列名 类型 约束
```
修改列名
```
alter table book change COLUMN publishDate pubDate datetime
column 可以省略
```
修改列的类型或约束
```
alter table book modify COLUMN pubDate TIMESTAMP;
```
添加新列
```
alter table author add column annual DOUBLE
```
删除列
```
alter table author drop COLUMN annual
```
修改表名
```
alter table author rename to book_author;
```
3. 表的删除
```
drop table if EXISTS book_author 
```
```
show TABLES
```
通用的写法
```
DROP DATABASE if EXISTS 旧库名
create DATABASE 新库名 
```
```
DROP TABLE if EXISTS 旧表名
create TABLE 新表名  
```
4. 表的复制
```
insert into author VALUES
(1,'村上春树','日本'),
(2,'莫言','中国'),
(3,'冯唐','中国'),
(4,'金庸','中国')
```
(1)仅仅复制表的结果
```
create table copy LIKE author
```

(2)复制表的结构和数据
```
create table copy2 
SELECT * FROM author
```
(3)只复制部分数据
```
create table copy3
SELECT id,au_name
FROM author
WHERE nation = '中国';
```
(4)只复制某些字段
```
create table copy4
SELECT id,au_name
FROM author
WHERE 1=2
```
表中没有符合条件的数据，所以只是一个空表

# 数据类型介绍
- 常见的数据类型
1. 数值型：
	- 整型
	- 小数：定点数
	- 浮点型
2. 字符型：
	- 较短的文本 char VARCHAR
	- 较长的文本 text blob(较长的二进制数据)

3. 日期：
一、整型
|类型|字节数|
|:----:|:----:|
|TINYINT    | 1字节|
|SMALLINT   | 2字节|
|MEDIUMINT  | 3字节|
|INT INTEGER| 4字节|
|BIGINT     | 8字节|

特点：
- 默认有符号，设置无符号要添加unsigned
- 如果不设置长度会有默认长度,
- 整形的长度代表显示结果的宽度，不够左填0，搭配zerofill，默认会变成无符号
```
t1 INT(7) ZEROFILL
```
案例1：如何设置无符号和有符号
```
drop table if EXISTS tab_int
create table tab_int(
  t1 INT,
  t2 int UNSIGNED
)

INSERT into tab_int VALUES(-123456)
INSERT into tab_int VALUES(-123456,-123456)
```
```
desc tab_int
```

二、小数

- 浮点型
|类型|字节数|
|:---:|:---:|
|FLOAT(M,D) |4|
|DOUBLE(M,D) |8|

精确度要求较高的使用

- 定点型
```
DEC(M,D)
DECIMAL(M,D)
```
特点
- M  代表整数部位+小数部位总长度
- D  为小数部分范围
- M，D都可以省略
- DECIMAL的(M,D)默认为(10,0)
- float 和double会根据插入数值的精度来决定精度
- 定点型精确度较高
原则：
- 所选的类型越简单越好，能保存数值的类型越小越好
```
CREATE table tab_float(
  f1 FLOAT(5,2),
  f2 DOUBLE(5,2),
  f3 DECIMAL(5,2)
)
```
正常
```
insert into tab_float values(123.45,123.45,123.45)
```
会四舍五入
```
insert into tab_float values(123.456,123.456,123.456)
```
会补0
```
insert into tab_float values(123.4,123.4,123.4)
```
报错
```
insert into tab_float values(1231.4,1233.4,1233.4)
```
三、字符型
- 较短的文本
	- M个 字符 数
	- char(M)       固定长度字符   比较耗费空间 性能略高  M可省略，默认1
	- VARCHAR(M)   可变长度字符   比较节省空间 性能略低  不可省略

- 较长的文本
	- text
	- blob(较大的二进制)

- 枚举只能从列表中选一个，集合可以选多个
- 不区分大小写，会自动做大小写转换

- 枚举 enum
```
create table tab_char(
  c1 enum('a','b','c')
)

insert into tab_char VALUES('a');
insert into tab_char VALUES('b');
insert into tab_char VALUES('c');
insert into tab_char VALUES('d');
insert into tab_char VALUES('A');
```
```
create table tab_set(
  s1 SET('a','b','c')
)
insert into tab_set VALUES('a');
insert into tab_set VALUES('a,b');
insert into tab_set VALUES('a,c,d');
```
四、 日期型：
|类型|字节|区别|
|:---:|:---:|:---:|
|date     |4字节 |  只保存日期|
|datetime |8字节 | 保存日期时间|
|TIMESTAMP|4字节 | 保存日期时间|
|time     |3字节 | 只保存时间|
|YEAR     |1字节 | 只保存年|
时间戳与datetime
|类型 |时间范围| 备注|
|:--:|:--:|:--:|
|datetime  |1000 - 9999 |  不受时区影响|
|TIMESTAMP |  1970 - 2038|   受时区影响|
```
create table tab_date(
​    t1 datetime,
​    t2 TIMESTAMP
)

insert into tab_date values(now(),NOW());
```
```
show VARIABLES like 'time_zone'
```
```
set time_zone = '+9:00'
```
# 常见约束
- 含义：一种限制，用来限制表中的数据，为了保证表中数据的准确和可靠性
- 分类：六大约束 NOT NULL 非空约束，用来保证该字段的值不为空
	- 如姓名，学号等 DEFAULT 默认约束，用来保证该字段的值有默认值
	- 比如性别 PRIMARY KEY 主键，保证该字段的值具有唯一性，并且非空
	- 比如学号，员工编号 UNIQUE 唯一约束，保证该字段的值具有唯一性，但是可以为空
	- 比如座位号 CHECK mysql不支持，不报错但是没效果 比如年龄性别 FOREIGN KEY 外键约束，用于限制两个表的关系，用于保证该字段的值必须来自主表的关联列的值,在从表添加外键约束，引用主表某字段的值
	- 比如学生表的专业编号，员工表的部门编号，员工表的工种编号

添加约束时机：创建表时和修改表时
总之在数据添加之前

约束的添加分类：列级约束
六大约束语法上都支持，但外键约束没效果
表级约束
出了非空和默认都可以写
主键和唯一的对比：

|名|唯一性|为空？|组合|
|:---:|:---:|:---:|:---:|
|主键| 保证唯一性 | 不允许为空 | 允许组合（不推荐）|
|唯一| 保证唯一性  |允许为空  | 允许组合（不推荐）|

外键：
1. 在从表设置外键关系 
2. 从表的外键列的类型和主表的关联列类型一致或兼容，名称无要求 
3. 主表的关联列必须是 KEY (主键或唯一键)
4. 插入数据时，先插入主表，在插入从表 删除时先删除从表再删除主表 CREATE TABLE 表名 ( 字段名 字段类型 列级约束，
字段名 字段类型，
表级约束 ) 
一、创建表时添加约束 1.添加列级约束 CREATE DATABASE students;
```
USE students CREATE TABLE stuinfo (
  id INT PRIMARY KEY,
  stuname VARCHAR ( 20 ) NOT NULL,
  gender CHAR ( 1 ) CHECK ( gender = '男' OR gender = '女' ),
  seat INT UNIQUE,
  age INT DEFAULT 18,
  majorId INT REFERENCES major ( id ) --   外键没报错，但是不支持
);
```
```
CREATE TABLE major ( id INT PRIMARY KEY, majorName VARCHAR ( 20 ) );
```
查看 stuInfo中所有的索引 
```
SHOW INDEX
```
```
FROM
  stuInfo 主键外键自动生成索引 2.添加表级约束 语法：在各个字段的最下面 [ CONSTRAINT 约束名 ] 约束类型 (字段名) DROP TABLE
IF
  EXISTS stuinfo CREATE TABLE stuinfo (
​    id INT,
​    stuname VARCHAR ( 20 ),
​    gender CHAR ( 1 ),
​    seat INT,
​    age INT,
​    majorId INT,
​    CONSTRAINT pk PRIMARY KEY ( id, stuname ),#组合主键
​    CONSTRAINT uq UNIQUE ( seat ),
​    CONSTRAINT ck CHECK ( gender = '男' OR gender = '女' ),
​    CONSTRAINT fk_stuinfo_major FOREIGN KEY ( majorId ) REFERENCES major ( id ) 
  ) 通用写法： CREATE TABLE
IF
  NOT EXISTS stuinfo (
​    id INT PRIMARY KEY,
​    stuname VARCHAR ( 20 ) NOT NULL,
​    sex CHAR ( 1 ),
​    age INT DEFAULT 18,
​    seat INT UNIQUE,
​    majorid INT,
​    CONSTRAINT fk_stuinfo_major FOREIGN KEY ( majorid ) REFERENCES major ( id ) 
  ) 
```
二、 修改表时添加约束 
语法：
列级约束
```
alter table 表名 MODIFY COLUMN 字段名 字段类型 新约束;
```
表级约束
```
alter table 表名 add [CONSTRAINT 约束名] 约束类型(字段名) [外键的引用]
```
1. 添加非空约束 ALTER TABLE stuinfo MODIFY CREATE TABLE
```
IF
  NOT EXISTS stuinfo ( id INT, 
​    stuname VARCHAR ( 20 ), 
​    sex CHAR ( 1 ), 
​    age INT, 
​    seat INT, 
​    majorid INT, 
  ) 
```
1. 添加非空约束
```
ALTER TABLE stuinfo MODIFY COLUMN stuname VARCHAR(20) not NULL
```
2. 添加默认约束  
```
alter table stuinfo modify column age int default 18;
```
3. 添加主键
(1)列级约束
```
alter table stuinfo modify COLUMN id INT PRIMARY KEY
```
(2)表级约束
```
alter table stuinfo add PRIMARY key(id)
```
4. 添加唯一键
(1)列级约束
```
alter table stuinfo modify COLUMN seat int UNIQUE;
```
(2)表级约束
```
alter table stuinfo add unique(seat)
```
5. 添加外键
```
ALTER TABLE stuinfo add FOREIGN key(majorid) REFERENCES major(id);
ALTER TABLE stuinfo add CONSTRAINT fk_stu_major FOREIGN key(majorid) REFERENCES major(id);
```
三、修改表时删除约束
1. 删除非空约束
```
alter table stuinfo modify COLUMN stuname VARCHAR(20) null;
```
2. 删除默认约束
```
alter table stuinfo modify COLUMN age INT;
```
3. 删除主键
```
alter table stuinfo MODIFY column id INT;
alter table stuinfo drop PRIMARY KEY
```
4. 删除唯一
```
alter table stuinfo drop index
```
5. 删除外键约束
```
alter table stuinfo drop FOREIGN key fk_stu_major
```
```
show index FROM stuinfo
```
# 标识列
- 又称为自增长列
- 含义：可以不用手动插入值，系统提供默认的序列值
特点：
1. 标识列必须合主键搭配吗？不一定，但要求是一个key 比如unique
2. 至多有一个自增长
3. 标识列类型只能是数值型，一般是int
4. 可以通过set auto_increment_increment=3
​          set auto_increment_offset=3
​          设置步长和起始值

一、 创建表时设置标识列
```
drop table id EXISTS tab_id
create table tab_id(
  id INT PRIMARY KEY auto_increment,
  Name VARCHAR(20)
)
```
```
insert into tab_id(id,Name) values(null,'join') 
insert into tab_id(Name) values('lucy') 
```
查看增长量和起始
MySQL可以设置起始,可以设置步长
```
show VARIABLES LIKE '%auto_increment%'
```
```
set auto_increment_increment=3
set auto_increment_offset=3
```
二、 修改表时设置标识列
```
alter table tab_id MODIFY COLUMN id int PRIMARY KEY auto_increment
```
三、 修改表时删除标识列
```
alter table tab_id MODIFY COLUMN id int;
```
# 事物机制
TCL --transaction control language
事物控制语言
- 事物：一个或一组sql语句组成一个执行单元，这个单元要全部执行，要么全部不执行
- 每条sql语句是互相依赖的，如果某一条出现错误，整个单元将会回滚

案例：转账
张三丰 1000
郭襄  1000
```
update 表 set 张三丰余额=500 WHERE name='张三丰';
```
意外
```
update 表 set 郭襄余额=1500 WHERE name='郭襄';
```
```
show ENGINEs
```
不是所有的存储引擎都支持事务 myisam，memory不支持

事物的ACID属性

- A(atomicity)原子性，  原子性指事物是一个不可分割的工作单位，要么同时成功，要么同时失败

- C(consistency)一致性，事物必须使数据库从一个一致性状态切换到另一个一致性的状态

- I(isolation)隔离性，  一个事物的直行不能被其他事物干扰，并发执行的各个事物之间不能相互干扰

- D(durability)持久性，  一个事物一旦被提交，就是永久性的改变，接下来的操作和数据库故障不应对其有任何影响

事物的创建
- 隐式事物：事物没有明显的开启和结束的标记
比如 insert update DELETE
```
show VARIABLES like 'autocommit'
```
自动提交的功能是开启的

- 显式事物：事物具有明显的开启和约束标记
前提：必须先设置自动提交功能禁用
开启事物
```
set autocommit = 0
```
步骤1 开启事务
写了第一句就默认开启事务了
```
set autocommit = 0;
start TRANSACTION; 可选的
```
步骤2 编写事物的sql语句(SELECT insert update DELETE) create之类的没有事物之说
```
语句1
语句2
。。。
```

步骤三 结束事物
```
commit;提交事务
rollback；回滚事务
```
```
create DATABASE test;
DROP TABLE IF EXISTS account ;
CREATE TABLE account (
  id INT PRIMARY KEY auto_increment,
  username VARCHAR(20) ,
  balance DOUBLE
);
```
```
INSERT INTO account(username,balance)
VALUES('张无忌' , 1000), ('赵敏' , 1000);
```
```
SELECT * FROM account;
```
- 开启事务
```
set autocommit = 0;

start TRANSACTION;
```
- 编写一组语句
```
update account set balance = 1000 WHERE username = '张无忌';
update account set balance = 1000 WHERE username = '赵敏';
```
- 结束事物
```
ROLLBACK
```
- 回滚他的钱不会有变化，在结束事物之前回滚会回到开始之前
```
- commit
```
