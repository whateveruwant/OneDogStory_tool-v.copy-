// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib, "dwrite.lib")	
#pragma comment(lib, "windowscodecs.lib")


#include "targetver.h"
#include <tchar.h>
#include <locale.h>


#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>


//Direct Header Files:
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>


// 자주사용하는 STL
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <map>


#ifdef UNICODE
typedef std::wstring tstring;
typedef wchar_t tchar;
#else
typedef std::string tstring;
typedef char tchar;
#endif


//==================================================================
//		## 내가 만든 헤더파일을 이곳에 추가한다 ##
//==================================================================
#include "util.h"
#include "commonMacroFunction.h"
#include "keyManager.h"
#include "timeManager.h"
#include "imageManager.h"
#include "iniData.h"
#include "soundManager.h"
#include "randomFunction.h"
#include "cameraManager.h"

//==================================================================
//		## 싱글톤을 추가한다 ##
//==================================================================
#define KEYMANAGER keyManager::getSingleton()
#define RND randomFunction::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define INIDATA iniData::getSingleton()
#define CAMERAMANAGER cameraManager::getSingleton()

//==================================================================
//		## 네임스페이스 추가한다 ##
//==================================================================
using namespace MY_UTIL;


//==================================================================
//		## 디파인문 ## (윈도우창 초기화)
//==================================================================
#define WINNAME (LPTSTR)(TEXT("ONE DOG STORY's TOOL"))
#define WINSTARTX 180
#define WINSTARTY 90
#define WINSIZEX_NUM 1280.f
#define WINSIZEY_NUM 720.f
#define WINSIZEX	static_cast<UINT>(ceil(WINSIZEX_NUM * dpiX / 94.9f))
#define WINSIZEY	static_cast<UINT>(ceil(WINSIZEY_NUM * dpiY / 91.3f))
#define WINSTYLE WS_CAPTION | WS_SYSMENU
#define LADDER_SPEED SPEED * 48 / 64
#define EFFECTSOUNDVOLUME 0.5f

typedef D2D_RECT_F RECTF;
typedef D2D1_ELLIPSE ELLIPSEF;


//==================================================================
//		## 메크로함수 ## (클래스에서 동적할당된 부분 해제시 사용한다)
//==================================================================
#define SAFE_DELETE(p)			{if(p) {delete(p); p = NULL;}}
#define SAFE_DELETE_ARRAY(p)		{if(p) {delete[] p; p = NULL;}}
#define SAFE_RELEASE(p)			{ if(p) { p->release(); p = NULL;} }


//==================================================================
//		## 전역변수 ## (클래스에서 동적할당된 부분 해제시 사용한다)
//==================================================================
//extern HINSTANCE _hInstance;
extern D2D1_POINT_2F _ptMouse;
extern HWND hWnd;
extern ID2D1Factory* Factory;
extern ID2D1SolidColorBrush* Brush;
extern IDWriteFactory* WriteFactory;
extern IDWriteTextFormat* TextFormat;
extern IDWriteTextLayout* textLayout;
extern ID2D1HwndRenderTarget* Rt;
extern IWICImagingFactory *imagingFactory;
extern ID2D1BitmapRenderTarget* CRt;
extern ID2D1Bitmap* pBitmap;
extern ID2D1RadialGradientBrush* m_pRadialGradientBrush;
extern ID2D1SolidColorBrush* WhiteSmokeBrush;
extern ID2D1SolidColorBrush* BlackBrush;
extern ID2D1SolidColorBrush* WhiteBrush;
extern ID2D1SolidColorBrush* AquaBrush;
extern ID2D1SolidColorBrush* BlueBrush;

//==================================================================
//		## 다이렉트용 ##
//==================================================================
template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}


#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif