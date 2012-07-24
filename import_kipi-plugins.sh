#/bin/sh

kipiplugins_dir="../kipi-plugins"

mkdir -p libkipiplugins
mkdir -p libkipiplugins/dialogs
mkdir -p libkipiplugins/tools
mkdir -p libkipiplugins/widgets


# Yes, this is pure evil. Children at home don't do this.
cp -uv ${kipiplugins_dir}/common/libkipiplugins/kipiplugins_export.h libkipiplugins/

cp -uv ${kipiplugins_dir}/common/libkipiplugins/dialogs/kpimagedialog.cpp libkipiplugins/dialogs/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/dialogs/kpimagedialog.h libkipiplugins/dialogs/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpaboutdata.cpp libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpaboutdata.h libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kphostsettings.cpp libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kphostsettings.h libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpimageinfo.cpp libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpimageinfo.h libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpmetadata.cpp libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpmetadata.h libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpmetasettings.cpp libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpmetasettings.h libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kprawthumbthread.cpp libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kprawthumbthread.h libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/tools/kpversion.cpp libkipiplugins/tools/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/widgets/kpimageslist.cpp libkipiplugins/widgets/
cp -uv ${kipiplugins_dir}/common/libkipiplugins/widgets/kpimageslist.h libkipiplugins/widgets/

# Yes, this is even worse
cp -uv ${kipiplugins_dir}/build/common/libkipiplugins/kpversion.h libkipiplugins/
cp -uv ${kipiplugins_dir}/build/common/libkipiplugins/gitversion.h libkipiplugins/
