#include "ui.h"
#include "slider.h"
#include "textbox.h"
#include "../connection.cpp"

UI* ui_ptr;

UI ui;

UI::UI()
{
}

UI::~UI()
{
	for(unsigned int i = 0; i < interface.size(); i++)
		delete interface[i];
}
bool UI::Interact(const Vec2d& MousePosition, int MouseState)
{
	Vec2d Position, Size;
	for(unsigned int i = 0; i < interface.size(); i++)
	{
		Position = interface[i]->GetInterfacePosition();
		Size = interface[i]->GetInterfaceSize();
		if( MousePosition.x >= Position.x &&
			MousePosition.x <= Position.x + Size.x &&
			MousePosition.y >= WindowSize.y - Position.y - Size.y &&
			MousePosition.y <= WindowSize.y - Position.y &&
			interface[i]->OnTrigger(MousePosition, MouseState))
			return true;
	}
	return false;
}

void UI::Move(int x, int y)
{
	Interact(Vec2d(x, y), MOUSEHOVER);
}

void UI::Hold(int x, int y)
{
	Interact(Vec2d(x, y), MOUSEHOLD);
}

void UI::Click(int Key, int State, int x, int y)
{
	Interact(Vec2d(x, y), State);
}

void UI::Resize(int Width, int Height)
{
	glutReshapeWindow(WindowSize.x, WindowSize.y);
}

void UI::Update(int Value)
{
	for(unsigned int i = 0; i < interface.size(); i++)
		interface[i]->Update();

	glutSetWindow(Window);
	glutPostRedisplay();
	glutTimerFunc(UpdateMs, OnUpdate, 0);
}

void UI::Draw()
{
	glutSetWindow(Window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	for(unsigned int i = 0; i < interface.size(); i++)
		interface[i]->Refresh();

	hub.Render();

	glutSwapBuffers();
}

bool UI::Init(const char* Name, const Vec2d& Position, const Vec2d& Size, int Ms)
{
	ui_ptr = this;

	WindowPosition = Position;
	WindowSize = Size;
	UpdateMs = Ms;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WindowSize.x, WindowSize.y);
	glutInitWindowPosition(Position.x, Position.y);
	Window = glutCreateWindow(Name);

	glClearColor(0, 0, 0, 0);

	glutDisplayFunc(OnDraw);
	glutMouseFunc(OnClick);
	glutPassiveMotionFunc(OnMove);
	glutMotionFunc(OnHold);
	glutReshapeFunc(OnResize);
	glutTimerFunc(UpdateMs, OnUpdate, 0);

	//color of polygons
	interface.push_back(new Slider(0, 100, 0));
	interface[0]->SetInterfacePosition(Vec2d(25, 100));
	interface[0]->SetInterfaceSize(Vec2d(200, 30));
	interface[0]->OnTriggerListener(Red);
	interface[0]->OnUpdateListener(SetRed);

	interface.push_back(new Slider(0, 100, 0));
	interface[1]->SetInterfacePosition(Vec2d(25, 60));
	interface[1]->SetInterfaceSize(Vec2d(200, 30));
	interface[1]->OnTriggerListener(Green);
	interface[1]->OnUpdateListener(SetGreen);

	interface.push_back(new Slider(0, 100, 0));
	interface[2]->SetInterfacePosition(Vec2d(25, 20));
	interface[2]->SetInterfaceSize(Vec2d(200, 30));
	interface[2]->OnTriggerListener(Blue);
	interface[2]->OnUpdateListener(SetBlue);
	
	interface.push_back(new Textbox("99"));
	interface[3]->SetInterfacePosition(Vec2d(25, 150));
	interface[3]->SetInterfaceSize(Vec2d(200, 30));
	interface[3]->OnUpdateListener(SetNum);

	//color of lights	
	interface.push_back(new Slider(0, 100, 0));
	interface[4]->SetInterfacePosition(Vec2d(275, 100));
	interface[4]->SetInterfaceSize(Vec2d(200, 30));
	interface[4]->OnTriggerListener(LightRed);
	interface[4]->OnUpdateListener(LightSetRed);

	interface.push_back(new Slider(0, 100, 0));
	interface[5]->SetInterfacePosition(Vec2d(275, 60));
	interface[5]->SetInterfaceSize(Vec2d(200, 30));
	interface[5]->OnTriggerListener(LightGreen);
	interface[5]->OnUpdateListener(LightSetGreen);

	interface.push_back(new Slider(0, 100, 0));
	interface[6]->SetInterfacePosition(Vec2d(275, 20));
	interface[6]->SetInterfaceSize(Vec2d(200, 30));
	interface[6]->OnTriggerListener(LightBlue);
	interface[6]->OnUpdateListener(LightSetBlue);

	interface.push_back(new Textbox("99"));
	interface[7]->SetInterfacePosition(Vec2d(275, 150));
	interface[7]->SetInterfaceSize(Vec2d(200, 30));
	interface[7]->OnUpdateListener(LightSetNum);

	if(!hub.Init(WindowSize))
		return false;

	if(!terminal.Init())
		return false;

	return true;
}

void UI::ULoop()
{
	terminal.Start();
}

//Global
void OnMove(int x, int y)
{
	if(ui_ptr != NULL)
		ui_ptr->Move(x, y);
}

void OnHold(int x, int y)
{
	if(ui_ptr != NULL)
		ui_ptr->Hold(x,y);
}

void OnClick(int key, int state, int x, int y)
{
	if(ui_ptr != NULL)
	ui_ptr->Click(key, state, x, y);
}

void OnResize(int w, int h)
{
	if(ui_ptr != NULL)
		ui_ptr->Resize(w, h);
}

void OnUpdate(int value)
{
	if(ui_ptr != NULL)
		ui_ptr->Update(value);
}

void OnDraw()
{
	if(ui_ptr != NULL)
		ui_ptr->Draw();
}

