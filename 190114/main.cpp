#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#include "definition.h"
// #include "bmptype.h"

#define  PI   3.141592653
#define Step  0.5
#define   TSIZE  64   /* define texture dimension */
#define   ON      1
#define   OFF     0

typedef int menu_t;
menu_t top_m, view_m, light_oc_m, light_chg_m, fog_chg_m;

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj  *sphere = NULL, *cylind = NULL, *disk;

/*-Declare car position, orientation--*/
float  self_ang = -90.0, glob_ang = 0.0;
float  blade_top_ang = 10.0, blade_back_ang = 10.0;
float  position[3] = { 8.0, 1.2, 0.0 };

/*-----Define window size----*/
int width = 600, height = 600;

/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 }, Vup[3] = { 0.0, 1.0, 0.0 };
double ang = 90.0;

float   u[3][3] = { { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */

/*-------Define a cube------*/
float  points[][3] = { { -1.0, 0.0, -1.0 }, { 1.0, 0.0, -1.0 }, { 1.0, 1.0, -1.0 }, { -1.0, 1.0, -1.0 },
{ -1.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 }, { 1.0, 1.0, 1.0 }, { -1.0, 1.0, 1.0 } };
int    face[][4] = { { 0, 3, 2, 1 }, { 0, 1, 5, 4 }, { 1, 2, 6, 5 }, { 4, 5, 6, 7 }, { 2, 3, 7, 6 }, { 0, 4, 7, 3 } };
int    cube[6] = { 0, 1, 2, 3, 4, 5 };
float  normal[][4] = { { 0.0, 0.0, -1.0 }, { 0.0, -1.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0 }, { -1.0, 0.0, 0.0 } };

/*----Define light source properties -------*/
float  lit_direction[] = { 0.0, -1.0, 0.0, 0.0 };
// float  lit_direction[] = { 1.0, 0.0, 0.0, 0.0 };
// float  lit_direction[] = { 0.0, 0.0, 1.0, 0.0 };
// float  lit_direction[] = { 0.0, 0.0, 0.0, 1.0 };
float  lit_diffuse[] = { 143.0 / 255.0, 188.0 / 255.0, 143.0 / 255.0, 1.0 }; // DarkSeaGreen => Deep Sea
float  lit_specular[] = { 155.0 / 255.0, 205.0 / 255.0, 155.0 / 255.0, 1.0 }; // DarkSeaGreen3

float  lit1_position[] = { -15.0, 0.0, 15.0, 1.0 };
float  lit1_diffuse[] = { 255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0, 1.0 }; // LightYellow
float  lit1_specular[] = { 238.0 / 255.0, 221.0 / 255.0, 130.0 / 255.0, 1.0 }; // LightGoldenrod

float  lit2_position[] = { 8.0, 10.2, 5.0, 1.0 };
float  lit2_direction[] = { 0.0, 0.0, 1.0 };
// float  lit2_direction[] = { 1.0, 0.0, 0.0 };
// float  lit2_direction[] = { 0.0, 1.0, 0.0 };
float  lit2_diffuse[] = { 255.0 / 255.0, 215.0 / 255.0, 0.0 / 255.0, 1.0 }; // Gold
float  lit2_specular[] = { 238.0 / 255.0, 221.0 / 255.0, 130.0 / 255.0, 1.0 }; // LightGoldenrod
float  lit2_emission[] = { 255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0, 1.0 }; // LightYellow
float  lit2_cutoff = 60.0;
float  lit2_exponent = 0.0;

float  global_ambient[] = { 119.0 / 255.0, 136.0 / 255.0, 139.0 / 255.0, 1.0 }; // SlateGray4

/*----Define material properties-----*/
float  flr_diffuse[] = { 139.0 / 255.0, 134.0 / 255.0, 130.0 / 255.0, 1.0 }; // Seashell4
float  flr_ambient[] = { 131.0 / 255.0, 139.0 / 255.0, 131.0 / 255.0, 1.0 }; // Honeydew4

// brass
float  rov_ambient[] = { 0.329412, 0.223529, 0.027451, 1.0 };
float  rov_diffuse[] = { 0.780392, 0.568627, 0.113725, 1.0 };
float  rov_specular[] = { 0.992157, 0.941176, 0.807843, 1.0 };
float  rov_shininess = 0.21794872;
float  rov_emission[] = { 255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0, 1.0 }; // LightYellow1

// black rubber
float  camera_ambient[] = { 0.02, 0.02, 0.02, 1.0 };
float  camera_diffuse[] = { 0.01, 0.01, 0.01, 1.0 };
float  camera_specular[] = { 0.4, 0.4, 0.4, 1.0 };
float  camera_shininess = 0.078125;

// black plastic
float  stone_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
float  stone_diffuse[] = { 0.01, 0.01, 0.01, 1.0 };
float  stone_specular[] = { 0.50, 0.50, 0.50, 1.0 };
float  stone_shininess = 0.25;

// bronze
float  coral_ambient[] = { 0.2125, 0.1275, 0.054, 1.0 };
float  coral_diffuse[] = { 0.714, 0.4284, 0.18144, 1.0 };
float  coral_specular[] = { 0.393548, 0.271906, 0.166721, 1.0 };
float  coral_shininess = 0.2;

// green rubber
float  grass_ambient[] = { 0.0, 0.05, 0.0, 1.0 };
float  grass_diffuse[] = { 0.4, 0.5, 0.4, 1.0 };
float  grass_specular[] = { 0.04, 0.7, 0.04, 1.0 };
float  grass_shininess = 0.078125;

// cyan rubber
float  cube_ambient[] = { 0.0, 0.05, 0.05, 1.0 };
float  cube_diffuse[] = { 0.4, 0.5, 0.5, 1.0 };
float  cube_specular[] = { 0.04, 0.7, 0.7, 1.0 };
float  cube_shininess = 0.078125;

/*-----Create image space for textures -----*/
// unsigned char  **seabed;  /* seabed textures */
unsigned char  brick[TSIZE][TSIZE][4];        /* brick wall textures */
unsigned char  checkboard[TSIZE][TSIZE][4];
// unsigned char  **seabed;
unsigned int   textName[3];                   /* declare two texture maps*/
GLuint seabed;

float  a[3], b[3];
float    mtx[16];     /* the modelview matrix */

float fog_color[] = { 0.15, 0.20, 0.20, 0.1 };

int k = 4;

/*----------------------------------------------------------
* Procedure to initialize the working environment.
*/
void  myinit()
{
	glClearColor(16.0 / 255.0, 78.0 / 255.0, 139.0 / 255.0, 0.5);
	/*Clear the Depth & Color Buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
	gluPerspective(ang, (double)width / (double)height, 1.5, 50.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);    /*Enable lighting effects */
	glEnable(GL_LIGHT0);      /*Turn on light0 */
	glEnable(GL_LIGHT1);      /*Turn on light1 */
	glEnable(GL_LIGHT2);      /*Turn on light2 */

	/*-----Define light0 : directional light from sky ---------*/
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);

	/*-----Define light1 : fixed point light under sea---------*/
	glLightfv(GL_LIGHT1, GL_POSITION, lit1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);

	/*-----Define light2 : spot light from ROV---------*/
	glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lit2_direction);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
	glLightfv(GL_LIGHT2, GL_EMISSION, lit2_emission);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);

	/*-----Define some global lighting status -----*/
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

	/*-----Enable face culling -----*/
	// glCullFace(GL_BACK);
	// glEnable(GL_CULL_FACE);

	/*-----Create Texture -----*/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(2, textName);

	make_texture_brick1();
	glBindTexture(GL_TEXTURE_2D, textName[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, brick);

	/*-----Create another texture ----*/
	make_texture_brick2();
	glBindTexture(GL_TEXTURE_2D, textName[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);

	/*-----Create Texture -----*/
	make_texture_seabed();

	/*----Set up fog conditions ---*/
	glEnable(GL_FOG);                /*enable fog fade */
	glFogi(GL_FOG_MODE, GL_EXP);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
	glFogf(GL_FOG_DENSITY, 0.05);    /*fog opacity(density)= 0.25*/
	glFogfv(GL_FOG_COLOR, fog_color); /*set the fog color */

	/*---Create quadratic objects---*/
	if (sphere == NULL) {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
	}
	if (cylind == NULL) {
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
	}
	if (disk == NULL) {
		disk = gluNewQuadric();
		gluQuadricDrawStyle(disk, GLU_FILL);
		gluQuadricNormals(disk, GLU_SMOOTH);
	}

	/*---- Compute cos(5.0) and sin(5.0) ----*/
	cv = cos(5.0*PI / 180.0);
	sv = sin(5.0*PI / 180.0);
	/*---- Copy eye position ---*/
	eye[0] = Eye[0];
	eye[1] = Eye[1];
	eye[2] = Eye[2];

	top_menu_func(k);
	// draw_scene();
}

/*-------------------------------------------------------
* Procedure to compute the a[] and b[] axes of billboard
* after eye parameters are specified.
*/
void compute_ab_axes(void)
{
	float  w0, w2;
	double len;

	/*----Get w0 and w2 from the modelview matrix mtx[] ---*/
	w0 = mtx[2]; w2 = mtx[10];

	len = sqrt(w0*w0 + w2*w2);
	/*---- Define the a and b axes for billboards ----*/
	b[0] = 0.0; b[1] = 1.0; b[2] = 0.0;
	a[0] = w2 / len; a[1] = 0.0; a[2] = -w0 / len;
}

void draw_billboard(float x, float y, float z, float w, float h)
{
	float  v0[3], v1[3], v2[3], v3[3];

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.3);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	v0[0] = x - (w / 2)* a[0];
	v0[1] = 0.0;
	v0[2] = z - (w / 2)*a[2];
	v1[0] = x + (w / 2)* a[0];
	v1[1] = 0.0;
	v1[2] = z + (w / 2)*a[2];
	v2[0] = x + (w / 2)* a[0];
	v2[1] = h;
	v2[2] = z + (w / 2)*a[2];
	v3[0] = x - (w / 2)* a[0];
	v3[1] = h;
	v3[2] = z - (w / 2)*a[2];
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(v0);
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(v1);
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(v2);
	glTexCoord2f(0.0, 1.0);
	glVertex3fv(v3);
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

void make_texture_brick1()
{
	for (int y = 0; y<12; y++){    /*----Create the first row of bricks----*/
		for (int x = 0; x<2; x++){
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (int x = 2; x<30; x++){
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (int x = 30; x<34; x++){
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (int x = 34; x<62; x++){
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (int x = 62; x<64; x++){
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}
	}

	for (int y = 12; y<16; y++)     /*--Create the cement between two breick layers */
		for (int x = 0; x<64; x++){
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

	for (int y = 16; y<28; y++){    /*--Create the 2nd layer of bricks---*/
		for (int x = 0; x<14; x++){
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (int x = 14; x<18; x++){
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (int x = 18; x<46; x++){
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (int x = 46; x<50; x++){
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (int x = 50; x<64; x++){
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}
	}

	for (int y = 28; y<32; y++) /*---Add another layer of cement----*/
		for (int x = 0; x<64; x++){
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

	for (int y = 32; y<64; y++)
		for (int x = 0; x<64; x++)
			for (int i = 0; i<4; i++) brick[y][x][i] = brick[y - 32][x][i];
}

void make_texture_brick2()
{
	int c;
	for (int i = 0; i < TSIZE; i++)
		for (int j = 0; j < TSIZE; j++){
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			checkboard[i][j][0] = c;
			checkboard[i][j][1] = c;
			checkboard[i][j][2] = c;
			checkboard[i][j][3] = 255;
		}
}

void make_texture_seabed()
{
	seabed = LoadTextureBMPWH("\seabed.bmp", 256, 256);
}

void view_func(int value)
{
	switch (value){
	case 1:
		k = 1;
		x_axis_func();
		break;
	case 2:
		k = 2;
		y_axis_func();
		break;

	case 3:
		k = 3;
		z_axis_func();
		break;

	case 4:
		k = 4;
		perspective_func();
		break;

	case 5:
		k = 5;
		view_port_func();
		break;

	default:
		break;
	}
}

void light_func(int value)
{
	switch (value){
	case 1:
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		break;
	case 2:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		break;
	case 3:
		glEnable(GL_LIGHT0);
		break;
	case 4:
		glDisable(GL_LIGHT0);
		break;
	case 5:
		glEnable(GL_LIGHT1);
		break;
	case 6:
		glDisable(GL_LIGHT1);
		break;
	case 7:
		glEnable(GL_LIGHT2);
		break;
	case 8:
		glDisable(GL_LIGHT2);
		break;
	default:
		break;
	}
}

void light_change_func(int value)
{
	switch (value){
	case 1: // Deep sea
		lit_diffuse[0] = 143.0 / 255.0; // DarkSeaGreen
		lit_diffuse[1] = 188.0 / 255.0;
		lit_diffuse[2] = 143.0 / 255.0;
		lit_diffuse[3] = 1.0;
		lit_specular[0] = 155.0 / 255.0; // DarkSeaGreen3
		lit_specular[1] = 205.0 / 255.0;
		lit_specular[2] = 155.0 / 255.0;
		lit_specular[3] = 1.0;

		glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
		break;
	case 2: // Morning
		lit_diffuse[0] = 162.0 / 255.0; // LightSteelBlue3
		lit_diffuse[1] = 181.0 / 255.0;
		lit_diffuse[2] = 205.0 / 255.0;
		lit_diffuse[3] = 1.0;
		lit_specular[0] = 180.0 / 255.0; // LightCyan3
		lit_specular[1] = 205.0 / 255.0;
		lit_specular[2] = 205.0 / 255.0;
		lit_specular[3] = 1.0;

		glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
		break;
	case 3: // Noon
		lit_diffuse[0] = 238.0 / 255.0; // LightGoldenrod
		lit_diffuse[1] = 221.0 / 255.0;
		lit_diffuse[2] = 130.0 / 255.0;
		lit_diffuse[3] = 1.0;
		lit_specular[0] = 245.0 / 255.0; // Wheat
		lit_specular[1] = 222.0 / 255.0;
		lit_specular[2] = 179.0 / 255.0;
		lit_specular[3] = 1.0;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
		break;

	case 4:
		lit1_diffuse[0] = 255.0 / 255.0; // LightYellow
		lit1_diffuse[1] = 255.0 / 255.0;
		lit1_diffuse[2] = 224.0 / 255.0;
		lit1_diffuse[3] = 1.0;
		lit1_specular[0] = 238.0 / 255.0; // LightGoldenrod
		lit1_specular[1] = 221.0 / 255.0;
		lit1_specular[2] = 130.0 / 255.0;
		lit1_specular[3] = 1.0;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);
		break;
	case 5:
		lit1_diffuse[0] = 255.0 / 255.0; // Seashell
		lit1_diffuse[1] = 245.0 / 255.0;
		lit1_diffuse[2] = 238.0 / 255.0;
		lit1_diffuse[3] = 1.0;
		lit1_specular[0] = 205.0 / 255.0; // Coral3
		lit1_specular[1] = 91.0 / 255.0;
		lit1_specular[2] = 69.0 / 255.0;
		lit1_specular[3] = 1.0;

		glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);
		break;
	case 6:
		lit1_diffuse[0] = 205.0 / 255.0; // Thistle3
		lit1_diffuse[1] = 181.0 / 255.0;
		lit1_diffuse[2] = 205.0 / 255.0;
		lit1_diffuse[3] = 1.0;
		lit1_specular[0] = 137.0 / 255.0; // MediumPurple3
		lit1_specular[1] = 104.0 / 255.0;
		lit1_specular[2] = 205.0 / 255.0;
		lit1_specular[3] = 1.0;
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);
		break;

	default:
		break;
	}
}

void fog_change_func(int value)
{
	switch (value){
	case 1:
		fog_color[0] = 0.15;
		fog_color[1] = 0.20;
		fog_color[2] = 0.20;
		fog_color[3] = 0.1;
		glFogfv(GL_FOG_COLOR, fog_color); /*set the fog color */
		break;
	case 2: // DarkSlateGray4
		fog_color[0] = 82.0 / 255.0;
		fog_color[1] = 139.0 / 255.0;
		fog_color[2] = 139.0 / 255.0;
		fog_color[3] = 0.1;
		glFogfv(GL_FOG_COLOR, fog_color); /*set the fog color */
		break;
	case 3:
		glEnable(GL_FOG);
		break;
	case 4:
		glDisable(GL_FOG);
		break;
	case 5:
		glFogi(GL_FOG_MODE, GL_EXP);
		break;
	case 6:
		glFogi(GL_FOG_MODE, GL_EXP2);
		break;
	case 7:
		glFogi(GL_FOG_MODE, GL_LINEAR);
		break;
	case 8:
		glFogf(GL_FOG_DENSITY, 0.05);
		break;
	case 9:
		glFogf(GL_FOG_DENSITY, 0.15);
		break;
	case 10:
		glFogf(GL_FOG_DENSITY, 0.25);
		break;
	default:
		break;
	}
}

/*---------------------------------------------------------------
* Callback function for top menu. Do nothing.
*/
void top_menu_func(int value)
{
	switch (value){
	case 1:
		k = 1;
		x_axis_func();
		break;
	case 2:
		k = 2;
		y_axis_func();
		break;

	case 3:
		k = 3;
		z_axis_func();
		break;

	case 4:
		k = 4;
		perspective_func();
		break;

	case 5:
		k = 5;
		view_port_func();
		break;

	default:
		k = 4;
		perspective_func();
		break;
	}
}

void x_axis_func()
{
	glViewport(0, 0, width, height);
	make_view(1);
	make_projection(5);
	draw_scene();
}

void y_axis_func()
{
	glViewport(0, 0, width, height);
	make_view(2);
	make_projection(5);
	draw_scene();
}

void z_axis_func()
{
	glViewport(0, 0, width, height);
	make_view(3);
	make_projection(5);
	draw_scene();
}

void perspective_func()
{
	glViewport(0, 0, width, height);
	make_view(0);
	make_projection(5);
	draw_scene();
}

void view_port_func()
{
	make_view(0);
	make_projection(5);
	glViewport(width / 2, 0, width / 2, height / 2);
	draw_scene();

	make_view(1);
	make_projection(0);
	glViewport(0, height / 2, width / 2, height / 2);
	draw_scene();
	make_view(1);
	draw_view();
	draw_view_volume();

	make_view(2);
	glViewport(width / 2, height / 2, width / 2, height / 2);
	draw_scene();
	make_view(2);
	draw_view();
	draw_view_volume();

	make_view(3);
	glViewport(0, 0, width / 2, height / 2);
	draw_scene();
	make_view(3);
	draw_view();
	draw_view_volume();
}

void draw_view_volume()
{
	double Hnear = 2 * tan((ang*PI / 180) / 2.0) * 1.5;
	double Wnear = Hnear * ((double)width / (double)height);
	double Hfar = 2 * tan((ang*PI / 180) / 2) * 50.0;
	double Wfar = Hfar * ((double)width / (double)height);
	double fc[3], ftl[3], ftr[3], fbl[3], fbr[3];
	double nc[3], ntl[3], ntr[3], nbl[3], nbr[3];
	fc[0] = eye[0] - u[2][0] * 50.0;
	fc[1] = eye[1] - u[2][1] * 50.0;
	fc[2] = eye[2] - u[2][2] * 50.0;
	ftl[0] = fc[0] + (u[1][0] * Hfar / 2) - (u[0][0] * Wfar / 2);
	ftl[1] = fc[1] + (u[1][1] * Hfar / 2) - (u[0][1] * Wfar / 2);
	ftl[2] = fc[2] + (u[1][2] * Hfar / 2) - (u[0][2] * Wfar / 2);
	ftr[0] = fc[0] + (u[1][0] * Hfar / 2) + (u[0][0] * Wfar / 2);
	ftr[1] = fc[1] + (u[1][1] * Hfar / 2) + (u[0][1] * Wfar / 2);
	ftr[2] = fc[2] + (u[1][2] * Hfar / 2) + (u[0][2] * Wfar / 2);
	fbl[0] = fc[0] - (u[1][0] * Hfar / 2) - (u[0][0] * Wfar / 2);
	fbl[1] = fc[1] - (u[1][1] * Hfar / 2) - (u[0][1] * Wfar / 2);
	fbl[2] = fc[2] - (u[1][2] * Hfar / 2) - (u[0][2] * Wfar / 2);
	fbr[0] = fc[0] - (u[1][0] * Hfar / 2) + (u[0][0] * Wfar / 2);
	fbr[1] = fc[1] - (u[1][1] * Hfar / 2) + (u[0][1] * Wfar / 2);
	fbr[2] = fc[2] - (u[1][2] * Hfar / 2) + (u[0][2] * Wfar / 2);

	nc[0] = eye[0] - u[2][0] * 1.5;
	nc[1] = eye[1] - u[2][1] * 1.5;
	nc[2] = eye[2] - u[2][2] * 1.5;
	ntl[0] = nc[0] + (u[1][0] * Hnear / 2) - (u[0][0] * Wnear / 2);
	ntl[1] = nc[1] + (u[1][1] * Hnear / 2) - (u[0][1] * Wnear / 2);
	ntl[2] = nc[2] + (u[1][2] * Hnear / 2) - (u[0][2] * Wnear / 2);
	ntr[0] = nc[0] + (u[1][0] * Hnear / 2) + (u[0][0] * Wnear / 2);
	ntr[1] = nc[1] + (u[1][1] * Hnear / 2) + (u[0][1] * Wnear / 2);
	ntr[2] = nc[2] + (u[1][2] * Hnear / 2) + (u[0][2] * Wnear / 2);
	nbl[0] = nc[0] - (u[1][0] * Hnear / 2) - (u[0][0] * Wnear / 2);
	nbl[1] = nc[1] - (u[1][1] * Hnear / 2) - (u[0][1] * Wnear / 2);
	nbl[2] = nc[2] - (u[1][2] * Hnear / 2) - (u[0][2] * Wnear / 2);
	nbr[0] = nc[0] - (u[1][0] * Hnear / 2) + (u[0][0] * Wnear / 2);
	nbr[1] = nc[1] - (u[1][1] * Hnear / 2) + (u[0][1] * Wnear / 2);
	nbr[2] = nc[2] - (u[1][2] * Hnear / 2) + (u[0][2] * Wnear / 2);

	glColor4f(240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 0.5);
	glBegin(GL_LINE_STRIP);
	glVertex3f(ntl[0], ntl[1], ntl[2]);
	glVertex3f(ftl[0], ftl[1], ftl[2]);
	glVertex3f(fbl[0], fbl[1], fbl[2]);
	glVertex3f(nbl[0], nbl[1], nbl[2]);
	glVertex3f(ntl[0], ntl[1], ntl[2]);
	glEnd();

	glColor4f(240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 0.5);
	glBegin(GL_LINE_STRIP);
	glVertex3f(ntr[0], ntr[1], ntr[2]);
	glVertex3f(ftr[0], ftr[1], ftr[2]);
	glVertex3f(fbr[0], fbr[1], fbr[2]);
	glVertex3f(nbr[0], nbr[1], nbr[2]);
	glVertex3f(ntr[0], ntr[1], ntr[2]);
	glEnd();

	glColor4f(240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 0.5);
	glBegin(GL_LINE_STRIP);
	glVertex3f(nbl[0], nbl[1], nbl[2]);
	glVertex3f(fbl[0], fbl[1], fbl[2]);
	glVertex3f(fbr[0], fbr[1], fbr[2]);
	glVertex3f(nbr[0], nbr[1], nbr[2]);
	glVertex3f(nbl[0], nbl[1], nbl[2]);
	glEnd();

	glColor4f(240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 0.5);
	glBegin(GL_LINE_STRIP);
	glVertex3f(ntl[0], ntl[1], ntl[2]);
	glVertex3f(ftl[0], ftl[1], ftl[2]);
	glVertex3f(ftr[0], ftr[1], ftr[2]);
	glVertex3f(ntr[0], ntr[1], ntr[2]);
	glVertex3f(ntl[0], ntl[1], ntl[2]);
	glEnd();

	glColor4f(240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 0.5);
	glBegin(GL_LINE_STRIP);
	glVertex3f(ntl[0], ntl[1], ntl[2]);
	glVertex3f(ntr[0], ntr[1], ntr[2]);
	glVertex3f(nbr[0], nbr[1], nbr[2]);
	glVertex3f(nbl[0], nbl[1], nbl[2]);
	glVertex3f(ntl[0], ntl[1], ntl[2]);
	glEnd();
}

/*---------------------------------------------------------
* Procedure to draw the floor.
*/
void draw_floor()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, seabed);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, flr_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, flr_ambient);
	glEnable(GL_COLOR_MATERIAL);

	glNormal3f(0.0, 0.0, 1.0);

	float range = 1.0;
	for (int i = 0; i<10; i++)
		for (int j = 0; j<10; j++) {
			glColor4f(96.0 / 255.0, 123.0 / 255.0, 139.0 / 255.0, 0.5);
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0);
			glVertex3f((i - 5.0)*10.0, -2.2, (j - 5.0)*10.0);
			glTexCoord2f(0.0, range);
			glVertex3f((i - 5.0)*10.0, -2.2, (j - 4.0)*10.0);
			glTexCoord2f(range, range);
			glVertex3f((i - 4.0)*10.0, -2.2, (j - 4.0)*10.0);
			glTexCoord2f(range, 0.0);
			glVertex3f((i - 4.0)*10.0, -2.2, (j - 5.0)*10.0);
			glEnd();
		}
	glDisable(GL_COLOR_MATERIAL);

	glDisable(GL_TEXTURE_2D);
}

/*-------------------------------------------------------
* Procedure to draw three axes and the orign
*/
void draw_axes()
{
	/*----Draw a white sphere to represent the original-----*/
	glColor3f(1.0, 1.0, 1.0);

	gluSphere(sphere, 1.0, 12, 12);
	// name, radius=2.0, composing of 12 slices, composing of 12 stacks

	/* Draw Z axis  */
	glColor3f(230.0 / 255.0, 230.0 / 255.0, 250.0 / 255.0);  // color: lavender
	gluCylinder(cylind, 0.5, 0.5, 10.0, 12, 3);
	// name, radius of top circle, radius of bottom circle, height of the cylinder, use 12-side polygon approximating circle, Divide it into 3 sections

	/* Draw Y axis */
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
	glColor3f(255.0 / 255.0, 193.0 / 255.0, 193.0 / 255.0);  // color: RosyBrown1
	gluCylinder(cylind, 0.5, 0.5, 10.0, 12, 3);
	// name, radius of top circle, radius of bottom circle, height of the cylinder, use 12-side polygon approximating circle, Divide it into 3 sections
	glPopMatrix();

	/* Draw X axis */
	glColor3f(255.0 / 255.0, 246.0 / 255.0, 143.0 / 255.0); // color: Khaki1
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
	gluCylinder(cylind, 0.5, 0.5, 10.0, 12, 3);
	// name, radius of top circle, radius of bottom circle, height of the cylinder, use 12-side polygon approximating circle, Divide it into 3 sections
	glPopMatrix();
	/*-- Restore the original modelview matrix --*/
	glPopMatrix();
}

void draw_obstacle()
{
	draw_stone();

	draw_coral();

	draw_grass1();
	draw_grass2();
	draw_grass3();

	draw_cube1();
	draw_cube2();
}

void draw_stone()
{
	glPushMatrix();
	glRotatef(180, 1.0, 0.0, 0.0);
	glTranslatef(-15.0, 0.0, -15.0);
	glColor3f(105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0);  // color: dim gray

	glMaterialfv(GL_FRONT, GL_AMBIENT, stone_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, stone_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, stone_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, stone_shininess);
	glDisable(GL_TEXTURE_2D);

	gluSphere(sphere, 3.0, 64, 64);
	glPopMatrix();
}

void draw_coral()
{
	glPushMatrix();
	glColor3f(240.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);  // color: light coral
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(-10.0, 0.0, -1.0);
	glRotatef(-65.0, 0.0, 1.0, 0.0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, coral_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, coral_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, coral_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, coral_shininess);
	glDisable(GL_TEXTURE_2D);

	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(45, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(45, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(45, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(255.0 / 255.0, 127.0 / 255.0, 80.0 / 255.0);  // color: coral
	glRotatef(-30.0, 1.0, 0.0, 0.0);
	glTranslatef(-10.0, 0.0, -1.0);
	glRotatef(-20.0, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(40.0, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(30.0, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(255.0 / 255.0, 127.0 / 255.0, 80.0 / 255.0);  // color: dark green
	glRotatef(-120.0, 1.0, 0.0, 0.0);
	glTranslatef(-10.0, 0.0, -1.0);
	glRotatef(-20.0, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(40.0, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(30.0, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glPopMatrix();
}

void draw_grass1()
{
	glColor3f(34.0 / 255.0, 139.0 / 255.0, 34.0 / 255.0);  // color: forest green

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, grass_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grass_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grass_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, grass_shininess);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(120, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();

}

void draw_grass2()
{
	glColor3f(50.0 / 255.0, 205.0 / 255.0, 50.0 / 255.0);  // color: lime green

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(30, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(30, 0.0, 1.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(30, 0.0, 1.0, 0.0);
	glRotatef(120, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();
}

void draw_grass3()
{
	glColor3f(85.0 / 255.0, 107.0 / 255.0, 47.0 / 255.0);  // color: dark olive green

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(-30, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(-30, 0.0, 1.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, -2.0, 0.0);
	glRotatef(-30, 0.0, 1.0, 0.0);
	glRotatef(120, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(3.0, 1.0, 0.0);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(7.0, 1.0, 0.0);
	glVertex3f(9.0, 4.0, 0.0);
	glVertex3f(6.0, 2.0, 0.0);
	glEnd();
	glPopMatrix();
}

void draw_cube1()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, textName[0]);

	glPushMatrix();
	glTranslatef(10.0, -2.0, 15.0);
	glScalef(2.0, 2.0, 2.0);
	glColor3f(131.0 / 255.0, 139.0 / 255.0, 139.0 / 255.0);  // color: 	Azure4

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glMaterialfv(GL_FRONT, GL_AMBIENT, cube_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cube_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, cube_shininess);
	glEnable(GL_TEXTURE_2D);

	float range = 1.0;
	for (int i = 0; i < 6; i++){     /* draw the six faces one by one */
		glNormal3fv(normal[i]);
		glBegin(GL_POLYGON);  /* Draw the face */
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(points[face[i][0]]);
		glTexCoord2f(0.0, range);
		glVertex3fv(points[face[i][1]]);
		glTexCoord2f(range, range);
		glVertex3fv(points[face[i][2]]);
		glTexCoord2f(range, 0.0);
		glVertex3fv(points[face[i][3]]);
		glEnd();
	}
	glPopMatrix();
}

void draw_cube2()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, textName[1]);

	glPushMatrix();
	glTranslatef(20.0, -2.0, 15.0);
	glScalef(2.0, 2.0, 2.0);
	glColor3f(131.0 / 255.0, 139.0 / 255.0, 139.0 / 255.0);  // color: 	Azure4

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glMaterialfv(GL_FRONT, GL_AMBIENT, cube_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cube_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, cube_shininess);
	glEnable(GL_TEXTURE_2D);

	float range = 1.0;
	for (int i = 0; i < 6; i++){     /* draw the six faces one by one */
		glNormal3fv(normal[i]);
		glBegin(GL_POLYGON);  /* Draw the face */
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(points[face[i][0]]);
		glTexCoord2f(0.0, range);
		glVertex3fv(points[face[i][1]]);
		glTexCoord2f(range, range);
		glVertex3fv(points[face[i][2]]);
		glTexCoord2f(range, 0.0);
		glVertex3fv(points[face[i][3]]);
		glEnd();
	}
	glPopMatrix();
}

void draw_ROV()
{
	glTranslatef(position[0], position[1], position[2]);
	glRotatef(self_ang, 0.0, 1.0, 0.0);

	draw_body(); // Draw the body which is a ellipsoid
	draw_arm();
	draw_ROV_light();

	draw_top_blade(); // Draw propeller
	draw_back_blade(); // Draw propeller

	draw_camera();

	draw_wheel();
}

void draw_body()
{
	glPushMatrix();
	glScalef(10.0, 3.0, 3.5);

	int uiSlices = 64, uiStacks = 64;

	float tStep = (PI) / (float)uiSlices;
	float sStep = (PI) / (float)uiStacks;

	glMaterialfv(GL_FRONT, GL_AMBIENT, rov_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, rov_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, rov_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, rov_shininess);
	glDisable(GL_TEXTURE_2D);

	for (float t = -PI / 2; t <= (PI / 2) + 0.0001; t += tStep)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(255.0 / 255.0, 193.0 / 255.0, 37.0 / 255.0);  // color: Goldenrod1
		for (float s = -PI; s <= PI + .0001; s += sStep)
		{
			glVertex3f(cos(t) * cos(s), cos(t) * sin(s), sin(t));
			glVertex3f(cos(t + tStep) * cos(s), cos(t + tStep) * sin(s), sin(t + tStep));
		}
		glEnd();
	}
	glPopMatrix();
}

void draw_arm()
{
	/*------Draw left arm ----*/
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.7);
	glColor3f(255.0 / 255.0, 185.0 / 255.0, 15.0 / 255.0);  // color: DarkGoldenrod1
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(20, 1.0, 0.0, 0.0);
	glTranslatef(-6.7, 0.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 5.0, 12, 3);
	glTranslatef(0.0, 0.0, 5.0);
	gluSphere(sphere, 1.0, 12, 12);
	glPopMatrix();

	/*------Draw right arm ----*/
	glPushMatrix();
	glRotatef(180, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1.7);
	glColor3f(255.0 / 255.0, 185.0 / 255.0, 15.0 / 255.0);  // color: DarkGoldenrod1
	gluCylinder(cylind, 0.5, 0.5, 7.0, 12, 3);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-20, 1.0, 0.0, 0.0);
	glTranslatef(-6.7, 0.0, 0.0);
	gluCylinder(cylind, 0.5, 0.5, 5.0, 12, 3);
	glTranslatef(0.0, 0.0, 5.0);
	gluSphere(sphere, 1.0, 12, 12);
	glPopMatrix();
}

void draw_ROV_light()
{
	glPushMatrix();
	glTranslatef(10.0, 0.0, 0.0);
	glColor3f(205.0 / 255.0, 205.0 / 255.0, 180.0 / 255.0);  // color: 	LightYellow3
	gluSphere(sphere, 1.0, 12, 12);
	glPopMatrix();
}

void draw_top_blade()
{
	glColor3f(255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0);

	/* draw the first blade */
	glPushMatrix();
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(blade_top_ang, 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glVertex3f(-1.0, 2.0, 0.0);
	glEnd();

	glPopMatrix();

	/* draw 2nd blade */
	glPushMatrix();
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef((blade_top_ang + 120.0), 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glVertex3f(-1.0, 2.0, 0.0);
	glEnd();

	glPopMatrix();

	/* draw 3rd blade */
	glPushMatrix();
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef((blade_top_ang + 240.0), 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glVertex3f(-1.0, 2.0, 0.0);
	glEnd();

	glPopMatrix();
}

void draw_back_blade()
{
	glColor3f(255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0);

	/* draw the first blade */
	glPushMatrix();
	glTranslatef(-10.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(blade_back_ang, 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glVertex3f(-1.0, 2.0, 0.0);
	glEnd();

	glPopMatrix();

	/* draw 2nd blade */
	glPushMatrix();
	glTranslatef(-10.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef((blade_back_ang + 120.0), 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glVertex3f(-1.0, 2.0, 0.0);
	glEnd();

	glPopMatrix();

	/* draw 3rd blade */
	glPushMatrix();
	glTranslatef(-10.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef((blade_back_ang + 240.0), 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glVertex3f(-1.0, 2.0, 0.0);
	glEnd();

	glPopMatrix();

	// glutSwapBuffers();
	// glFlush(); /*--Display the results----*/
}

void draw_camera()
{
	/* Vertices of the box */
	float  points[][3] = { { -1.0, -1.0, -1.0 }, { 1.0, -1.0, -1.0 }, { 1.0, 1.0, -1.0 }, { -1.0, 1.0, -1.0 },
	{ -1.0, -1.0, 1.0 }, { 1.0, -1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { -1.0, 1.0, 1.0 } };
	/* face of box, each face composing of 4 vertices */
	int    face[][4] = { { 0, 3, 2, 1 }, { 0, 1, 5, 4 }, { 1, 2, 6, 5 },
	{ 4, 5, 6, 7 }, { 2, 3, 7, 6 }, { 0, 4, 7, 3 } };

	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(5.0, 0.0, 1.5);
	glColor3f(255.0 / 255.0, 185.0 / 255.0, 15.0 / 255.0);  // color: DarkGoldenrod1

	glMaterialfv(GL_FRONT, GL_AMBIENT, camera_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, camera_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, camera_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, camera_shininess);
	glDisable(GL_TEXTURE_2D);

	gluCylinder(cylind, 0.5, 0.5, 6.0, 8, 3);

	glTranslatef(0.0, 0.0, 6.0);

	for (int i = 0; i<6; i++) {
		glColor3f(28.0 / 255.0, 28.0 / 255.0, 28.0 / 255.0);
		glBegin(GL_POLYGON);  /* Draw the face */
		glVertex3fv(points[face[i][0]]);
		glVertex3fv(points[face[i][1]]);
		glVertex3fv(points[face[i][2]]);
		glVertex3fv(points[face[i][3]]);
		glEnd();
	}

	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 1.0);
	glColor3f(255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0);
	for (float i = 0.0; i <= 0.5; i += 0.001){
		gluCylinder(cylind, 0.5, i, 0.5, 32, 3);
		// name, radius of top circle, radius of bottom circle, height of the cylinder, use 12-side polygon approximating circle, Divide it into 3 sections
	}

	glPopMatrix();
}

void draw_wheel()
{
	/*------Draw front wheels ----*/
	glColor3f(156.0 / 255.0, 156.0 / 255.0, 156.0 / 255.0);
	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(4.5, -2.5, 2.0); /* Go to left wheel position */
	glutSolidTorus(0.3, 0.7, 24, 12);
	// inner radius, outer radius, divided into 24 segments, 12 rings
	glPopMatrix();

	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(4.5, -2.5, -2.0);/* Go to right wheel position */
	glutSolidTorus(0.3, 0.7, 24, 12);
	// inner radius, outer radius, divided into 24 segments, 12 rings
	glPopMatrix();

	/*------Draw back wheels ----*/
	glColor3f(130.0 / 255.0, 130.0 / 255.0, 130.0 / 255.0);
	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(-4.5, -2.5, 2.0); /* Go to left wheel position */
	glutSolidTorus(0.3, 0.7, 24, 12);
	// inner radius, outer radius, divided into 24 segments, 12 rings
	glPopMatrix();

	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(-4.5, -2.5, -2.0);/* Go to right wheel position */
	glutSolidTorus(0.3, 0.7, 24, 12);
	// inner radius, outer radius, divided into 24 segments, 12 rings
	glPopMatrix();
}

/*-------------------------------------------------------
* Make viewing matrix
*/
void make_view(int x)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (x){
	case 0:
		gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
		break;

	case 1: /* X direction parallel viewing */
		gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;

	case 2: /* Y direction parallel viewing */
		gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
		break;

	case 3: /* Z direction parallel viewing */
		gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;

	default:
		break;
	}

}

/*------------------------------------------------------
* Procedure to make projection matrix
*/
void make_projection(int x)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (x == 5){
		gluPerspective(ang, (double)width / (double)height, 1.5, 50.0);
	}
	else{
		if (width > height){
			glOrtho(-40.0, 40.0, -40.0*(float)height / (float)width, 40.0*(float)height / (float)width, -0.0, 100.0);
		}
		else{
			glOrtho(-40.0*(float)width / (float)height, 40.0*(float)width / (float)height, -40.0, 40.0, -0.0, 100.0);
		}
	}
	glMatrixMode(GL_MODELVIEW);
}


/*---------------------------------------------------------
* Procedure to draw view volume, eye position, focus ,...
* for perspective projection
*/
void draw_view()
{
	glMatrixMode(GL_MODELVIEW);

	/*----Draw Eye position-----*/
	glPushMatrix();
	glTranslatef(eye[0], eye[1], eye[2]);
	glColor3f(0.0, 1.0, 0.0);
	glutWireSphere(1.0, 10, 10);
	glPopMatrix();

	/*----Draw eye coord. axes -----*/
	glColor3f(1.0, 1.0, 0.0);           /* Draw Xe */
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[0][0], eye[1] + 20.0*u[0][1], eye[2] + 20.0*u[0][2]);
	glEnd();

	glColor3f(1.0, 0.0, 1.0);          /* Draw Ye */
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[1][0], eye[1] + 20.0*u[1][1], eye[2] + 20.0*u[1][2]);
	glEnd();

	glColor3f(0.0, 1.0, 1.0);          /* Draw Ze */
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[2][0], eye[1] + 20.0*u[2][1], eye[2] + 20.0*u[2][2]);
	glEnd();
}


/*--------------------------------------------------------
* Display callback procedure to draw the scene
*/
void display()
{
	/*Clear previous frame and the depth buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glFogf(GL_FOG_START, position[2]);       /*Setup two ends for GL_LINEAR*/
	glFogf(GL_FOG_END, position[2] + 10);

	top_menu_func(k);

	glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, position);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lit2_specular);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);

	// glEnable(GL_TEXTURE_2D);

	/*-------Swap the back buffer to the front --------*/
	glutSwapBuffers();
	glFlush(); /*--Display the results----*/
}

/*-------------------------------------------------------
* This procedure draw the car
*/
void draw_scene()
{
	draw_floor();

	draw_axes();

	draw_obstacle();

	draw_ROV();
}

void idle_func()
{
	blade_back_ang += 1.0;
	if (blade_back_ang > 360.0) blade_back_ang -= 360.0;
	blade_top_ang += 1.0;
	if (blade_top_ang > 360.0) blade_top_ang -= 360.0;
	display(); /* show the scene again */
}

/*--------------------------------------------------
* Reshape callback function which defines the size
* of the main window when a reshape event occurrs.
*/
void my_reshape(int w, int h)
{
	width = w;
	height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (double)w / (double)h, 1.5, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/*--------------------------------------------------
* Keyboard callback func. When a 'q' key is pressed,
* the program is aborted.
*/
void my_quit(unsigned char key, int ix, int iy)
{
	int    i;
	float  x[3], y[3], z[3];

	if (key == 'Q' || key == 'q') exit(0);

	if (key == ' ') {                              /* move car ahead, SPACE key */
		if (position[0] <= -2.5 && position[2] >= 5){
		}
		else if (position[0] <= 6.5 && position[2] <= 2){
		}
		else{
			blade_back_ang += 10.0;
			if (blade_back_ang > 360.0){
				blade_back_ang -= 360.0;
			}
			position[0] += Step*cos(self_ang*PI / 180.0);
			position[2] -= Step*sin(self_ang*PI / 180.0);
		}
	}
	else if (key == 'b') {                        /* move car back b-key */
		blade_back_ang -= 10.0;
		if (blade_back_ang < 0.0){
			blade_back_ang += 360.0;
		}
		position[0] -= Step*cos(self_ang*PI / 180.0);
		position[2] += Step*sin(self_ang*PI / 180.0);
	}
	else if (key == 'T') {                        /* make a left turn */
		self_ang += 10.0;
		if (self_ang > 360.0) self_ang -= 360.0;
	}
	else if (key == 't') {                        /* make a right turn */
		self_ang += -10.0;
		if (self_ang < 0.0) self_ang += 360.0;
	}
	else if (key == 'M') {                        /* move car ascend M-key */
		if (self_ang < 180.0){
			blade_top_ang += 10.0;
			if (blade_top_ang > 360.0){
				blade_top_ang -= 360.0;
			}
			position[1] -= Step*sin(self_ang*PI / 180.0);
		}
		else{
			if (position[1] >= 2) {
				blade_top_ang -= 10.0;
				if (blade_top_ang < 0.0){
					blade_top_ang += 360.0;
				}
				position[1] += Step*sin(self_ang*PI / 180.0);
			}
		}
	}
	else if (key == 'm') {                        /* move car descend m-key */
		if (self_ang < 180.0){
			if (position[1] >= 2) {
				blade_top_ang -= 10.0;
				if (blade_top_ang < 0.0){
					blade_top_ang += 360.0;
				}
				position[1] += Step*sin(self_ang*PI / 180.0);
			}
		}
		else{
			blade_top_ang += 10.0;
			if (blade_top_ang > 360.0){
				blade_top_ang -= 360.0;
			}
			position[1] -= Step*sin(self_ang*PI / 180.0);
		}
	}

	/*------transform the EYE coordinate system ------*/
	else if (key == 'u') {
		eyeDy += 0.5;       /* move up */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[1][i];
	}
	else if (key == 'd') {
		eyeDy += -0.5;       /* move down */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[1][i];
	}
	else if (key == 'w') {
		eyeDx += -0.5;       /* move left */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[0][i];
	}
	else if (key == 'e') {
		eyeDx += 0.5;        /* move right */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[0][i];
	}
	else if (key == 'n') {
		eyeDz += 0.5;        /* zoom in */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[2][i];
	}
	else if (key == 'f') {
		eyeDz += -0.5;       /* zoom out */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[2][i];
	}
	else if (key == 'p') {             /* pitching */
		eyeAngx += 5.0;
		if (eyeAngx > 360.0) eyeAngx -= 360.0;
		y[0] = u[1][0] * cv - u[2][0] * sv;
		y[1] = u[1][1] * cv - u[2][1] * sv;
		y[2] = u[1][2] * cv - u[2][2] * sv;

		z[0] = u[2][0] * cv + u[1][0] * sv;
		z[1] = u[2][1] * cv + u[1][1] * sv;
		z[2] = u[2][2] * cv + u[1][2] * sv;

		for (i = 0; i<3; i++) {
			u[1][i] = y[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'P') {
		eyeAngx += -5.0;
		if (eyeAngx<0.0) eyeAngx += 360.0;
		y[0] = u[1][0] * cv + u[2][0] * sv;
		y[1] = u[1][1] * cv + u[2][1] * sv;
		y[2] = u[1][2] * cv + u[2][2] * sv;

		z[0] = u[2][0] * cv - u[1][0] * sv;
		z[1] = u[2][1] * cv - u[1][1] * sv;
		z[2] = u[2][2] * cv - u[1][2] * sv;

		for (i = 0; i<3; i++) {
			u[1][i] = y[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'h') {            /* heading */
		eyeAngy += 5.0;
		if (eyeAngy>360.0) eyeAngy -= 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] - sv*u[2][i];
			z[i] = sv*u[0][i] + cv*u[2][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'H') {
		eyeAngy += -5.0;
		if (eyeAngy<0.0) eyeAngy += 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] + sv*u[2][i];
			z[i] = -sv*u[0][i] + cv*u[2][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'r') {            /* rolling */
		eyeAngz += 5.0;
		if (eyeAngz>360.0) eyeAngz -= 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] - sv*u[1][i];
			y[i] = sv*u[0][i] + cv*u[1][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[1][i] = y[i];
		}
	}
	else if (key == 'R') {
		eyeAngz += -5.0;
		if (eyeAngz<0.0) eyeAngz += 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] + sv*u[1][i];
			y[i] = -sv*u[0][i] + cv*u[1][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[1][i] = y[i];
		}
	}
	else if (key == 'L') {
		ang += 1.0;
	}
	else if (key == 'l'){
		ang -= 1.0;
	}

	else if (key == '+'){
		lit2_cutoff += 1.0;
		if (lit2_cutoff > 180.0)
			lit2_cutoff -= 180.0;
	}
	else if (key == '-'){
		lit2_cutoff -= 1.0;
		if (lit2_cutoff < 0.0)
			lit2_cutoff += 180.0;
	}
	else if (key == '>'){
		lit2_exponent += 1.0;
		if (lit2_exponent > 128.0)
			lit2_exponent -= 128.0;
	}
	else if (key == '<'){
		lit2_exponent -= 1.0;
		if (lit2_exponent < 0.0)
			lit2_exponent += 128.0;
	}
	display();
}


/*---------------------------------------------------
* Main procedure which defines the graphics environment,
* the operation style, and the callback func's.
*/
void main(int argc, char **argv)
{
	/*-----Initialize the GLUT environment-------*/
	glutInit(&argc, argv);

	/*-----Depth buffer is used, be careful !!!----*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("00557149-Remotely Operated Vehicle");

	myinit();      /*---Initialize other state varibales----*/

	view_m = glutCreateMenu(view_func);
	glutAddMenuEntry("X-axis", 1);
	glutAddMenuEntry("Y-axis", 2);
	glutAddMenuEntry("Z-axis", 3);
	glutAddMenuEntry("Perspective", 4);
	glutAddMenuEntry("4 view-ports", 5);

	light_oc_m = glutCreateMenu(light_func);
	glutAddMenuEntry("Open light", 1);
	glutAddMenuEntry("Close light", 2);
	glutAddMenuEntry("Open Directional", 3);
	glutAddMenuEntry("Close Directional", 4);
	glutAddMenuEntry("Open Point", 5);
	glutAddMenuEntry("Close Point", 6);
	glutAddMenuEntry("Open Sopt", 7);
	glutAddMenuEntry("Close Sopt", 8);

	light_chg_m = glutCreateMenu(light_change_func);
	glutAddMenuEntry("Dir: Deep sea", 1);
	glutAddMenuEntry("Dir: Morning", 2);
	glutAddMenuEntry("Dir: Noon", 3);
	glutAddMenuEntry("Pnt: LightYellow", 4);
	glutAddMenuEntry("Pnt: Coral", 5);
	glutAddMenuEntry("Pnt: MediumPurple", 6);

	fog_chg_m = glutCreateMenu(fog_change_func);
	glutAddMenuEntry("Color: Original", 1);
	glutAddMenuEntry("Color: DarkSlateGray", 2);
	glutAddMenuEntry("Mode: Open", 3);
	glutAddMenuEntry("Mode: Close", 4);
	glutAddMenuEntry("Mode: EXP", 5);
	glutAddMenuEntry("Mode: EXP2", 6);
	glutAddMenuEntry("Mode: Linear", 7);
	glutAddMenuEntry("Opacity: 0.05", 8);
	glutAddMenuEntry("Opacity: 0.15", 9);
	glutAddMenuEntry("Opacity: 0.25", 10);

	top_m = glutCreateMenu(top_menu_func);/* Create top menu */
	glutAddSubMenu("View port", view_m);
	glutAddSubMenu("Light:open/close", light_oc_m);
	glutAddSubMenu("Light:change color", light_chg_m);
	glutAddSubMenu("Fog:change", fog_chg_m);

	glutAttachMenu(GLUT_RIGHT_BUTTON);    /* associate top-menu with right but*/

	/*----Associate callback func's whith events------*/
	glutDisplayFunc(display);
	glutIdleFunc(idle_func);
	glutReshapeFunc(my_reshape);
	glutKeyboardFunc(my_quit);

	glutMainLoop();
}