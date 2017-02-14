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
	world.UpdateView((x-window_width/2)*0.002, (y-window_height/2)*0.002);
	//glutWarpPointer(window_width / 2, window_height / 2);
}

void GL::hold(int x, int y)
{
	world.UpdateView((x-window_width/2)*0.002, (y-window_height/2)*0.002);
	//glutWarpPointer(window_width / 2, window_height / 2);
}

void GL::click(int key, int state, int x, int y)
{
	world.UpdateMouse(key, !state);
	//glutWarpPointer(window_width / 2, window_height / 2);
}

void GL::keyDown(unsigned char index, int x, int y)
{
	world.UpdateKey(index, 1);
}

void GL::keyUp(unsigned char index, int x, int y)
{
	world.UpdateKey(index, 0);
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
	world.Update();

	glutSetWindow(Window);
	glutPostRedisplay();
	glutTimerFunc(update_ms, onUpdate, 0);
}

void GL::draw()
{
	glutSetWindow(Window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	world.Lock();
	renderer.Draw(world.GetPolygonStream(), world.GetLightStream(), RAYTRACE);
	world.Unlock();;

	glutSwapBuffers();	
}


bool GL::init(const char* name, int height, int width, int x, int y, int ms)
{
	gl_ptr = this;

	window_height = height;
	window_width = width;
	update_ms = ms;

	int argc = 1;
	char* argv[1] = {(char*)"NULL"};
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(x, y);

	Window = glutCreateWindow(name);
	
	glEnable(GL_DEPTH_TEST);
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

	if(!renderer.Init(Vec2d(width, height), PROJECTION_XY))
		return false;	

	if(!world.Init())
		return false;

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

