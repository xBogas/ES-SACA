#!/bin/sh

# opencv
# eigen
# boost

# postgre sql for central 

# go to home/user directory
cd /home/saca

# create download folder for saca dependencies
mkdir saca_downloads && cd saca_downloads

sudo apt-get update -y

# OpenCV

# Install opencv dependencies
sudo apt-get install -y cmake g++ wget unzip git

mkdir opencv && cd opencv
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.x.zip

unzip opencv.zip
unzip opencv_contrib.zip

mkdir -p build && cd build

cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.x/modules ../opencv-4.x
cmake --build .

sudo make install -j3


# Eigen

sudo apt install -y libeigen3-dev


# boost 

sudo apt-get install -y libboost-all-dev

# postgre 

sudo apt-get install postgresql
sudo apt-get install libpqxx-dev


# Qt
# for deployment https://github.com/probonopd/linuxdeployqt

sudo apt-get -y install build-essential openssl libssl-dev libssl1.0 libgl1-mesa-dev libqt5x11extras5

# for help https://web.stanford.edu/dept/cs_edu/resources/qt/install-linux
# go to  https://www.qt.io/download-qt-installer
# chmod +x qt*.run  
# ./qt*.run