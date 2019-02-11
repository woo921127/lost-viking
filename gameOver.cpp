#include "stdafx.h"
#include "gameOver.h"


gameOver::gameOver()
{
}


gameOver::~gameOver()
{
}

HRESULT gameOver::init()
{
	_over = IMAGEMANAGER->addFrameImage("gameOver", "image/gameOver.bmp", 16000, 609, 16, 1, true, RGB(255, 0, 255));

	_frameX = 0;
	_oldTime = GetTickCount();



	return S_OK;
}

void gameOver::release()
{
}

void gameOver::update()
{
	if (GetTickCount() - _oldTime >= 0.5 * 1000)
	{
		if (_frameX < 15)
		{
			_frameX++;
		}
		_oldTime = GetTickCount();
	}




}

void gameOver::render()
{
	_over->frameRender(getMemDC(), 0, 0, _frameX, 0);
}
