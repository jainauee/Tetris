#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <gl\glew.h>
#include <gl\glut.h>
#include "board.h"
#include "box.h"
#include <string.h>

GLfloat xpos = 4;
GLfloat ypos = 15;

board *board::b = NULL;

int mWidth  [7 /*kind */ ][4 /* rotation */ ] ={{2,2,2,2},
												{4,1,4,1},
												{2,3,2,3},
												{2,3,2,3},
												{2,3,2,3},
												{2,3,2,3},
												{2,3,2,3}};

int mHeight  [7 /*kind */ ][4 /* rotation */ ] ={{2,2,2,2},
												{1,4,1,4},
												{3,2,3,2},
												{3,2,3,2},
												{3,2,3,2},
												{3,2,3,2},
												{3,2,3,2}};

board::board(void)
{
	for(int i=0;i<15;i++){
		for(int j=0;j<10;j++){
			board::boardMap[i][j] = 0;
		}
	}

	b=this;
	srand (time(NULL));
	curType = rand() % 7;
	nextType = rand() % 7;
	rotation = 0;
	newShape = true;
	totScore = 0;
}

board::~board(void)
{
}

void board::init() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(0.01, 0.01, 0.0);
		glVertex3f(0.61, 0.01, 0.0);
		glVertex3f(0.01, 0.91, 0.0);
		glVertex3f(0.61, 0.91, 0.0);
	glEnd();
	//Drawing vertical lines on the board for my ease. To be removed later. Can this be done by VBO?
	glColor3f(0.0, 0.5, 0.5);
	glBegin(GL_LINES);
		glVertex3f(0.07, 0.01, 0.0);
		glVertex3f(0.07, 0.91, 0.0);
		glVertex3f(0.13, 0.01, 0.0);
		glVertex3f(0.13, 0.91, 0.0);
		glVertex3f(0.19, 0.01, 0.0);
		glVertex3f(0.19, 0.91, 0.0);
		glVertex3f(0.25, 0.01, 0.0);
		glVertex3f(0.25, 0.91, 0.0);
		glVertex3f(0.31, 0.01, 0.0);
		glVertex3f(0.31, 0.91, 0.0);
		glVertex3f(0.37, 0.01, 0.0);
		glVertex3f(0.37, 0.91, 0.0);
		glVertex3f(0.43, 0.01, 0.0);
		glVertex3f(0.43, 0.91, 0.0);
		glVertex3f(0.49, 0.01, 0.0);
		glVertex3f(0.49, 0.91, 0.0);
		glVertex3f(0.55, 0.01, 0.0);
		glVertex3f(0.55, 0.91, 0.0);
	glEnd();
	//Drawing vertical lines on the board for my ease. To be removed later
	glColor3f(0.0, 0.5, 0.5);
	glBegin(GL_LINES);
		glVertex3f(0.01, 0.07, 0.0);
		glVertex3f(0.61, 0.07, 0.0);
		glVertex3f(0.01, 0.13, 0.0);
		glVertex3f(0.61, 0.13, 0.0);
		glVertex3f(0.01, 0.19, 0.0);
		glVertex3f(0.61, 0.19, 0.0);
		glVertex3f(0.01, 0.25, 0.0);
		glVertex3f(0.61, 0.25, 0.0);
		glVertex3f(0.01, 0.31, 0.0);
		glVertex3f(0.61, 0.31, 0.0);
		glVertex3f(0.01, 0.37, 0.0);
		glVertex3f(0.61, 0.37, 0.0);
		glVertex3f(0.01, 0.43, 0.0);
		glVertex3f(0.61, 0.43, 0.0);
		glVertex3f(0.01, 0.49, 0.0);
		glVertex3f(0.61, 0.49, 0.0);
		glVertex3f(0.01, 0.55, 0.0);
		glVertex3f(0.61, 0.55, 0.0);
		glVertex3f(0.01, 0.61, 0.0);
		glVertex3f(0.61, 0.61, 0.0);
		glVertex3f(0.01, 0.67, 0.0);
		glVertex3f(0.61, 0.67, 0.0);
		glVertex3f(0.01, 0.73, 0.0);
		glVertex3f(0.61, 0.73, 0.0);
		glVertex3f(0.01, 0.79, 0.0);
		glVertex3f(0.61, 0.79, 0.0);
		glVertex3f(0.01, 0.85, 0.0);
		glVertex3f(0.61, 0.85, 0.0);
	glEnd();
}

void board::drawSquare(int row, int col){
	float x = .06*col+0.01;
	float y = .06*(14-row)+0.01;
	glColor3f(0.8, 0.1, 0.4);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(x, y, 0.0);
		glVertex3f(x, y+0.06, 0.0);
		glVertex3f(x+0.06, y, 0.0);
		glVertex3f(x+0.06, y+0.06, 0.0);
	glEnd();
}


void board::drawPiecesOnBoard(){
	for (int i = 0;i<15;i++){
		for(int j=0;j<10;j++){
			if (b->boardMap[i][j] == 1){
				drawSquare(i,j);
			}
		}
	}

}

void board::checkLines(){
	bool lineComplete;
	int totLines = 0;

	for(int row = 14; row >=0;row--){
		lineComplete=true;
		for(int col=0;col<10;col++){
			if(b->boardMap[row][col] == 0){
				lineComplete=false;
			}
		}
		if(lineComplete){//line is complete. Shift board pieces down
			totLines++;
			int temp = row-1;
			for(;temp>=0;temp--){
				for(int j=0;j<10;j++){
					if(b->boardMap[temp][j] ==1){
						b->boardMap[temp][j] = 0;
						b->boardMap[temp+1][j] = 1;
					}else if(b->boardMap[temp][j] == 0){
						b->boardMap[temp+1][j] = 0;
					}
				}
			}
		}
		int temp = 0;
		switch(totLines) {
		case 1:
			temp = totLines * 10;
			break;
		case 2:
			temp = totLines *20;
			break;
		case 3:
			temp = totLines * 30;
			break;
		case 4:
			temp = totLines * 50;
			break;
		}
		b->totScore += temp;
	}
}

void board::moveY(){
			
	if (b->newShape) {
		xpos = 4;
		ypos = 15;
		//set new shape type
		srand (time(NULL));
		b->curType = b->nextType;
		b->nextType = rand() % 7;
		b->rotation = 0;
		b->newShape = false;
	} else {
		ypos--;
	}
}

void board::displayNextType() {
	b->currentShape.drawShape(12, 12, b->nextType, 0, b->boardMap);
}

std::string s = "Next:";
std::string score = "Score:";
void board::renderStrings() {
	glColor3d(1.0,0.0,0.0);
	glRasterPos2d(0.75, 0.75);
	for(int n=0; n<s.size();++n){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[n]);
	}
	glRasterPos2d(0.75, 0.40);
	for(int n=0; n<score.size();++n){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[n]);
	}
	glRasterPos2d(0.75, 0.30);
	char sc [10];
	itoa (b->totScore, sc, 10);
	//std::cout<<"sc="<<sc<<std::endl;
	for(int n=0; n<10;++n){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, sc[n]);
	}
}

void board::gameOver(){
	glColor3d(0.0,0.0,0.0);
	glRasterPos2d(0.25, 0.55);
	std::string s = "Game Over";
	for(int n=0; n<s.size();++n){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[n]);
	}
}

void board::draw() {
	init();
	int ret;
	int s = b->currentShape.isShapeColliding(xpos, ypos, b->curType, b->rotation, b->boardMap);
	int temp = (ypos - mHeight[b->curType][b->rotation] +1);
	switch (s){
	case 1:
		if (temp == 0){
			ypos++;
			b->currentShape.addToBoard(xpos, ypos, b->curType, b->rotation, b->boardMap);
			b->newShape = true;
		}
		break;
	case 2:
		ypos++;
		ret = b->currentShape.addToBoard(xpos, ypos, b->curType, b->rotation, b->boardMap);
		if(ret) {
			gameOver();
		}
		b->newShape = true;
		break;
	case -1:
		b->currentShape.drawShape(xpos, ypos, b->curType, b->rotation, b->boardMap);
		break;
	}
	drawPiecesOnBoard();
	displayNextType();
	renderStrings();
	checkLines();
}

void Timer(int iUnused)
{
    glutPostRedisplay();
	glutSwapBuffers();
    glutTimerFunc(1000, Timer, 0);
	board::moveY();
}

void board::keyPressed(int key, int x, int y){
	switch(key)
	{
	case GLUT_KEY_UP:
		if(b->rotation == 3) {
			if(xpos + mWidth[b->curType][0] < 10.5) {
				if((b->currentShape.isShapeColliding(xpos, ypos, b->curType, b->rotation, b->boardMap)) < 0){
					b->rotation = 0;
					glutPostRedisplay();
					glutSwapBuffers();
				}
			} 
		} else if(xpos + mWidth[b->curType][b->rotation+1] < 10.5) {
			if((b->currentShape.isShapeColliding(xpos, ypos, b->curType, b->rotation, b->boardMap)) < 0){
				b->rotation += 1;
				glutPostRedisplay();
				glutSwapBuffers();
			}
		}

	break;	
	case GLUT_KEY_DOWN:
		ypos--;
		glutPostRedisplay();
		glutSwapBuffers();
	break;
	case GLUT_KEY_LEFT:
		if(xpos > 0){
			xpos -= 1;
			if((b->currentShape.isShapeColliding(xpos, ypos, b->curType, b->rotation, b->boardMap)) < 0){
				glutPostRedisplay();
				glutSwapBuffers();
			} else {
				xpos++;
			}
		}
	break;
	case GLUT_KEY_RIGHT:
		if(xpos + mWidth[b->curType][b->rotation] < 10){
			xpos++;
			if((b->currentShape.isShapeColliding(xpos, ypos, b->curType, b->rotation, b->boardMap)) < 0){
				glutPostRedisplay();
				glutSwapBuffers();
			}else {
				xpos--;
			}
		}
	break;
	}	
}

void board::initialize(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void board::initializeBoard(int argc, char* argv[]){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 700);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Tetris");
	initialize();
	glutDisplayFunc(draw);
	glutSpecialFunc(keyPressed);
	Timer(0);
	glutMainLoop();
}

