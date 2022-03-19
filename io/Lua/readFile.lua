require "Tools" --引utils包

local file,err=io.open("luaFileStreamTest.txt","rb")   --wb:打开模式为二进制，覆盖，写
if(not err) then
local buffer=file:read("*a") -- a=all把文件一次性读出来
print(buffer)
file:close()
end

local file,err=io.open("luaFileStreamTest.txt","rb")   --wb:打开模式为二进制，覆盖，写
if(not err) then
local buffer=file:read("*l") --l=line
print(buffer)
local buffer=file:read("*l") --l=line
print(buffer)
file:close()
end

