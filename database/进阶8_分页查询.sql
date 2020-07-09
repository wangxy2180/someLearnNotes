-- 进阶8 分页查询  重点！！！！！
/*
应用场景:要显示的数据一页显示不全，分页提交请求
语法：
	SELECT 查询列表
	from 表
	where 筛选条件
	limit offset, size;
	limit 起始索引，从0开始；条目数

limit放在最后
公式：要显示的页数为page，每页条目为size
		LIMIT (page-1)*size size





*/

-- 案例1：查询前五条员工信息
select * from employees LIMIT 0,5;
select * from employees LIMIT 5;

-- 案例2：查询第11到第25条
select * from employees LIMIT 10,15

案例3：有奖金的员工信息，并且工资较高的前10名
select * FROM employees where commission_pct is not null ORDER BY salary DESC LIMIT 10












