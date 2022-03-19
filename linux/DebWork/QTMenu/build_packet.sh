#!/bin/sh  
exe="Package/opt/QMenu/QMenu" #你需要发布的程序名称
des="Package/opt/QMenu/" #步骤1中的目录即本文件所在目录
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des
