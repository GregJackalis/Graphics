#include <math.h>
#include "Matrix_4D.h"

#define PI 3.1415926535897932384626433832795

Matrix::Matrix()
{
	SetIdentity();
}

Matrix::Matrix(const Matrix& other)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			SetM(i,j, other.GetM(i,j));
}

Matrix::~Matrix()
{

}

void Matrix::SetZero()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_m[i][j] = 0.0f;
}

void Matrix::SetIdentity()
{
	SetZero();
	_m[0][0] = _m[1][1] = _m[2][2] = _m[3][3] = 1.0f;
}

void Matrix::SetMatrix(const Matrix& other)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_m[i][j] = other.GetM(i,j);
}

float Matrix::GetM(const int row, const int col) const
{
	return _m[row][col];
}

void Matrix::SetM(const int row, const int col, const float val)
{
	_m[row][col] = val;
}


const Matrix  Matrix::operator* (const Matrix& other) const
{
	//TODO: Implement Matrix-Matrix Multiplcation
	Matrix result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			float temp = 0;
			for (int k = 0; k < 4; ++k) {
				temp += GetM(i, k) * other.GetM(k, j);
			}
			result.SetM(i, j, temp);
		}
	}

	return result;
}

Matrix& Matrix::operator= (const Matrix &rhs)
{
	if (this != &rhs)
	{
		SetMatrix(rhs);
	}
	return *this;

}

const Vertex Matrix::operator*(const Vertex &other) const
{
	Vertex result;
	
	//TODO: Implement Matrix-Vector Multiplcation

	for (int i = 0; i < 4; ++i) {
		float temp = 0;
		for (int j = 0; j < 4; j++) {
			if (j == 0) {
				temp += GetM(i, j) * other.GetX();
			}
			else if (j == 1) {
				temp += GetM(i, j) * other.GetY();
			}
			else if (j == 2) {
				temp += GetM(i, j) * other.GetZ();
			}
			else {
				temp += GetM(i, j) * other.GetW();
			}
		}

		if (i == 0) {
			result.SetX(temp);
		}
		else if (i == 1) {
			result.SetY(temp);
		}
		else if (i == 2) {
			result.SetZ(temp);
		}
		else {
			result.SetW(temp);
		}
	}

	return result;
}

void Matrix::SetRotationX(float degrees)
{
	//TODO: Set this matrix as a rotation matrix representing a rotation about the X axis by degrees
	float radians = degrees*PI / 180.0f;
	SetIdentity();
	_m[1][1] =  cos(radians); _m[1][2] = -sin(radians); 
	_m[2][1] =  sin(radians); _m[2][2] =  cos(radians);
}

void Matrix::SetRotationY(float degrees)
{
	//TODO: Set this matrix as a rotation matrix representing a rotation about the Y axis by degrees
	float radians = degrees*PI / 180.0f;
	SetIdentity();
	_m[0][0] =  cos(radians); _m[0][2] =  sin(radians); 
	_m[2][0] = -sin(radians); _m[2][2] =  cos(radians);
}

void Matrix::SetRotationZ(float degrees)
{
	//TODO: Set this matrix as a rotation matrix representing a rotation about the Z axis by degrees
	float radians = degrees*PI / 180.0f;
	SetIdentity();
	_m[0][0] =  cos(radians); _m[0][1] = -sin(radians); 
	_m[1][0] =  sin(radians); _m[1][1] =  cos(radians);
}

void Matrix::SetTranslate(float tx, float ty, float tz)
{
	//TODO: Set this matrix as a translation matrix given the translation vector (tx, ty, tz)
	SetIdentity();
	_m[0][3] = tx;
	_m[1][3] = ty; 
	_m[2][3] = tz;
	
}

void Matrix::SetScale(float sx, float sy, float sz)
{
	//TODO: Set this matrix as a scaling matrix given the scaling factors sx, sy, sz
	SetIdentity();
	_m[0][0] = sx;
	_m[1][1] = sy; 
	_m[2][2] = sz;
}

void Matrix::SetViewMatrix(const Vertex& camera_position, const Vertex& view_vector, const Vertex& up_vector)
{
	//TODO: Slightly more advanced
	//Set this matrix as a view matrix based on the given camera_position, view_vector and up_vector
}


void Matrix::SetCamera(float xRotation, float yRotation, float zRotation, float xPosition, float yPosition, float zPosition)
{
	Matrix Camera, CameraPos, CameraRotX, CameraRotY, CameraRotZ;
	CameraPos.SetTranslate(-xPosition, -yPosition, -zPosition);
	CameraRotX.SetRotationX(-xRotation);
	CameraRotY.SetRotationY(-yRotation);
	CameraRotZ.SetRotationZ(-zRotation);
	Camera = CameraPos*CameraRotZ*CameraRotY*CameraRotX;
	SetMatrix(Camera);
}

void Matrix::SetOrtho(float d)
{
	SetZero();
	_m[0][0] = d;
	_m[1][1] = d; 
	_m[2][2] = -d;
	_m[3][3] = 1.0f;

}

void Matrix::SetPerspective(float d, float aspectRatio)
{
	SetZero();
	_m[0][0] = -d/aspectRatio;
	_m[1][1] = -d; 
	_m[2][2] = 1.0f; //for keeping depth
	_m[3][2] = 1.0f;

}

void Matrix::SetViewvolume(float left, float right, float bottom, float top, float front, float rear)
{
	float width = right-left;
	float height = top-bottom;
	float depth = front-rear;
	SetIdentity();
	_m[0][0] = 1.0f/width;
	_m[1][1] = 1.0f/height; 
	_m[2][2] = 1.0f/depth; 
}

void Matrix::SetViewport(int left, int right, int top, int bottom)
{
	int width = right-left;
	int height = bottom-top;
	SetIdentity();
	_m[0][0] = width;
	_m[1][1] = -height; 
	_m[2][2] = 1.0f; //for keeping depth
	_m[0][3] = left+width/2.0f;
	_m[1][3] = top+height/2.0f;
	_m[2][3] = 0.0f; //depth buffer
	_m[3][3] = 1.0f;
}

