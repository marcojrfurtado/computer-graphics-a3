FLAGS=-Wall -std=gnu++0x 
LOCAL_LIBRARY=joint.cpp camera.cpp motion.cpp model.cpp sparsematrixoperations.o

all: personviewer
personviewer: sparsematrixoperations
	g++ $(FLAGS) -o personviewer personviewer.cpp $(LOCAL_LIBRARY)  -lglut -lGLEW -lGL -lGLU -lX11 -lm -g
sparsematrixoperations:
	g++ $(FLAGS) -c sparsematrixoperations.cpp
clean:
	rm motionviewer
