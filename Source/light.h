#pragma once
#include <windows.h>
#include <vector>
#include "Vertex_4D.h"

using namespace std;

//=======================================================================
//Light Class Declaration

class Light
{
public:
	Light();
	~Light();
	Light(const Light& other);
	// Accessors to Light's position
	Vertex GetLPos() const;
	void SetLPos(Vertex& pos);
	// Accessors to Light's directional
	bool IsDirectional() const;
	void SetDirectional(bool value);
	// Accessors to light's colors
	COLORREF GetColorLa() const;
	void SetColorLa(COLORREF color);
	COLORREF GetColorLd() const;
	void SetColorLd(COLORREF color);
	COLORREF GetColorLs() const;
	void SetColorLs(COLORREF color);

	void SetLightIntensity(float value);

	// Assignment operator
	Light& operator= (const Light& rhs);

	// Accessors to attenuation variables/values
	float GetAttenuation_A() const;
	void SetAttenuation_A(float value);

	float GetAttenuation_B() const;
	void SetAttenuation_B(float value);

	float GetAttenuation_C() const;
	void SetAttenuation_C(float value);

private:
	COLORREF _La;
	COLORREF _Ld;
	COLORREF _Ls;
	Vertex _Lpos;
	bool _directional;

	// for attenuation
	float _a;
	float _b;
	float _c;
};


