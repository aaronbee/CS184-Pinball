#include <stdio.h>

#include "Transform.h"
#include "nv/objload.h"
#include "TGAimage.h"
#include <GLUT/glut.h>

int amount;

vec3 look;
vec3 up;
vec3 right;
vec3 pos;
bool updateLight0, updateLight1;
int w, h;

int oldx = 0, oldy = 0 ; // For mouse motion

Image* tex;
unsigned int e_nindices;
unsigned int p_nindices;
unsigned int *e_indices;
unsigned int *p_indices;

unsigned int e_nverts;
unsigned int p_nverts;
float *e_vertexdata, *e_normaldata, *e_texcoords;
float *p_vertexdata, *p_normaldata, *p_texcoords;
float *e_tangendata, *e_binormdata;
float *p_tangendata, *p_binormdata; //you can ignore these two

// Bumper variables
unsigned int b_nindices; 
unsigned int *b_indices;
unsigned int b_nverts;
float *b_vertexdata, *b_normaldata, *b_texcoords;
float *b_tangendata, *b_binormdata;

// Foot variables
unsigned int f_nindices;
unsigned int *f_indices;
unsigned int f_nverts;
float *f_vertexdata, *f_normaldata, *f_texcoords;
float *f_tangendata, *f_binormdata;

// Ramp
unsigned int r_nindices;
unsigned int *r_indices;
unsigned int r_nverts;
float *r_vertexdata, *r_normaldata, *r_texcoords;
float *r_tangendata, *r_binormdata;



void printHelp() {
	printf("press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n");
}

void keyboard(unsigned char key, int x, int y) {
	vec3 change_in_pos;
	switch(key) {
		case '+':
			amount++;
			printf("amount set to %d\n", amount);
			break;
		case '-':
			amount--;
			printf("amount set to %d\n", amount);
			break;
		case 'w':
			change_in_pos = vec3(look.x * 0.2, look.y * 0.2, look.z * 0.2);
			break;
		case 's':
			change_in_pos = vec3(-look.x * 0.2, -look.y * 0.2, -look.z * 0.2);
			break;
		case 'd':
			change_in_pos = vec3(right.x * 0.2, right.y * 0.2, right.z * 0.2);
			break;
		case 'a':
			change_in_pos = vec3(-right.x * 0.2, -right.y * 0.2, -right.z * 0.2);
			break;
	}
	pos = pos + change_in_pos;
	glutPostRedisplay();
}

nv_scalar to_radians(float degrees) {
	return nv_scalar(degrees) * nv_to_rad;
}

void drag(int x, int y) {
	float pitch = - (y - oldy) * 0.3;
	float yaw = (x - oldx) * 0.2;
	oldx = x;
	oldy = y;
	
	mat3 pitch_rot = mat3();
	pitch_rot.set_rot(to_radians(pitch), right);
	
	up = up * pitch_rot;
	look = look * pitch_rot;
	
	mat3 yaw_rot = mat3();
	yaw_rot.set_rot(to_radians(yaw), vec3(0,0,-1));
	
	look = look * yaw_rot;
	up = up * yaw_rot;
	right = right * yaw_rot;

	
	glutPostRedisplay();

}

void mouse(int x, int y) {
	oldx = x;
	oldy = y;
}

void camera() {
	gluLookAt(pos.x, pos.y, pos.z,
			  pos.x + look.x, pos.y + look.y, pos.z + look.z,
			  up.x, up.y, up.z);
}

void specialKey(int key, int x, int y) {
//	switch(key) {
//	case 100: //left
//			yrot += 5;
//		break;
//	case 101: //up
//			xrot += 5;
//		break;
//	case 102: //right
//			yrot -= 5;
//		break;
//	case 103: //down
//			xrot -= 5;
//		break;
//	}
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
	look = vec3(0, 1, 0);
	up = vec3(0, 0, 1);
	right = vec3(1, 0, 0);
	pos = vec3(0, 0, 0);
	amount = 5;

  
  //lighting
  
  glEnable(GL_LIGHTING);
  
  
    GLfloat one[] = {1, 1, 1, 1};
    GLfloat medium[] = {0.5, 0.5, 0.5, 1};
    GLfloat small[] = {0.2, 0.2, 0.2, 1};
    GLfloat high[] = {100};
    GLfloat light_specular[] = {1, 0.5, 0, 1};
    GLfloat light_specular1[] = {0, 0.5, 1, 1};
    GLfloat light_position[] = {5, 5, 0, 1};
    GLfloat light_position1[] = {5, -5, 0, 1};
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, medium);
	glLightfv(GL_LIGHT0, GL_AMBIENT, medium);
	glEnable(GL_LIGHT0);
	
	
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  medium);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  

  
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_AMBIENT, GL_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_AMBIENT, one);
	glMaterialfv(GL_FRONT, GL_SPECULAR, one);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, small);
	glMaterialfv(GL_FRONT, GL_SHININESS, high);
	
	
	tex = new TGAImage();
	tex->open("texture3.tga");
	tex->updateTexture();

	
	LoadObjModel( "elephant2.obj", e_nverts, e_nindices, e_indices,
				 e_vertexdata, e_normaldata, e_tangendata, e_binormdata, e_texcoords );

	LoadObjModel( "plunger.obj", p_nverts, p_nindices, p_indices,
				 p_vertexdata, p_normaldata, p_tangendata, p_binormdata, p_texcoords );
	
	LoadObjModel( "rampnormals.obj", r_nverts, r_nindices, r_indices,
				 r_vertexdata, r_normaldata, r_tangendata, r_binormdata, r_texcoords );
	
	LoadObjModel( "bumper.obj", b_nverts, b_nindices, b_indices,
				 b_vertexdata, b_normaldata, b_tangendata, b_binormdata, b_texcoords );
	
	LoadObjModel( "crazyfoot.obj", f_nverts, f_nindices, f_indices,
				 f_vertexdata, f_normaldata, f_tangendata, f_binormdata, f_texcoords );

	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glLoadIdentity();
	
	camera();
	


	glVertexPointer(3, GL_FLOAT, 0, e_vertexdata);
	glNormalPointer(GL_FLOAT, 0, e_normaldata);
	
	glPushMatrix();
//	glColor3f(1.0,0,0);
	glTranslatef(2.5, 0, 0);
	glDrawElements( GL_TRIANGLES, e_nindices, GL_UNSIGNED_INT, e_indices );
	glPopMatrix();

	glVertexPointer(3, GL_FLOAT, 0, p_vertexdata);
	glNormalPointer(GL_FLOAT, 0, p_normaldata);

	glPushMatrix();
	glTranslatef(2, 0, 5);
	glDrawElements( GL_TRIANGLES, p_nindices, GL_UNSIGNED_INT, p_indices );
	glPopMatrix();
	
	glVertexPointer(3, GL_FLOAT, 0, f_vertexdata);
	glNormalPointer(GL_FLOAT, 0, f_normaldata);
	
	glPushMatrix();
	glTranslatef(-0.5, 0, 2);
	glDrawElements( GL_TRIANGLES, f_nindices, GL_UNSIGNED_INT, f_indices );
	glPopMatrix();
	
	glVertexPointer(3, GL_FLOAT, 0, b_vertexdata);
	glNormalPointer(GL_FLOAT, 0, b_normaldata);
	
	glPushMatrix();
	glTranslatef(0, -2, 0);
	glDrawElements( GL_TRIANGLES, b_nindices, GL_UNSIGNED_INT, b_indices );
	glPopMatrix();
	
	glVertexPointer(3, GL_FLOAT, 0, r_vertexdata);
	glNormalPointer(GL_FLOAT, 0, r_normaldata);
	
	glPushMatrix();
	glTranslatef(-2, -3, 0);
	glDrawElements( GL_TRIANGLES, r_nindices, GL_UNSIGNED_INT, r_indices );
	glPopMatrix();
	
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
//	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse);
	glutMotionFunc(drag);
	printHelp();
	glutMainLoop();
	return 0;
}
