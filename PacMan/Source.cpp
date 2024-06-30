#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;
// Variables de cámara
float camaraX = -20;
float camaraY = 15;
float camaraZ = 60;

// Variables de Pac-Man
float pacmanX = 0;
float pacmanY = 2.5;
float pacmanZ = -35;
float rotax = 25;
float rotay = 0;
float trany = 0;
float tranz = 0;
float tranx = 0;
float rotaz = 0;
float pacmanSpeed = 0.1f;

// Variables para el ángulo de la boca
float anguloBoca = 45.0f;
float velocidadBoca = 1.0f;

//
float radio = sqrt(pow(camaraX, 2) + pow(camaraZ, 2));
float angulo = acos(camaraX / radio);
float centroY = 10;


// Puntos que Pac-Man debe comer
std::vector<std::pair<float, float>> puntos;

void inicializarLuces() {
	GLfloat luz_ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat luz_difusa[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat luz_especular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat posicion_luz[] = { 0.0, 20.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void iniciarVentana(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / (float)h, 1, 200);
}

void piso() {
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3ub(30, 30, 30);
	glVertex3f(-50.0, 0.0, -50.0);
	glVertex3f(-50.0, 0.0, 50.0);
	glVertex3f(50.0, 0.0, 50.0);
	glVertex3f(50.0, 0.0, -50.0);
	glEnd();
	glPopMatrix();
}

void paredVertical(float x, float z, float length) {
	glPushMatrix();
	glColor3ub(0, 0, 128);
	glTranslatef(x, 0, z);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(-0.5, 10.0, 0.0);
	glVertex3f(-0.5, 10.0, length);
	glVertex3f(-0.5, 0.0, length);
	glEnd();
	glPopMatrix();
}

void paredHorizontal(float x, float z, float length) {
	glPushMatrix();
	glColor3ub(0, 0, 128);
	glTranslatef(x, 0, z);
	glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, -0.5);
	glVertex3f(0.0, 10.0, -0.5);
	glVertex3f(length, 10.0, -0.5);
	glVertex3f(length, 0.0, -0.5);
	glEnd();
	glPopMatrix();
}

// Puntos Amarillos
void drawDot(float x, float z) {
	glPushMatrix();
	glColor3ub(255, 255, 0);
	glTranslatef(x, 0, z);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();
}

void ojosPacman() {
	// Ojo izquierdo
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glTranslatef(-1.5, 1.5, -1.0);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	// Ojo Derecho
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glTranslatef(1.5, 1.5, -1.0);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();
}

void bocaPacman() {
	//boca
	glPushMatrix();
	glColor3ub(0, 0, 0);
	glTranslatef(0, -2, 2.5);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(-2, 2.5, 0.0);
	glVertex3f(2, 2.5, 0.0);
	glEnd();
	glPopMatrix();
}
// Variables globales para animación
const float PI = 3.14159265358979323846;

// Función para dibujar una semiesfera
void drawSemiSphere(float radius, int slices, int stacks) {
	for (int i = 0; i <= stacks / 2; ++i) {
		float lat0 = PI * (-0.5 + (float)(i - 1) / stacks);
		float z0 = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = PI * (-0.5 + (float)i / stacks);
		float z1 = sin(lat1);
		float zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= slices; ++j) {
			float lng = 2 * PI * (float)(j - 1) / slices;
			float x = cos(lng);
			float y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);

			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
		}
		glEnd();
	}
}


void Pacman() {


	glPushMatrix();
	glRotatef(rotay, 0, 1, 0);
	glRotatef(rotaz, 0, 0, 1);
	glTranslatef(pacmanX, pacmanY, pacmanZ);
	//glTranslatef(tranx, trany, tranz);
	glRotated(290, 1, 0, 0);
	glColor3ub(0, 0, 0);
	drawSemiSphere(2.30, 55, 55);// BOCA ABAJO ADENTRO
	glColor3ub(255, 255, 0);
	drawSemiSphere(2.45, 55, 55);//ABAJO
	glColor3ub(255, 0, 0);
	glutSolidSphere(.5, 150, 50);
	glPushMatrix();
	glRotatef(rotax, 1, 0, 0);

	glTranslatef(0, -.2, 0);
	glRotated(90, 1, 0, 0);
	glScaled(1, 1, 1.10);
	ojosPacman();
	glColor3ub(255, 255, 0);
	drawSemiSphere(2.45, 55, 55);//ARRIBA
	glColor3ub(0, 0, 0);
	drawSemiSphere(2.30, 55, 55);//BOCA ARRIBA ADENTRO
	glPopMatrix();
	glPopMatrix();
	//bocaPacman();
}

// Fantasma 
void Fantasma(float x, float z, int r, int g, int b) {
	glPushMatrix();
	glColor3ub(r, g, b);
	glTranslatef(x, 5, z);
	glutSolidSphere(2.5, 50, 50);

	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(-1, 1.5, 1.5);
	glutSolidSphere(0.75, 20, 20);
	glTranslatef(2, 0, 0);
	glutSolidSphere(0.75, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0, 0, 0);
	glTranslatef(-1, 1.5, 1.75);
	glutSolidSphere(0.25, 20, 20);
	glTranslatef(2, 0, 0);
	glutSolidSphere(0.25, 20, 20);
	glPopMatrix();
	glPopMatrix();
}
bool abrir = true;
/*
void moverPacman() {
	pacmanZ += pacmanSpeed;
	if (pacmanZ > 45) {
		pacmanZ = -45; // Reinicia la posición de Pacman
	}

	if (rotax < 25) {
		abrir = true;
	}
	else if (rotax > 90) {
		abrir = false;
	}
	if (abrir) {
		rotax += .8;
	}
	else {
		rotax -= .8;
	}

	// Verificar colisiones con los puntos
	for (auto it = puntos.begin(); it != puntos.end();) {
		if (std::abs(it->first - pacmanX) < 2.5 && std::abs(it->second - pacmanZ) < 2.5) {
			it = puntos.erase(it); // Elimina el punto si Pacman lo come
		}
		else {
			++it;
		}
	}
}
*/
//Arreglo prueba laberinto Mejorado
const int WIDTH = 800;
const int HEIGHT = 600;
const int FILAS = 21;
const int COLUMNAS = 19;

char tablero[FILAS][COLUMNAS] = {
	{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', ' ', '#'},
	{'#', 'O', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', ' ', '#', '#', ' ', ' ', '#', '#', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#'},
	{'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'},
	{'#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#'},
	{'#',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#','#','#'}
};

void drawWall(float x, float y) {
	glBegin(GL_QUADS);
	for (float i = 0.0f;i > -2;i -= 0.0016f) {
		glVertex3f(x, y, i);
		glVertex3f(x + 1.0f, y, i);
		glVertex3f(x + 1.0f, y + 1.0f, i);
		glVertex3f(x, y + 1.0f, i);
	}
	glEnd();
}
/*void drawWall(float x, float y, float height) {
	glBegin(GL_QUADS);
	// Cara frontal de la pared
	glVertex3f(x, y, 0.0f);
	glVertex3f(x + 1.0f, y, 0.0f);
	glVertex3f(x + 1.0f, y + 1.0f, 0.0f);
	glVertex3f(x, y + 1.0f, 0.0f);

	// Cara trasera de la pared (altura)
	glVertex3f(x, y, height);
	glVertex3f(x + 1.0f, y, height);
	glVertex3f(x + 1.0f, y + 1.0f, height);
	glVertex3f(x, y + 1.0f, height);

	// Lados de la pared
	glVertex3f(x, y, 0.0f);
	glVertex3f(x, y, height);
	glVertex3f(x + 1.0f, y, height);
	glVertex3f(x + 1.0f, y, 0.0f);

	glVertex3f(x + 1.0f, y + 1.0f, 0.0f);
	glVertex3f(x + 1.0f, y + 1.0f, height);
	glVertex3f(x, y + 1.0f, height);
	glVertex3f(x, y + 1.0f, 0.0f);

	glEnd();
}*/

void drawBoard() {
	float startX = -COLUMNAS / 4.0f;
	float startY = FILAS / 4.0f;
	glPushMatrix();
	glColor3f(1.0, 0, 0);
	glRotated(90, 1, 0, 0);
	glScaled(5, 5, 3);
	int i = 0;
	int j = 0;
	for (i = 0; i < FILAS; ++i) {
		for (j = 0; j < COLUMNAS; ++j) {
			if (tablero[i][j] == '#') {
				float x = startX + j;
				float y = startY - i;
				//float height = 10.0f; // Altura de la pared
				drawWall(x, y);
			}
		}
	}

	glPopMatrix();
	glPushMatrix();
	glScaled(5,2, 5);
	glPointSize(10);
	glBegin(GL_POINTS);
	glColor3f(0, 1, 0);
	glVertex3i(0, 0, 0);
	glEnd();glPopMatrix();
}

void dibujar() {
	inicializarLuces();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(radio * cos(angulo), camaraY, radio * sin(angulo), 0, centroY, 0, 0, 1, 0);

	//drawBoard();

	glClearColor(0, 0, 0, 1);

	glPushMatrix();
	glRotated(angulo, 0, 1, 0);
	piso();
	
	// Paredes del laberinto basadas en la imagen
	paredHorizontal(-45, -45, 90); // Pared superior
	paredHorizontal(-45, 45, 90);  // Pared inferior
	paredVertical(-45, -45, 90);   // Pared izquierda
	paredVertical(45, -45, 90);    // Pared derecha

	// Paredes internas del laberinto (adaptadas de la imagen)
	paredVertical(-35, -45, 20);
	paredVertical(-35, -5, 10);
	paredVertical(-35, 15, 20);

	paredHorizontal(-35, -35, 20);
	paredHorizontal(-35, 25, 20);
	paredHorizontal(-35, -15, 10);
	paredHorizontal(-35, 5, 10);

	paredVertical(-15, -35, 20);
	paredVertical(-15, -5, 20);
	paredVertical(-15, 25, 20);

	paredHorizontal(-15, -25, 10);
	paredHorizontal(-15, 35, 10);

	paredVertical(5, -45, 30);
	paredVertical(5, -5, 10);
	paredVertical(5, 15, 10);
	paredVertical(5, 35, 10);

	paredHorizontal(5, -15, 10);
	paredHorizontal(5, 25, 10);
	paredHorizontal(5, 45, 10);

	paredVertical(25, -35, 20);
	paredVertical(25, 5, 20);
	paredVertical(25, 25, 20);

	paredHorizontal(25, -25, 10);
	paredHorizontal(25, 15, 10);
	paredHorizontal(25, 35, 10);

	paredVertical(45, -15, 30);
	paredVertical(45, 15, 30);

	paredHorizontal(45, -25, 10);
	paredHorizontal(45, 25, 10);

	

	// Dibujar los puntos
	for (auto& punto : puntos) {
		drawDot(punto.first, punto.second);
	}

	// Dibujar Pacman y fantasmas



	Fantasma(10, 10, 255, 0, 0); // Fantasma rojo
	Fantasma(-10, -10, 0, 255, 0); // Fantasma verde

	//moverPacman();
	glPopMatrix();
	Pacman();
	glutSwapBuffers();
}


void inicializarPuntos() {
	// Inicializar los puntos en posiciones estratégicas del laberinto
	for (int x = -40; x <= 40; x += 10) {
		for (int z = -40; z <= 40; z += 10) {
			if (std::abs(x) > 2 || std::abs(z) > 2) { // Evitar el área inicial de Pacman
				puntos.emplace_back(x, z);
			}
		}
	}
}
void teclas2(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'x':
		pacmanX += 1;
		cout << rotax << endl;
		break;
	case 'X':
		pacmanX -= 1;
		cout << rotax << endl;
		break;
	case 'y':
		pacmanY += 1;
		break;
	case 'Y':
		pacmanY += 1;
		break;
	case 'z':
		pacmanZ -= 1;
		break;
	case 'Z':
		pacmanZ -= 1;
		break;
	default:
		break;
	}
}
void teclas(int tecla, int x, int y) {
	switch (tecla) {
	case 101:
		camaraY += 1;
		break;
	case 103:
		camaraY -= 1;
		break;
	case 100:
		angulo += 0.05;
		break;
	case 102:
		angulo -= 0.05;
		break;
	case 104:
		radio -= 2;
		break;
	case 105:
		radio += 2;
		break;
	case 106:
		centroY += 2;
		break;
	case 107:
		centroY -= 2;
		break;

	default:
		break;
	}
	//glutPostRedisplay();
}

void idle() {
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Pacman 3D");
	inicializarLuces();
	inicializarPuntos();
	glutDisplayFunc(dibujar);
	glutReshapeFunc(iniciarVentana);
	glutKeyboardFunc(teclas2);
	glutSpecialFunc(teclas);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}