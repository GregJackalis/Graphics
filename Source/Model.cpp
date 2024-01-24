#include <windowsx.h>
#include <math.h>
#include <algorithm>  
#include "Model.h"


//=======================================================================
//Triangle Class Implementation

Triangle::Triangle()
{
	_indices[0] = 0;
	_indices[1] = 0;
	_indices[2] = 0;
	_visible = true;
	_normal.SetX(0.0f);
	_normal.SetY(0.0f);
	_normal.SetZ(0.0f);
	_normal.SetW(1.0f);
	_center.SetX(0.0f);
	_center.SetY(0.0f);
	_center.SetZ(0.0f);
	_center.SetW(1.0f);
	_color = RGB(128, 128, 128);
}

Triangle::Triangle(int index0, int index1, int index2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
	_visible = true;
	_normal.SetX(0.0f);
	_normal.SetY(0.0f);
	_normal.SetZ(0.0f);
	_normal.SetW(1.0f);
	_center.SetX(0.0f);
	_center.SetY(0.0f);
	_center.SetZ(0.0f);
	_center.SetW(1.0f);
	_color = RGB(128, 128, 128);
}

// Copy constructor implementation
// Note that the parameter other is passed by reference, rather than value (that is what
// '&' indicates).  Note also the use of 'const' to indicate that the parameter other cannot
// be changed.

Triangle::Triangle(const Triangle& other)
{
	_indices[0] = other.GetIndex(0);
	_indices[1] = other.GetIndex(1);
	_indices[2] = other.GetIndex(2);
	_visible = other.IsVisible();
	_normal = other.GetNormal();
	_center = other.GetCenter();
	_color = other.GetColor();
}

// Destructor does not need to do anything

Triangle::~Triangle()
{
}


int Triangle::GetIndex(int i) const
{
	return _indices[i];
}

bool Triangle::IsVisible() const
{
	return _visible;
}

void Triangle::SetVisible(bool value)
{
	_visible = value;
}


Vertex Triangle::GetNormal() const
{
	return _normal;
}

void Triangle::SetNormal(Vertex& normal)
{
	_normal = normal;
}

Vertex Triangle::GetCenter() const
{
	return _center;
}

void Triangle::SetCenter(Vertex& center)
{
	_center = center;
}

COLORREF Triangle::GetColor() const
{
	return _color;
}

void Triangle::SetColor(COLORREF color)
{
	_color = color;
}



Triangle& Triangle::operator=(const Triangle& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		_indices[0] = rhs.GetIndex(0);
		_indices[1] = rhs.GetIndex(1);
		_indices[2] = rhs.GetIndex(2);
		_visible = rhs.IsVisible();
		_normal = rhs.GetNormal();
		_center = rhs.GetCenter();
	}
	return *this;
}

//=======================================================================
//Model Class Implementation

Model::Model()
{
	_Ka = RGB(192, 192, 192);
	_Kd = RGB(192, 192, 192);
	_Ks = RGB(192, 192, 192);
	_shininess = 30.0f;
}

Model::~Model()
{
}

void Model::Clear()
{
	ClearTriangles();
	ClearVertices();
}

void Model::ClearVertices()
{
	_vertices.clear();
}

void Model::ClearTriangles()
{
	_triangles.clear();
}

vector<Triangle>& Model::GetTriangles()
{
	return _triangles;
}

vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

size_t Model::GetTriangleCount() const
{
	return _triangles.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex v;
	v = Vertex(x, y, z);
	_vertices.push_back(v);
}

void Model::AddTriangle(int i0, int i1, int i2)
{
	Triangle t;
	t = Triangle(i0, i1, i2);
	_triangles.push_back(t);
}

COLORREF Model::GetColorKa() const
{
	return _Ka;
}

void Model::SetColorKa(COLORREF color)
{
	_Ka = color;
}

COLORREF Model::GetColorKd() const
{
	return _Kd;
}

void Model::SetColorKd(COLORREF color)
{
	_Kd = color;
}

COLORREF Model::GetColorKs() const
{
	return _Ks;
}

void Model::SetColorKs(COLORREF color)
{
	_Ks = color;
}

float Model::GetShininess() const
{
	return _shininess;
}
void Model::SetShininess(float value)
{
	_shininess = value;
}


void Model::CopyModel(Model& rhs)
{
	vector<Triangle> triangles;
	vector<Vertex> vertices;


	triangles = rhs.GetTriangles();
	vertices = rhs.GetVertices();
	for (int i = 0; i < rhs.GetTriangleCount(); ++i)
	{
		// Call model member function to add a new polygon to the list of triangles
		AddTriangle(triangles[i].GetIndex(0), triangles[i].GetIndex(1), triangles[i].GetIndex(2));
	}

	// Vertex array initialization
	for (int i = 0; i < rhs.GetVertexCount(); ++i)
	{
		// Call model member function to add a new vertex to the list of vertices		
		AddVertex(vertices[i].GetX(), vertices[i].GetY(), vertices[i].GetZ());
	}

	_Ka = rhs.GetColorKa();
	_Kd = rhs.GetColorKd();
	_Ks = rhs.GetColorKs();
	_shininess = rhs.GetShininess();

}



void Model::ApplyTransform(const Matrix& transform)
{
	Vertex vC;
	for (size_t i = 0; i < _vertices.size(); i++)
	{
		_vertices[i] = transform * _vertices[i];
	}
	for (size_t i = 0; i < _triangles.size(); i++)
	{
		vC = transform * _triangles[i].GetCenter();
		_triangles[i].SetCenter(vC);
	}
}

void Model::Dehomogenise()
{
	for (size_t i = 0; i < _vertices.size(); i++)
	{
		_vertices[i].Dehomogenise();
	}
}

// Calculates Face Normals of Model
void Model::CalculateNormals(bool CCW)
{
	Vertex v0, v1, v2, vA, vB, vN;

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		v0 = _vertices[_triangles[i].GetIndex(0)];
		v1 = _vertices[_triangles[i].GetIndex(1)];
		v2 = _vertices[_triangles[i].GetIndex(2)];

		vA = v1 - v0;
		vB = v2 - v0;

		if (CCW)
			vN = vA.CrossProduct(vB);
		else
			vN = vB.CrossProduct(vA);

		vN.Normalise();
		_triangles[i].SetNormal(vN);
	}
}

// Calculates Face Center of Model
void Model::CalculateCenters()
{
	Vertex v0, v1, v2, vC;

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		v0 = _vertices[_triangles[i].GetIndex(0)];
		v1 = _vertices[_triangles[i].GetIndex(1)];
		v2 = _vertices[_triangles[i].GetIndex(2)];

		vC.SetX((v0.GetX() + v1.GetX() + v2.GetX()) / 3.0f);
		vC.SetY((v0.GetY() + v1.GetY() + v2.GetY()) / 3.0f);
		vC.SetZ((v0.GetZ() + v1.GetZ() + v2.GetZ()) / 3.0f);
		vC.SetW(1.0f);

		_triangles[i].SetCenter(vC);
	}
}

// Marks Backfaces of Model
void Model::MarkBackfaces(float eX, float eY, float eZ, bool ortho)
{
	float result;
	Vertex v0, v1, v2, vN, vC, vE; //Vertex, normal, center and eye vectors

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		//v0 = _vertices[_triangles[i].GetIndex(0)];
		//v1 = _vertices[_triangles[i].GetIndex(1)];
		//v2 = _vertices[_triangles[i].GetIndex(2)];

		vN = _triangles[i].GetNormal();
		vC = _triangles[i].GetCenter();

		if (ortho) vE = Vertex(eX, eY, eZ);
		else vE = Vertex(eX, eY, eZ) - vC;

		result = vN.DotProduct(vE);

		if (result <= 0)
			_triangles[i].SetVisible(false);
		else
			_triangles[i].SetVisible(true);

	}
}

// Apply Depth Sorting on Model Triangles
void Model::ApplyDepthSorting()
{

	std::sort(_triangles.begin(), _triangles.end(), [](const Triangle& lhs, const Triangle& rhs) {return lhs.GetCenter().GetZ() < rhs.GetCenter().GetZ(); });

}


void Model::DrawWireFrame(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;

	if (_triangles.size() == 0)
	{
		return;
	}
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HGDIOBJ oldPen = SelectObject(hdc, pen);

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i].IsVisible() == true)
		{
			v0_inx = _triangles[i].GetIndex(0);
			v1_inx = _triangles[i].GetIndex(1);
			v2_inx = _triangles[i].GetIndex(2);

			MoveToEx(hdc, (int)_vertices[v0_inx].GetX(), (int)_vertices[v0_inx].GetY(), NULL);

			LineTo(hdc, (int)_vertices[v1_inx].GetX(), (int)_vertices[v1_inx].GetY());
			LineTo(hdc, (int)_vertices[v2_inx].GetX(), (int)_vertices[v2_inx].GetY());
			LineTo(hdc, (int)_vertices[v0_inx].GetX(), (int)_vertices[v0_inx].GetY());
		}

	}
	SelectObject(hdc, oldPen);
	DeleteObject(pen);

}

void Model::DrawSolidFill(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;
	POINT points[3];
	POINT point;

	if (_triangles.size() == 0)
	{
		return;
	}
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	//HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(0, 255, 255));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i].IsVisible() == true)
		{
			v0_inx = _triangles[i].GetIndex(0);
			v1_inx = _triangles[i].GetIndex(1);
			v2_inx = _triangles[i].GetIndex(2);

			point.x = (int)_vertices[v0_inx].GetX(); point.y = (int)_vertices[v0_inx].GetY();
			points[0] = point;
			point.x = (int)_vertices[v1_inx].GetX(); point.y = (int)_vertices[v1_inx].GetY();
			points[1] = point;
			point.x = (int)_vertices[v2_inx].GetX(); point.y = (int)_vertices[v2_inx].GetY();
			points[2] = point;

			Polygon(hdc, points, 3);

			point.x = (int)_triangles[i].GetCenter().GetX(); point.y = (int)_triangles[i].GetCenter().GetY();
			//SetPixel(hdc, point.x, point.y, RGB(255, 0, 0));
			//SetPixel(hdc, 100+i, 200+i, RGB(255, 0, 0));

			//point.x = 200 + i; point.y = 100 + i;
			MoveToEx(hdc, point.x, point.y, NULL);

			LineTo(hdc, point.x + 1, point.y);
			LineTo(hdc, point.x + 1, point.y + 1);
			LineTo(hdc, point.x, point.y + 1);
			LineTo(hdc, point.x, point.y);

		}

	}
	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);

}

// Function used to caclulate the "final stage" of the application. The Calculation of the Flat Shading for each model happens with the help
// of the CalculateLighting function that can be found right after this function.
void Model::CalcFlatShading(HDC hdc, Light light1, Light light2, Light light3, float eX, float eY, float eZ, bool ortho, bool directional)
{
	COLORREF faceColor;

	if (_triangles.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		sumR = 0;
		sumG = 0;
		sumB = 0;


		// For Light1
		
		CalculateLighting(hdc, light1, eX, eY, eZ, ortho, _triangles[i], directional);

		// For Light2
		CalculateLighting(hdc, light2, eX, eY, eZ, ortho, _triangles[i], directional);

		// For Light3
		CalculateLighting(hdc, light3, eX, eY, eZ, ortho, _triangles[i], directional);

		if (sumR > 255.0f) sumR = 255.0f;
		if (sumG > 255.0f) sumG = 255.0f;
		if (sumB > 255.0f) sumB = 255.0f;

		faceColor = RGB((int)sumR, (int)sumG, (int)sumB);

		_triangles[i].SetColor(faceColor);

	}

}

void Model::CalculateLighting(HDC hdc, Light light, float eX, float eY, float eZ, bool ortho, Triangle triangle, bool directional)
{

	Vertex vN, vC, vL, vE, vH;
	float cD, cS;
	float totR, totG, totB;
	float attenuation, a, b, c;


	vN = triangle.GetNormal();
	vN.Normalise();
	vC = triangle.GetCenter();

	if (light.IsDirectional()) vL = light.GetLPos();
	else vL = light.GetLPos() - vC;
	vL.Normalise();

	float distance = vL.Length();

	cD = vN.DotProduct(vL);

	if (ortho) vE = Vertex(eX, eY, eZ);
	else vE = Vertex(eX, eY, eZ) - vC;
	vE.Normalise();

	vH = vL + vE;
	vH.Normalise();

	cS = pow(vN.DotProduct(vH), GetShininess());

	if (cD <= 0) { cD = 0; cS = 0; }

	
	// Attenuation Setup
	light.SetAttenuation_A(2.0f);
	light.SetAttenuation_B(1.0f);
	light.SetAttenuation_C(1.0f);


	a = light.GetAttenuation_A();
	b = light.GetAttenuation_B();
	c = light.GetAttenuation_C();


	attenuation = 1.0f / (a + b * distance + c * pow(distance, 2));


	totR = (GetRValue(GetColorKa()) * GetRValue(light.GetColorLa()) / 255.0f +
		cD * GetRValue(GetColorKd()) * GetRValue(light.GetColorLd()) / 255.0f +
		cS * GetRValue(GetColorKs()) * GetRValue(light.GetColorLs()) / 255.0f);
	totG = (GetGValue(GetColorKa()) * GetGValue(light.GetColorLa()) / 255.0f +
		cD * GetGValue(GetColorKd()) * GetGValue(light.GetColorLd()) / 255.0f +
		cS * GetGValue(GetColorKs()) * GetGValue(light.GetColorLs()) / 255.0f);
	totB = (GetBValue(GetColorKa()) * GetBValue(light.GetColorLa()) / 255.0f +
		cD * GetBValue(GetColorKd()) * GetBValue(light.GetColorLd()) / 255.0f +
		cS * GetBValue(GetColorKs()) * GetBValue(light.GetColorLs()) / 255.0f);

	if (!directional) {
		totR = totR * attenuation;
		totG = totG * attenuation;
		totB = totB * attenuation;
	}


	if (totR > 255.0f) totR = 255.0f;
	if (totG > 255.0f) totG = 255.0f;
	if (totB > 255.0f) totB = 255.0f;

	sumR += totR;
	sumG += totG;
	sumB += totB;

}

void Model::DrawFlatShading(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;
	POINT points[3];
	POINT point;
	COLORREF faceColor;

	if (_triangles.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < _triangles.size(); i++)
	{
		if (_triangles[i].IsVisible() == true)
		{
			v0_inx = _triangles[i].GetIndex(0);
			v1_inx = _triangles[i].GetIndex(1);
			v2_inx = _triangles[i].GetIndex(2);

			point.x = (int)_vertices[v0_inx].GetX(); point.y = (int)_vertices[v0_inx].GetY();
			points[0] = point;
			point.x = (int)_vertices[v1_inx].GetX(); point.y = (int)_vertices[v1_inx].GetY();
			points[1] = point;
			point.x = (int)_vertices[v2_inx].GetX(); point.y = (int)_vertices[v2_inx].GetY();
			points[2] = point;

			faceColor = _triangles[i].GetColor();

			HPEN pen = CreatePen(PS_SOLID, 1, faceColor);
			HGDIOBJ oldPen = SelectPen(hdc, pen);

			HBRUSH brush = CreateSolidBrush(faceColor);
			HBRUSH oldBrush = SelectBrush(hdc, brush);

			Polygon(hdc, points, 3);

			SelectBrush(hdc, oldBrush);
			DeleteObject(brush);

			SelectPen(hdc, oldPen);
			DeleteObject(pen);

		}

	}


}
