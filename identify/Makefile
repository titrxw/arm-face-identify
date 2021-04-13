TARGET = ./main

SRCS := $(wildcard ./src/*.cpp ./*.cpp)

OBJS := $(patsubst %cpp,%o,$(SRCS))

CFLG = -g -Wall -I/usr/local/Cellar/opencv@3/3.4.12_3/include -Iinc -I./ -std=c++11

LDFG = -Wl -I /usr/local/Cellar/opencv@3/3.4.12_3/include/opencv -I /usr/local/Cellar/opencv@3/3.4.12_3/include -L /usr/local/Cellar/opencv@3/3.4.12_3/lib -l opencv_stitching -l opencv_superres -l opencv_videostab -l opencv_aruco -l opencv_bgsegm -l opencv_bioinspired -l opencv_ccalib -l opencv_dnn_objdetect -l opencv_dpm -l opencv_highgui -l opencv_videoio -l opencv_face -l opencv_freetype -l opencv_fuzzy -l opencv_hfs -l opencv_img_hash -l opencv_line_descriptor -l opencv_optflow -l opencv_reg -l opencv_rgbd -l opencv_saliency -l opencv_sfm -l opencv_stereo -l opencv_structured_light -l opencv_phase_unwrapping -l opencv_surface_matching -l opencv_tracking -l opencv_datasets -l opencv_dnn -l opencv_plot -l opencv_xfeatures2d -l opencv_shape -l opencv_video -l opencv_ml -l opencv_ximgproc -l opencv_xobjdetect -l opencv_objdetect -l opencv_calib3d -l opencv_imgcodecs -l opencv_features2d -l opencv_flann -l opencv_xphoto -l opencv_photo -l opencv_imgproc -l opencv_core

CXX = g++

$(TARGET) : $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFG)

%.o:%.cpp
	$(CXX) $(CFLG) -c $< -o $@ 

.PHONY : clean
clean:
	-rm ./*.o