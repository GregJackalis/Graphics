#include <windowsx.h>
#include <algorithm>  
#include "Light.h"


//=======================================================================
//Light Class Implementation

Light::Light()
{
	_directional = true;
	_Lpos.SetX(0.0f);
	_Lpos.SetY(0.0f);
	_Lpos.SetZ(1.0f);
	_Lpos.SetW(1.0f);
	_La = RGB(32, 32, 32);
	_Ld = RGB(128, 128, 128);
	_Ls = RGB(255, 255, 255);
	_a = 1.0f;
	_b = 0.0f;
	_c = 0.0f;
}

// Copy constructor implementation
// Note that the parameter other is passed by reference, rather than value (that is what
// '&' indicates).  Note also the use of 'const' to indicate that the parameter other cannot
// be changed.

Light::Light(const Light& other)
{
	_directional = other.IsDirectional();
	_Lpos = other.GetLPos();
	_La = other.GetColorLa();
	_Ld = other.GetColorLd();
	_Ls = other.GetColorLs();
}

// Destructor does not need to do anything

Light::~Light()
{
}


bool Light::IsDirectional() const
{
	return _directional;
}

void Light::SetDirectional(bool value)
{
	_directional = value;
}


Vertex Light::GetLPos() const
{
	return _Lpos;
}

void Light::SetLPos(Vertex& pos)
{
	_Lpos = pos;
}

COLORREF Light::GetColorLa() const
{
	return _La;
}

void Light::SetColorLa(COLORREF color)
{
	_La = color;
}

COLORREF Light::GetColorLd() const
{
	return _Ld;
}

void Light::SetColorLd(COLORREF color)
{
	_Ld = color;
}

COLORREF Light::GetColorLs() const
{
	return _Ls;
}

void Light::SetColorLs(COLORREF color)
{
	_Ls = color;
}

// Attenuation Accessors
float Light::GetAttenuation_A() const
{
	return _a;
}

void Light::SetAttenuation_A(float value)
{
	_a = value;
}

float Light::GetAttenuation_B() const
{
	return _b;
}

void Light::SetAttenuation_B(float value)
{
	_b = value;
}

float Light::GetAttenuation_C() const
{
	return _c;
}

void Light::SetAttenuation_C(float value)
{
	_c = value;
}

