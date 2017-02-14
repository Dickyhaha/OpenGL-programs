#include "gl.h"

GL* gl_ptr;

GL::GL()
{
}

GL::~GL()
{
}

void GL::move(int x, int y)
{
	world->updateView((x-window_width/2)*0.002, (y-window_height/2)*0.002);
}

void GL::hold(int x, int y)
{
	world->updateView((x-window_width/2)*0.002, (y-window_height/2)*0.002);
}

void GL::click(int key, int state, int x, int y)
{
	world->updateMouse(key, !state);
}

void GL::keyDown(unsigned char index, int x, int y)
{
	world->updateKey(index, 1);
}

void GL::keyUp(unsigned char index, int x, int y)
{
	world->updateKey(index, 0);
}

void GL::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0 , 200.0);
}

void GL::update(int value)
{
	glutWarpPointer(window_width / 2, window_height / 2);

	world->update();

	glutPostRedisplay();
	glutTimerFunc(update_ms, onUpdate, 0);
}

void GL::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	//Crosshair
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex3f(window_width/2 - 10.0f, window_height/2, 0.0f);
	glVertex3f(window_width/2 + 10.0f, window_height/2, 0.0f);
	glVertex3f(window_width/2, window_height/2 - 10.0f, 0.0f);
	glVertex3f(window_width/2, window_height/2 + 10.0f, 0.0f);

	glEnd();

	world->lock();

	list<Word>* words = world->getUI();
	for(list<Word>::iterator it = words->begin(); it != words->end(); it++)
	{
		glRasterPos2i(it->position[0], window_height - it->position[1] - 15.0f);
		glColor3f(it->color[0], it->color[1], it->color[2]);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)it->content);
		break;
	}
	
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	float* cam_pos = world->getPosition();
	float* cam_rot = world->getRotation();
	gluLookAt(cam_pos[0], cam_pos[1] - 0.5f, cam_pos[2], cam_pos[0] + sin(cam_rot[1]), cam_pos[1] - 0.5f + sin(cam_rot[0]), cam_pos[2] - cos(cam_rot[1]), 0.0f, 1.0, 0.0f);

	GLfloat* ambient = world->getAmbientColor();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	GLfloat* light_color = world->getDiffuseColor();
	GLfloat* light_pos = world->getDiffusePosition();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	list<Cube>* cubes = world->getCubes();
	for(list<Cube>::iterator it = cubes->begin(); it != cubes->end(); it++)
	{
		glPushMatrix();
		glTranslatef(it->position[0], it->position[1], it->position[2]);
		glColor3f(it->color[0], it->color[1], it->color[2]);
		glutSolidCube(it->size);
		glPopMatrix();
	}

	int i = 0;
	list<Sphere>* spheres = world->getSpheres();
	for(list<Sphere>::iterator it = spheres->begin(); i < world->getSphereCount() && it != spheres->end(); i++, it++)
	{
		glPushMatrix();
		glTranslatef(it->position[0], it->position[1], it->position[2]);
		glColor3f(it->color[0], it->color[1], it->color[2]);
		glutSolidSphere(it->radius, 10, 10);
		glPopMatrix();
	}

	i = 0;
	list<Player>* players = world->getPlayers();
	for(list<Player>::iterator it = players->begin(); i < world->getPlayerCount() && it != players->end(); i++, it++)
	{
		glPushMatrix();
		glTranslatef(it->position[0], it->position[1] - 0.5f, it->position[2]);
		glColor3f(it->color[0], it->color[1], it->color[2]);
		glutSolidSphere(0.5f, 20, 20);
		glTranslatef(0.0f, -0.5f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glutSolidCone(0.5f, 1.5f, 20, 20);
		glPopMatrix();
	}

	world->unlock();

	//Land
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glEnd();

	//Sky
	glColor3f(0.0f, 0.3f, 1.0f);
	glutSolidSphere(100, 10, 10);

	glutSwapBuffers();	
}

bool GL::init(const char* name, int height, int width, int x, int y, int ms, World* w)
{
	gl_ptr = this;
	world = w;

	window_height = height;
	window_width = width;
	update_ms = ms;

	int argc = 1;
	char* argv[1] = {(char*)"NULL"};
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(x, y);
	glutCreateWindow(name);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(window_height / 2, window_width / 2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutDisplayFunc(onDraw);
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutMouseFunc(onClick);
	glutPassiveMotionFunc(onMove);
	glutMotionFunc(onHold);
	glutReshapeFunc(onResize);
	glutTimerFunc(update_ms, onUpdate, 0);

	return true;
}

void GL::GLoop()
{
	glutMainLoop();
}

//Global function
void onMove(int x, int y)
{
	if(gl_ptr != NULL)
		gl_ptr->move(x, y);
}

void onHold(int x, int y)
{
	if(gl_ptr != NULL)
		gl_ptr->hold(x,y);
}

void onClick(int key, int state, int x, int y)
{
	if(gl_ptr != NULL)
		gl_ptr->click(key, state, x, y);
}

void onKeyDown(unsigned char index, int x, int y)
{
	if(gl_ptr != NULL)
		gl_ptr->keyDown(index, x, y);
}

void onKeyUp(unsigned char index, int x, int y)
{
	if(gl_ptr != NULL)
		gl_ptr->keyUp(index, x, y);
}

void onResize(int w, int h)
{
	if(gl_ptr != NULL)
		gl_ptr->resize(w, h);
}

void onUpdate(int value)
{
	if(gl_ptr != NULL)
		gl_ptr->update(value);
}

void onDraw()
{
	if(gl_ptr != NULL)
		gl_ptr->draw();
}

