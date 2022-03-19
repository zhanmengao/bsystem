#!/usr/bin/python3

a = "hello"
print(a)

#可以使用三个引号将多行字符串赋值给变量
a = """Python is a widely used general-purpose, high level programming language. 
It was initially designed by Guido van Rossum in 1991 
and developed by Python Software Foundation. 
It was mainly developed for emphasis on code readability, 
and its syntax allows programmers to express concepts in fewer lines of code."""
print(a)

#python字符串地实现是unicode字符的字节数组
print(a[0])

#返回部分切片（类似go）
b = "Hello,World!"
print(b[2:5])

#使用负索引从字符串末尾开始切片
print(b[-5:-2])

#len获取字符串长度
print(len(b))