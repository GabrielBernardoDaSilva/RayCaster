// OpenGl.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <GL/glut.h>
#include <cmath>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533


float px, py, pdx, pdy, pa;


void drawPlayer() {
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(px, py);
	glEnd();
	
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(px, py);
	glVertex2i(px+ pdx * 5, py + pdy * 5);
	glEnd();
}



void buttons(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		pa -= .1;
		if (pa < 0)
			pa += 2 * PI;
		pdx = cos(pa) * 5;
		pdy = sin(pa) * 5;
	}
	if (key == 'w')
	{
		px += pdx;
		py += pdy;
	}
	if (key == 's')
	{
		px -= pdx;
		py -= pdy;
	}
	if (key == 'd')
	{
		pa += .1;
		if (pa > 2 * PI)
			pa -= 2 * PI;
		pdx = cos(pa) * 5;
		pdy = sin(pa) * 5;
	}
	glutPostRedisplay();
}

int mapX = 8, mapY = 8, mapS = 64;
int map[] = {
1,1,1,1,1,1,1,1,
1,0,1,0,0,0,0,1,
1,0,1,0,0,0,0,1,
1,0,1,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,
};

float dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void drawRays2D() {
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo,disT;

	ra = pa-DR*30;
	if (ra < 0)
	{
		ra += 2 * PI;
	}
	if (ra > 2 * PI)
	{
		ra -= 2 * PI;
	}
	for (r = 0; r < 60; r++)
	{
		//Horizontal 
		dof = 0;
		float disH = 100000000, hx = px, hy = py;
		float aTan = -1 / tan(ra);
		if (ra > PI)
		{
			ry = (((int)py >> 6) << 6) - .0001;
			rx = (py - ry) * aTan + px;
			yo = -64;
			xo = -yo * aTan;

		}
		if (ra < PI)
		{
			ry = (((int)py >> 6) << 6) + 64;
			rx = (py - ry) * aTan + px;
			yo = 64;
			xo = -yo * aTan;

		}
		if (ra == 0 || ra == PI)
		{
			rx = px;
			ry = py;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
			{
				hx = rx;
				hy = ry;
				disH = dist(px, py, hx, hy, ra);
				dof = 8;
			}
			else {
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
/*		glColor3f(0, 1, 0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px, py);
		glVertex2i(rx, ry);
		glEnd();	*/	
		//vertical 
		dof = 0;
		float disV = 100000000, vx = px, vy = py;
		float nTan = - tan(ra);
		if (ra > P2 && ra<P3)
		{
			rx = (((int)px >> 6) << 6) - .0001;
			ry = (px - rx) * nTan + py;
			xo = -64;
			yo = -xo * nTan;

		}
		if (ra < P2 || ra > P3)
		{
			rx = (((int)px >> 6) << 6) + 64;
			ry = (px - rx) * nTan + py;
			xo = 64;
			yo = -xo * nTan;

		}
		if (ra == 0 || ra == PI)
		{
			rx = px;
			ry = py;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mp>0 && mp < mapX * mapY && map[mp] == 1)
			{
				vx = rx;
				vy = ry;
				disV = dist(px, py, vx, vy, ra);
				dof = 8;
			}
			else {
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		if (disV < disH)
		{
			rx = vx;
			ry = vy;
			disT = disV;
			glColor3f(.9,0,0);
		}
		else {
			rx = hx;
			ry = hy;
			disT = disH;
			glColor3f(.7, 0, 0);
		}
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px, py);
		glVertex2i(rx, ry);
		glEnd();
		float ca = pa - ra;
		if (ca < 0)
		{
			ca += 2 * PI;
		}
		if (ca > 2 * PI)
		{
			ca -= 2 * PI;
		}
		disT = disT * cos(ca);
		float lineH = (mapS * 320) / disT;
		if (lineH > 320) {
			lineH = 320;
		}
		float lineO = 160 - lineH / 2;

		glLineWidth(8);
		glBegin(GL_LINES);
		glVertex2i(r * 8 + 530, lineO);
		glVertex2i(r * 8 + 530, lineH + lineO);
		glEnd();
		


		ra += DR;
		if (ra < 0)
		{
			ra += 2 * PI;
		}
		if (ra > 2 * PI)
		{
			ra -= 2 * PI;
		}
	}
}


void drawMap2D() {
	int x, y, xo, yo;
	for (y = 0; y < mapY; y++) {
		for (x = 0; x < mapY; x++) {
			if (map[y * mapX + x] == 1)
				glColor3f(1, 1, 1);
			else
				glColor3f(0, 0, 0);
			xo = x * mapS;
			yo = y * mapS;
			glBegin(GL_QUADS);
			glVertex2i(xo + 1, yo + 1);
			glVertex2i(xo + 1, yo + mapS - 1);
			glVertex2i(xo + mapS - 1, yo + mapS - 1);
			glVertex2i(xo + mapS - 1, yo + 1);
			glEnd();

		}
	}

}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawPlayer();
	drawRays2D();
	glutSwapBuffers();

}

void init() {
	glClearColor(.3, .3, .3, 0);
	gluOrtho2D(0, 1024, 510, 0);
	px = 300;
	py = 300;
	pdx = cos(pa) * 5;
	pdy = sin(pa) * 5;
}




int main(int agrc, char** argv)
{
	glutInit(&agrc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 510);
	glutCreateWindow("Top");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);
	glutMainLoop();
	return 0;

}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
