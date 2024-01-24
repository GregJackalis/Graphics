#include <windowsx.h>
#include "MD2Loader.h"
#include "Rasteriser.h"

Rasteriser app;

//=== Implementations of Rasterizer behaviour ===

void Rasteriser::AddModel(Model& model)
{
	_models.push_back(model);
}

void Rasteriser::TransformModel(int model_inx, Matrix transform)
{
	if (model_inx > _models.size() - 1)
		return;

	_models[model_inx].ApplyTransform(transform);
}

void Rasteriser::SetViewTransform(Matrix view)
{
	_view.SetMatrix(view);
}

void Rasteriser::SetCameraTransform(Matrix cam)
{
	_camera.SetMatrix(cam);
}

void Rasteriser::SetProjectionTransform(Matrix proj)
{
	_projection.SetMatrix(proj);
}

void Rasteriser::SetViewport(int left, int right, int top, int bottom)
{
	_viewportrect.left = left;
	_viewportrect.top = top;
	_viewportrect.right = right;
	_viewportrect.bottom = bottom;
	_viewport.SetViewport(left, right, top, bottom);
}

void Rasteriser::DrawViewportFrame(HDC hdc)
{
	POINT points[4];
	//Draw Viewport Frame
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	points[0].x = (int)_viewportrect.left;      points[0].y = (int)_viewportrect.top;
	points[1].x = (int)_viewportrect.right - 1; points[1].y = (int)_viewportrect.top;
	points[2].x = (int)_viewportrect.right - 1; points[2].y = (int)_viewportrect.bottom - 1;
	points[3].x = (int)_viewportrect.left;      points[3].y = (int)_viewportrect.bottom - 1;

	Polygon(hdc, points, 4);

	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);

	pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hdc, pen);

	int width = (int)_viewportrect.right - (int)_viewportrect.left;
	int height = (int)_viewportrect.bottom - (int)_viewportrect.top;

	MoveToEx(hdc, (int)_viewportrect.left, (int)_viewportrect.top + height / 2, NULL);
	LineTo(hdc, (int)_viewportrect.right - 1, (int)_viewportrect.top + height / 2);

	MoveToEx(hdc, (int)_viewportrect.left + width / 2, (int)_viewportrect.top, NULL);
	LineTo(hdc, (int)_viewportrect.left + width / 2, (int)_viewportrect.bottom - 1);


	SelectObject(hdc, oldPen);
	DeleteObject(pen);
}

void Rasteriser::DrawTextFrame(HDC hdc)
{
	POINT points[4];
	wchar_t outstr[256];

	//Draw Text Frame
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	points[0].x = 700;       points[0].y = 50;
	points[1].x = 700 + 520; points[1].y = 50;
	points[2].x = 700 + 520; points[2].y = 50 + 500;
	points[3].x = 700;       points[3].y = 50 + 500;

	Polygon(hdc, points, 4);

	// Draw lines within the border
	MoveToEx(hdc, 700, 50 + 220, nullptr);
	LineTo(hdc, 700 + 520, 50 + 220);

	MoveToEx(hdc, 700, 50 + 340, nullptr);
	LineTo(hdc, 700 + 520, 50 + 340);

	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);


	// Draw actions strings for user interface
	DrawString(hdc, 700 + 10, 50 + 10, 18, RGB(200, 50, 50), L"Actions:");
	DrawString(hdc, 700 + 10, 50 + 30, 18, RGB(134, 55, 55), L"- Move Camera with Arrow Keys or WASD Keys");
	DrawString(hdc, 700 + 10, 50 + 50, 18, RGB(134, 55, 55), L"- Left-click to enable/disable Backface Culling");
	DrawString(hdc, 700 + 10, 50 + 70, 18, RGB(134, 55, 55), L"- Right-click to enable/disable Depth Sorting");
	DrawString(hdc, 700 + 10, 50 + 90, 18, RGB(134, 55, 55), L"- Press Spacebar to change between Solid-Fill, Flath-Shading,");
	DrawString(hdc, 700 + 10, 50 + 110, 18, RGB(134, 55, 55), L"  and Wireframe");
	DrawString(hdc, 700 + 10, 50 + 130, 18, RGB(134, 55, 55), L"- Press R to reset camera's position");
	DrawString(hdc, 700 + 10, 50 + 150, 18, RGB(134, 55, 55), L"- Press L to change lighting mode");
	DrawString(hdc, 700 + 10, 50 + 170, 18, RGB(134, 55, 55), L"- Press P to change projection mode");
	DrawString(hdc, 700 + 10, 50 + 190, 18, RGB(134, 55, 55), L"- Press +/- to zoom in/out");



	// Draw information about state of application
	switch (_render_mode)
	{
	case  0: DrawString(hdc, 700 + 10, 50 + 230, 18, RGB(0, 0, 128), L"Draw: Wireframe");
		break;
	case  1: DrawString(hdc, 700 + 10, 50 + 230, 18, RGB(128, 0, 0), L"Draw: Solid-Fill");
		break;
	case  2: DrawString(hdc, 700 + 10, 50 + 230, 18, RGB(128, 0, 128), L"Draw: Flat-Shading");
		break;
	default: DrawString(hdc, 700 + 10, 50 + 230, 18, RGB(0, 128, 0), L"Draw: Wireframe");
		break;
	}

	if (_facecull)	DrawString(hdc, 700 + 10, 50 + 250, 18, RGB(0, 128, 0), L"Back-face Culling: ON");
	else DrawString(hdc, 700 + 10, 50 + 250, 18, RGB(255, 0, 0), L"Back-face Culling: OFF");

	if (_depthsort)	DrawString(hdc, 700 + 10, 50 + 270, 18, RGB(0, 128, 0), L"Depth Sorting: ON");
	else DrawString(hdc, 700 + 10, 50 + 270, 18, RGB(255, 0, 0), L"Depth Sorting: OFF");

	if (_ortho)	DrawString(hdc, 700 + 10, 50 + 290, 18, RGB(0, 204, 204), L"Projection: Ortho");
	else DrawString(hdc, 700 + 10, 50 + 290, 18, RGB(102, 51, 0), L"Projection: Perspective");

	if (_directional)	DrawString(hdc, 700 + 10, 50 + 310, 18, RGB(128, 128, 0), L"Light: Directional");
	else DrawString(hdc, 700 + 10, 50 + 310, 18, RGB(0, 192, 0), L"Light: Point-light");


	// Draw information for camera and lights position, also lights colours
	swprintf(outstr, 256, L"Camera: RotX=%.2f; RotY=%.2f; RotZ=%.2f; PosZ=%.2f;\0", _camRx, _camRy, _camRz, _camZ);
	DrawString(hdc, 700 + 10, 50 + 350, 18, RGB(203, 97, 5), (LPCTSTR)outstr);

	swprintf(outstr, 256, L"Light 1 Position: PosX=%.2f; PosY=%.2f; PosZ=%.2f;\0",
		_light1.GetLPos().GetX(), _light1.GetLPos().GetY(), _light1.GetLPos().GetZ());
	DrawString(hdc, 700 + 10, 50 + 370, 18, RGB(175, 164, 4), (LPCTSTR)outstr);

	swprintf(outstr, 256, L"Light 2 Position: PosX=%.2f; PosY=%.2f; PosZ=%.2f;\0",
		_light2.GetLPos().GetX(), _light2.GetLPos().GetY(), _light2.GetLPos().GetZ());
	DrawString(hdc, 700 + 10, 50 + 390, 18, RGB(175, 164, 4), (LPCTSTR)outstr);

	swprintf(outstr, 256, L"Light 3 Position: PosX=%.2f; PosY=%.2f; PosZ=%.2f;\0",
		_light3.GetLPos().GetX(), _light3.GetLPos().GetY(), _light3.GetLPos().GetZ());
	DrawString(hdc, 700 + 10, 50 + 410, 18, RGB(175, 164, 4), (LPCTSTR)outstr);

	swprintf(outstr, 256, L"Light 1 Color: La(%d, %d, %d); Ld(%d, %d, %d); Ls(%d, %d, %d);\0",
		GetRValue(_light1.GetColorLa()), GetGValue(_light1.GetColorLa()), GetBValue(_light1.GetColorLa()),
		GetRValue(_light1.GetColorLd()), GetGValue(_light1.GetColorLd()), GetBValue(_light1.GetColorLd()),
		GetRValue(_light1.GetColorLs()), GetGValue(_light1.GetColorLs()), GetBValue(_light1.GetColorLs()));
	DrawString(hdc, 700 + 10, 50 + 430, 18, RGB(3, 172, 76), (LPCTSTR)outstr);


	swprintf(outstr, 256, L"Light 2 Color: La(%d, %d, %d); Ld(%d, %d, %d); Ls(%d, %d, %d);\0",
		GetRValue(_light2.GetColorLa()), GetGValue(_light2.GetColorLa()), GetBValue(_light2.GetColorLa()),
		GetRValue(_light2.GetColorLd()), GetGValue(_light2.GetColorLd()), GetBValue(_light2.GetColorLd()),
		GetRValue(_light2.GetColorLs()), GetGValue(_light2.GetColorLs()), GetBValue(_light2.GetColorLs()));
	DrawString(hdc, 700 + 10, 50 + 450, 18, RGB(3, 172, 76), (LPCTSTR)outstr);

	swprintf(outstr, 256, L"Light 3 Color: La(%d, %d, %d); Ld(%d, %d, %d); Ls(%d, %d, %d);\0",
		GetRValue(_light3.GetColorLa()), GetGValue(_light3.GetColorLa()), GetBValue(_light3.GetColorLa()),
		GetRValue(_light3.GetColorLd()), GetGValue(_light3.GetColorLd()), GetBValue(_light3.GetColorLd()),
		GetRValue(_light3.GetColorLs()), GetGValue(_light3.GetColorLs()), GetBValue(_light3.GetColorLs()));
	DrawString(hdc, 700 + 10, 50 + 470, 18, RGB(3, 172, 76), (LPCTSTR)outstr);

}

void Rasteriser::DrawString(HDC hdc, int xPos, int yPos, int fSize, COLORREF textColor, LPCTSTR text)
{
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(fSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		//SetTextColor(hdc, RGB(255, 255, 255));
		SetTextColor(hdc, textColor);
		SetBkColor(hdc, RGB(255, 255, 255));

		// Display the text string.  
		TextOut(hdc, xPos, yPos, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}

void Rasteriser::RenderScene(HDC hdc)
{
	Model model;
	Vertex lightPos;

	float eX, eY, eZ; // Get the eye position
	eX = -_camera.GetM(0, 3);
	eY = -_camera.GetM(1, 3);
	eZ = -_camera.GetM(2, 3);

	DrawViewportFrame(hdc);

	for (size_t i = 0; i < _models.size(); i++)
	{
		model = _models[i];

		model.ApplyTransform(_camera);

		model.CalculateCenters();

		if (_depthsort)	model.ApplyDepthSorting();

		model.CalculateCenters();
		model.CalculateNormals(true);

		lightPos = _camera * _lPos1;
		_light1.SetLPos(lightPos);

		lightPos = _camera * _lPos2;
		_light2.SetLPos(lightPos);

		lightPos = _camera * _lPos3;
		_light3.SetLPos(lightPos);

		if (_render_mode == 2) model.CalcFlatShading(hdc, _light1, _light2, _light3, eX, eY, eZ, _ortho, _directional);

		model.ApplyTransform(_projection);
		model.Dehomogenise();

		model.CalculateCenters();
		model.CalculateNormals(true);

		if (_facecull)
		{
			model.MarkBackfaces(eX, eY, eZ, _ortho);
		}

		model.ApplyTransform(_viewport);

		switch (_render_mode)
		{
		case  0: model.DrawWireFrame(hdc);
			break;
		case  1: model.DrawSolidFill(hdc);
			break;
		case  2: model.DrawFlatShading(hdc);
			break;
		default: model.DrawWireFrame(hdc);
			break;
		}

	}

	DrawTextFrame(hdc);
}


void Rasteriser::InitializeScene(HWND _hWnd)
{
	// Load a model and add it in scene's model list
	Model MyModel;
	MD2Loader MyLoader;

	//Load and Add the first model
	MyLoader.LoadModel("fighter.md2", MyModel);
	MyModel.SetColorKa(RGB(45, 3, 3));
	MyModel.SetColorKd(RGB(157, 11, 11));
	MyModel.SetColorKs(RGB(186, 79, 79));
	//MyModel.SetShininess(76.8f);
	MyModel.SetShininess(100.8f);

	AddModel(MyModel);
	//Load and Add the second model
	MyLoader.LoadModel("policecar.md2", MyModel);
	MyModel.SetColorKa(RGB(31, 28, 8));
	MyModel.SetColorKd(RGB(97, 88, 23));
	MyModel.SetColorKs(RGB(223, 204, 53));
	//MyModel.SetShininess(83.2f);
	MyModel.SetShininess(110.8f);

	AddModel(MyModel);


	// Settin a model's transformation matrix to use to transform the models
	Matrix model_transform, transform;


	// Fighter Jet's Initialization
	model_transform.SetIdentity();
	transform.SetIdentity();
	transform.SetRotationZ(30);
	model_transform = transform * model_transform;
	transform.SetRotationY(-30);
	model_transform = transform * model_transform;
	transform.SetRotationX(30);
	model_transform = transform * model_transform;

	//transform.SetTranslate(100, 0, 0);
	//transform.SetTranslate(100, 100, 0);
	transform.SetTranslate(0, -35, 0);
	model_transform = transform * model_transform;


	transform.SetScale(0.5f, 0.5f, 0.5f);
	model_transform = transform * model_transform;

	// Transforming the first model in the model list
	TransformModel(0, model_transform);

	// Police car's Initialization
	model_transform.SetIdentity();
	transform.SetIdentity();
	transform.SetRotationY(-90);
	model_transform = transform * model_transform;

	transform.SetTranslate(0, 35, 0);
	model_transform = transform * model_transform;


	transform.SetScale(0.5f, 0.5f, 0.5f);
	model_transform = transform * model_transform;

	// Transforming the second model in the model list
	TransformModel(1, model_transform);


	// Set a specific client area of the window as the screen viewport
	SetViewport(50, 50 + 600, 50, 50 + 600);

	// Set the camera matrix
	_camRx = 0.0f;
	_camRy = 0.0f;
	_camRz = 0.0f;
	_camZ = 200.0f;
	Matrix camera;
	camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
	SetCameraTransform(camera);

	// Setting up the projection matrix
	_ortho = false;
	Matrix projection;
	if (_ortho) projection.SetOrtho(5.0f);
	else projection.SetPerspective(200.0f, 1.0f);

	// Setting up the viewvolume matrix
	Matrix volume;
	volume.SetViewvolume(-200, +200, -200, +200, +200, -200);

	// Setting up the overall view matrixmatrix
	Matrix view;
	view = volume * projection;
	SetProjectionTransform(view);

	_render_mode = 0;
	_depthsort = true;
	_facecull = true;
	_directional = true;

	// Light1 Initialization
	_light1.SetDirectional(_directional);
	_lPos1.SetX(-100.0f);
	_lPos1.SetY(100.0f);
	_lPos1.SetZ(100.0f);
	_light1.SetLPos(_lPos1);

	_light1.SetColorLa(RGB(128, 128, 128));
	_light1.SetColorLd(RGB(255, 255, 255));
	_light1.SetColorLs(RGB(255, 255, 255));


	// Light2 Initialization
	_light2.SetDirectional(_directional);
	_lPos2.SetX(-100.0f);
	_lPos2.SetY(100.0f);
	_lPos2.SetZ(100.0f);
	_light2.SetLPos(_lPos2);

	_light2.SetColorLa(RGB(128, 128, 128));
	_light2.SetColorLd(RGB(255, 255, 255));
	_light2.SetColorLs(RGB(255, 255, 255));


	// Light3 Initialization
	_light3.SetDirectional(_directional);
	_lPos3.SetX(-100.0f);
	_lPos3.SetY(100.0f);
	_lPos3.SetZ(100.0f);
	_light3.SetLPos(_lPos3);

	_light3.SetColorLa(RGB(128, 128, 128));
	_light3.SetColorLd(RGB(255, 255, 255));
	_light3.SetColorLs(RGB(255, 255, 255));


}

void Rasteriser::SetRenderParams(int action)
{
	Matrix camera, projection, volume, view;

	switch (action)
	{
	case  0: if (_render_mode < 2) _render_mode = _render_mode + 1; else _render_mode = 0;
		break;
	case  1: _ortho = !_ortho;

		if (_ortho) projection.SetOrtho(6.0f);
		else projection.SetPerspective(200.0f, 1.0f);

		volume.SetViewvolume(-200, +200, -200, +200, +200, -200);
		view = volume * projection;
		SetProjectionTransform(view);
		break;
	case  2: _facecull = !_facecull;
		break;
	case  3: _depthsort = !_depthsort;
		break;
	case  4: _directional = !_directional;
		_light1.SetDirectional(_directional);
		_light2.SetDirectional(_directional);
		_light3.SetDirectional(_directional);
		break;
	case 10:	 _camRx = 0.0f;
		_camRy = 0.0f;
		_camRz = 0.0f;
		_camZ = 200.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 11:	 _camRx = (int)(_camRx + 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 12:	_camRx = (int)(_camRx - 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 13:	_camRy = (int)(_camRy + 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 14:	_camRy = (int)(_camRy - 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 15:	_camRz = (int)(_camRz + 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 16:	_camRz = (int)(_camRz - 10.0f) % 360;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 17:	_camZ = _camZ + 10.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	case 18:	_camZ = _camZ - 10.0f;
		camera.SetCamera(_camRx, _camRy, _camRz, 0, 0, _camZ);
		SetCameraTransform(camera);
		break;
	}
}



