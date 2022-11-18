#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace std;

int screenWidth = 640;
int screenHeight = 480;

//Define aquí las constantes y variables globales que necesites (angulo, angulo_total, posicion, direccion,....)
float xm = 0;
float zm = 0;
float ang = 0;
float const GIRO = 0.01;
float ANGULO_TOTAL = 0;
bool movimiento = false;

float direccion[3] = { 1, 0, 0 };

//Actividad 1 Rellena estas funciones

//Eje de coordenadas
void dibuja_ejes_coordenadas() {
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	glColor3f(0, 1, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);

	glColor3f(0, 0, 1);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);

	glEnd();
}

//Cuerpo del patinete
void dibuja_cuerpo() {
	glColor3f(1.0, 0.0, 0.0);

	glScalef(4, 0.4, 1);
	glutWireCube(1);
	glScalef(0.25, 2.5, 1);
}

//Manillar del patinete
void dibuja_manillar() {
	glColor3f(1.0, 1.0, 1.0);

	//Eje longitudinal del manillar 
	glTranslatef(0, 1.25, 0);
	glScalef(0.25, 2.5, 0.25);
	glutWireCube(1);
	glScalef(4, 0.4, 4);
	glTranslatef(0, -1.25, 0);

	//Agarradera del manillar
	glTranslatef(0, 2.65, 0);
	glScalef(0.25, 0.25, 2);
	glutWireCube(1);
	glScalef(4, 4, 0.4);
	glTranslatef(0, -2.65, 0);
}

//Rueda del patinete
void dibuja_rueda() {
	glColor3f(1.0, 1.0, 1.0);

	glRotatef(ang, 0, 0, 1);		//Angulo de giro de la rueda
	glScalef(1, 1, 0.2);
	glutWireSphere(0.5, 5, 5);
	glScalef(1, 1, 5);
}

//Dibujar el patin
void dibuja_patin() {
	//Cuerpo del patinete
	glPushMatrix();
	dibuja_cuerpo();
	glPopMatrix();

	//Manillar del patinete
	glPushMatrix();
	glTranslatef(2, 0, 0);
	dibuja_manillar();
	glPopMatrix();

	//Ruedas del patinete
	glPushMatrix();
	glTranslatef(-2, 0, 0);
	dibuja_rueda();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0, 0);
	dibuja_rueda();
	glPopMatrix();
}

//DEFINICION DE FUNCIONES DE CALLBACK

//Control por ratón
void myMouse(int button, int state, int mx, int  my) {
	//glLoadIdentity();

	if (button == GLUT_LEFT_BUTTON)
	{
		xm += 0.1;
		ang -= 2;
	}
	if (button == GLUT_RIGHT_BUTTON) {

	}
	glutPostRedisplay();
}

//Cambia las dimensiones de la pantalla
void myReshape(int w, int h) {
	screenWidth = w;
	screenHeight = h;
}

//Funcion iddle
void myIdle()
{
	static long int  Time = 0;

	// Número de milisegundos que han pasado desde que se inicio el programa
	long int currentTime = glutGet(GLUT_ELAPSED_TIME);

	if (currentTime - Time > 10) {
		//----> Actividad 2
		if (movimiento) {
			float posicion[2] = { xm += direccion[0] * 0.05,
								 zm += direccion[2] * 0.05 };
			ang -= 2;		//Rotacion ruedas
		}
		Time = currentTime;
	}
	glutPostRedisplay();
}

//Control por teclado
void myKeys(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q': exit(0);
	case 'S': case 's':
		if (!movimiento)
			movimiento = true;
		else
			movimiento = false;
		break;
	case 'R': case 'r':
		if (movimiento) {
			direccion[0] = direccion[0] * cos(+GIRO) + direccion[2] * sin(+GIRO);
			direccion[2] = -direccion[0] * sin(+GIRO) + direccion[2] * cos(+GIRO);
			ANGULO_TOTAL += GIRO * 50;
		}
		break;
	case 'L': case 'l':
		if (movimiento) {
			direccion[0] = direccion[0] * cos(-GIRO) + direccion[2] * sin(-GIRO);
			direccion[2] = -direccion[0] * sin(-GIRO) + direccion[2] * cos(-GIRO);
			ANGULO_TOTAL -= GIRO * 50;
		}
		break;
	}
	glutPostRedisplay();
}

//Dibuja el raster
void myDisplay(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();	//Actividad 2.2

	gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);
	dibuja_ejes_coordenadas();

	//Colocar transformaciones para mover/girar el patinete
	glTranslatef(xm, 0, zm);

	glRotatef(ANGULO_TOTAL, 0, 1, 0);

	//puedes poner aqui cada una de tus funciones de piezas individualmente para comprobar 
	//que son correctas. Luego las comentas y llamas a dibuja_patin()
	dibuja_patin();

	glFlush();
}

//Funcion main del programa
int main(int argc, char ** argv) {
	//CREACION DEL ENTORNO GRAFICO
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(30, 30);
	glutInit(&argc, argv);
	glutCreateWindow("Lab. Inf. Grafica");
	//--------------------


	//Damos de alta a las funciones de Callback
	glutKeyboardFunc(myKeys);
	glutMouseFunc(myMouse);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutIdleFunc(myIdle); //COMENTADA por eficiencia. Descomentar en actividad 2!!!!!!!!
	//--------------------

	//Funciones de limpieza de la pantalla
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//---------------------

	glFlush();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Puedes experimentar con dos tipos de vista diferente (Ortografica o Perspectiva) comentando o descomentando
	//una de las dos siguientes funciones (deja activa solo una de ellas)

	//glOrtho(-6.0,6.0,-6.0,6.0,-5,5);  //Vista ortográfica en 3D
	gluPerspective(45, 4.0 / 3.0, 0.1, 100); //Vista perpectiva en 3D

	glMatrixMode(GL_MODELVIEW);
	//x,y,anchura,altura
	glViewport(0, 0, 640, 480);

	//Llamada al bucle principal de recoleccion de eventos
	glutMainLoop();
}