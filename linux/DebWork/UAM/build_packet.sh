#!/bin/sh  
exe="Package/opt/vesystem/uam/UAMConfig" #你需要发布的程序名称
des="Package/opt/vesystem/uam/" #步骤1中的目录即本文件所在目录
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des

exe="Package/opt/vesystem/uam/UAMLinuxClient" #你需要发布的程序名称
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des

exe="Package/opt/vesystem/uam/CloudAppOpen" #你需要发布的程序名称
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des