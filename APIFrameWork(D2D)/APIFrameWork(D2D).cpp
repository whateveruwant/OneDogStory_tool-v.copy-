// APIFrameWork(D2D).cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "APIFrameWork(D2D).h"
#include "mainGame.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

HWND hWnd;
D2D1_POINT_2F _ptMouse = { 0, 0 };

ID2D1Factory* Factory;
IDWriteFactory* WriteFactory;
IDWriteTextFormat* TextFormat;
IDWriteTextLayout* textLayout;
ID2D1HwndRenderTarget* Rt;
ID2D1SolidColorBrush* Brush;
IWICImagingFactory *imagingFactory;
ID2D1BitmapRenderTarget* CRt;
ID2D1Bitmap* pBitmap;
ID2D1SolidColorBrush*		WhiteSmokeBrush;
ID2D1SolidColorBrush*		 BlackBrush;
ID2D1SolidColorBrush*		 WhiteBrush;
ID2D1SolidColorBrush*		 AquaBrush;
ID2D1SolidColorBrush*		 BlueBrush;
ID2D1RadialGradientBrush*	m_pRadialGradientBrush;

//�������� or ������ �ҷ�����
bool tile_init_select;
//�̹��� �ε� �ѹ���
bool loading_game_first;


//���ΰ��� Ŭ���� ����
mainGame _mg;

//==================================================================
//		## �Լ� ���漱�� ##
//==================================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void clean();

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
)
{

	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.


	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	CoInitialize(0);

	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &Factory);
	CoCreateInstance(CLSID_WICImagingFactory,
		0, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&imagingFactory));

	// Register the window class.
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = L"ONE DOG STORY's TOOL";

	RegisterClassEx(&wcex);


	// Because the CreateWindow function takes its size in pixels,
	// obtain the system DPI and use it to scale the window size.
	FLOAT dpiX, dpiY;

	// The factory returns the current system DPI. This is also the value it will use
	// to create its own windows.
	Factory->GetDesktopDpi(&dpiX, &dpiY);

	// Create the window.
	//m_hwnd = CreateWindow(
	//	WINNAME,				//������ Ŭ������ �̸�
	//	WINNAME,				//������ Ÿ��Ʋ�� �̸�
	//	WS_OVERLAPPEDWINDOW,	//������ ��Ÿ��
	//	CW_USEDEFAULT,			//������ ȭ����ǥ x
	//	CW_USEDEFAULT,			//������ ȭ����ǥ y
	//	static_cast<UINT>(ceil(1280.f * dpiX / 96.f)),				//������ ȭ����ǥ width
	//	static_cast<UINT>(ceil(720.f * dpiY / 96.f)),				//������ ȭ����ǥ height
	//	NULL,					//�θ� ������
	//	NULL,					//�޴��ڵ�
	//	HINST_THISCOMPONENT,	//�ν��Ͻ� ����
	//	NULL					//������ �� �ڽ� �����츦 �����ϸ� �������ֵ� �׷��� ������ NULL
	//	);

	hWnd = CreateWindow(
		L"ONE DOG STORY's TOOL",
		L"ONE DOG STORY's TOOL",
		WS_OVERLAPPEDWINDOW,
		WINSTARTX,
		WINSTARTY,
		WINSIZEX,
		WINSIZEY,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		NULL
	);

	RECT clrc;
	GetClientRect(hWnd, &clrc);
	
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = WINSIZEX_NUM + (WINSIZEX_NUM - (clrc.right - clrc.left));
	rc.bottom = WINSIZEY_NUM + (WINSIZEY_NUM - (clrc.bottom - clrc.top));
	AdjustWindowRect(&rc, WINSTYLE, false);
	
	SetWindowPos(hWnd, NULL, WINSTARTX, WINSTARTY, (rc.right - rc.left), (rc.bottom - rc.top), FALSE);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);


	if (FAILED(_mg.init()))
	{
		return 0;
	}




	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�ʴ� 60���������� ������Ʈ ���Ѷ�
			TIMEMANAGER->update(60.0f);

			_mg.update();
			_mg.render();
		}
	}


	clean();

	UnregisterClass(WINNAME, HINST_THISCOMPONENT);

	return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//���ΰ��� Ŭ������ ���� ���ν������� ó������ ���� ������ �޼����� ó�����ش�
	return _mg.MainProc(hWnd, iMessage, wParam, lParam);
}

void clean()
{
	_mg.release();
	Brush->Release();
	Rt->Release();
	TextFormat->Release();
	WriteFactory->Release();
	Factory->Release();
	CRt->Release();
	imagingFactory->Release();
	WhiteSmokeBrush->Release();
	BlackBrush->Release();
	WhiteBrush->Release();
	m_pRadialGradientBrush->Release();
}