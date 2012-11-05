default: facedetect

facedetect2: facedetect2.cpp
	g++ -o facedetect2 facedetect2.cpp `pkg-config opencv --cflags --libs`


facedetect: facedetect.cpp
	c++ -o facedetect facedetect.cpp cmd_arg_reader.cpp `pkg-config opencv --cflags --libs`
	
