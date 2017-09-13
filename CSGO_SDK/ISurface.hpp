#pragma once
#include "Color.hpp"

#include "../Globals.hpp"
#include "../VirtualFunction.hpp"

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

class ISurface
{
public:

	void DrawSetColor(Color col)
	{
		typedef void(__thiscall* oDrawSetColor)(void*, Color);
		auto func = call_vfunc< oDrawSetColor >(this, 14);

		if (func)
			func(this, col);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawSetColor)(PVOID, int, int, int, int);
		auto func = call_vfunc< oDrawSetColor >(this, 16);

		if (func)
			func(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawOutlinedRect)(PVOID, int, int, int, int);
		auto func = call_vfunc< oDrawOutlinedRect >(this, 18);

		if (func)
			func(this, x0, y0, x1, y1);
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawLine)(PVOID, int, int, int, int);
		auto func = call_vfunc< oDrawLine >(this, 19);

		if (func)
			func(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long hFont)
	{
		typedef void(__thiscall* oDrawSetTextFont)(PVOID, unsigned long);
		auto func = call_vfunc< oDrawSetTextFont >(this, 23);

		if (func)
			func(this, hFont);
	}
	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		auto func = call_vfunc< oDrawSetTextColor >(this, 24);

		if (func)
			func(this, col);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* oDrawSetTextPos)(PVOID, int, int);
		auto func = call_vfunc< oDrawSetTextPos >(this, 26);

		if (func)
			func(this, x, y);
	}
	void DrawPrintText(wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
	{
		typedef void(__thiscall* oDrawPrintText)(PVOID, wchar_t*, int, FontDrawType_t);
		auto func = call_vfunc< oDrawPrintText >(this, 28);

		if (func)
			func(this, text, textLen, drawType);
	}
	void DrawFlushText()
	{
		typedef void(__thiscall* oDrawFlushText)();
		auto func = call_vfunc< oDrawFlushText >(this, 30);

		if (func)
			return func();
	}

	unsigned long FontCreate()
	{
		typedef unsigned long(__thiscall* oCreateFont)(PVOID);
		auto func = call_vfunc< oCreateFont >(this, 71);

		if (func == nullptr)
			return 0;

		return func(this);
	}
	bool SetFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool(__thiscall* oCreateFont)(PVOID, unsigned long, const char *, int, int, int, int, int, int, int);
		auto func = call_vfunc< oCreateFont >(this, 72);

		if (func == nullptr)
			return false;

		return func(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}
	bool GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef bool(__thiscall* oGetTextSize)(PVOID, unsigned long, const wchar_t*, int&, int&);
		auto func = call_vfunc< oGetTextSize >(this, 79);

		if (func == nullptr)
			return false;

		return func(this, font, text, wide, tall);
	}
	void DrawOutlinedCircle(int a, int b, int c, int d)
	{
		typedef void(__thiscall* DrawOutlinedCircleType)(void*, int, int, int, int);
		auto func = call_vfunc< DrawOutlinedCircleType >(this, 103);

		if (func)
			func(this, a, b, c, d);
	}
};
