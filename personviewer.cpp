#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif



#include "joint.hpp"
#include "motion.hpp"
#include "camera.hpp"
#include "model.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <glm/glm.hpp>


#define EXIT_ERROR_STATUS -1


// OpenGL Window attributes
#define WIN_SIZE 1000
#define WIN_POS 100

// Default orientation
#define X_LEFT -1.0
#define X_RIGHT 1.0
#define Y_LEFT -1.0
#define Y_RIGHT 1.0
#define NEAR 1.0
#define FAR 150.0
#define PROJECTIVE_OFFSET 50.0

using namespace std;

// Global vectors of vertex and faces
std::vector< glm::vec3 > v;
std::vector< glm::vec3 > vn;
std::vector< std::vector< std::pair<int,int> > > f;

// Point to the frame currently being rendered
static std::vector< Motion::frame_data >::const_iterator current_frame_it ;



// Value for FPS

#define FPS_DEFAULT 120
// The frame rate defined in the BVH file is ignored
// This frame rate is virtual, it defines how we interpolate the frames
int fps = FPS_DEFAULT;

// DEBUGGING TOOL
// press 'f' to show the fps on the screen
bool show_fps = false;

// Used to start/stop the animation
bool modePlay = false;
bool pause_animation = false;

// Object representing the loaded model
Model mdl;

// Root element in the hierarchy
Joint root(true);
Motion mt;

// Object representing the camera
Camera cam;

glm::vec3 pos(0,0,0);

void restore() {

	// Restart object to its original position


	current_frame_it = mt.get_frame_set().begin();

}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {

	FILE *fp;
	switch(key) {
		case 'w':
			fp = fopen("meshout.bvh","w");
			mdl.exportScheme(fp);
			fclose(fp);
			fp = fopen("motionout.bvh","w");
			fprintf(fp,"HIERARCHY\n");
			root.print(fp);
			fprintf(fp,"MOTION\n");
			mt.print(fp);
			fclose(fp);
			break;
		case 'f':
			show_fps=true;
			break;
		case 'F':
			show_fps=false;
			break;
		case 'q':
			exit(0);
			break;
		case 'T':
	       		cam.rotate(-10.0,0.0,0.0);
       			break;
		case 't':
			cam.rotate(10.0,0.0,0.0);
			break;
		case 'A':
			cam.rotate(0.0,-10.0,0.0);
			break;
		case 'a':
			cam.rotate(0.0,10.0,0.0);
			break;
		case 'C':
			cam.rotate(0.0,0.0,-10.0);
			break;
		case 'c':
			cam.rotate(0.0,0.0,10.0);
			break;
	 	// CAMERA TRANSLATIONS
		case 'I':
	       		cam.translate(0.0,0.0,0.1);      
			break;
	    	case 'i':
		       	cam.translate(0.0,0.0,-0.1);
			break; 
		// ANIMATION CONTROL
		case 'p':
			modePlay=true;
			pause_animation=false;
			break;
		case 'P':
			pause_animation=true;
			break;
		case 's':
			modePlay=false;
			restore();
			fps=FPS_DEFAULT;
			break;
		case '+':
			fps+=10;
			break;
		case '-':
			fps-=10;
			break;
		default:
			break;
	}
   // This avoids changing the animation speed if afer keystrokes
	if( !modePlay )
		glutPostRedisplay();
}


// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{ 
   if(key == GLUT_KEY_UP) cam.translate(0.0,0.1,0.0);
   else if(key == GLUT_KEY_DOWN) cam.translate(0.0,-0.1,0.0);
   else if(key == GLUT_KEY_LEFT) cam.translate(-0.1,0.0,0.0);
   else if(key == GLUT_KEY_RIGHT) cam.translate(0.1,0.0,0.0);
   
   // This avoids changing the animation speed if afer keystrokes
   if ( !modePlay )
	   glutPostRedisplay();
}


// Process BVH file. Calls the Joint and Motion object to parse most of the content
int process_bvh_file(const char *filename) {

	FILE *bvh_fp;
	char next_string[100];
	
	if ( ! (bvh_fp = fopen(filename,"r") ) ) {
		fprintf(stderr,"Problem opening file %s.\n",filename);
		return EXIT_ERROR_STATUS;
	}


	// Test HIERARCHY keyword
	fscanf(bvh_fp," %s",next_string);


	// Check if we are reading hierarchy description or motion description

	if ( !strcmp(next_string,"HIERARCHY") ) {
		fscanf(bvh_fp," %s",next_string);
		if ( strcmp(next_string,"ROOT" ) ) {
			fprintf(stderr,"Hierarchy descriptions must start from ROOT element.\n");
			return EXIT_ERROR_STATUS;
		}
		root.process(bvh_fp);
	} else {
		fprintf(stderr,"Wrong hierarchy header.\n");
		return EXIT_ERROR_STATUS;
	} 
	
	// Test HIERARCHY keyword
	fscanf(bvh_fp," %s",next_string);

	
	
	if ( !strcmp(next_string,"MOTION"  ) ) {
		mt.set_frame_data_size(root.count_hierarchy_channels());
		mt.process(bvh_fp);

		// Animation is set to start at the first frame
		current_frame_it = mt.get_frame_set().begin();
	} else {
		fprintf(stderr,"Wrong motion header.\n");
		return EXIT_ERROR_STATUS;
	} 

	fclose(bvh_fp);

	return 0;

}

// Reads OBJ file and populates the vertex and faces vectors
bool process_obj_file ( const char *path , std::vector< glm::vec3 > &vertices, std::vector< glm::vec3 > &vertexNormals ,std::vector< std::vector< std::pair<int,int> > > &faces ) {

	FILE *fp = fopen(path,"r");

	if ( fp == NULL )
		return false;


	while ( true )  {

		// Checks whether it's a face or a vertex
		char header[100];
		int res = fscanf(fp," %s",header );

		if ( res == EOF  ) break;


		if (  strcmp( header, "v" ) == 0 ) {

			glm::vec3 vertex;
			fscanf(fp, " %f %f %f", &vertex.x, &vertex.y, &vertex.z );
			vertices.push_back(vertex);
		}
		else if (  strcmp( header, "vn" ) == 0 ) {

			glm::vec3 vertexNormal;
			fscanf(fp, " %f %f %f", &vertexNormal.x, &vertexNormal.y, &vertexNormal.z );
			vertexNormals.push_back(vertexNormal);
		} else if ( strcmp( header, "f" ) == 0 ) {

			char line[500];
			fgets(line,500,fp);

			// Faces can have only 3 vertices
			int faceVertexIndices[3];
			int faceVertexNormalIndices[3];
			int count =  sscanf(line," %d//%d %d//%d %d//%d",&faceVertexIndices[0],&faceVertexNormalIndices[0],&faceVertexIndices[1],&faceVertexNormalIndices[1],&faceVertexIndices[2],&faceVertexNormalIndices[2]) ;


			std::vector< std::pair<int,int> > newFace;
			for( int i = 0; i < count; i++ ) {
				newFace.push_back(std::pair<int,int>(--faceVertexIndices[i],--faceVertexNormalIndices[i]));
			}
			faces.push_back(newFace);


		} else {
			char discardBuffer[1000];
			fgets(discardBuffer,1000,fp);
		}

	}

	fclose(fp);

	return true;
}

// Position camera
void setCamera() {

	glLoadIdentity();
	// set the camera
	gluLookAt(cam.eye.x,cam.eye.y,cam.eye.z,cam.center.x,cam.center.y,cam.center.z,cam.up.x,cam.up.y,cam.up.z);
	
	
	// Rotates on the camera
	glTranslatef(cam.eye.x,cam.eye.y,cam.eye.z);
	glRotatef(cam.angle.z, 0.0, 0.0, 1.0);
	glRotatef(cam.angle.y, 0.0, 1.0, 0.0);
	glRotatef(cam.angle.x, 1.0, 0.0, 0.0);
	glTranslatef(-cam.eye.x,-cam.eye.y,-cam.eye.z);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	gluPerspective(90,1 , 1.0, 100);
	glFrustum(X_LEFT,X_RIGHT,Y_LEFT,Y_RIGHT,NEAR,FAR);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

}


// Small timer function, which tells glut to redisplay the object
void Timer( int id) {

	glutPostRedisplay();
}




// Render next frame
void processNextFrame() {


	static float lambda = 0;




	// If the FPS is the default ( lambda == 0 ), we simply show the frames. Otherwise we interpolate between two frames
	if  (  ( fps > 0 ) && ( (current_frame_it+1) != mt.get_frame_set().end() ) )
		root.render_transformation(*current_frame_it, *(current_frame_it+1),lambda, !modePlay );
	else if ( ( fps < 0 ) && ( (current_frame_it+1) != mt.get_frame_set().begin() ) ) {
		root.render_transformation(*current_frame_it,*(current_frame_it-1),lambda , !modePlay );
	} else
		root.render_transformation(*current_frame_it,  !modePlay);

	// Go to the next frame, if we are playing
	if  ( modePlay && !pause_animation ) {

		lambda+=(float) abs(fps)/FPS_DEFAULT;

		while ( lambda >= 1.0    ) {
			lambda-=1.0;
			if ( ( fps > 0 ) && ( current_frame_it != mt.get_frame_set().end() ) ) 
				current_frame_it++;
			else if ( current_frame_it != mt.get_frame_set().begin()   )
				current_frame_it--;

		}

	}

	// If we reached the end of the animation, we should go back ( unless we are reversing  it)
	if ( current_frame_it == mt.get_frame_set().end() )
		restore();
	else if ( ( fps < 0  )  && ( current_frame_it  == mt.get_frame_set().begin()   ) ) {
		current_frame_it = mt.get_frame_set().end();
		current_frame_it--;
	}




}


// Used for debugging display the FPS on the screen. Use 'F' and 'f' to switch it off and on, respectively.
void draw_fps_message() {

	glPushMatrix();
	glRasterPos2f(0.0,0.0);

	char message[15];
	sprintf(message,"fps: %d",fps);

	int i = 0;
	while ( *(message +i ) ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *(message+(i++)));
//		glutStrokeCharacter(GLUT_STROKE_ROMAN,*(message+(i++)));
	}
	glPopMatrix();
}


// Drawing (display) routine.
void drawScene(void) {


	// Clear Screen to background color.
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	setCamera();


	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	glColor3f(0.0,0.0,0.0);

	glTranslatef(pos.x,pos.y,pos.z);
	
	processNextFrame();

	// Only for debugging purposes
	if ( show_fps )
		draw_fps_message();	


	glutSwapBuffers();

	// Computes the frame time, based on the fps
	int frametime = 1000/FPS_DEFAULT;
	glutTimerFunc( frametime , Timer, 0);
}


// INTIAL procedures
int setup(const char *obj_filename, const char *bvh_filename) {
	
	// Set background (or clearing) color.
	glClearColor(1.0, 1.0, 1.0, 0.0);	

	glEnableClientState(GL_VERTEX_ARRAY);
	
	if ( !process_obj_file(obj_filename,v,vn,f) ) {
		fprintf(stderr,"Object file could not be parsed\n");
		return EXIT_ERROR_STATUS;
	}
	
	if ( process_bvh_file(bvh_filename)== EXIT_ERROR_STATUS  )
		return EXIT_ERROR_STATUS;	
	
	// Sets model to the initial position and scales it.
	mdl.reset();
	mdl.setup(v,vn,f);

	// Initial camera positioning ( Based on motion data)
	cam.center = mt.get_mean();
	cam.translate((mt.get_max().x+mt.get_min().x)/2,(mt.get_max().y+mt.get_min().y)/2,mt.get_max().z+PROJECTIVE_OFFSET,true);



	return 0;
	
}



int main(int argc, char *argv[]) {



	if ( argc < 3 ) {
		fprintf(stderr,"Wrong number of arguments.\nCorrect usage: ./personviewer <OBJ filename> <BVH filename>\n");
		return EXIT_ERROR_STATUS;
	}



	//INIT GLUT PROCEDURES
	glutInit( &argc, argv );
	
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE  );

	glutInitWindowSize( WIN_SIZE, WIN_SIZE );
	
	glutInitWindowPosition(WIN_POS, WIN_POS );


	glutCreateWindow("Motion Viewer");
	
	if ( setup(argv[1],argv[2]) == EXIT_ERROR_STATUS )
		return EXIT_ERROR_STATUS;
	
	// Register the callback function for non-ASCII key entry.
	glutSpecialFunc(specialKeyInput);

	glutKeyboardFunc(keyInput);
	glutDisplayFunc(drawScene);

	glutMainLoop();

	return 0;
}
