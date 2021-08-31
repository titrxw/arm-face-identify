### opencv 3.4.14 构建
下载地址：https://github.com/opencv/opencv/archive/3.4.14.zip

1： 解压 ./opencv-source
2: cd ./opencv-source
3: mkdir build 
4: cd build
5：cmake ..
6: make
7: make install

安装完成后默认没有face模块
扩展模块构建：
下载地址 https://github.com/opencv/opencv_contrib/archive/refs/tags/3.4.14.tar.gz

1：解压 tar -xzf .. ./opencv_contrib
2: cd ./opencv-source/build
3: cmake -DOPENCV_EXTRA_MODULES_PATH=<opencv_contrib_dir>/modules/face ..
4: make
5: make install
