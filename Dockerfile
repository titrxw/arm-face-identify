FROM ubuntu:16.04

ADD ./ /home/arm-face-identify

RUN cp /home/arm-face-identify/vendor/libstdc++.so.6.0.25 /usr/lib/x86_64-linux-gnu/ && \
    cd /usr/lib/x86_64-linux-gnu/ && \
    rm libstdc++.so.6 && \
    ln -s libstdc++.so.6.0.25 libstdc++.so.6

RUN apt-get -y update
RUN apt-get install -y --fix-missing \
    build-essential \
    cmake \
    git \
    curl \
    wget \
    libgtk2.0-dev \
    libjpeg-dev \
    pkg-config \
    zip \
    gcc \
    g++ \
    && apt-get clean && rm -rf /tmp/* /var/tmp/*

RUN mkdir -p ~/opencv && \
    cd ~/opencv && \
    wget https://github.com/Itseez/opencv/archive/3.4.14.zip && \
    unzip 3.4.14.zip && \
    rm 3.4.14.zip && \
    cd opencv-3.4.14 && \
    mkdir build

RUN cd ~/opencv && \
    wget https://github.com/opencv/opencv_contrib/archive/refs/tags/3.4.14.zip && \
    unzip 3.4.14.zip && \
    rm 3.4.14.zip && \
    cd ~/opencv/opencv-3.4.14/build && \
    cmake -DOPENCV_EXTRA_MODULES_PATH=~/opencv/opencv_contrib-3.4.14/modules/face .. &&\
    make -j4 && \
    make install && \ 
    ldconfig


# RUN cd /home/arm-face-identify &&\
# mkdir build && \ 
# cd build && \
# cmake .. &&\
# make -j4 && \
# make install && \ 
# ldconfig

CMD ["bash", "-c", "tail -f /dev/null"]

