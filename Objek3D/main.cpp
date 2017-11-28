#include <GL\glut.h>
#include <math.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

//deklarasi tipe data primitif
float sudut = 0.0;
float tes;
//deklarasi tipe data object

typedef struct {
	float r;
	float g;
	float b;
} color_t;

typedef struct {
	float x;
	float y;
	color_t col;
} point2D_t;

typedef struct {
	float vector[3];
} vector2D_t;

typedef struct {
	float matrix[3][3];
} matrix2D_t;

typedef struct {
	float x;
	float y;
	float z;
	color_t col;
} point3D_t;

typedef struct {
	float m[4][4];
} matrix3D_t;

typedef struct {
	float v[4];
} vector3D_t;

typedef struct {
	int numofVertices;
	long int pnt[34];
} face_t;

typedef struct {
	int numofVertices;
	point3D_t pnt[700];
	int numofFaces;
	face_t fc[1200];
} object3D_t;

//operasi vektor
//untuk 2D
vector2D_t point2vector(point2D_t pnt) {
	vector2D_t vec;
	vec.vector[0] = pnt.x;
	vec.vector[1] = pnt.y;
	vec.vector[2] = 1;
	return vec;
}

point2D_t vector2point(vector2D_t vec) {
	point2D_t pnt;
	pnt.x = vec.vector[0];
	pnt.y = vec.vector[1];
	return pnt;
}

point2D_t vector2point2D(vector3D_t vec) {
	point2D_t pnt;
	pnt.x = vec.v[0];
	pnt.y = vec.v[1];
	return pnt;
}

matrix2D_t createIdentity() {
	matrix2D_t mat;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat.matrix[i][j] = 0.;
		}
		mat.matrix[i][i] = 1.;
	}
	return mat;
}

matrix2D_t translationMTX(float dx, float dy) {
	matrix2D_t trans = createIdentity();
	trans.matrix[0][2] = dx;
	trans.matrix[1][2] = dy;
	return trans;
}

matrix2D_t scalingMTX(float mx, float my) {
	matrix2D_t scale = createIdentity();
	scale.matrix[0][0] = mx;
	scale.matrix[1][1] = my;
	return scale;
}

matrix2D_t rotationMTX(float theta) {
	matrix2D_t rotate = createIdentity();
	float cs = cos(theta);
	float sn = sin(theta);
	rotate.matrix[0][0] = cs;
	rotate.matrix[0][1] = -sn;
	rotate.matrix[1][0] = sn;
	rotate.matrix[1][1] = cs;
	return rotate;
}

matrix2D_t operator *(matrix2D_t a, matrix2D_t b) {
	matrix2D_t c;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			c.matrix[i][j] = 0;
			for (int k = 0; k < 3; k++)
			{
				c.matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
			}
		}
	}
	return c;
}

vector2D_t operator *(matrix2D_t a, vector2D_t b) {
	vector2D_t c;
	for (int i = 0; i < 3; i++)
	{
		c.vector[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			c.vector[i] += a.matrix[i][j] * b.vector[j];
		}
	}
	return c;
}

//untuk 3D

matrix3D_t createIdentityMatrix3D() {
	matrix3D_t rotate;
	rotate.m[0][0] = 0.0;
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = 0.0;
	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = 0.0;
	rotate.m[1][2] = 0.0;
	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = 0.0;
	rotate.m[2][1] = 0.0;
	return rotate;
}

vector3D_t point2vector3D(point3D_t pnt) {
	vector3D_t vec;
	vec.v[0] = pnt.x;
	vec.v[1] = pnt.y;
	vec.v[2] = pnt.z;
	vec.v[3] = 1;
	return vec;
}

matrix3D_t rotationX(float teta) {
	matrix3D_t rotate = createIdentityMatrix3D();
	rotate.m[0][0] = 1.0;
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = 0.0;
	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = cos(teta / 57.3);
	rotate.m[1][2] = -sin(teta / 57.3);
	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = sin(teta / 57.3);
	rotate.m[2][2] = cos(teta / 57.3);
	return rotate;
}

matrix3D_t rotationY(float teta) {
	matrix3D_t rotate = createIdentityMatrix3D();
	rotate.m[0][0] = cos(teta / 57.3);
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = sin(teta / 57.3);
	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = 1.0;
	rotate.m[1][2] = 0.0;
	rotate.m[2][0] = -sin(teta / 57.3);
	rotate.m[2][1] = 0.0;
	rotate.m[2][2] = cos(teta / 57.3);
	return rotate;
}

matrix3D_t rotationZ(float teta) {
	matrix3D_t rotate = createIdentityMatrix3D();
	rotate.m[0][0] = cos(teta / 57.3);
	rotate.m[0][1] = -sin(teta / 57.3);
	rotate.m[0][2] = 0.0;
	rotate.m[1][0] = sin(teta / 57.3);
	rotate.m[1][1] = cos(teta / 57.3);
	rotate.m[1][2] = 0.0;
	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = 0.0;
	rotate.m[2][2] = 1.0;
	return rotate;
}

vector3D_t operator +(vector3D_t a, vector3D_t b) {
	vector3D_t c;
	for (int i = 0; i<3; i++) {
		c.v[i] = a.v[i] + b.v[i];
	}
	return c;
}

vector3D_t operator -(vector3D_t a, vector3D_t b) {
	vector3D_t c;
	for (int i = 0; i<3; i++) {
		c.v[i] = a.v[i] - b.v[i];
	}
	return c;
}

vector3D_t operator *(matrix3D_t a, vector3D_t b) {
	vector3D_t c;
	for (int i = 0; i<3; i++) {
		c.v[i] = 0;
		for (int j = 0; j<3; j++) {
			c.v[i] += a.m[i][j] * b.v[j];
		}
	}
	return c;
}

vector3D_t operator ^ (vector3D_t a, vector3D_t b) {
	vector3D_t c;//c=a*b 
	c.v[0]=a.v[1]*b.v[2]-a.v[2]*b.v[1]; 
	c.v[1]=a.v[2]*b.v[0]-a.v[0]*b.v[2]; 
	c.v[2]=a.v[0]*b.v[1]-a.v[1]*b.v[0]; 
	c.v[3]=1.; 
	return c; 
}


//pengaturan method

void setColor(color_t col) {
	glColor3f(col.r, col.g, col.b);
}

void drawLine(point2D_t pnt[], int n, color_t col) {
	int i;
	glColor3f(col.r, col.g, col.b);
	glBegin(GL_LINES);
	for (i = 0; i<n; i++) {
		glVertex2i(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolygon(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_POLYGON);
	for (i = 0; i<n; i++) {
		glColor3f(pnt[i].col.r, pnt[i].col.g, pnt[i].col.b);
		glVertex2i(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolylineWithColor(point2D_t pnt[], int n, color_t c) {
	int i;
	setColor(c);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<n; i++) {
		glVertex2i(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolyline(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void create3DObject(object3D_t prisma) {
	int i,j;
	vector3D_t vec[1200];
	vector3D_t vecbuff[100];
	point2D_t titik2D[100];
	for (i = 0;i<prisma.numofVertices;i++) { 
		vec[i] = point2vector3D(prisma.pnt[i]); 
	} 
	for (i = 0;i<prisma.numofFaces;i++) { 
		for (j = 0;j<prisma.fc[i].numofVertices;j++) 
			vecbuff[j] = vec[prisma.fc[i].pnt[j]]; 
			vector3D_t NormalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
			if (NormalVector.v[2] > 0.) {
				for (j = 0;j < prisma.fc[i].numofVertices;j++) {
					titik2D[j] = vector2point2D(vec[prisma.fc[i].pnt[j]]);
					titik2D[j].col = prisma.pnt[prisma.fc[i].pnt[j]].col;
				}
				drawPolygon(titik2D, prisma.fc[i].numofVertices); 
			}
	}
}

object3D_t readFile() {
	std::ifstream file("limas.off");
	std::string str;
	std::string tmp = "";
	int numOfVertex = 0;
	int numOfFace = 0;
	int i = 0,j,k=0;
	object3D_t obj = {};
	if (file.is_open()) {
		while (std::getline(file, str))
		{
			if (i == 1) {
				for (j = 0;j<str.length();j++) {
					if (str[j] != ' ') {
						tmp = tmp + str[j];
					}
					else {
						k++;
						if (k == 1) {
							numOfVertex = atoi(tmp.c_str());
							obj.numofVertices = numOfVertex;
							tmp = "";
						}
						else if (k == 2) {
							numOfFace = atoi(tmp.c_str());
							obj.numofFaces = numOfFace;
							tmp = "";
						}
					}
				}
			}
			else if (i > 1 && i < numOfVertex + 2) {
				tmp = "";
				k = 0;
				for (j = 0;j<str.length();j++) {
					if (str[j] != ' ') {
						tmp = tmp + str[j];
					}
					else {
						switch (k)
						{
						case 0:
							obj.pnt[i - 2].x = atof(tmp.c_str()) * 100;
							break;
						case 1:
							obj.pnt[i - 2].y = atof(tmp.c_str()) * 100;
							break;
						case 2:
							obj.pnt[i - 2].z = atof(tmp.c_str()) * 100;
							break;
						case 3:
							obj.pnt[i - 2].col.r = atof(tmp.c_str()) / 255;
							break;
						case 4:
							obj.pnt[i - 2].col.g = atof(tmp.c_str()) / 255;
							break;
						case 5:
							obj.pnt[i - 2].col.b = atof(tmp.c_str()) / 255;
							break;
						default:
							break;
						}
						tmp = "";
						k++;
					}
				}
			}
			else if (i > numOfVertex + 1) {
				int face = 0;
				tmp = "";
				k = 0;
				for (j = 0;j<str.length();j++) {
					if (str[j] != ' ') {
						tmp = tmp + str[j];
					}
					else {
						switch (k)
						{
						case 0:
							obj.fc[i - (numOfVertex + 2)].numofVertices = atof(tmp.c_str());
							break;
						default:
							obj.fc[i - (numOfVertex + 2)].pnt[face] = atof(tmp.c_str());
							face++;
							break;
						}
						tmp = "";
						k++;
					}
					if (j == str.length() - 1) {
						obj.fc[i - (numOfVertex + 2)].pnt[face] = atof(tmp.c_str());
					}
				}
			}
			i++;
		}
	}
	return obj;
}

void createLimas() {
	object3D_t kubus = readFile();
	matrix3D_t matrix_X = rotationX(sudut);
	matrix3D_t matrix_Y = rotationY(sudut);
	matrix3D_t matrix_Z = rotationZ(sudut);
	for (int i = 0; i<kubus.numofVertices; i++) {
		//teta=15.0;
		vector3D_t p;
		p.v[0] = kubus.pnt[i].x;
		p.v[1] = kubus.pnt[i].y;
		p.v[2] = kubus.pnt[i].z;
		p = (matrix_Y)*(p); //p=operator *(matrix_Y,p);
		p = (matrix_X)*(p); //p=operator *(matrix_X,p);
		p = (matrix_Z)*(p); //p=operator *(matrix_Z,p);
		kubus.pnt[i].x = p.v[0];
		kubus.pnt[i].y = p.v[1];
		kubus.pnt[i].z = p.v[2];
	}
	create3DObject(kubus);
	sudut+=2; if (sudut >= 360.0) sudut = 0.0;
}

void userdraw() {
	createLimas();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void initialize() {
	glClearColor(0, 0, 0, 0);
	glLoadIdentity();
	gluOrtho2D(-360., 360., -360., 360.);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(10, 0);
	glutInitWindowSize(720, 720);
	glutCreateWindow("Object 3D");
	initialize();
	glutDisplayFunc(display);
	glutTimerFunc(1, timer, 0);
	glutMainLoop();
	return 0;
}
