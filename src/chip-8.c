#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include <stdbool.h>

#include <GL/glut.h>
// http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

#define MODIFIER 10
int display_width = SCREEN_WIDTH * MODIFIER;
int display_height = SCREEN_HEIGHT * MODIFIER;
char* filename;

void main_cycle();
void setUpOpenGL();
void updateScreen();
void display();
void reshapeWindow(GLsizei w, GLsizei h);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);

int main(int argc, char **argv) {
	filename = argv[1];
	cpu_loadRom(argv[1]);

	// Open GL //
	glutInit(&argc, argv);
	setUpOpenGL();


	//glutReshapeFunc(reshapeWindow);
	glutDisplayFunc(display);
	glutIdleFunc(main_cycle);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecial);
	glutMainLoop();
	return 0;
}

void setUpOpenGL(){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(display_width, display_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Chip-8");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, display_width, display_height, 0.0);

	glMatrixMode(GL_MODELVIEW);
}

void drawPixel(int x, int y){
	glBegin(GL_QUADS);
		glVertex3f((x * MODIFIER) + 0.0f,     (y * MODIFIER) + 0.0f,	 0.0f);
		glVertex3f((x * MODIFIER) + 0.0f,     (y * MODIFIER) + MODIFIER, 0.0f);
		glVertex3f((x * MODIFIER) + MODIFIER, (y * MODIFIER) + MODIFIER, 0.0f);
		glVertex3f((x * MODIFIER) + MODIFIER, (y * MODIFIER) + 0.0f,	 0.0f);
	glEnd();
}

void updateScreen(){
	// Draw
	//debugRender();
	for(int y = 0; y < SCREEN_HEIGHT; ++y){
		for(int x = 0; x < SCREEN_WIDTH; ++x){
			if(gfx[(y * SCREEN_WIDTH) + x] == 0){
				glColor3f(0.0f,0.0f,0.0f);
			}
			else{
				glColor3f(1.0f,1.0f,1.0f);
			}
			drawPixel(x, y);
		}
	}
}

void main_cycle() {
	emulateCycle();
	/*
	if(drawFlag){
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	*/
	if(drawFlag){
		glutPostRedisplay();
	}
	
}


void display(){

	if(drawFlag){
		glClear(GL_COLOR_BUFFER_BIT);
		updateScreen();
	}
	glutSwapBuffers();
	drawFlag = false;
}

void reshapeWindow(GLsizei w, GLsizei h){

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Resize quad
	display_width = w;
	display_height = h;

}
/*
void reshapeWindow(GLsizei w, GLsizei h){
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	display_width = w;
	display_height = h;
}
*/

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		keypad[0x1] = 1;
	else if(key == '2')	keypad[0x2] = 1;
	else if(key == '3')	keypad[0x3] = 1;
	else if(key == '4')	keypad[0xC] = 1;

	else if(key == 'q')	keypad[0x4] = 1;
	else if(key == 'w')	keypad[0x5] = 1;
	else if(key == 'e')	keypad[0x6] = 1;
	else if(key == 'r')	keypad[0xD] = 1;

	else if(key == 'a')	keypad[0x7] = 1;
	else if(key == 's')	keypad[0x8] = 1;
	else if(key == 'd')	keypad[0x9] = 1;
	else if(key == 'f')	keypad[0xE] = 1;

	else if(key == 'z')	keypad[0xA] = 1;
	else if(key == 'x')	keypad[0x0] = 1;
	else if(key == 'c')	keypad[0xB] = 1;
	else if(key == 'v')	keypad[0xF] = 1;

	//printf("Press key %c\n", key);
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		keypad[0x1] = 0;
	else if(key == '2')	keypad[0x2] = 0;
	else if(key == '3')	keypad[0x3] = 0;
	else if(key == '4')	keypad[0xC] = 0;

	else if(key == 'q')	keypad[0x4] = 0;
	else if(key == 'w')	keypad[0x5] = 0;
	else if(key == 'e')	keypad[0x6] = 0;
	else if(key == 'r')	keypad[0xD] = 0;

	else if(key == 'a')	keypad[0x7] = 0;
	else if(key == 's')	keypad[0x8] = 0;
	else if(key == 'd')	keypad[0x9] = 0;
	else if(key == 'f')	keypad[0xE] = 0;

	else if(key == 'z')	keypad[0xA] = 0;
	else if(key == 'x')	keypad[0x0] = 0;
	else if(key == 'c')	keypad[0xB] = 0;
	else if(key == 'v')	keypad[0xF] = 0;
	//printf("Unpressed key %c\n", key);
}

void keyboardSpecial(int key, int x, int y){
	if(key == GLUT_KEY_F1) cpu_loadRom(filename);
}
