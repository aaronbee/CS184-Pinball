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
vec3 marble_pos;
float foot_height;
bool wireframe_foot;
bool light0_on;
bool all_lights_off;
int w, h;

bool bumper1_lit, bumper2_lit, bumper3_lit, bumper4_lit;
bool foot_up, foot_move;

int oldx = 0, oldy = 0 ; // For mouse motion

Image* tex;
Image* tex1;
Image* tex2;
Image* tex3;
Image* tex4;
Image* tex5;
Image* tex6;
Image* ground;

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

// Ground variables
unsigned int g_nindices; 
unsigned int *g_indices;
unsigned int g_nverts;
float *g_vertexdata, *g_normaldata, *g_texcoords;
float *g_tangendata, *g_binormdata;

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

//Number Variables
unsigned int one_nindices;
unsigned int *one_indices;
unsigned int one_nverts;
float *one_vertexdata, *one_normaldata, *one_texcoords, *one_tangendata, *one_binormdata;

unsigned int two_nindices;
unsigned int *two_indices;
unsigned int two_nverts;
float *two_vertexdata, *two_normaldata, *two_texcoords, *two_tangendata, *two_binormdata;
unsigned int three_nindices;
unsigned int *three_indices;
unsigned int three_nverts;
float *three_vertexdata, *three_normaldata, *three_texcoords, *three_tangendata, *three_binormdata;
unsigned int four_nindices;
unsigned int *four_indices;
unsigned int four_nverts;
float *four_vertexdata, *four_normaldata, *four_texcoords, *four_tangendata, *four_binormdata;
unsigned int five_nindices;
unsigned int *five_indices;
unsigned int five_nverts;
float *five_vertexdata, *five_normaldata, *five_texcoords, *five_tangendata, *five_binormdata;
unsigned int six_nindices;
unsigned int *six_indices;
unsigned int six_nverts;
float *six_vertexdata, *six_normaldata, *six_texcoords, *six_tangendata, *six_binormdata;
unsigned int seven_nindices;
unsigned int *seven_indices;
unsigned int seven_nverts;
float *seven_vertexdata, *seven_normaldata, *seven_texcoords, *seven_tangendata, *seven_binormdata;
unsigned int eight_nindices;
unsigned int *eight_indices;
unsigned int eight_nverts;
float *eight_vertexdata, *eight_normaldata, *eight_texcoords, *eight_tangendata, *eight_binormdata;

// Colors
GLfloat gray[] = {0.5, 0.5, 0.5, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat shiny_hi[] = {100};
GLfloat shiny_lo[] = {5};
GLfloat red[] = {0.7, 0, 0, 1};
GLfloat reddish[] = {0.3, 0, 0, 1};
GLfloat green[] = {0, 0.7, 0, 1};
GLfloat greenish[] = {0, 0.3, 0, 1};
GLfloat blue[] = {0, 0, 0.7, 1};
GLfloat blueish[] = {0, 0, 0.3, 1};
GLfloat yellow[] = {0.7, 0.7, 0, 1};
GLfloat yellowish[] = {0.3, 0.3, 0, 1};
GLfloat zero[] = {0.0, 0.0, 0.0, 0.0};
GLfloat foot_color[] = {0.65, 0.5, 0.5, 1.0};

GLfloat elephant_color[] = {0.6, 0.4, 0, 1};

GLfloat one[] = {1, 1, 1, 1};
GLfloat medium[] = {0.5, 0.5, 0.5, 1};
GLfloat small[] = {0.2, 0.2, 0.2, 1};
GLfloat high[] = {100};
GLfloat light_specular[] = {1, 1, 1, 1};
GLfloat light_position[] = {0, 1, 1, 0};

// Disco lights
GLfloat disco_light_pos[] = {0, 0, 4, 1};
GLfloat disco_light1_dir[] = {1, 0, -1};
GLfloat disco_light2_dir[] = {0, 1, -1};
GLfloat disco_light3_dir[] = {-1, 0, -1};
GLfloat disco_light4_dir[] = {0, -1, -1};
GLfloat cutoff_angle = 30.0;
GLfloat disco_rotation = 0;

void printHelp() {
	printf("press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n");
}

void keyboard(unsigned char key, int x, int y) {
	vec3 change_in_pos = vec3(0,0,0);
	float scale = 0.1 * amount;
	switch(key) {
		case '+':
			amount++;
			printf("amount set to %d\n", amount);
			break;
		case '-':
			amount--;
			if (amount == 0) amount = 1;
			printf("amount set to %d\n", amount);
			break;
		case 'q':
			wireframe_foot = !wireframe_foot;
			break;
		case 'e':
			foot_move = !foot_move;
			break;
		case 'l':
			if (light0_on) {
				glDisable(GL_LIGHT0);
				light0_on = false;
			} else if (!all_lights_off) {
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHT2);
				glDisable(GL_LIGHT3);
				glDisable(GL_LIGHT4);
				all_lights_off = true;
			} else {
				glEnable(GL_LIGHT0);
				glEnable(GL_LIGHT1);
				glEnable(GL_LIGHT2);
				glEnable(GL_LIGHT3);
				glEnable(GL_LIGHT4);
				light0_on = true;
				all_lights_off = false;
			}
			break;
		case 'w':
			change_in_pos = vec3(look.x * scale, look.y * scale, look.z * scale);
			break;
		case 's':
			change_in_pos = vec3(-look.x * scale, -look.y * scale, -look.z * scale);
			break;
		case 'd':
			change_in_pos = vec3(right.x * scale, right.y * scale, right.z * scale);
			break;
		case 'a':
			change_in_pos = vec3(-right.x * scale, -right.y * scale, -right.z * scale);
			break;
	}
	pos = pos + change_in_pos;
	glutPostRedisplay();
}

nv_scalar to_radians(float degrees) {
	return nv_scalar(degrees) * nv_to_rad;
}

void rotate_camera(float pitch, float yaw) {
	mat3 pitch_rot = mat3();
	pitch_rot.set_rot(to_radians(pitch), right);
	
	up = up * pitch_rot;
	look = look * pitch_rot;
	
	mat3 yaw_rot = mat3();
	yaw_rot.set_rot(to_radians(yaw), vec3(0,0,-1));
	
	look = look * yaw_rot;
	up = up * yaw_rot;
	right = right * yaw_rot;
	
	normalize(up);
	normalize(look);
	normalize(right);
	
	glutPostRedisplay();
}

void drag(int x, int y) {
	float pitch = - (y - oldy) * 0.3;
	float yaw = (x - oldx) * 0.2;
	oldx = x;
	oldy = y;
	
	rotate_camera(pitch, yaw);
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
	switch(key) {
	case 100: //left
			rotate_camera(0, 5);
		break;
	case 101: //up
			rotate_camera(-5, 0);
		break;
	case 102: //right
			rotate_camera(0, -5);
		break;
	case 103: //down
			rotate_camera(5, 0);
		break;
	}
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

/************************************************************************/
/*	Render a skybox with center point position and dimension sizes size */
/************************************************************************/

void RenderSkybox(vec3 position,vec3 size)
{	
  glEnable(GL_TEXTURE_2D);
	// Begin DrawSkybox
	glColor4f(1.0, 1.0, 1.0,1.0f);
  
	// Save Current Matrix
	glPushMatrix();
  
	// Second Move the render space to the correct position (Translate)
	glTranslatef(position.x,position.y,position.z);
  
	// First apply scale matrix
	glScalef(size.x,size.y,size.z);
  
	float cz = -0.0f,cx = 1.0f;
	float r = 1.0f; // If you have border issues change this to 1.005f
	// Common Axis Z - FRONT Side
	//glBindTexture(GL_TEXTURE_2D,SkyBox[4]);

  tex2->updateTexture();
	glBegin(GL_QUADS);	
  glTexCoord2f(cx, cz); glVertex3f(-r ,1.0f,-r);
  glTexCoord2f(cx,  cx); glVertex3f(-r,1.0f,r);
  glTexCoord2f(cz,  cx); glVertex3f( r,1.0f,r); 
  glTexCoord2f(cz, cz); glVertex3f( r ,1.0f,-r);
	glEnd();
  
  
	// Common Axis Z - BACK side
	//glBindTexture(GL_TEXTURE_2D,SkyBox[5]);
  tex1->updateTexture();
	glBegin(GL_QUADS);		
  glTexCoord2f(cx,cz);  glVertex3f(-r,-1.0f,-r);
  glTexCoord2f(cx,cx);  glVertex3f(-r,-1.0f, r);
  glTexCoord2f(cz,cx);  glVertex3f( r,-1.0f, r); 
  glTexCoord2f(cz,cz);  glVertex3f( r,-1.0f,-r);
	glEnd();
  
	// Common Axis X - Left side
	//glBindTexture(GL_TEXTURE_2D,SkyBox[3]);

  tex3->updateTexture();
	glBegin(GL_QUADS);		
  glTexCoord2f(cx,cx); glVertex3f(-1.0f, -r, r);	
  glTexCoord2f(cz,cx); glVertex3f(-1.0f,  r, r); 
  glTexCoord2f(cz,cz); glVertex3f(-1.0f,  r,-r);
  glTexCoord2f(cx,cz); glVertex3f(-1.0f, -r,-r);		
	glEnd();
  
	// Common Axis X - Right side
	//glBindTexture(GL_TEXTURE_2D,SkyBox[2]);
  tex4->updateTexture();
	glBegin(GL_QUADS);		
  glTexCoord2f( cx,cx); glVertex3f(1.0f, -r, r);	
  glTexCoord2f(cz, cx); glVertex3f(1.0f,  r, r); 
  glTexCoord2f(cz, cz); glVertex3f(1.0f,  r,-r);
  glTexCoord2f(cx, cz); glVertex3f(1.0f, -r,-r);
	glEnd();
  
	// Common Axis Y - Draw Up side
	//glBindTexture(GL_TEXTURE_2D,SkyBox[0]);
  tex6->updateTexture();
	glBegin(GL_QUADS);		
  glTexCoord2f(cz, cz); glVertex3f( r, -r,1.0f);
  glTexCoord2f(cx, cz); glVertex3f( r,  r,1.0f); 
  glTexCoord2f(cx, cx); glVertex3f(-r,  r,1.0f);
  glTexCoord2f(cz, cx); glVertex3f(-r, -r,1.0f);
	glEnd();
  
	// Common Axis Y - Down side
	//glBindTexture(GL_TEXTURE_2D,SkyBox[1]);
  tex5->updateTexture();
	glBegin(GL_QUADS);		
  glTexCoord2f(cz,cz);  glVertex3f( r, -r,-1.0f);
  glTexCoord2f( cx,cz); glVertex3f( r,  r,-1.0f); 
  glTexCoord2f( cx,cx); glVertex3f(-r,  r,-1.0f);
  glTexCoord2f(cz, cx); glVertex3f(-r, -r,-1.0f);
	glEnd();
 
	// Load Saved Matrix
	glPopMatrix();
 glDisable(GL_TEXTURE_2D);
};

void draw_ground() {
  glEnable(GL_TEXTURE_2D);
  ground->updateTexture();

  
  //ground->updateTexture();
	glBegin( GL_QUADS );
  glNormal3d(0.0,0.0,1.0) ;
  glTexCoord2d(0.0,0.0); 
  glVertex3d(-50.0,-50.0, -1.4); 
glTexCoord2d(16.0,0.0); 
  glVertex3d(50.0,-50.0, -1.4);
glTexCoord2d(16.0,16.0); 
  glVertex3d(50.0,50.0, -1.4);
glTexCoord2d(0.0,16.0); 
  glVertex3d(-50.0,50.0, -1.4);
	glEnd();
  glDisable(GL_TEXTURE_2D);
  

}

void draw_seven() {
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  
  ground->updateTexture();
  //top half of sandwich
  glNormal3d(0.0,0.0,1.0);
  glVertex3d(0.0,0.0,3.0);
  glVertex3d(1.0,0.0,3.0);
  glVertex3d(3.0,4.0,3.0);
  glVertex3d(2.0,4.0,3.0);
  
  glVertex3d(0.0,4.0,3.0);
  glVertex3d(0.0,5.0,3.0);
  glVertex3d(3.0,5.0,3.0);
  glVertex3d(3.0,4.0,3.0);
  
  
  //bottom half of sandwich
  glNormal3d(0.0,0.0,-1.0);
  glVertex3d(0.0,0.0,2.0);
  glVertex3d(1.0,0.0,2.0);
  glVertex3d(3.0,4.0,2.0);
  glVertex3d(2.0,4.0,2.0);
  
  glVertex3d(0.0,4.0,2.0);
  glVertex3d(0.0,5.0,2.0);
  glVertex3d(3.0,5.0,2.0);
  glVertex3d(3.0,4.0,2.0);
  
  //start from top left
  glNormal3d(0.0,1.0,0.0);
  glVertex3d(0.0,5.0,3.0);
  glVertex3d(3.0,5.0,3.0);
  glVertex3d(3.0,5.0,2.0);
  glVertex3d(0.0,5.0,2.0);
  
  glNormal3d(1.0,0.0,0.0);
  glVertex3d(3.0,5.0,3.0);
  glVertex3d(3.0,4.0,3.0);
  glVertex3d(3.0,4.0,2.0);
  glVertex3d(3.0,5.0,2.0);

  glNormal3d(0.895,-0.447,0.0);
  glVertex3d(3.0,4.0,3.0);
  glVertex3d(1.0,0.0,3.0);
  glVertex3d(1.0,0.0,2.0);
  glVertex3d(3.0,4.0,2.0);

  glNormal3d(0.0,-1.0,0.0);
  glVertex3d(1.0,0.0,3.0);
  glVertex3d(0.0,0.0,3.0);
  glVertex3d(0.0,0.0,2.0);
  glVertex3d(1.0,0.0,2.0);

  glNormal3d(-0.895,0.447,0.0);
  glVertex3d(0.0,0.0,3.0);
  glVertex3d(2.0,4.0,3.0);
  glVertex3d(2.0,4.0,2.0);
  glVertex3d(0.0,0.0,2.0);
  
  glNormal3d(0.0,-1.0,0.0);
  glVertex3d(2.0,4.0,3.0);
  glVertex3d(0.0,4.0,3.0);
  glVertex3d(0.0,4.0,2.0);
  glVertex3d(2.0,4.0,2.0);
  
  glNormal3d(-1.0,0.0,0.0);
  glVertex3d(0.0,4.0,3.0);
  glVertex3d(0.0,5.0,3.0);
  glVertex3d(0.0,5.0,2.0);
  glVertex3d(0.0,4.0,2.0);
  
  glEnd();
 glDisable(GL_TEXTURE_2D);
}

void draw_ground2(float x, float y, float z) {
  glVertexPointer(3, GL_FLOAT, 0, g_vertexdata);
	glNormalPointer(GL_FLOAT, 0, g_normaldata);
  
	glPushMatrix();
	glTranslatef(x, y, z);
  glScalef(2.0, 2.0, 2.0);
	glDrawElements( GL_TRIANGLES, g_nindices, GL_UNSIGNED_INT, g_indices );
  glScalef(0.5, 0.5, 0.5);

}

void draw_elephant(float x, float y, float z) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, elephant_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, elephant_color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, elephant_color);
	
	glVertexPointer(3, GL_FLOAT, 0, e_vertexdata);
	glNormalPointer(GL_FLOAT, 0, e_normaldata);
	
	glPushMatrix();
	glTranslatef(x, y, z);
	glDrawElements( GL_TRIANGLES, e_nindices, GL_UNSIGNED_INT, e_indices );
	glPopMatrix();
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, white);
}

void draw_one(float x, float y, float z, float rot, float *vertexdata, float *normaldata, unsigned int nindices, unsigned int *indices) {
	glVertexPointer(3, GL_FLOAT, 0, vertexdata);
	glNormalPointer(GL_FLOAT, 0, normaldata);
	
	glPushMatrix();
	glRotatef(rot, 0, 0.0, 1.0);
	glTranslatef(x, y, z);
  glRotatef(90.0, 1.0, 0.0, 0.0);
	glDrawElements( GL_TRIANGLES, nindices, GL_UNSIGNED_INT, indices );
	glPopMatrix();
}

void draw_plunger(float x, float y, float z) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_hi);
	
	glVertexPointer(3, GL_FLOAT, 0, p_vertexdata);
	glNormalPointer(GL_FLOAT, 0, p_normaldata);
	
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-disco_rotation, 0, 0, 1);
	glRotatef(-90, 0, 1, 0);
	glDrawElements( GL_TRIANGLES, p_nindices, GL_UNSIGNED_INT, p_indices );
	glPopMatrix();
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_lo);
}

void draw_foot(float x, float y, float z) {
	glVertexPointer(3, GL_FLOAT, 0, f_vertexdata);
	glNormalPointer(GL_FLOAT, 0, f_normaldata);
		
	glMaterialfv(GL_FRONT, GL_DIFFUSE, foot_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, foot_color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, foot_color);
	
	glPushMatrix();
	glTranslatef(x, y, z);
	if (wireframe_foot)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements( GL_TRIANGLES, f_nindices, GL_UNSIGNED_INT, f_indices );
	if (wireframe_foot)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, white);
}

void draw_bumper(float x, float y, float z) {
	glVertexPointer(3, GL_FLOAT, 0, b_vertexdata);
	glNormalPointer(GL_FLOAT, 0, b_normaldata);
	
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.7, 0.7, 0.7);
	glDrawElements( GL_TRIANGLES, b_nindices, GL_UNSIGNED_INT, b_indices );
	glPopMatrix();
}

void draw_ramp(float x, float y, float z) {
	glVertexPointer(3, GL_FLOAT, 0, r_vertexdata);
	glNormalPointer(GL_FLOAT, 0, r_normaldata);
	
	glPushMatrix();
	glTranslatef(x, y, z);
	glDrawElements( GL_TRIANGLES, r_nindices, GL_UNSIGNED_INT, r_indices );
	glPopMatrix();
}

void draw_marble() {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_hi);
	
	glPushMatrix();
	glTranslatef(marble_pos.x, marble_pos.y, marble_pos.z);
	glutSolidSphere(0.4, 10, 10);
	glPopMatrix();
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_lo);
}

bool ball_foot_side_collision() {
	return (foot_height < 2 &&
			marble_pos.y > 1.19 && marble_pos.y < 1.21 &&
			marble_pos.x > 0.98 && marble_pos.x < 0.99);
}

bool ball_foot_under_collision() {
	return (marble_pos.y > 1.19 && marble_pos.y < 1.21 &&
			marble_pos.x > -0.98 && marble_pos.x < 0.98 &&
			foot_height < 2);
}

float marble_speed = 0.01;

void move_marble() {
	if (ball_foot_side_collision())
		return;
	if (marble_pos.x >= 1.2) {
		marble_pos.x = 1.2;
		if (marble_pos.y >= -1)
			bumper4_lit = false;
		if (marble_pos.y < 1.2) 
			marble_pos.y += marble_speed;
		if (marble_pos.y > 1.19)
			bumper1_lit = true;
	} if (marble_pos.y >= 1.2) {
		marble_pos.y = 1.2;
		if (marble_pos.x <= 1)
			bumper1_lit = false;
		if (marble_pos.x > -1.2)
			marble_pos.x -= marble_speed;
		if (marble_pos.x < -1.19)
			bumper2_lit = true;
	} if (marble_pos.x <= -1.2) {
		marble_pos.x = -1.2;
		if (marble_pos.y <= 1)
			bumper2_lit = false;
		if (marble_pos.y > -1.2)
			marble_pos.y -= marble_speed;
		if (marble_pos.y < -1.19)
			bumper3_lit = true;
	} if (marble_pos.y <= -1.2) {
		marble_pos.y = -1.2;
		if (marble_pos.x >= -1)
			bumper3_lit = false;
		if (marble_pos.x < 1.2) 
			marble_pos.x += marble_speed;
		if (marble_pos.x > 1.19)
			bumper4_lit = true;
	}
}

void move_foot() {
	if (ball_foot_under_collision()) {
		foot_up = true;
		foot_height += marble_speed;
		return;
	}
	if (!foot_move)
		return;
	if (foot_up) {
		foot_height += marble_speed;
		if (foot_height > 5)
			foot_up = false;
	} else {
		foot_height -= marble_speed;
		if (foot_height < 1)
			foot_up = true;
	}
}

void rotate_disco_lights() {
	disco_rotation ++;
	if (disco_rotation >= 360)
		disco_rotation -= 360;
}

void animation() {
	move_foot();
	move_marble();
	rotate_disco_lights();
	glutPostRedisplay();
}

void move_light(const GLfloat old_pos[], GLfloat new_pos[]) {
	new_pos[0] = old_pos[0] - pos.x;
	new_pos[1] = old_pos[1] - pos.y;
	new_pos[2] = old_pos[2] - pos.z;
	new_pos[3] = old_pos[3];
}

void place_lights(const GLfloat view_matrix[]) {

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, medium);
	glLightfv(GL_LIGHT0, GL_AMBIENT, medium);
	
	glPushMatrix();
	glRotatef(disco_rotation, 0, 0, 1);
	
	glLightfv(GL_LIGHT1, GL_POSITION, disco_light_pos);
	glLightfv(GL_LIGHT2, GL_POSITION, disco_light_pos);
	glLightfv(GL_LIGHT3, GL_POSITION, disco_light_pos);
	glLightfv(GL_LIGHT4, GL_POSITION, disco_light_pos);
	
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff_angle);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cutoff_angle);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, cutoff_angle);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, cutoff_angle);
	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, disco_light1_dir);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, disco_light2_dir);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, disco_light3_dir);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, disco_light4_dir);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, zero);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT1, GL_SPECULAR, red);
	
	glLightfv(GL_LIGHT2, GL_AMBIENT, zero);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT2, GL_SPECULAR, green);
	
	glLightfv(GL_LIGHT3, GL_AMBIENT, zero);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, blue);
	glLightfv(GL_LIGHT3, GL_SPECULAR, blue);
	
	glLightfv(GL_LIGHT4, GL_AMBIENT, zero);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, yellow);
	glLightfv(GL_LIGHT4, GL_SPECULAR, yellow);
	
	glPopMatrix();
	
	if (light0_on) {
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
	}
}

void init() {
  
  GLuint texture;
  glGenTextures( 1, &texture );
  
  //textures
  tex = new TGAImage();
  tex1 = new TGAImage();
  tex2 = new TGAImage();
  tex3 = new TGAImage();
  tex4 = new TGAImage();
  tex5 = new TGAImage();
  tex6 = new TGAImage();
  tex1->open("1.tga");
  tex2->open("2.tga");
  tex3->open("3.tga");
  tex4->open("4.tga");
  tex5->open("5.tga");
  tex6->open("6.tga");
  
  ground = new TGAImage();
  ground->open("ground.tga");
  
	look = vec3(0, 1, 0);
	up = vec3(0, 0, 1);
	right = vec3(1, 0, 0);
	pos = vec3(0, -7, 4);
	amount = 3;
	marble_pos = vec3(1.2, 1.2, 0);
	wireframe_foot = false;
	foot_height = 1;
	foot_up = true;
	foot_move = false;
	light0_on = true;
  
	reshape(w,h);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, one);
	glMaterialfv(GL_FRONT, GL_SPECULAR, one);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, small);
	glMaterialfv(GL_FRONT, GL_SHININESS, high);
  
  LoadObjModel( "ground.obj", g_nverts, g_nindices, g_indices,
               g_vertexdata, g_normaldata, g_tangendata, g_binormdata, g_texcoords );
	
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

  LoadObjModel( "1.obj", one_nverts, one_nindices, one_indices, one_vertexdata, one_normaldata, one_tangendata, one_binormdata, one_texcoords );
  LoadObjModel( "2.obj", two_nverts, two_nindices, two_indices, two_vertexdata, two_normaldata, two_tangendata, two_binormdata, two_texcoords );
  LoadObjModel( "3.obj", three_nverts, three_nindices, three_indices, three_vertexdata, three_normaldata, three_tangendata, three_binormdata, three_texcoords );
  LoadObjModel( "4.obj", four_nverts, four_nindices, four_indices, four_vertexdata, four_normaldata, four_tangendata, four_binormdata, four_texcoords );
  LoadObjModel( "5.obj", five_nverts, five_nindices, five_indices, five_vertexdata, five_normaldata, five_tangendata, five_binormdata, five_texcoords );
  LoadObjModel( "6.obj", six_nverts, six_nindices, six_indices, six_vertexdata, six_normaldata, six_tangendata, six_binormdata, six_texcoords );
  LoadObjModel( "8.obj", eight_nverts, eight_nindices, eight_indices, eight_vertexdata, eight_normaldata, eight_tangendata, eight_binormdata, eight_texcoords );
  
  
  
  
  
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	reshape(w, h);
	glMatrixMode(GL_MODELVIEW);
	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glLoadIdentity();
	
	camera();
	GLfloat view_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix);

	glEnable(GL_LIGHTING);
	//draw_elephant(2.5, 0, 0);

	draw_plunger(0, 0, 4);

	draw_foot(0, 2, foot_height);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, reddish);
	glMaterialfv(GL_FRONT, GL_SPECULAR, reddish);
	glMaterialfv(GL_FRONT, GL_AMBIENT, reddish);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_hi);
	
	if (bumper1_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, red);

	draw_bumper(2, 2, 0.5);
	
	if (bumper1_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, zero);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenish);
	glMaterialfv(GL_FRONT, GL_SPECULAR, greenish);
	glMaterialfv(GL_FRONT, GL_AMBIENT, greenish);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_hi);
	
	if (bumper2_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, green);
	
	draw_bumper(-2, 2, 0.5);
	
	if (bumper2_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, zero);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueish);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blueish);
	glMaterialfv(GL_FRONT, GL_AMBIENT, blueish);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_hi);
	
	if (bumper3_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, blue);
	
	draw_bumper(-2, -2, 0.5);
	
	if (bumper3_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, zero);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowish);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellowish);
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowish);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_hi);
	
	if (bumper4_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
	
	draw_bumper(2, -2, 0.5);
	
	if (bumper4_lit) 
		glMaterialfv(GL_FRONT, GL_EMISSION, zero);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny_lo);
	
	//draw_ramp(-2, -3, 0);
	
	draw_marble();
  draw_one(0,5.5,0, 0, one_vertexdata, one_normaldata, one_nindices, one_indices);
  draw_one(0,5.5,0, -45, two_vertexdata, two_normaldata, two_nindices, two_indices);
  draw_one(0,5.5,0, -90, three_vertexdata, three_normaldata, three_nindices, three_indices);
  draw_one(0,5.5,0, -135, four_vertexdata, four_normaldata, four_nindices, four_indices);
  draw_one(0,5.5,0, -180, five_vertexdata, five_normaldata, five_nindices, five_indices);
  draw_one(0,5.5,0, -225, six_vertexdata, six_normaldata, six_nindices, six_indices);
	glPushMatrix();
	glTranslatef(-8, -1,-2);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_seven();
	glPopMatrix();
  draw_one(0,5.5,0, -315, eight_vertexdata, eight_normaldata, eight_nindices, eight_indices);


	draw_elephant(0.0, -1.5, 1.0);
  draw_ground2(0,0,0);

  
  RenderSkybox(pos, vec3(50,50,50));
  	
	place_lights(view_matrix);
	
	glDisable(GL_LIGHTING);
	
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
	glutIdleFunc(animation);
	printHelp();
	glutMainLoop();
	return 0;
}
