#pragma once

#include <windows.h>
#include "resource.h"
#include "Model.h"
#include "Matrix_4D.h"

class Framework
{
public:
	Framework();
	virtual ~Framework();

	int Run(HINSTANCE hInstance, int nCmdShow);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//virtual void AddModel(Model & model);
	//virtual void TransformModel(int model_inx, Matrix transform);
	//virtual void SetViewTransform(Matrix view);
	//virtual void SetCameraTransform(Matrix cam);
	//virtual void SetProjectionTransform(Matrix proj);
	//virtual void SetViewport(int left, int right, int top, int bottom);

	virtual void InitializeScene(HWND _hWnd);
	virtual void RenderScene(HDC hdc);

	virtual void SetRenderParams(int action);


private:
	HINSTANCE	_hInstance;
	HWND		_hWnd;

	bool InitialiseMainWindow(int nCmdShow);
	int MainLoop();

};