#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}


HRESULT playGround::init()
{
	gameNode::init(true);

	_sound = new sound;
	_sound->init();

	_menu = new menu;
	_menu->init();
	SCENEMANAGER->addScene("menu", _menu);
	_stage1 = new stage1;
	SCENEMANAGER->addScene("stage1", _stage1);

	_gameOver = new gameOver;
	SCENEMANAGER->addScene("gameOver", _gameOver);

	_gameClear = new gameClear;
	SCENEMANAGER->addScene("gameClear", _gameClear);

	SCENEMANAGER->changeScene("menu");

	_gameOverCount = 0;
	_isGameOver = false;
	_isGameClear = false;
	_isMenu = false;
	
	
	return S_OK;
}


void playGround::release()
{
	gameNode::release();

	SAFE_DELETE(_sound);

	SCENEMANAGER->release();

}


void playGround::update()
{
	gameNode::update();
	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE)) PostQuitMessage(0);

	

	if (_menu->getMenuQuit() && !_isMenu)
	{
		_isMenu = true;
		SCENEMANAGER->changeScene("stage1");
	}
	
	if (_menu->getMenuQuit())
	{
		if (_isGameOver == false && _isGameClear != true)
		{
			if (_stage1->getPlayerManager()->getEricDead() == true &&
				_stage1->getPlayerManager()->getBaleogDead() == true &&
				_stage1->getPlayerManager()->getOlafDead() == true)
			{
				_gameOverCount++;
				if (_gameOverCount == 80)
				{
					_isGameOver = true;
					SCENEMANAGER->changeScene("gameOver");
				}
			}
		}
		if (_isGameClear == false && _isGameOver != true)
		{

			_end = RectMake(2580 - _stage1->getPlayerManager()->getCamera()->getCameraX(), 1130 - _stage1->getPlayerManager()->getCamera()->getCameraY(), 150, 150);
			if (IntersectRect(&temp, &_stage1->getPlayerManager()->getEricRect(), &_end) && IntersectRect(&temp, &_stage1->getPlayerManager()->getBaleogRect(), &_end) && IntersectRect(&temp, &_stage1->getPlayerManager()->getOlafRect(), &_end))
			{
				_isGameClear = true;

				SCENEMANAGER->changeScene("gameClear");
				SOUNDMANAGER->play("ending");
			}
		}
	}

	SCENEMANAGER->update();

	


}



void playGround::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	//===========================================================
	SCENEMANAGER->render();
	//Rectangle(getMemDC(), _end);


	TIMEMANAGER->render(getMemDC());
	//===========================================================
	this->getBackBuffer()->render(getHDC(), 0, 0);
}

