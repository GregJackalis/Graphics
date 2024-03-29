#include <math.h>
#include "Vertex_4D.h"

Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 1.0f;
}

Vertex::Vertex(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
	_w = 1.0f;
}

// Copy constructor implementation
// Note that the parameter v is passed by reference, rather than value (that is what
// '&' indicates).  Note also the use of 'const' to indicate that the parameter v cannot
// be changed.

Vertex::Vertex(const Vertex& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
	_w = v.GetW();
}

// Destructor does not need to do anything

Vertex::~Vertex()
{
}

void Vertex::Dehomogenise()
{
	_x = _x / _w;
	_y = _y / _w;
	_z = _z / _w;
	_w = 1.0f;
}

float Vertex::Length()
{
	Dehomogenise();
	return sqrt(_x * _x + _y * _y + _z * _z);
}

void Vertex::Normalise()
{
	Dehomogenise();
	float len = Length();
	_x = _x / len;
	_y = _y / len;
	_z = _z / len;
	_w = 1.0f;
}


float Vertex::GetX() const
{
	return _x;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

float Vertex::GetY() const
{
	return _y;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

float Vertex::GetZ() const
{
	return _z;
}

void Vertex::SetZ(const float z)
{
	_z = z;
}

float Vertex::GetW() const
{
	return _w;
}

void Vertex::SetW(const float w)
{
	_w = w;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		_x = rhs.GetX();
		_y = rhs.GetY();
		_z = rhs.GetZ();
		_w = rhs.GetW();
	}
	return *this;
}

// The const at the end of the declaraion for '==" indicates that this operation does not change
// any of the member variables in this class.

bool Vertex::operator==(const Vertex& rhs) const
{
	//They have to be dehomogenised before the comparison
	return ((_x == rhs.GetX()) && (_y == rhs.GetY()) && (_z == rhs.GetZ()));
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	//They have to be dehomogenised before the addition
	return Vertex(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());
}

const Vertex Vertex::operator-(const Vertex& rhs) const
{
	//They have to be dehomogenised before the substraction
	return Vertex(_x - rhs.GetX(), _y - rhs.GetY(), _z - rhs.GetZ());
}

float Vertex::DotProduct(const Vertex& rhs) const
{
	//They have to be dehomogenised before the dot product
	return _x * rhs.GetX() + _y * rhs.GetY() + _z * rhs.GetZ();
}

Vertex Vertex::CrossProduct(const Vertex& rhs) const
{
	//They have to be dehomogenised before the cross product
	return Vertex(_y * rhs.GetZ() - _z * rhs.GetY(), _z * rhs.GetX() - _x * rhs.GetZ(), _x * rhs.GetY() - _y * rhs.GetX());
}
