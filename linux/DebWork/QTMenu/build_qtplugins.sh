QT_DIR="/home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/plugins/platforms"

cp -r ${QT_DIR}/  Package/opt/QMenu/
mLibDir=Package/opt/QMenu/

cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libicudata.so.56 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libicui18n.so.56 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libicuuc.so.56 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Core.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Gui.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Widgets.so.5 $mLibDir

cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5XcbQpa.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5DBus.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5EglFSDeviceIntegration.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Network.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5WaylandClient.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Quick.so.5 $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5WebSockets.so.5  $mLibDir
cp /home/zhanmengao/Qt5.12.9/5.12.9/gcc_64/lib/libQt5Qml.so.5 $mLibDir