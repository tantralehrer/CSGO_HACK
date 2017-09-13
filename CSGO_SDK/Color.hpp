#pragma once

class Vector;

class Color
{
public:
	// constructors
	Color();
	Color(int r, int g, int b);
	Color(int r, int g, int b, int a);

	void SetColor(int r, int g, int b, int a = 0);

	void GetColor(int &r, int &g, int &b, int &a) const;

	void SetRawColor(int color32);

	int GetRawColor() const;

	inline int red() const { return _color[0]; }
	inline int green() const { return _color[1]; }
	inline int blue() const { return _color[2]; }
	inline int alpha() const { return _color[3]; }

	unsigned char &operator[](int index);

	const unsigned char &operator[](int index) const;

	bool operator == (const Color &rhs) const;

	bool operator != (const Color &rhs) const;
	
	Color &operator=(const Color &rhs);
	
	Vector ToVector();

	float GetAlphaFloat() {
		return ((float)alpha() / 255.f);
	}

	unsigned char _color[4];
};
