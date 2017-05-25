#pragma once
#include <gl\glut.h>
#include "box.h"

extern GLfloat xpos;
extern GLfloat ypos;

class board
{
	int boardMap[15][10];
	box currentShape;
	static board *b;
	int curType;
	int nextType;
	int rotation;
	bool newShape;
	int totScore;
			
public:
	board(void);
	~board(void);
	void drawObject();
	static void initializeBoard(int a, char* b[]);
	static void initialize();
	static void draw();
	static void keyPressed(int key, int x, int y);
	static void init();
	static void idle(void);
	static void moveY();
	static void drawPiecesOnBoard();
	static void drawSquare(int i, int j);
	static void checkLines();
	static void displayNextType();
	static void renderStrings();
	static void gameOver();
};

