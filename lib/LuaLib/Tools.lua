Tools={}  --定义一个包，可以对代码进行总结归纳

function Tools.add(a,b)   --属于utils包的函数
	return a+b;
end

function Tools.PrintTable(a)
	for k,v in pairs(a) do
		print("key=",k,"value=",v)
	end
end
