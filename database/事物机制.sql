#TCL
-- transaction control language
-- 事物控制语言
事物：一个或一组sql语句组成一个执行单元，这个单元要全部执行，要么全部不执行
每条sql语句是互相依赖的，如果某一条出现错误，整个单元将会回滚

案例：转账
张三丰 1000
郭襄	 1000

update 表 set 张三丰余额=500 where name='张三丰';
意外
update 表 set 郭襄余额=1500 where name='郭襄';

show ENGINEs
不是所有的存储引擎都支持事务 myisam，memory不支持

事物的ACID属性
A(atomicity)原子性，	原子性指事物是一个不可分割的工作单位，要么同时成功，要么同时失败
C(consistency)一致性，事物必须使数据库从一个一致性状态切换到另一个一致性的状态
I(isolation)隔离性，	一个事物的直行不能被其他事物干扰，并发执行的各个事物之间不能相互干扰
D(durability)持久性，	一个事物一旦被提交，就是永久性的改变，接下来的操作和数据库故障不应对其有任何影响


事物的创建
隐式事物：事物没有明显的开启和结束的标记
比如 insert update delete

show VARIABLES like 'autocommit'
自动提交的功能是开启的

显式事物：事物具有明显的开启和约束标记
前提：必须先设置自动提交功能禁用
-- 开启事物
set autocommit = 0

步骤1 开启事务
写了第一句就默认开启事务了
set autocommit = 0;
start TRANSACTION; 可选的
步骤2 编写事物的sql语句(select insert update delete)  create之类的没有事物之说
语句1
语句2
。。。
步骤三 结束事物
commit;提交事务
rollback；回滚事务

create DATABASE test;
DROP TABLE IF EXISTS account ;
CREATE TABLE account (
	id INT PRIMARY KEY auto_increment,
	username VARCHAR(20) ,
	balance DOUBLE
);
INSERT INTO account(username,balance)
VALUES('张无忌' , 1000), ('赵敏' , 1000);

select * from account;

-- 开启事务
set autocommit = 0;
start TRANSACTION;
-- 编写一组语句
update account set balance = 1000 where username = '张无忌';
update account set balance = 1000 where username = '赵敏';

-- 结束事物
ROLLBACK
-- 回滚他的钱不会有变化，在结束事物之前回滚会回到开始之前
-- commit




