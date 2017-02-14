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
	world->UpdateView((x-window_width/2)*0.002, (y-window_height/2)*0.002);
}

void GL::hold(int x, int y)
{
	world->UpdateView((x-window_width/2)*0.002, (y-window_height/2)*0.002);
}

void GL::click(int key, int state, int x, int y)
{
	world->UpdateMouse(key, !state);
}

void GL::keyDown(unsigned char index, int x, int y)
{
	world->UpdateKey(index, 1);
}

void GL::keyUp(unsigned char index, int x, int y)
{
	world->UpdateKey(index, 0);
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

	world->Update();

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

	world->Lock();
	
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	Vec3 Position = world->GetPosition();
	Vec3 Rotation = world->GetRotation();
	gluLookAt(Position.x, Position.y - 0.5f, Position.z, Position.x + sin(Rotation.y), Position.y - 0.5f + sin(Rotation.x), Position.z - cos(Rotation.y), 0.0f, 1.0, 0.0f);

	const GLfloat* ambient = world->GetAmbientColor();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	const GLfloat* light_color = world->GetDiffuseColor();
	const GLfloat* light_pos = world->GetDiffusePosition();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	world->Draw();

	world->Unlock();

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

