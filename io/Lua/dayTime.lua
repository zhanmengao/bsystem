require "Tools" --引utils包

print(os.time());  --从某一时刻至今的秒数

print(os.time({year=2018,month=5,day=23}));  --到该时刻为止的秒数

print(os.time({year=2018,month=5,day=23,hour=11})); 

print(os.clock()) --返回当程序开始到函数调用消耗的时间

--time返回的字符我们看不懂，需要用date类进行格式化
---------------------------------------date Begin------------------------------------------
local t=os.date("*t",os.time()) --拿到当前时间的pair
Tools.PrintTable(t)

t=os.date("%a",os.time()) --拿到当前星期的简写
print("\n",t)

t=os.date("%A",os.time()) --拿到当前星期的全程
print("\n",t)

t=os.date("%Y-%m-%d",os.time()) --标记的组合
print("\n",t)