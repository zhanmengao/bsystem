math.randomseed(os.time());--设置随机数种子
for i=0,20,1 do
local num=math.random()  --默认会随机出[0,1]区间的数
if(num>=0.9) then
print(num)
end
end 


print(math.random(100))  --[1,100]

print(math.random(5,20)) --[5,20]

--三角函数以弧度作为参数


print(math.floor(1.1)) --向下取整
print(math.ceil(1.5)) --向上取整

print(math.max(1,2)) --取大
print(math.min(1,2))  --取小