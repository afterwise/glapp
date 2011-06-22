
#include "glapp.h"

unsigned long long g_clock;
double g_period;
float g_elapse;

int g_window;
int g_reshape;
float g_width;
float g_height;
float g_aspect;

unsigned g_keys[2][256 >> 5];
unsigned g_buttons[2];
int g_mousex;
int g_mousey;

void onKeyDown(unsigned char k, int x, int y)
{
	(void) x;
	(void) y;

	if (k == 0x1b)
		exit(0);

	g_keys[0][k >> 5] |= 1 << (k & 0x1f);
}

void onKeyUp(unsigned char k, int x, int y)
{
	(void) x;
	(void) y;

	g_keys[0][k >> 5] &= ~(1 << (k & 0x1f));
}

void onReshape(int w, int h)
{
	g_reshape = 1;
	g_width = w;
	g_height = h;
	g_aspect = (float) w / (float) h;

	glViewport(0, 0, w, h);
}

void onMouse(int button, int state, int x, int y)
{
	(void) x;
	(void) y;

	if (!state)
		g_buttons[0] |= 1 << button;
	else
		g_buttons[0] &= ~(1 << button);
}

void onMotion(int x, int y)
{
	g_mousex = x;
	g_mousey = y;
}

extern void display(float elapse) __attribute__((weak));

void onDisplay()
{
	unsigned long long clock;
	float elapse;

	tbcount(clock);
	elapse = (float) (((double) clock - (double) g_clock) * g_period);
	g_clock = clock;

	elapse = (1.f / 3.f) * elapse + (1.f - 1.f / 3.f) * g_elapse;
	g_elapse = elapse;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(.1f, .1f, .3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPrintf(.9f, .05f, 0x00cf7f7f, "FPS %.02f", 1.f / elapse);
	glPrintf(.9f, .07f, 0x00cf7f7f, "%dx%d", g_mousex, g_mousey);

	if (display)
		display(elapse);

	glDrawFont(g_width, g_height);

	memcpy(g_keys[1], g_keys[0], sizeof(g_keys[0]));
	g_buttons[1] = g_buttons[0];
	g_reshape = 0;

	snooze(15);
	glutSwapBuffers();
	glutPostWindowRedisplay(g_window);
}

void init(int* argc, char* argv[])
{
	unsigned long long freq;

	tbcount(g_clock);
	tbfreq(freq);
	g_period = 1. / (double) freq;
	g_elapse = 1.f / 60.f;

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	const int scrw = glutGet(GLUT_SCREEN_WIDTH);
	const int scrh = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowPosition(scrw / 4, scrh / 4);
	glutInitWindowSize(scrw / 2, scrh / 2);

	g_window = glutCreateWindow("glapp");

	glutKeyboardFunc(&onKeyDown);
	glutKeyboardUpFunc(&onKeyUp);
	glutMouseFunc(&onMouse);
	glutMotionFunc(&onMotion);
	glutPassiveMotionFunc(&onMotion);
	glutReshapeFunc(&onReshape);
	glutDisplayFunc(&onDisplay);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	glInitFont();
}

void end()
{
	glutDestroyWindow(g_window);
}

int main(int argc, char* argv[])
{
	init(&argc, argv);
	atexit(&end);

	glutMainLoop();
	return 0;
}
