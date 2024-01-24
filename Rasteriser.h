#pragma once
#include "Framework.h"
#include "Light.h"
#include "Model.h"
#include "Matrix_4D.h"

using namespace std;


class Rasteriser : public Framework
{
private:
	//Models in scene to be rendered
	vector<Model> _models; // the list of original models in scene
	vector<Model> _models_transformed; // the list of transformed models in scene
	//vector<Light> _lights; // the list of lights in scene
	RECT	 _viewportrect; // the viewport rectangle
	Matrix	 _viewport; // the viewport matrix
	Matrix	 _view; // the view matrix
	Matrix	 _camera; // the camera matrix
	Matrix	 _projection; // the projection matrix
	int      _render_mode;
	bool     _depthsort;
	bool     _facecull;
	bool     _ortho;
	float    _camRx, _camRy, _camRz, _camZ;
	bool     _directional;
	Light    _light1;
	Light	 _light2;
	Light	 _light3;
	Vertex   _lPos1;
	Vertex	 _lPos2;
	Vertex   _lPos3;

public:

	void AddModel(Model& model);
	void TransformModel(int model_inx, Matrix transform);
	void SetViewTransform(Matrix view);
	void SetCameraTransform(Matrix cam);
	void SetProjectionTransform(Matrix proj);
	void SetViewport(int left, int right, int top, int bottom);

	virtual void InitializeScene(HWND _hWnd);
	virtual void RenderScene(HDC hdc);
	virtual void SetRenderParams(int action);

	void DrawString(HDC hdc, int xPos, int yPos, int fSize, COLORREF textColor, LPCTSTR text);
	void DrawTextFrame(HDC hdc);
	void DrawViewportFrame(HDC hdc); // draw the viewport frame
};

