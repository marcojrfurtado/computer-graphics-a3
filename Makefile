FLAGS=-Wall
LOCAL_LIBRARY=joint.cpp camera.cpp motion.cpp model.cpp
all:
	g++ $(FLAGS) -o personviewer personviewer.cpp $(LOCAL_LIBRARY)  -lglut -lGLEW -lGL -lGLU -lX11 -lm -g
clean:
	rm motionviewer
