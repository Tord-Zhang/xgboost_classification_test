
APP:=Test

CPP=g++

#SRCS:= $(wildcard *.cpp *.c)
SRCS:= \
	Test.cpp 
	
OBJS:= $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS)))

CPPFLAGS:= \
	-I/usr/local/include/opencv4 \
	-I/root/xgboost/include \
        -I/root/xgboost/rabit/include

LDFLAGS:= \
        -L/root/xgboost/lib -lxgboost \
	-L/root/xgboost/rabit/lib -lrabit \
	-L/root/xgboost/dmlc-core -ldmlc \
        


#LDFLAGS += `pkg-config --libs opencv`	
LDFLAGS += -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs -lopencv_core -lopencv_imgproc

CXX_RULE := -std=c++11 -Wall

all: $(APP)

%.o: %.c
	$(CPP) ${CXX_RULE} $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CPP) ${CXX_RULE} $(CPPFLAGS) -c $< -o $@
	
$(APP): $(OBJS)
	$(CPP) -o $@ $(OBJS) $(CPPFLAGS) $(LDFLAGS) 
	
clean:
#	echo $(SRCS) 
#	echo $(OBJS)
	rm -rf $(OBJS) $(APP)
	
