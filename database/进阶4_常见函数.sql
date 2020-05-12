# 进阶4：常见函数
/*
分类：1.单行函数 如concat length ifnull等
			2.分组函数 传入一组值，返回一个值，做统计用，又称作统计函数，聚合函数，组函数

*/

-- 一、字符函数
-- 1.LENGTH(str)获取参数值的字节个数
SELECT LENGTH('join');
SELECT LENGTH('张三');
-- utf8字母占一个字节，中文占三个
SHOW VARIABLES LIKE '%CHAR%'

-- 2.CONCAT(str1,str2,...)拼接字符串
SELECT CONCAT(last_name,'_',first_name) FROM employees;

-- 3.upper,lower
SELECT UPPER('join')
SELECT LOWER('join')
-- 示例：性变大写，名变小写，然后拼接
select CONCAT(upper(last_name),'_',lower(first_name)) FROM employees;

-- 4.截取字符 substr substring
-- 注意，索引从1开始
select SUBSTR('李莫愁爱上了陆展元',7);#陆展元
select SUBSTR('李莫愁爱上了陆展元',1,3);#李莫愁

-- 案例：姓名中姓首字符大写，其他小写然后用_拼接
SELECT CONCAT(upper(SUBSTR(first_name,1,1)),SUBSTR(first_name,2),'_',LOWER(last_name)) output 
FROM employees;

-- 5.instr 子串第一次下标查找，找不到返回0
SELECT INSTR('杨不悔爱上了殷六侠','殷六侠');

-- 6.trim 去掉空格
SELECT LENGTH(TRIM('        张翠山           '));
SELECT TRIM('a' from 'aaaaaaaa张aa翠aa山aaaaaaaaaaa') output;# 只去前后

-- 7.lpad，rpad指定字符实现左填充，右填充，超过长度会截断
SELECT LPAD('殷素素',10,'*') output;
SELECT LPAD('殷素素',2,'*') output;
SELECT RPAD('殷素素',10,'*') output;
SELECT RPAD('殷素素',2,'*') output;

-- 8.replace替换
SELECT REPLACE('张无忌周芷若爱周芷若上了周芷若','周芷若','赵敏')


-- 二、数学函数
-- 1.ROUND(X)四舍五入
select round(1.65);
select round(1.65456,2);

-- 2.ceil()向上取整 返回>=该参数的最小整数
-- 	 FLOOR(X)向下取整 返回<=该参数的最小整数
-- 	 truncate()截断
SELECT CEIL(1.01)
SELECT CEIL(1.00)
SELECT FLOOR(-9.99)
SELECT TRUNCATE(1.6634,2)

-- 3.mod取余
-- mod(a,b) a-a/b*b
-- mod(-10,-3) -10-(-10)/(-3)*(-3)=-1
SELECT MOD(10,3);
SELECT 10%3


-- 三、日期函数

-- 1.now系统日期
SELECT NOW();
-- curdate()返回当前系统日期/时间，不包含另一个
SELECT CURRENT_DATE
SELECT CURDATE()
SELECT CURRENT_TIME
SELECT CURTIME()
-- 可以获取指定部分的年月日
SELECT year(NOW()) 年;
SELECT year(2020-1-1) 年;
SELECT YEAR(hiredate) from employees

SELECT MONTHNAME(now())

-- str_to_date
-- 查询入职日期为1992-4-3的员工信息
SELECT *FROM employees WHERE hiredate = '1992-4-3'
SELECT *FROM employees WHERE hiredate = STR_TO_DATE('4-3 1992','%c-%d %Y')

-- 将日期转换成字符
-- 查询有奖金的员工名和入职日期(xx月/xx日 xx年)
SELECT last_name,DATE_FORMAT(hiredate,'%m月/%d日 %y年') 入职日期 from employees WHERE commission_pct is not null


-- 四、其他函数
SELECT VERSION()
SELECT DATABASE
SELECT user


-- 五、流程控制函数
-- 1.if函数
select if(10>5,'大于','小于')
select last_name,commission_pct,if(commission_pct is null,'没奖金','有奖金') 备注 from employees 

-- 2.case结构
-- 使用一，类似switch case，处理等值判断
-- 查询员工工资，部门号=30，工资1.1倍; =40，1.2倍; =50，1.3倍
SELECT salary 原始工资, department_id,
CASE department_id
	WHEN 30 THEN
		salary*1.1
	when 40 THEN
		salary*1.2
	when 50 THEN
		salary*1.3
	ELSE
		salary
END AS 新工资
FROM employees;

-- 使用二 类似于多重if，处理区间判断
-- 查询员工工资情况，工资大于20000，显示A级别，大于15000B，大于10000C，否则D
SELECT salary,
CASE 
	WHEN salary>20000 THEN 'A'
	when salary>15000 then 'B'	
	when salary>10000 then 'C'	
	ELSE
		'D'
END 工资级别
from employees;


