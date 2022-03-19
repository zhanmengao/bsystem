chmod -R 755 Package
#拷库
#mkdir $mLibDir
#cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Widgets.so.5 $mLibDir
#cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Core.so.5 $mLibDir
#cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Gui.so.5 $mLibDir
#cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libicui18n.so.56 $mLibDir
#cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libicuuc.so.56 $mLibDir
#cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libicudata.so.56 $mLibDir
#cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5XcbQpa.so.5 $mLibDir
#./build_packet.sh
./build_qtplugins.sh
dpkg -b Package UAM.deb
cp UAM.deb /mnt/hgfs/Space
