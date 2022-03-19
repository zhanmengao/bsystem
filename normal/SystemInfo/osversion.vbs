strComputer="."
Set objWMIService = GetObject("winmgmts:" & "{impersonationLevel = impersonate}!\\" & strComputer & "\root\cimv2")
Set colOperatingSystems = objWMIService.ExecQuery("SELECT * FROM Win32_OperatingSystem")
For Each objOperatingSystem in colOperatingSystems
	Wscript.Echo "Caption:" & objOperatingSystem.Caption
	Wscript.Echo "Debug:" & objOperatingSystem.Debug
	Wscript.Echo "Version:" & objOperatingSystem.Version
Next