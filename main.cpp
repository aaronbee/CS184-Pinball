#include <stdio.h>

#include "Transform.h"
#include "nv/objload.h"
#include "TGAimage.h"
#include <GLUT/glut.h>

int amount;

vec3 eye;
vec3 up;
bool updateLight0, updateLight1;
int w, h;
Image* tex;
unsigned int nindices;
unsigned int *indices;

void printHelp() {
	printf("press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n");
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case '+':
		amount++;
		printf("amount set to %d\n", amount);
		break;
	case '-':
		amount--;
		printf("amount set to %d\n", amount);
		break;
	}
	glutPostRedisplay();
}

void specialKey(int key, int x, int y) {
	switch(key) {
	case 100: //left
		Transform::left(static_cast<float>(amount), eye,  up);
		break;
	case 101: //up
		Transform::up(static_cast<float>(amount),  eye,  up);
		break;
	case 102: //right
		Transform::left(static_cast<float>(-amount),  eye,  up);
		break;
	case 103: //down
		Transform::up(static_cast<float>(-amount),  eye,  up);
		break;
	}
	up.normalize();
	glutPostRedisplay();
}

void reshape(int width, int height){
	w = width;
	h = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, w/(float)h, 0.1, 99);
	glViewport(0, 0, w, h);
}

void init() {
	eye = vec3(0, 0, 5);
	up = vec3(0, 1, 0);
	amount = 5;

  
  //lighting
  
  glEnable(GL_LIGHTING);
  
  
	GLfloat light_white[] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_white);
	glEnable(GL_LIGHT0);
  
  GLfloat light_position[] = {1,1,0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
	
  	GLfloat one[] = {1, 1, 1, 1};
	GLfloat small[] = {0.2, 0.2, 0.2, 1};
	GLfloat high[] = {100};
	glMaterialfv(GL_FRONT, GL_AMBIENT, one);
	glMaterialfv(GL_FRONT, GL_SPECULAR, one);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, small);
	glMaterialfv(GL_FRONT, GL_SHININESS, high);
	
	
	tex = new TGAImage();
	tex->open("texture.tga");
	tex->updateTexture();
    unsigned int nverts;
    float *vertexdata, *normaldata, *texcoords;
	float *tangendata, *binormdata; //you can ignore these two

	LoadObjModel( "cat.obj", nverts, nindices, indices,

		vertexdata, normaldata, tangendata, binormdata, texcoords );
	glVertexPointer(3, GL_FLOAT, 0, vertexdata);

	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z,
			0, 0, 0,
			up.x, up.y, up.z);

	glDrawElements( GL_TRIANGLES, nindices, GL_UNSIGNED_INT, indices );

	glBegin( GL_QUADS );
		glTexCoord2d(0.0,0.0); glVertex3d(-50.0,-50.0, -1.4);
		glTexCoord2d(40.0,0.0); glVertex3d(50.0,-50.0, -1.4);
		glTexCoord2d(40.0,40.0); glVertex3d(50.0,50.0, -1.4);
		glTexCoord2d(0.0,40.0); glVertex3d(-50.0,50.0, -1.4);
	glEnd();
  
  glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.0); glVertex3d(-25.0,-25.0, -1.4);
    glTexCoord2d(40.0,40.0); glVertex3d(-25.0,25.0, -1.4);
    glTexCoord2d(0.0,40.0); glVertex3d(-25.0,25.0, 5.0);
    glTexCoord2d(40.0,0.0); glVertex3d(-25.0,-25.0, 5.0);
	glEnd();
  
  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex3d(-25.0,-25.0, -1.4);
  glTexCoord2d(40.0,40.0); glVertex3d(-25.0,25.0, -1.4);
  glTexCoord2d(0.0,40.0); glVertex3d(-25.0,25.0, 5.0);
  glTexCoord2d(40.0,0.0); glVertex3d(-25.0,-25.0, 5.0);
	glEnd();
  
  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex3d(25.0,25.0, -1.4);
  glTexCoord2d(40.0,40.0); glVertex3d(25.0,-25.0, -1.4);
  glTexCoord2d(0.0,40.0); glVertex3d(25.0,-25.0, 5.0);
  glTexCoord2d(40.0,0.0); glVertex3d(25.0,25.0, 5.0);
	glEnd();

  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex3d(-25.0,-25.0, -1.4);
  glTexCoord2d(40.0,40.0); glVertex3d(25.0,-25.0, -1.4);
  glTexCoord2d(0.0,40.0); glVertex3d(25.0,-25.0, 5.0);
  glTexCoord2d(40.0,0.0); glVertex3d(-25.0,-25.0, 5.0);
	glEnd();
  
  
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HW3: Helper Scene");
	init();
	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutReshapeWindow(800, 600);
	printHelp();
	glutMainLoop();
	return 0;
}
