#include "stdafx.h"
#include "stage1.h"


stage1::stage1()
{
}


stage1::~stage1()
{
}

HRESULT stage1::init()
{
	IMAGEMANAGER->addImage("backGround", "image/map.bmp", 3000, 2240, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("pixel", "image/pixel.bmp", 3000, 2240, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("item", "image/item.bmp", 330, 55, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("bubble", "image/bubble.bmp", 900, 180, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("dari", "image/dari.bmp", 2200, 436, 10, 2, true, RGB(255, 0, 255));

	_playerManager = new playerManager;
	_playerManager->init();

	_redKey = new item;
	_redLock = new item;
	_dari = new item;
	_banana = new item;
	_meat = new item;

	_bubble = new item;
	_bubble->init();

	//아이템
	_redKey->getPlayerManagerLinkAddress(_playerManager);
	_playerManager->getItemLinkAddress(_redKey);
	_redLock->getPlayerManagerLinkAddress(_playerManager);
	_playerManager->getItemLinkAddress(_redLock);
	_dari->getPlayerManagerLinkAddress(_playerManager);
	_playerManager->getItemLinkAddress(_dari);
	_banana->getPlayerManagerLinkAddress(_playerManager);
	_playerManager->getItemLinkAddress(_banana);
	_meat->getPlayerManagerLinkAddress(_playerManager);
	_playerManager->getItemLinkAddress(_meat);

	//버블
	_bubble->getPlayerManagerLinkAddress(_playerManager);
	_playerManager->getItemLinkAddress(_bubble);

	//ui
	_ui = new ui;
	_ui->init();

	_uiChange = false;

	_bubbleTime = 5.0f;
	_worldTime = TIMEMANAGER->getWorldTime();

	_redKey->createItem("item", 2930, 390, REDKEY, 0);
	_redKey->createItem("item", 2450, 220, REDKEY, 0);
	_redKey->createItem("item", 1450, 230, REDKEY, 0);
	_redLock->createItem("item", 1100, 760, REDLOCK, 0);
	_dari->createItem("dari", 870, 675, 0, 0);
	_banana->createItem("item", 1435, 1670, BANANA, 0);
	_banana->createItem("item", 2300, 262, BANANA, 0);
	_meat->createItem("item", 2880, 2000, MEAT, 0);
	_meat->createItem("item", 2370, 242, MEAT, 0);
	_meat->createItem("item", 2520, 242, MEAT, 0);

	_redKeyOn = false;
	_oldTime = GetTickCount();

	return S_OK;
}

void stage1::release()
{
	SAFE_DELETE(_playerManager);
	SAFE_DELETE(_redKey);
	SAFE_DELETE(_redLock);
	SAFE_DELETE(_dari);
	SAFE_DELETE(_bubble);
	SAFE_DELETE(_banana);
	SAFE_DELETE(_meat);
	SAFE_DELETE(_ui);
}

void stage1::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		if (_uiChange) _uiChange = false;
		else _uiChange = true;
	}

	if (!_uiChange)
	{
		_playerManager->update();
		//픽셀 보이게하는 함수
		viewPixel();
		_redKey->update();
		_redLock->update();
		_dari->update();
		_banana->update();
		_meat->update();
		_bubble->update();
		createBubble();
		itemCollision();
		dariOn();
	}
	characterCollision();
	_ui->update(_playerManager->getChoice(), _uiChange);
	_ui->setEricDead(_playerManager->getEricDead());
	_ui->setBaleogDead(_playerManager->getBaleogDead());
	_ui->setOlafDead(_playerManager->getOlafDead());
	_ui->profileUpdate(_playerManager->getChoice());

	

	

	//다리 움직이기
	//if (KEYMANAGER->isOnceKeyDown(VK_NUMPAD9))
	//{
	//	for (int i = 0; i < _clockDari->getVItem().size(); ++i)
	//	{
	//		(*_clockDari->getVItemAddress())[i].frameX += 1;
	//	}
	//}
	//if (KEYMANAGER->isOnceKeyDown(VK_NUMPAD8))
	//{
	//	for (int i = 0; i < _clockDari->getVItem().size(); ++i)
	//	{
	//		(*_clockDari->getVItemAddress())[i].frameX -= 1;
	//	}
	//}
}

void stage1::render()
{
	IMAGEMANAGER->render("backGround", getMemDC(), 0, 0, _playerManager->getCamera()->getCameraX(), _playerManager->getCamera()->getCameraY(), WINSIZEX, WINSIZEY);

	if (_isPixel)
	{
		IMAGEMANAGER->render("pixel", getMemDC(), 0, 0, _playerManager->getCamera()->getCameraX(), _playerManager->getCamera()->getCameraY(), WINSIZEX, WINSIZEY);
	}
	_redKey->render();
	_redLock->render();
	_dari->render();
	_banana->render();
	_meat->render();
	_bubble->render();
	_playerManager->render();
	_ui->render();

	//char str[100];
	//sprintf_s(str, "플레이어 x : %d", _playerManager->getEricDead());
	//TextOut(getMemDC(), 300, 30, str, strlen(str));
	//sprintf_s(str, "플레이어 y : %f", _playerManager->getEricY());
	//TextOut(getMemDC(), 300, 40, str, strlen(str));
	//sprintf_s(str, "카메라 X : %f", _playerManager->getCamera()->getCameraX());
	//TextOut(getMemDC(), 300, 60, str, strlen(str));
}

void stage1::viewPixel()
{
	//픽셀보이기용
	if (KEYMANAGER->isOnceKeyDown(VK_NUMPAD0))
	{
		if (_isPixel)
		{
			_isPixel = false;
		}
		else
		{
			_isPixel = true;
		}
	}
}

void stage1::createBubble()
{
	//_bubbleTime 설정한 시간 마다 버블 생성
	if (_bubbleTime + _worldTime <= TIMEMANAGER->getWorldTime())
	{
		_worldTime = TIMEMANAGER->getWorldTime();

		_bubble->createItem("bubble", 1666, 2060, 0, 0);
	}

	_bubbleFrameTime++;
	for (int i = 0; i < _bubble->getVItem().size(); ++i)
	{
		//버블이 730높이보다 크면 높이가 감소되며 프레임이 0 1 반복
		if (_bubble->getVItem()[i].y > 730 && _bubbleFrameTime % 50 < 25)
		{
			(*_bubble->getVItemAddress())[i].y -= 2;
			(*_bubble->getVItemAddress())[i].frameX = 0;
		}
		else if (_bubble->getVItem()[i].y > 730 && _bubbleFrameTime % 50 >= 25)
		{
			(*_bubble->getVItemAddress())[i].y -= 2;
			(*_bubble->getVItemAddress())[i].frameX = 1;
		}
		//버블 높이가 730보다 작아지면 시간에 따라 프레임이 증가하며 삭제된다.
		else if (_bubble->getVItem()[i].y <= 730)
		{
			_bubbleRemoveTime++;
			if (_bubbleRemoveTime < 10)
			{
				(*_bubble->getVItemAddress())[i].frameX = 2;
			}
			else if (_bubbleRemoveTime < 20)
			{
				(*_bubble->getVItemAddress())[i].frameX = 3;
			}
			else if (_bubbleRemoveTime < 30)
			{
				(*_bubble->getVItemAddress())[i].frameX = 4;
			}
			else if (_bubbleRemoveTime < 35)
			{
				(*_bubble->getVItemAddress())[i].isLive = false;
				_bubbleRemoveTime = 0;
			}
		}
	}
}

void stage1::itemCollision()
{
	//레드키 충돌
	for (int i = 0; i < _redKey->getVItem().size(); ++i)
	{
		//에릭
		if (IntersectRect(&temp, &(*_redKey->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			_currentFrameX = (*_redKey->getVItemAddress())[i].frameX + 1;
			(*_redKey->getVItemAddress())[i].isLive = false;
			_ui->setEricFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//벨로그
		if (IntersectRect(&temp, &(*_redKey->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			_currentFrameX = (*_redKey->getVItemAddress())[i].frameX + 1;
			(*_redKey->getVItemAddress())[i].isLive = false;
			_ui->setBaleogFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//올라프
		if (IntersectRect(&temp, &(*_redKey->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			_currentFrameX = (*_redKey->getVItemAddress())[i].frameX + 1;
			(*_redKey->getVItemAddress())[i].isLive = false;
			_ui->setOlafFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
	}
	//레드자물쇠 충돌
	for (int i = 0; i < _redLock->getVItem().size(); ++i)
	{
		//에릭
		if (IntersectRect(&temp, &(*_redLock->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			//다리 내리고 올리는 키
			if (KEYMANAGER->isOnceKeyDown('E'))
			{
				if (!_redKeyOn)
				{
					_redKeyOn = true;
					_ui->itemUse(_redKeyOn, _playerManager->getChoice());
					_redKeyOn = false;
				}
			}
		}
		//벨로그
		if (IntersectRect(&temp, &(*_redLock->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			//다리 내리고 올리는 키
			if (KEYMANAGER->isOnceKeyDown('E'))
			{
				if (!_redKeyOn)
				{
					_redKeyOn = true;
					_ui->itemUse(_redKeyOn, _playerManager->getChoice());
					_redKeyOn = false;
				}
			}
		}
		//올라프
		if (IntersectRect(&temp, &(*_redLock->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			//다리 내리고 올리는 키
			if (KEYMANAGER->isOnceKeyDown('E'))
			{
				if (!_redKeyOn)
				{
					_redKeyOn = true;
					_ui->itemUse(_redKeyOn, _playerManager->getChoice());
					_redKeyOn = false;
				}
			}
		}
	}
	//바나나 충돌
	for (int i = 0; i < _banana->getVItem().size(); ++i)
	{
		//에릭
		if (IntersectRect(&temp, &(*_banana->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			_currentFrameX = (*_banana->getVItemAddress())[i].frameX + 1;
			(*_banana->getVItemAddress())[i].isLive = false;
			_ui->setEricFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//벨로그
		if (IntersectRect(&temp, &(*_banana->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			_currentFrameX = (*_banana->getVItemAddress())[i].frameX + 1;
			(*_banana->getVItemAddress())[i].isLive = false;
			_ui->setBaleogFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//올라프
		if (IntersectRect(&temp, &(*_banana->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			_currentFrameX = (*_banana->getVItemAddress())[i].frameX + 1;
			(*_banana->getVItemAddress())[i].isLive = false;
			_ui->setOlafFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
	}
	//고기 충돌
	for (int i = 0; i < _meat->getVItem().size(); ++i)
	{
		//에릭
		if (IntersectRect(&temp, &(*_meat->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			_currentFrameX = (*_meat->getVItemAddress())[i].frameX + 1;
			(*_meat->getVItemAddress())[i].isLive = false;
			_ui->setEricFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//벨로그
		if (IntersectRect(&temp, &(*_meat->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			_currentFrameX = (*_meat->getVItemAddress())[i].frameX + 1;
			(*_meat->getVItemAddress())[i].isLive = false;
			_ui->setBaleogFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//올라프
		if (IntersectRect(&temp, &(*_meat->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			_currentFrameX = (*_meat->getVItemAddress())[i].frameX + 1;
			(*_meat->getVItemAddress())[i].isLive = false;
			_ui->setOlafFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
	}
	//버블 충돌
	for (int i = 0; i < _bubble->getVItem().size(); ++i)
	{
		//에릭
		if (IntersectRect(&temp, &(*_bubble->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			if (_playerManager->getEricRect().left > (*_bubble->getVItemAddress())[i].rc.left && _playerManager->getEricRect().right < (*_bubble->getVItemAddress())[i].rc.right 
				&& _playerManager->getEricRect().top < (*_bubble->getVItemAddress())[i].rc.bottom - 70)
			{
				if ((*_bubble->getVItemAddress())[i].rc.bottom - 50 < _playerManager->getEricRect().bottom)
				{
					_bottomTum = (_playerManager->getEricRect().bottom) - ((*_bubble->getVItemAddress())[i].rc.bottom - 50);

					_playerManager->setEricY(_playerManager->getEricY() - _bottomTum);
					_playerManager->getEric()->setIsJumpMotion(false);
					_playerManager->setJumpNum(1);

				}
			}
		}

		//벨로그
		if (IntersectRect(&temp, &(*_bubble->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			if (_playerManager->getBaleogRect().left > (*_bubble->getVItemAddress())[i].rc.left && _playerManager->getBaleogRect().right < (*_bubble->getVItemAddress())[i].rc.right)
			{
				if ((*_bubble->getVItemAddress())[i].rc.bottom - 50 < _playerManager->getBaleogRect().bottom)
				{
					_bottomTum = (_playerManager->getBaleogRect().bottom) - ((*_bubble->getVItemAddress())[i].rc.bottom - 50);

					_playerManager->setBalogY(_playerManager->getBalogY() - _bottomTum );
				}
			}
		}

		//올라프
		if (IntersectRect(&temp, &(*_bubble->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			if (_playerManager->getOlafRect().left > (*_bubble->getVItemAddress())[i].rc.left && _playerManager->getOlafRect().right < (*_bubble->getVItemAddress())[i].rc.right)
			{
				if ((*_bubble->getVItemAddress())[i].rc.bottom - 50 < _playerManager->getOlafRect().bottom)
				{
					_bottomTum = (_playerManager->getOlafRect().bottom) - ((*_bubble->getVItemAddress())[i].rc.bottom - 50);

					_playerManager->setOlafY(_playerManager->getOlafY() - _bottomTum );
				}
			}
		}
	}
}

void stage1::dariOn()
{
	//다리 내리기
	if (_ui->getItemUse())
	{
		for (int i = 0; i < _dari->getVItem().size(); ++i)
		{
			if (GetTickCount() - _oldTime >= 0.03 * 1000)
			{
				if ((*_dari->getVItemAddress())[i].frameX < 9)
				{
					(*_dari->getVItemAddress())[i].frameX += 1;
				}
				_oldTime = GetTickCount();
			}
		}
	}
	else
	{
		for (int i = 0; i < _dari->getVItem().size(); ++i)
		{
			if (GetTickCount() - _oldTime >= 0.03 * 1000)
			{
				if ((*_dari->getVItemAddress())[i].frameX > 0)
				{
					(*_dari->getVItemAddress())[i].frameX -= 1;
				}
				_oldTime = GetTickCount();
			}
		}
	}

	//다리 건너기
	if (_ui->getItemUse())
	{
		//다리가 다 놓여진후 건널수있게용
		_dariCount++;
		if (_dariCount <= 50)
		{
			for (int i = 0; i < _dari->getVItem().size(); ++i)
			{
				//에릭
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.right > _playerManager->getEricRect().left)
					{
						_dariRightTum = (*_dari->getVItemAddress())[i].rc.right - _playerManager->getEricRect().left;

						_playerManager->setEricX(_playerManager->getEricX() + _dariRightTum);
					}
				}
				//벨로그
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.right > _playerManager->getBaleogRect().left)
					{
						_dariRightTum = (*_dari->getVItemAddress())[i].rc.right - _playerManager->getBaleogRect().left;

						_playerManager->setBalogX(_playerManager->getBalogX() + _dariRightTum);
					}
				}
				//올라프
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.right > _playerManager->getOlafRect().left)
					{
						_dariRightTum = (*_dari->getVItemAddress())[i].rc.right - _playerManager->getOlafRect().left;

						_playerManager->setOlafX(_playerManager->getOlafX() + _dariRightTum);
					}
				}
			}
		}
		if (_dariCount > 50)
		{
			for (int i = 0; i < _dari->getVItem().size(); ++i)
			{
				//에릭
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.bottom - 60 < _playerManager->getEricRect().bottom)
					{
						_dariTum = _playerManager->getEricRect().bottom - ((*_dari->getVItemAddress())[i].rc.bottom - 60) ;

						_playerManager->setEricY(_playerManager->getEricY() - _dariTum);
					}
				}
				//벨로그
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.bottom - 60 < _playerManager->getBaleogRect().bottom)
					{
						_dariTum = _playerManager->getBaleogRect().bottom - ((*_dari->getVItemAddress())[i].rc.bottom - 60);

						_playerManager->setBalogY(_playerManager->getBalogY() - _dariTum);
					}
				}
				//올라프
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.bottom - 60 < _playerManager->getOlafRect().bottom)
					{
						_dariTum = _playerManager->getOlafRect().bottom - ((*_dari->getVItemAddress())[i].rc.bottom - 60);

						_playerManager->setOlafY(_playerManager->getOlafY() - _dariTum);
					}
				}
			}
		}
	}
}

void stage1::characterCollision()
{
	//에릭 벨로그 충돌
	if (IntersectRect(&temp, &_playerManager->getEricRect(), &_playerManager->getBaleogRect()))
	{
		_ui->ericItemTrade(BALEOG);
		//_ui->baleogItemTrade(ERIC);
	}
	//에릭 올라프 충돌
	if (IntersectRect(&temp, &_playerManager->getEricRect(), &_playerManager->getOlafRect()))
	{
		_ui->ericItemTrade(OLAF);
		//_ui->olafItemTrade(ERIC);
	}
	//벨로그 올라프 충돌
	if (IntersectRect(&temp, &_playerManager->getBaleogRect(), &_playerManager->getOlafRect()))
	{
		//_ui->olafItemTrade(BALEOG);
		//_ui->baleogItemTrade(OLAF);
	}
}





