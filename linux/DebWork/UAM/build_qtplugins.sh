QT_DIR=${HOME}/Qt5.12.9//5.12.9/gcc_64

cp -r ${QT_DIR}/plugins/platforms/  Package/opt/vesystem/uam/
mLibDir=Package/opt/vesystem/uam/

cp ${QT_DIR}/lib/libicudata.so.56 $mLibDir
cp ${QT_DIR}/lib/libicui18n.so.56 $mLibDir
cp ${QT_DIR}/lib/libicuuc.so.56 $mLibDir
cp ${QT_DIR}/lib/libQt5Core.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5Gui.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5Widgets.so.5 $mLibDir

cp ${QT_DIR}/lib/libQt5XcbQpa.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5DBus.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5EglFSDeviceIntegration.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5Network.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5WaylandClient.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5Quick.so.5 $mLibDir
cp ${QT_DIR}/lib/libQt5WebSockets.so.5  $mLibDir
cp ${QT_DIR}/lib/libQt5Qml.so.5 $mLibDir