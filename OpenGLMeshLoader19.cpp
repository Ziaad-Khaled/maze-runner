#define GLUT_DISABLE_ATEXIT_HACK
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <iostream>
#include <random>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <windows.h>
#include <future>

using namespace std;


int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;
bool maze0 = false;

//0:empty space 1:wall 2:coin 3:obstacle
int position=0;
bool firstFrame = true;
int maze1[20][16] =
{
	{ 1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1 },
	{ 1,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1 },
	{ 1,0,0,1,0,0,0,1,1,1,1,0,0,1,0,1 },
	{ 1,1,0,0,1,1,0,1,1,1,0,0,1,0,0,1 },
	{ 1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1 },
	{ 1,1,0,0,1,0,1,1,0,1,0,1,1,1,1,1 },
	{ 1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1 },
	{ 1,0,1,0,1,0,0,1,1,1,1,1,0,1,1,1 },
	{ 1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,1 },
	{ 1,1,0,1,1,1,1,1,1,0,1,1,1,0,1,1 },
	{ 1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1 },
	{ 1,0,1,1,0,1,0,1,1,1,0,1,0,1,1,1 },
	{ 1,0,0,1,0,0,0,1,1,1,0,0,0,0,0,1 },
	{ 1,1,0,0,1,1,0,1,1,0,0,1,0,1,0,1 },
	{ 1,1,1,0,0,0,0,1,1,0,1,1,0,1,1,1 },
	{ 1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,1,0,1,1,0,1,0,1,0,1 },
	{ 1,0,1,0,1,0,0,0,0,1,0,1,0,1,1,1 },
	{ 1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1 },
};
int maze2[30][30] =
{
	{  1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },//1
	{  1,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,1 },//2
	{  1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,0,0,1,0,1,0,1,0,0,0,0,1,1 },//3
	{  1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,0,0,1,0,1,1,0,1,1 },//4
	{  1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1 },//5
	{  1,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1 },//6
	{  1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1,0,1,1 },//7
	{  1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,0,1,1,1,1,1,1,0,1,1 },//8
	{  1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,0,1 },//9
	{  1,1,1,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1 },//10
	{  1,0,1,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,1,0,0,1 },//11
	{  1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,0,1,1 },//12
	{  1,0,1,0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,1,0,1,1 },//13
	{  1,0,0,0,0,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1 },//14
	{  1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,1 },//15
	{  1,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,1,0,1,1 },//16
	{  1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,1,1 },//17
	{  1,1,0,1,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,1,0,0,0,1,0,1 },//18
	{  1,0,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1 },//19
	{  1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1 },//20
	{  1,0,1,1,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1 },//21
	{  1,0,1,1,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,1,1,1 },//22
	{  1,0,0,0,0,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,1,1 },//23
	{  1,1,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,1,1,1 },//24
	{  1,1,0,1,1,0,1,1,1,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,1 },//25
	{  1,1,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,0,0,1,1,1,1,0,1,1 },//26
	{  1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1 },//27	
	{  1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,1,1,0,1,1 },//28
	{  1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1 },//29
	{  1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },//30
};
/*int maze2[20][20] =
{
	{  1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },
	{  1,0,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1 },
	{  1,1,0,1,0,1,0,0,0,0,1,1,1,1,1,1,1,0,1,1 },
	{  1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,1,0,0,1,1 },
	{  1,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,0,1,1,1 },
	{  1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1 },
	{  1,0,0,0,0,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1 },
	{  1,0,1,0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,1 },
	{  1,0,1,0,1,0,0,1,1,0,1,1,1,1,0,1,1,0,1,1 },
	{  1,0,1,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,1 },
	{  1,1,1,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1 },
	{  1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1 },
	{  1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1 },
	{  1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1 },
	{  1,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1 },
	{  1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,1,0,1,1 },
	{  1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1 },
	{  1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,0,0,1 },
	{  1,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,0,1,1 },
	{  1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1 },
};*/

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};
//don't play with the intial values before telling ziad!!!!!
Vector Eye(-7, 5, 20);
Vector At(-7, -12, 10);
Vector Up(0, 2, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;


// Textures
GLTexture tex_ground;
GLTexture tex_coin;
GLTexture tex_wall;
GLTexture tex_rock;


//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void drawMazes()
{
	srand((unsigned int)time(0));

	
	glPushMatrix();
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);

	
	if (firstFrame == true) {
		int flip = 0;

		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				if (maze2[i][j]==0)
				{
					flip = rand() % (6 - 1 + 1) + 1;

					if (flip == 3) //coin
					{
						maze2[i][j] = 2;
					}
					if (flip == 6)//obstacle
					{
						maze2[i][j] = 3;
					}
				}
			}
		}
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				std::cout << maze2[i][j] << ' ';
			}
			std::cout << std::endl;
		}
		firstFrame = false;
	}


	//glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);

	if (maze0)
	{
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 16; j++) {
				if (maze1[i][j] == 1) {  // Means there is a cube there
					glPushMatrix();
					glTranslatef(3 + j * 1, 0, i * 1);
					glBindTexture(GL_TEXTURE_2D, tex_wall.texture[0]);
					glutSolidCube(1);
					glPopMatrix();
				}
				else
				{
					/*flip++;

					if (flip % 10 == 0) //coin
					{
						glPushMatrix();
						glPushMatrix();
						glTranslatef(3 + j * 1, 0, i * 1);//10 + just for positioning
						glutSolidSphere(0.2, 200, 200);
				//		glBindTexture(GL_TEXTURE_2D, tex_coin.texture[0]);
						glPopMatrix();
						glPopMatrix();*/
			//		}

				}
			}
		}
	}

	else
	{
		for (int i = 29; i >= 0; i--) {
			for (int j = 0; j < 30; j++) {
				if (maze2[i][j] == 1) {  // Means there is a cube there
					glPushMatrix();
					glTranslatef(j * 1 - 18, 0, i * 1 - 18);
					glBindTexture(GL_TEXTURE_2D, tex_wall.texture[0]);
					glutSolidCube(1);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(j * 1 - 18, 1, i * 1 - 18);
					glBindTexture(GL_TEXTURE_2D, tex_wall.texture[0]);
					glutSolidCube(1);
					glPopMatrix();
				}
				else
				{
					/*flip++;

					if (flip % 10 == 0) //coin
					{
						maze2[i][j] << 2;
						glPushMatrix();
						glTranslatef(j * 1 - 18, 0.5, i * 1 - 18);
						glBindTexture(GL_TEXTURE_2D, tex_coin.texture[0]);
						glutSolidSphere(0.1, 200, 200);
						glPopMatrix();
					}
					if (flip % 10 == 8) //obstalce
					{
						maze2[i][j] << 3;
						glPushMatrix();
						glTranslatef(j * 1 - 18, 0, i * 1 - 18);
						glBindTexture(GL_TEXTURE_2D, tex_rock.texture[0]);
						glRotatef(-90, 1, 0, 0);
						glutSolidCone(0.1, 0.2, 200, 200);
						//		glBindTexture(GL_TEXTURE_2D, tex_coin.texture[0]);
						glPopMatrix();
					}*/
				}
			}
		}
		glDisable(GL_TEXTURE_GEN_S); 
		glDisable(GL_TEXTURE_GEN_T);
		glPopMatrix();
	}
	
}
//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	myInit();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	// Draw Ground
	RenderGround();

	// Draw Tree Model
	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.7, 0.7, 0.7);
	//model_tree.Draw();
	glPopMatrix();

	// Draw house Model
	glPushMatrix();
	glRotatef(90.f, 1, 0, 0);
	//model_house.Draw();
	glPopMatrix();


	//sky box
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);



	

	glPopMatrix();

	//if(!mazeInitialized)
		drawMazes();

	



	//glFlush();

	glutSwapBuffers();
}
void jump(void)
{
	cout << "heree";
	for (int i = 0;i < 7;i++)
	{
		Eye.y += 0.1;
		At.y += 0.1;
		myDisplay();
		cout << "heree";
	}
	Eye.y -= 0.1;
	At.y -= 0.1;
	myDisplay();
	for (int i = 0;i < 3;i++)
	{
		Eye.y -= 0.2;
		At.y -= 0.2;
		myDisplay();
	}
}
//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{	
	std::future<void> result_future;
	switch (button)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ' ':
		jump();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}


	glutPostRedisplay();
}


//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	tex_coin.Load("Textures/coin.bmp");
	tex_wall.Load("Textures/wall.bmp");
	tex_rock.Load("Textures/rock.bmp");
}

//=======================================================================
// Main Function
//=======================================================================

void Anim() {

	glutPostRedisplay();
}

void key(int key, int mx, int my) {
	float oldEyeX = Eye.x;
	float oldAtX = At.x;
	float oldEyeZ = Eye.z;
	float oldAtZ = At.z;
	
	if (key == GLUT_KEY_LEFT) {
		Eye.x--;
		At.x--;
	}
	if (key == GLUT_KEY_RIGHT) {
		Eye.x++;
		At.x++;
	}
	if (key == GLUT_KEY_DOWN) {
		Eye.z++;
		At.z++;

	}
	if (key == GLUT_KEY_UP) {
		Eye.z--;
		At.z--;

	}
	//erfa3 el camera l fo2
	//At.z--;
	cout << "X: ";
	cout << Eye.x;
	cout << "\n";
	cout << "Z: ";
	cout << Eye.z;
	cout << "\n";

	int positionXMaze;
	int positionYMaze;

		if (maze0)
		{
		
		}
		else
		{
			positionYMaze = (int)-Eye.x +2;
			positionXMaze = ((int)-Eye.z) + 11;
			cout << "positionXMaze: ";
			cout << positionXMaze;
			cout << "\n";
			cout << "positionYMaze: ";
			cout << positionYMaze;
			cout << "\n";
			if (positionYMaze < 0 || positionXMaze < 0)
				position = 0;
			else
			{
				position = maze2[positionXMaze][positionYMaze];
			}
		}
		cout << "Position: ";
		cout << position;
		cout << "\n";
		if (position == 1)
		{
			//in a wall
			Eye.x = oldEyeX;
			At.x = oldAtX;
			Eye.z = oldEyeZ;
			At.z = oldAtZ;
			//make sound of can't go to a wall 
		}


	glutPostRedisplay();
}

void handlerFunc(int x, int y)
{
	/* code to handle mouse position deltas */
}
void main(int argc, char** argv)

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);
	glutSpecialFunc(key);

	glutKeyboardFunc(myKeyboard);

	//glutMotionFunc(myMotion);
	//glutPassiveMotionFunc(handlerFunc);
	//glutMouseFunc(myMouse);

	//glutReshapeFunc(myReshape);

	LoadAssets();
	myInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutIdleFunc(Anim);

	glutMainLoop();
}

