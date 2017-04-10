#ifndef __COLOR_H__
#define __COLOR_H__


struct Color
{
	float r, g, b, a;

	Color() : r(0.f), g(0.f), b(0.f), a(0.f)
	{}

	Color(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a)
	{}

	virtual ~Color()
	{}

	void Set(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	float* operator & ()
	{
		return (float*)&r;
	}
};

extern Color Red;
extern Color Green;
extern Color Blue;
extern Color Black;
extern Color White;
extern Color Yellow;
extern Color Pink;
extern Color ClearBlue;

#endif // !__COLOR_H__