#include "stdafx.h"
#include "gameClear.h"


gameClear::gameClear()
{
}


gameClear::~gameClear()
{
}

HRESULT gameClear::init()
{
	_bg = IMAGEMANAGER->addImage("1", "image/ending_6.bmp", 1000, 609, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("2", "image/ending_1.bmp", 1000, 609, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("3", "image/ending_3.bmp", 1000, 609, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("4", "image/ending_4.bmp", 1000, 609, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("5", "image/ending_5.bmp", 1000, 609, true, RGB(255, 0, 255));

	_worldTime = TIMEMANAGER->getWorldTime();
	_nextTime = 5.0f;
	picture = 0;

	return S_OK;
}

void gameClear::release()
{
}

void gameClear::update()
{
	if(picture < 4)
	{
		if (_nextTime + _worldTime <= TIMEMANAGER->getWorldTime())
		{
			picture += 1;

			_worldTime = TIMEMANAGER->getWorldTime();
		}
	}

	switch (picture)
	{
	case 1:
		_bg = IMAGEMANAGER->findImage("2");
		break;
	case 2:
		_bg = IMAGEMANAGER->findImage("3");
		break;
	case 3:
		_bg = IMAGEMANAGER->findImage("4");
		break;
	case 4:
		_bg = IMAGEMANAGER->findImage("5");
		break;
	}


}

void gameClear::render()
{
	_bg->render(getMemDC(), 0, 0);

}
