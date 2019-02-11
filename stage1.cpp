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

	//������
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

	//����
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
		//�ȼ� ���̰��ϴ� �Լ�
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

	

	

	//�ٸ� �����̱�
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
	//sprintf_s(str, "�÷��̾� x : %d", _playerManager->getEricDead());
	//TextOut(getMemDC(), 300, 30, str, strlen(str));
	//sprintf_s(str, "�÷��̾� y : %f", _playerManager->getEricY());
	//TextOut(getMemDC(), 300, 40, str, strlen(str));
	//sprintf_s(str, "ī�޶� X : %f", _playerManager->getCamera()->getCameraX());
	//TextOut(getMemDC(), 300, 60, str, strlen(str));
}

void stage1::viewPixel()
{
	//�ȼ����̱��
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
	//_bubbleTime ������ �ð� ���� ���� ����
	if (_bubbleTime + _worldTime <= TIMEMANAGER->getWorldTime())
	{
		_worldTime = TIMEMANAGER->getWorldTime();

		_bubble->createItem("bubble", 1666, 2060, 0, 0);
	}

	_bubbleFrameTime++;
	for (int i = 0; i < _bubble->getVItem().size(); ++i)
	{
		//������ 730���̺��� ũ�� ���̰� ���ҵǸ� �������� 0 1 �ݺ�
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
		//���� ���̰� 730���� �۾����� �ð��� ���� �������� �����ϸ� �����ȴ�.
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
	//����Ű �浹
	for (int i = 0; i < _redKey->getVItem().size(); ++i)
	{
		//����
		if (IntersectRect(&temp, &(*_redKey->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			_currentFrameX = (*_redKey->getVItemAddress())[i].frameX + 1;
			(*_redKey->getVItemAddress())[i].isLive = false;
			_ui->setEricFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//���α�
		if (IntersectRect(&temp, &(*_redKey->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			_currentFrameX = (*_redKey->getVItemAddress())[i].frameX + 1;
			(*_redKey->getVItemAddress())[i].isLive = false;
			_ui->setBaleogFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//�ö���
		if (IntersectRect(&temp, &(*_redKey->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			_currentFrameX = (*_redKey->getVItemAddress())[i].frameX + 1;
			(*_redKey->getVItemAddress())[i].isLive = false;
			_ui->setOlafFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
	}
	//�����ڹ��� �浹
	for (int i = 0; i < _redLock->getVItem().size(); ++i)
	{
		//����
		if (IntersectRect(&temp, &(*_redLock->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			//�ٸ� ������ �ø��� Ű
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
		//���α�
		if (IntersectRect(&temp, &(*_redLock->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			//�ٸ� ������ �ø��� Ű
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
		//�ö���
		if (IntersectRect(&temp, &(*_redLock->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			//�ٸ� ������ �ø��� Ű
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
	//�ٳ��� �浹
	for (int i = 0; i < _banana->getVItem().size(); ++i)
	{
		//����
		if (IntersectRect(&temp, &(*_banana->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			_currentFrameX = (*_banana->getVItemAddress())[i].frameX + 1;
			(*_banana->getVItemAddress())[i].isLive = false;
			_ui->setEricFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//���α�
		if (IntersectRect(&temp, &(*_banana->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			_currentFrameX = (*_banana->getVItemAddress())[i].frameX + 1;
			(*_banana->getVItemAddress())[i].isLive = false;
			_ui->setBaleogFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//�ö���
		if (IntersectRect(&temp, &(*_banana->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			_currentFrameX = (*_banana->getVItemAddress())[i].frameX + 1;
			(*_banana->getVItemAddress())[i].isLive = false;
			_ui->setOlafFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
	}
	//��� �浹
	for (int i = 0; i < _meat->getVItem().size(); ++i)
	{
		//����
		if (IntersectRect(&temp, &(*_meat->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
		{
			_currentFrameX = (*_meat->getVItemAddress())[i].frameX + 1;
			(*_meat->getVItemAddress())[i].isLive = false;
			_ui->setEricFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//���α�
		if (IntersectRect(&temp, &(*_meat->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
		{
			_currentFrameX = (*_meat->getVItemAddress())[i].frameX + 1;
			(*_meat->getVItemAddress())[i].isLive = false;
			_ui->setBaleogFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
		//�ö���
		if (IntersectRect(&temp, &(*_meat->getVItemAddress())[i].rc, &_playerManager->getOlafRect()))
		{
			_currentFrameX = (*_meat->getVItemAddress())[i].frameX + 1;
			(*_meat->getVItemAddress())[i].isLive = false;
			_ui->setOlafFrameX(_currentFrameX);
			_ui->setItemCollision(true);
		}
	}
	//���� �浹
	for (int i = 0; i < _bubble->getVItem().size(); ++i)
	{
		//����
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

		//���α�
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

		//�ö���
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
	//�ٸ� ������
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

	//�ٸ� �ǳʱ�
	if (_ui->getItemUse())
	{
		//�ٸ��� �� �������� �ǳμ��ְԿ�
		_dariCount++;
		if (_dariCount <= 50)
		{
			for (int i = 0; i < _dari->getVItem().size(); ++i)
			{
				//����
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.right > _playerManager->getEricRect().left)
					{
						_dariRightTum = (*_dari->getVItemAddress())[i].rc.right - _playerManager->getEricRect().left;

						_playerManager->setEricX(_playerManager->getEricX() + _dariRightTum);
					}
				}
				//���α�
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.right > _playerManager->getBaleogRect().left)
					{
						_dariRightTum = (*_dari->getVItemAddress())[i].rc.right - _playerManager->getBaleogRect().left;

						_playerManager->setBalogX(_playerManager->getBalogX() + _dariRightTum);
					}
				}
				//�ö���
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
				//����
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getEricRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.bottom - 60 < _playerManager->getEricRect().bottom)
					{
						_dariTum = _playerManager->getEricRect().bottom - ((*_dari->getVItemAddress())[i].rc.bottom - 60) ;

						_playerManager->setEricY(_playerManager->getEricY() - _dariTum);
					}
				}
				//���α�
				if (IntersectRect(&temp, &(*_dari->getVItemAddress())[i].rc, &_playerManager->getBaleogRect()))
				{
					if ((*_dari->getVItemAddress())[i].rc.bottom - 60 < _playerManager->getBaleogRect().bottom)
					{
						_dariTum = _playerManager->getBaleogRect().bottom - ((*_dari->getVItemAddress())[i].rc.bottom - 60);

						_playerManager->setBalogY(_playerManager->getBalogY() - _dariTum);
					}
				}
				//�ö���
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
	//���� ���α� �浹
	if (IntersectRect(&temp, &_playerManager->getEricRect(), &_playerManager->getBaleogRect()))
	{
		_ui->ericItemTrade(BALEOG);
		//_ui->baleogItemTrade(ERIC);
	}
	//���� �ö��� �浹
	if (IntersectRect(&temp, &_playerManager->getEricRect(), &_playerManager->getOlafRect()))
	{
		_ui->ericItemTrade(OLAF);
		//_ui->olafItemTrade(ERIC);
	}
	//���α� �ö��� �浹
	if (IntersectRect(&temp, &_playerManager->getBaleogRect(), &_playerManager->getOlafRect()))
	{
		//_ui->olafItemTrade(BALEOG);
		//_ui->baleogItemTrade(OLAF);
	}
}





