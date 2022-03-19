require "Tools" --引utils包

local file,err=io.open("luaFileStreamTest.txt","rb")   --wb:打开模式为二进制，覆盖，写
if(not err) then
local fileSize=file:seek("end") --把文件指针移到文件的末尾，并且返回此时文件指针的位置
print(fileSize)

file:seek("set",0) --将文件指针移到0号位置（头部)
local buffer=file:read("*a")
print(buffer)
file:close()
end

