#pragma once
#include "define.h"

struct Vec2
{
	float x;
	float y;

public:
	
	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(float _x, float _y)
		: x(_x), y(_y)
	{}
	
	Vec2(UINT _x, UINT _y)
		: x((float)_x), y((float)_y)
	{}

	Vec2(int _x, int _y)
		: x((float)_x), y((float)_y)
	{}

	void operator = (const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}

	Vec2 operator +(const Vec2& _vec)
	{
		return Vec2(x + _vec.x,	y + _vec.y);
	}

	Vec2 operator +(const Vec2& _vec)const
	{
		return Vec2(x + _vec.x, y + _vec.y);
	}

	Vec2 operator -(const Vec2& _vec)
	{
		return Vec2(x - _vec.x, y - _vec.y);
	}

	Vec2 operator -(const Vec2& _vec) const
	{
		return Vec2(x - _vec.x, y - _vec.y);
	}

};

struct tColor
{
	BYTE red;
	BYTE green;
	BYTE blue;

	tColor() {}
	~tColor() {}
	tColor(int _red, int _green, int _blue)
	{
		red = BYTE(_red);
		green = BYTE(_green);
		blue = BYTE(_blue);
	}
};

struct tRect
{
	float left;
	float top;
	float right;
	float bottom;

	tRect()
		: left(0.f), top(0.f), right(0.f), bottom(0.f)
	{}
	tRect(float _l, float _t, float _r, float _b)
		: left(_l), top(_t), right(_r), bottom(_b)
	{}
	tRect(UINT _l, UINT _t, UINT _r, UINT _b)
		: left((float)_l), top((float)_t), right((float)_r), bottom((float)_b)
	{}
	tRect(int _l, int _t, int _r, int _b)
		: left((float)_l), top((float)_t), right((float)_r), bottom((float)_b)
	{}
};

struct tSize
{
	int x;
	int y;
	tSize()
		:x(0),y(0)
	{}
	tSize(UINT _x, UINT _y)
		:x(_x),y(_y)
	{}
};

struct tSave
{
	STAGE eStage;
	Vec2  vPos;

	tSave()
		:eStage(STAGE::END), vPos(Vec2(0,0))
	{}
	tSave(STAGE _eStage, Vec2 _vPos)
		:eStage(_eStage),vPos(_vPos)
	{}
};

struct tAnimSize
{
	UINT horizontal;
	UINT vertical;
	tAnimSize()
		:horizontal(0), vertical(0)
	{}
	tAnimSize(UINT _hor, UINT _ver)
		:horizontal(_hor), vertical(_ver)
	{}
};

union tRGB
{
	struct
	{
		BYTE b = 0; BYTE g = 0; BYTE r = 0;	BYTE a = 0;
	}color;

	DWORD dwColor;

	tRGB()
		:dwColor(0)
	{}
	tRGB(BYTE _b, BYTE _g, BYTE _r, BYTE _a)
		:color{ _b ,_g ,_r ,_a }
	{}
};

struct tFrame
{
	Vec2 vPos;
	int  iAnimFrame;
	wstring wcsAnimKey;

	tFrame()
		:vPos{},iAnimFrame(0),wcsAnimKey(L"")
	{}
	tFrame(const Vec2& _vPos, int _iFrame, const wstring& _wcsKey)
		:vPos(_vPos),iAnimFrame(_iFrame),wcsAnimKey(_wcsKey)
	{}
};