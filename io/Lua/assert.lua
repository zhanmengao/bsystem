local function main01()
function dofile(filename)
local f=assert(loadfile(filename));
return f()
end;
end;

local function main02()
f=assert(loadstring("i=i+1"));
i=0;
f();
print(i);
f();
print(i);
end;

local function main03()
print"enter your expression:"
local l=io.read();
local func=assert(loadstring("return "..(l+999)));
print(func());
end;

local function main04()
function foo(x) print(x) end;
f=loadstring("foo.lua");
print(foo);
if(f~=nil) then f() end;
print(foo("ok"));
end

main04();




