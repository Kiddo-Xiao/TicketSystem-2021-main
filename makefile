main : Connect.cpp \
		OrderRelated.cpp \
		TrainRelated.cpp \
		UserRelated.cpp \
		main.cpp
	g++ Connect.cpp \
		OrderRelated.cpp \
		TrainRelated.cpp \
		UserRelated.cpp \
		main.cpp \
		-o code \
		-std=c++11