#include "stdafx.h"
#include "image.h"

image::image() : _imageInfo(NULL), _fileName(NULL), _alpha(1.0f)
{
}


image::~image()
{
}

//이미지 파일로 초기화
HRESULT image::init(ID2D1RenderTarget* RenderTarget, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter)
{
	HRESULT hr = S_OK;
	hr = D2DLoadBitmap(fileName, RenderTarget, imagingFactory, &_bitmap);
	if (FAILED(hr)) {
		return hr;
	}

	//재초기화 방지용, 이미지 정보에 값이 들어 있다면 릴리즈먼저 해줄것!
	if (_imageInfo != NULL) release();
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	int len = wcslen(fileName);
	_fileName = new TCHAR[len + 1];
	wcsncpy_s(_fileName, len + 1, fileName, len + 1);

	_isCenter = isCenter;
	return S_OK;
}

//프레임 이미지 파일로 초기화
HRESULT image::init(ID2D1RenderTarget* RenderTarget, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter)
{
	HRESULT hr = S_OK;
	hr = D2DLoadBitmap(fileName, RenderTarget, imagingFactory, &_bitmap);
	if (FAILED(hr)) {
		return hr;
	}

	//재초기화 방지용, 이미지 정보에 값이 들어 있다면 릴리즈먼저 해줄것!
	if (_imageInfo != NULL) release();
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;

	int len = wcslen(fileName);
	_fileName = new TCHAR[len + 1];
	wcsncpy_s(_fileName, len + 1, fileName, len + 1);

	_isCenter = isCenter;
	return S_OK;
}

//==================================================================
//		## 일반렌더 ##
//==================================================================
void image::render(ID2D1RenderTarget* RenderTarget)
{
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			_imageInfo->x - _imageInfo->width / 2,
			_imageInfo->y - _imageInfo->height / 2,
			_imageInfo->x + _imageInfo->width / 2,
			_imageInfo->y + _imageInfo->height / 2), _alpha);
	}
	else {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(_imageInfo->x, _imageInfo->y,
				_imageInfo->x + _imageInfo->width,
				_imageInfo->y + _imageInfo->height), _alpha);
	}
}

void image::render(ID2D1RenderTarget* RenderTarget, float x, float y)
{
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			x - _imageInfo->width / 2,
			y - _imageInfo->height / 2,
			x + _imageInfo->width / 2,
			y + _imageInfo->height / 2), _alpha);
	}
	else {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(x, y,
				x + _imageInfo->width,
				y + _imageInfo->height), _alpha);
	}
}

void image::render(ID2D1RenderTarget* RenderTarget, float destX, float destY, float destWidth, float destHeight, float sorceX, float sorceY, float sourceWidth, float sourceHeight)
{
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			destX,
			destY,
			destX + destWidth,
			destY + destHeight), _alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(
				sorceX - sourceWidth/2,
				sorceY - sourceHeight/2,
				sorceX + sourceWidth/2,
				sorceY + sourceHeight/2));
	}
	else {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			destX,
			destY,
			destX + destWidth,
			destY + destHeight), _alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(
				sorceX,
				sorceY,
				sorceX + sourceWidth,
				sorceY + sourceHeight));
	}
}

void image::renderByMask(ID2D1RenderTarget * RenderTarget)
{
	if (_isCenter) {
		RenderTarget->FillOpacityMask(_bitmap, BlackBrush, D2D1_OPACITY_MASK_CONTENT_GRAPHICS,  &D2D1::RectF(
			_imageInfo->x - _imageInfo->width / 2,
			_imageInfo->y - _imageInfo->height / 2,
			_imageInfo->x + _imageInfo->width / 2,
			_imageInfo->y + _imageInfo->height / 2));
	}
	else {
		RenderTarget->FillOpacityMask(_bitmap, BlackBrush, D2D1_OPACITY_MASK_CONTENT_GRAPHICS, &D2D1::RectF(_imageInfo->x, _imageInfo->y,
			_imageInfo->x + _imageInfo->width,
			_imageInfo->y + _imageInfo->height));
	}
}

//==================================================================
//		## 일반렌더(배율) ##
//==================================================================
void image::magnificateRender(ID2D1RenderTarget * RenderTarget, float magnification)
{
	D2D1_POINT_2F tmp = CAMERAMANAGER->getAbsPosition({_imageInfo->x , _imageInfo->y});
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			tmp.x - (_imageInfo->width*magnification) / 2,
			tmp.y - (_imageInfo->height*magnification) / 2,
			tmp.x + (_imageInfo->width*magnification) / 2,
			tmp.y + (_imageInfo->height*magnification) / 2), _alpha);
	}
	else {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(tmp.x, tmp.y,
				tmp.x + _imageInfo->width *magnification,
				tmp.y + _imageInfo->height*magnification), _alpha);
	}
}

void image::magnificateRender(ID2D1RenderTarget * RenderTarget, float magnification, float x, float y)
{
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			x - (_imageInfo->width*magnification) / 2,
			y - (_imageInfo->height*magnification) / 2,
			x + (_imageInfo->width*magnification) / 2,
			y + (_imageInfo->height*magnification) / 2), _alpha);
	}
	else {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(x, y,
				x + _imageInfo->width *magnification,
				y + _imageInfo->height*magnification), _alpha);
	}
}

//==================================================================
//		## 알파렌더 ##
//==================================================================
void image::alphaRender(ID2D1RenderTarget* RenderTarget, float alpha)
{
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			_imageInfo->x - _imageInfo->width / 2,
			_imageInfo->y - _imageInfo->height / 2,
			_imageInfo->x + _imageInfo->width / 2,
			_imageInfo->y + _imageInfo->height / 2), alpha);
	}
	else {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(_imageInfo->x, _imageInfo->y,
				_imageInfo->x + _imageInfo->width,
				_imageInfo->y + _imageInfo->height), alpha);
	}
}

void image::alphaRender(ID2D1RenderTarget * RenderTarget, float alpha, float magnification, float x, float y)
{
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
			x - (_imageInfo->width*magnification) / 2,
			y - (_imageInfo->height*magnification) / 2,
			x + (_imageInfo->width*magnification) / 2,
			y + (_imageInfo->height*magnification) / 2), alpha);
	}
	else {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(x, y,
				x + _imageInfo->width *magnification,
				y + _imageInfo->height*magnification), alpha);
	}
}

//==================================================================
//		## 프레임렌더 ##
//==================================================================
void image::frameRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}
	
	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(_imageInfo->x - _imageInfo->frameWidth / 2,
				_imageInfo->y - _imageInfo->frameHeight / 2,
				_imageInfo->x + _imageInfo->frameWidth / 2,
				_imageInfo->y + _imageInfo->frameHeight / 2),
			_alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF((_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*_imageInfo->currentFrameX,
			(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*_imageInfo->currentFrameY,
				(_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*(_imageInfo->currentFrameX + 1),
				(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*(_imageInfo->currentFrameY + 1)));
	}
	else {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(_imageInfo->x, _imageInfo->y,
			_imageInfo->x + _imageInfo->frameWidth,
			_imageInfo->y + _imageInfo->frameHeight), _alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF((_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*_imageInfo->currentFrameX,
			(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*_imageInfo->currentFrameY,
				(_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*(_imageInfo->currentFrameX + 1),
				(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*(_imageInfo->currentFrameY + 1)));
	}
}

//==================================================================
//		## 알파프레임렌더 ##
//==================================================================
void image::frameAlphaRender(ID2D1RenderTarget * RenderTarget, int currentFrameX, int currentFrameY, float alpha)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	if (_isCenter) {
		RenderTarget->DrawBitmap(_bitmap,
			D2D1::RectF(_imageInfo->x - _imageInfo->frameWidth / 2,
				_imageInfo->y - _imageInfo->frameHeight / 2,
				_imageInfo->x + _imageInfo->frameWidth / 2,
				_imageInfo->y + _imageInfo->frameHeight / 2),
			alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF((_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*_imageInfo->currentFrameX,
			(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*_imageInfo->currentFrameY,
				(_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*(_imageInfo->currentFrameX + 1),
				(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*(_imageInfo->currentFrameY + 1)));
	}
	else {
		RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(_imageInfo->x, _imageInfo->y,
			_imageInfo->x + _imageInfo->frameWidth,
			_imageInfo->y + _imageInfo->frameHeight), alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF((_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*_imageInfo->currentFrameX,
			(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*_imageInfo->currentFrameY,
				(_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*(_imageInfo->currentFrameX + 1),
				(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*(_imageInfo->currentFrameY + 1)));
	}
}

void image::bottomRender(ID2D1RenderTarget * RenderTarget)
{
	RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
		_imageInfo->x - _imageInfo->frameWidth / 2,
		_imageInfo->y - _imageInfo->frameHeight,
		_imageInfo->x + _imageInfo->frameWidth / 2,
		_imageInfo->y), _alpha);
}

void image::bottomAlphaRender(ID2D1RenderTarget * RenderTarget, float alpha)
{
	RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
		_imageInfo->x - _imageInfo->frameWidth / 2,
		_imageInfo->y - _imageInfo->frameHeight,
		_imageInfo->x + _imageInfo->frameWidth / 2,
		_imageInfo->y),	alpha);
}

void image::bottomFrameRender(ID2D1RenderTarget * RenderTarget, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
		_imageInfo->x - _imageInfo->frameWidth / 2,
		_imageInfo->y - _imageInfo->frameHeight,
		_imageInfo->x + _imageInfo->frameWidth / 2,
		_imageInfo->y)	, _alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF((_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*_imageInfo->currentFrameX,
		(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*_imageInfo->currentFrameY,
		(_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*(_imageInfo->currentFrameX + 1),
		(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*(_imageInfo->currentFrameY + 1)));
}

void image::bottomFrameAlphaRender(ID2D1RenderTarget * RenderTarget, int currentFrameX, int currentFrameY, float alpha)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
		_imageInfo->x - _imageInfo->frameWidth / 2, 
		_imageInfo->y - _imageInfo->frameHeight,
		_imageInfo->x + _imageInfo->frameWidth / 2,
		_imageInfo->y), alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF((_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*_imageInfo->currentFrameX,
		(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*_imageInfo->currentFrameY,
		(_bitmap->GetSize().width / (_imageInfo->maxFrameX + 1))*(_imageInfo->currentFrameX + 1),
		(_bitmap->GetSize().height / (_imageInfo->maxFrameY + 1))*(_imageInfo->currentFrameY + 1)));
}

void image::aniRender(ID2D1RenderTarget * RenderTarget, int destX, int destY, animation* ani)
{
	RenderTarget->DrawBitmap(_bitmap, D2D1::RectF(
		destX - ani->getFrameWidth() / 2,
		destY - ani->getFrameHeight() / 2,
		destX + ani->getFrameWidth() / 2,
		destY + ani->getFrameHeight() / 2), _alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(ani->getFramePos().x,
			ani->getFramePos().y,
			ani->getFramePos().x+ ani->getFrameWidth(),
			ani->getFramePos().y+ ani->getFrameHeight()));
}

void image::release()
{
	if (_imageInfo)
	{
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);
		//SAFE_RELEASE(_bitmap);
	}
}