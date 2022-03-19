
local function main()
	myFile=io.open("test_data.lua","w");
	if(myFile~=nil) then
		myFile:write("--Test lua file");
		myFile:write(string.char(10));
		myFile:write(string.char(10));
		myFile:write(string.format("%s%s","--File created on","--aaa"));
		myFile:write(string.char(10));
		myFile:write(string.char(10));
		myFile:write("print(\"hello world!\")");
		io.close(myFile);
	end
end


main()
