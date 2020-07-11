常见约束
含义：一种限制，用来限制表中的数据，为了保证表中数据的准确和可靠性
分类：六大约束 NOT NULL 非空约束，用来保证该字段的值不为空
如姓名，学号等 DEFAULT 默认约束，用来保证该字段的值有默认值
比如性别 PRIMARY KEY 主键，保证该字段的值具有唯一性，并且非空
比如学号，员工编号 UNIQUE 唯一约束，保证该字段的值具有唯一性，但是可以为空
比如座位号 CHECK mysql不支持，不报错但是没效果 比如年龄性别 FOREIGN KEY 外键约束，用于限制两个表的关系，用于保证该字段的值必须来自主表的关联列的值
在从表添加外键约束，引用主表某字段的值
比如学生表的专业编号，员工表的部门编号，员工表的工种编号

添加约束时机：创建表时
修改表时
总之在数据添加之前

约束的添加分类：列级约束
六大约束语法上都支持，但外键约束没效果
表级约束
出了非空和默认都可以写

主键和唯一的对比：

主键	保证唯一性	不允许为空	允许组合（不推荐）
唯一	保证唯一性	允许为空	  允许组合（不推荐）

外键： 1.在从表设置外键关系 2.从表的外键列的类型和主表的关联列类型一致或兼容，名称无要求 3.主表的关联列必须是 KEY (主键或唯一键) 4.插入数据时，先插入主表，在插入从表 删除时先删除从表再删除主表 CREATE TABLE 表名 ( 字段名 字段类型 列级约束，
	字段名 字段类型，
表级约束 ) 一 创建表时添加约束 1.添加列级约束 CREATE DATABASE students;
USE students CREATE TABLE stuinfo (
	id INT PRIMARY KEY,
	stuname VARCHAR ( 20 ) NOT NULL,
	gender CHAR ( 1 ) CHECK ( gender = '男' OR gender = '女' ),
	seat INT UNIQUE,
	age INT DEFAULT 18,
	majorId INT REFERENCES major ( id ) -- 		外键没报错，但是不支持
	
);
CREATE TABLE major ( id INT PRIMARY KEY, majorName VARCHAR ( 20 ) );
查看 stuInfo中所有的索引 SHOW INDEX 
FROM
	stuInfo 主键外键自动生成索引 2.添加表级约束 语法：在各个字段的最下面 [ CONSTRAINT 约束名 ] 约束类型 (字段名) DROP TABLE
IF
	EXISTS stuinfo CREATE TABLE stuinfo (
		id INT,
		stuname VARCHAR ( 20 ),
		gender CHAR ( 1 ),
		seat INT,
		age INT,
		majorId INT,
		CONSTRAINT pk PRIMARY KEY ( id, stuname ),#组合主键
		CONSTRAINT uq UNIQUE ( seat ),
		CONSTRAINT ck CHECK ( gender = '男' OR gender = '女' ),
		CONSTRAINT fk_stuinfo_major FOREIGN KEY ( majorId ) REFERENCES major ( id ) 
	) 通用写法： CREATE TABLE
IF
	NOT EXISTS stuinfo (
		id INT PRIMARY KEY,
		stuname VARCHAR ( 20 ) NOT NULL,
		sex CHAR ( 1 ),
		age INT DEFAULT 18,
		seat INT UNIQUE,
		majorid INT,
		CONSTRAINT fk_stuinfo_major FOREIGN KEY ( majorid ) REFERENCES major ( id ) 
	) 
	
	
	
二 修改表时添加约束 
/*
语法：
列级约束
alter table 表名 MODIFY COLUMN 字段名 字段类型 新约束;
表级约束
alter table 表名 add [CONSTRAINT 约束名] 约束类型(字段名) [外键的引用]


*/
1.添加非空约束 ALTER TABLE stuinfo MODIFY CREATE TABLE
IF
	NOT EXISTS stuinfo ( id INT, 
		stuname VARCHAR ( 20 ), 
		sex CHAR ( 1 ), 
		age INT, 
		seat INT, 
		majorid INT, 
	) 
1.添加非空约束
ALTER TABLE stuinfo MODIFY COLUMN stuname VARCHAR(20) not NULL
2.添加默认约束	
alter table stuinfo modify column age int default 18;
3.添加主键
(1)列级约束
alter table stuinfo modify COLUMN id INT PRIMARY KEY
(2)表级约束
alter table stuinfo add PRIMARY key(id)

4.添加唯一键
(1)列级约束
alter table stuinfo modify COLUMN seat int UNIQUE;
(2)表级约束
alter table stuinfo add unique(seat)

5.添加外键
ALTER TABLE stuinfo add FOREIGN key(majorid) REFERENCES major(id);
ALTER TABLE stuinfo add CONSTRAINT fk_stu_major FOREIGN key(majorid) REFERENCES major(id);


三 修改表时删除约束
1.删除非空约束
alter table stuinfo modify COLUMN stuname VARCHAR(20) null;

2.删除默认约束
alter table stuinfo modify COLUMN age INT;

3.删除主键
alter table stuinfo MODIFY column id INT;
alter table stuinfo drop PRIMARY KEY

4.删除唯一
alter table stuinfo drop index

5.删除外键约束
alter table stuinfo drop FOREIGN key fk_stu_major
show index from stuinfo



