
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int g_glFontList;
const unsigned g_glFont[] =
{
	0x00000000, 0x00000000, 0x00000800, 0x08080808, 0x00000000, 0x14141400, 0x3e141400, 0x14143e14,
	0x0a3c0800, 0x081e281c, 0x10260600, 0x30320408, 0x2a241a00, 0x10282810, 0x00000000, 0x02040800,
	0x08040200, 0x02040808, 0x08102000, 0x20100808, 0x1c2a0800, 0x082a1c08, 0x08080000, 0x0008083e,
	0x00080810, 0x00000000, 0x00000000, 0x0000003e, 0x00181800, 0x00000000, 0x10200000, 0x00020408,
	0x32221c00, 0x1c22262a, 0x08083e00, 0x08182808, 0x18203e00, 0x1c220204, 0x02221c00, 0x1c22020c,
	0x3e040400, 0x040c1424, 0x02221c00, 0x3e203c02, 0x22221c00, 0x0c10203c, 0x08080800, 0x3e220408,
	0x22221c00, 0x1c22221c, 0x02041800, 0x1c22221e, 0x00080000, 0x00000800, 0x00080810, 0x00000800,
	0x180c0600, 0x060c1830, 0x3e000000, 0x00003e00, 0x0c183000, 0x30180c06, 0x08000800, 0x1c220204,
	0x2e201c00, 0x1c222e2a, 0x3e222200, 0x08142222, 0x12123c00, 0x3c12123c, 0x20120c00, 0x0c122020,
	0x12143800, 0x38141212, 0x20203e00, 0x3e20203c, 0x20202000, 0x3e20203c, 0x22221c00, 0x1c22202e,
	0x22222200, 0x2222223e, 0x08081c00, 0x1c080808, 0x04241800, 0x0e040404, 0x28242200, 0x22242830,
	0x20203e00, 0x20202020, 0x22222200, 0x22362a2a, 0x26262200, 0x2232322a, 0x22221c00, 0x1c222222,
	0x20202000, 0x3c22223c, 0x2a241a00, 0x1c222222, 0x28242200, 0x3c22223c, 0x02221c00, 0x1c22201c,
	0x08080800, 0x3e080808, 0x22221c00, 0x22222222, 0x14140800, 0x22222222, 0x2a362200, 0x2222222a,
	0x14222200, 0x22221408, 0x08080800, 0x2222221c, 0x10203e00, 0x3e020408, 0x08080e00, 0x0e080808,
	0x04020000, 0x00201008, 0x08083800, 0x38080808, 0x00000000, 0x08142200, 0x00003e00, 0x00000000,
	0x00000000, 0x00000000, 0x1e221e00, 0x00001c02, 0x22322c00, 0x20202c32, 0x20221c00, 0x00001c22,
	0x22261a00, 0x02021a26, 0x3e201c00, 0x00001c22, 0x08080800, 0x040a083e, 0x261a021c, 0x00001a26,
	0x22222200, 0x20202c32, 0x08081c00, 0x08001808, 0x08084830, 0x08001808, 0x30282400, 0x20202428,
	0x08081c00, 0x18080808, 0x2a2a2a00, 0x0000342a, 0x22222200, 0x00002c32, 0x22221c00, 0x00001c22,
	0x322c2020, 0x00002c32, 0x261a0202, 0x00001a26, 0x20202000, 0x00002c32, 0x3c023c00, 0x00001e20,
	0x10120c00, 0x10103c10, 0x24241a00, 0x00002424, 0x22140800, 0x00002222, 0x2a2a1400, 0x0000222a,
	0x08142200, 0x00002214, 0x261a021c, 0x00002222, 0x08103e00, 0x00003e04, 0x10100c00, 0x0c101020,
	0x08080800, 0x08080808, 0x08083000, 0x30080804, 0x00000000, 0x3e000000, 0x00000000, 0x00000000
};

struct GLText
{
	float x, y;
	unsigned color, length;
	char string[0];
};

char g_glTextBuffer[2048];
char* g_glTextEnd;

void glPrintf(float x, float y, unsigned c, const char* fmt, ...)
{
	va_list ap;
	unsigned n;
	char s[256];

	va_start(ap, fmt);
	n = vsnprintf(s, sizeof(s), fmt, ap);
	va_end(ap);

	if ((size_t) (&g_glTextBuffer[sizeof(g_glTextBuffer)] - g_glTextEnd) >= sizeof(struct GLText) + n + 1)
	{
		((struct GLText*) g_glTextEnd)->x = x;
		((struct GLText*) g_glTextEnd)->y = y;
		((struct GLText*) g_glTextEnd)->color = c;
		((struct GLText*) g_glTextEnd)->length = n;
		memcpy(((struct GLText*) g_glTextEnd)->string, s, n + 1);

		g_glTextEnd += sizeof(struct GLText) + n + 1;
	}
}

void glDrawFont(float w, float h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.f, w, h, 0.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (
		struct GLText* t = (struct GLText*) g_glTextBuffer;
		t != (struct GLText*) g_glTextEnd;
		t = (struct GLText*) &t->string[t->length + 1])
	{
		glPushAttrib(GL_LIST_BIT);
		glListBase(g_glFontList);

		glColor4ub(0, 0, 0, 0);
		glRasterPos2i((int) (w * t->x) + 1, (int) (h * t->y) + 1);
		glCallLists(t->length, GL_UNSIGNED_BYTE, t->string);

		glColor4ubv((unsigned char*) &t->color);
		glRasterPos2i((int) (w * t->x), (int) (h * t->y));
		glCallLists(t->length, GL_UNSIGNED_BYTE, t->string);

		glPopAttrib();
	}

	g_glTextEnd = g_glTextBuffer;
}

void glInitFont()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	g_glFontList = glGenLists(256);

	for (int i = 0, j = ' '; j < 256; i += 2, ++j)
	{
		glNewList(g_glFontList + j, GL_COMPILE);
		glBitmap(8, 8, 0.f, 0.f, 8.f, 0.f, (unsigned char*) &g_glFont[i]);
		glEndList();
	}

	g_glTextEnd = g_glTextBuffer;
}
