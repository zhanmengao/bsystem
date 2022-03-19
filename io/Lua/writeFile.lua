require "Tools" --引utils包

local file,err=io.open("luaFileStreamTest.txt","wb")   --wb:打开模式为二进制，覆盖，写
if(not err) then
file:write("Lua fileStream Test覆盖写\n")
file:close()
end

local file,err=io.open("luaFileStreamTest.txt","ab")   --wb:打开模式为二进制，追加，写
if(not err) then
file:write("Lua fileStream Test2追加写\n")
file:close()
end