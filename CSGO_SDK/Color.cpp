#include "Color.hpp"
#include "Vector.hpp"

Color::Color()
{
	*((int *)this) = 0;
}

Color::Color(int r, int g, int b)
{
	SetColor(r, g, b, 0);
}

Color::Color(int r, int g, int b, int a)
{
	SetColor(r, g, b, a);
}

void Color::SetColor(int r, int g, int b, int a)
{
	_color[0] = (unsigned char)r;
	_color[1] = (unsigned char)g;
	_color[2] = (unsigned char)b;
	_color[3] = (unsigned char)a;
}

void Color::GetColor(int &r, int &g, int &b, int &a) const
{
	r = _color[0];
	g = _color[1];
	b = _color[2];
	a = _color[3];
}

void Color::SetRawColor(int color32)
{
	*((int*)this) = color32;

}

int Color::GetRawColor() const
{
	return *((int *)this);
}


unsigned char &Color::operator[](int index)
{
	return _color[index];
}

const unsigned char &Color::operator[](int index) const
{
	return _color[index];
}

bool Color::operator == (const Color &rhs) const
{
	return (*((int *)this) == *((int *)&rhs));
}

bool Color::operator != (const Color &rhs) const
{
	return !(operator==(rhs));
}

Color &Color::operator=(const Color &rhs)
{
	SetRawColor(rhs.GetRawColor());
	return *this;
}

Vector Color::ToVector() {
	return Vector((float)red() / 255.f, (float)green() / 255.f, (float)blue() / 255.f);
}

//BOOST_PYTHON_MODULE(Color)
//{
//	using namespace boost::python;
//
//	class_<Color>("Color", init<>())
//		.def(init<int, int, int>())
//		.def(init<int, int, int, int>())
//		.def("SetColor", &Color::SetColor)
//		.def("GetColor", &Color::GetColor)
//		.def("SetRawColor", &Color::SetRawColor)
//		.def("GetRawColor", &Color::GetRawColor)
//		.def("red", &Color::red)
//		.def("green", &Color::green)
//		.def("blue", &Color::blue)
//		.def("alpha", &Color::alpha)
//		;
//}
//
//void Import_Color()
//{
//	PyImport_AppendInittab("Color", PyInit_Color);
//}
