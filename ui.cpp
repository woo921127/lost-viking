#include "stdafx.h"
#include "ui.h"
#include "playerManager.h"


ui::ui()
{
}


ui::~ui()
{
}

HRESULT ui::init()
{
	IMAGEMANAGER->addImage("UI", "image/ui.bmp", 1000, 191, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("profile", "image/profile.bmp", 375, 270, 3, 3, false, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("life", "image/life.bmp", 105, 25, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("box", "image/box.bmp", 62, 62, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("uiItem", "image/uiItem.bmp", 440, 55, 8, 1, true, RGB(255, 0, 255));

	_ericCurrentX = _ericCurrentY = 0;
	_baleogCurrentX = 1;
	_baleogCurrentY = 1;
	_olafCurrentX = 2;
	_olafCurrentY = 1;

	_ericLife = 105;
	_baleogLife = 105;
	_olafLife = 105;

	_itemMove = 0;
	_saveTradeFrameX = 0;

	_ericBox_X = 187; 
	_ericBox_Y = 671;
	_baleogBox_X = 470; 
	_baleogBox_Y = 671;
	_olafBox_X = 751; 
	_olafBox_Y = 671;

	_setEricFrameX = _setBaleogFrameX = _setOlafFrameX = 0;
	
	for(int i = 0; i < 4; i++)
	{ 
		_ericItemFrameX[i] = 0, _ericItemFrameY[i] = 0;
		_baleogItemFrameX[i] = 0, _baleogItemFrameY[i] = 0;
		_olafItemFrameX[i] = 0, _olafItemFrameY[i] = 0;
	}

	_fKeyMod = false;

	_itemCollision = false;

	_itemUse = false;

	_itemTrade = false;

	_ericDead = _baleogDead = _olafDead = false;

	_ericItemNumber = _baleogItemNumber = _olafItemNumber = 0;

	_deleteItemFrameX = 7;

	_ericCursor = _baleogCursor = _olafCursor = false;
	_cursorOldTime = GetTickCount();

	return S_OK;
}

void ui::release()
{
}

void ui::update(int choice, bool uiChange)
{
	_choice = choice;
	vikingItemNumber(_choice);
	profileDead();
	if (_choice == 0)
	{
		uiEricItemSave();
	}
	else if (_choice == 1)
	{
		uiBaleogItemSave();
	}
	else if (_choice == 2)
	{
		uiOlafItemSave();
	}
	if (uiChange)
	{
	
		keyControl(_choice);									//커서 움직임 키값

		if (!_fKeyMod)
		{
			if (GetTickCount() - _cursorOldTime >= 1 * 300)
			{
				if (_choice == 0)
				{
					if (!_ericCursor) _ericCursor = true;
					else _ericCursor = false;
				}
				else if (_choice == 1)
				{
					if (!_baleogCursor)_baleogCursor = true;
					else _baleogCursor = false;
				}
				else if (_choice == 2)
				{
					if (!_olafCursor) _olafCursor = true;
					else _olafCursor = false;
				}
				_cursorOldTime = GetTickCount();
			}
		}
	}
	else													//커서 에러 예외 처리
	{
		_ericCursor = _baleogCursor = _olafCursor = false;
		_fKeyMod = false;
		_deleteItemFrameX = 7;
	}
}

void ui::render()
{
	IMAGEMANAGER->findImage("UI")->render(getMemDC(), 0, 609);
	IMAGEMANAGER->findImage("profile")->frameRender(getMemDC(), 62, 671, _ericCurrentX, _ericCurrentY);
	IMAGEMANAGER->findImage("profile")->frameRender(getMemDC(), 344, 671, _baleogCurrentX, _baleogCurrentY);
	IMAGEMANAGER->findImage("profile")->frameRender(getMemDC(), 625, 671, _olafCurrentX, _olafCurrentY);
	IMAGEMANAGER->findImage("life")->render(getMemDC(), 70, 770, 0, 0, _ericLife, 25);						// life -35씩 줄이면 피가 하나씩 깍임
	IMAGEMANAGER->findImage("life")->render(getMemDC(), 350, 770, 0, 0, _baleogLife, 25);
	IMAGEMANAGER->findImage("life")->render(getMemDC(), 630, 770, 0, 0, _olafLife, 25);
	if (!_ericCursor)
	{
		IMAGEMANAGER->findImage("box")->render(getMemDC(), _ericBox_X, _ericBox_Y);
	}
	if (!_baleogCursor)
	{
		IMAGEMANAGER->findImage("box")->render(getMemDC(), _baleogBox_X, _baleogBox_Y);
	}
	if (!_olafCursor)
	{
		IMAGEMANAGER->findImage("box")->render(getMemDC(), _olafBox_X, _olafBox_Y);
	}
	uiItemRender();
	//char str[128];
	//sprintf_s(str, "에릭템넘버 : %d", _ericDead);
	//TextOut(getMemDC(), 500, 100, str, strlen(str));
	//sprintf_s(str, "템 2번자리 : %d", _ericItemFrameX[2]);
	//TextOut(getMemDC(), 600, 100, str, strlen(str));
	//sprintf_s(str, "템사용여부 : %d", _itemUse);
	//TextOut(getMemDC(), 700, 100, str, strlen(str));
}
//프로필 업데이트
void ui::profileUpdate(int choice)
{
	_choice = choice;
	if (choice == 0 && !_ericDead)
	{
		_ericCurrentX = _ericCurrentY = 0;
	}
	else if(!_ericDead)
	{
		_ericCurrentX = 0;
		_ericCurrentY = 1;
	}
	else
	{
		_ericCurrentX = 0;
		_ericCurrentY = 2;

	}
	if (choice == 1 && !_baleogDead)
	{
		_baleogCurrentX = 1;
		_baleogCurrentY = 0;
	}
	else if(!_baleogDead)
	{
		_baleogCurrentX = 1;
		_baleogCurrentY = 1;
	}
	else
	{
		_baleogCurrentX = 1;
		_baleogCurrentY = 2;
	}
	if (choice == 2 && !_olafDead)
	{
		_olafCurrentX = 2;
		_olafCurrentY = 0;
	}
	else if(!_olafDead)
	{
		_olafCurrentX = 2;
		_olafCurrentY = 1;
	}
	else
	{
		_olafCurrentX = 2;
		_olafCurrentY = 2;
	}
}
void ui::profileDead()
{
	if (_ericDead)
	{
		_ericCurrentX = 0;
		_ericCurrentY = 2;
	}
	if (_baleogDead)
	{
		_baleogCurrentX = 1;
		_baleogCurrentY = 2;
	}
	if (_olafDead)
	{
		_olafCurrentX = 2;
		_olafCurrentY = 2;
	}
}
//키 컨트롤
void ui::keyControl(int choice)
{
	if (choice == 0)
	{
		if (!(_ericItemFrameX[_ericItemNumber] == 0))
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				if (_deleteItemFrameX == 7)
				{
					if (!_fKeyMod)
					{
						_fKeyMod = true;
					}
					else
					{
						_fKeyMod = false;
					}
					_ericCursor = false;
				}
			}
		}
		else
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				if (!(_deleteItemFrameX == 7))
				{
					_deleteItemFrameX = 7;
					_fKeyMod = false;
				}
			}
		}

		if (!_fKeyMod)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				if (_ericBox_X == 250)
				{
					_ericBox_X = 187;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				if (_ericBox_X == 187)
				{
					_ericBox_X = 250;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				if (_ericBox_Y == 734)
				{
					_ericBox_Y = 671;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				if (_ericBox_Y == 671)
				{
					_ericBox_Y = 734;
				}
			}
		}
		else
		{
			if (!_itemTrade)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) || KEYMANAGER->isOnceKeyDown(VK_LEFT))
				{
					if (_itemMove == 0)
					{
						_itemMove++;
						_saveFrameX = _ericItemFrameX[_ericItemNumber];
						_deleteItemFrameX = _ericItemFrameX[_ericItemNumber];
						_ericItemFrameX[_ericItemNumber] = 0;
					}
					else
					{
						_itemMove--;
						_ericItemFrameX[_ericItemNumber] = _saveFrameX;
						_deleteItemFrameX = 7;
					}
				}
			}
		}
	}
	else if (choice == 1)
	{
		if (!(_baleogItemFrameX[_baleogItemNumber] == 0))
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				if (_deleteItemFrameX == 7)
				{
					if (!_fKeyMod) _fKeyMod = true;
					else _fKeyMod = false;
					_baleogCursor = false;
				}
			}
		}
		else
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				if (!(_deleteItemFrameX == 7))
				{
					_deleteItemFrameX = 7;
					_fKeyMod = false;
					
				}
			}
		}

		if (!_fKeyMod)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				if (_baleogBox_X == 533)
				{
					_baleogBox_X = 470;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				if (_baleogBox_X == 470)
				{
					_baleogBox_X = 533;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				if (_baleogBox_Y == 734)
				{
					_baleogBox_Y = 671;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				if (_baleogBox_Y == 671)
				{
					_baleogBox_Y = 734;
				}
			}
		}
		else
		{
			if (!_itemTrade)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) || KEYMANAGER->isOnceKeyDown(VK_LEFT))
				{
					if (_itemMove == 0)
					{
						_itemMove++;
						_saveFrameX = _baleogItemFrameX[_baleogItemNumber];
						_deleteItemFrameX = _baleogItemFrameX[_baleogItemNumber];
						_baleogItemFrameX[_baleogItemNumber] = 0;
					}
					else
					{
						_itemMove--;
						_baleogItemFrameX[_baleogItemNumber] = _saveFrameX;
						_deleteItemFrameX = 7;
					}
				}
			}
		}
	}
	else if (choice == 2)
	{
		if (!(_olafItemFrameX[_olafItemNumber] == 0))
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				if (_deleteItemFrameX == 7)
				{
					if (!_fKeyMod) _fKeyMod = true;
					else _fKeyMod = false;
					_olafCursor = false;
				}
			}
		}
		else
		{
			if (KEYMANAGER->isOnceKeyDown('F'))
			{
				if (!(_deleteItemFrameX == 7))
				{
					_deleteItemFrameX = 7;
					_fKeyMod = false;
				}
			}
		}
		if (!_fKeyMod)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
			{
				if (_olafBox_X == 814)
				{
					_olafBox_X = 751;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
			{
				if (_olafBox_X == 751)
				{
					_olafBox_X = 814;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				if (_olafBox_Y == 734)
				{
					_olafBox_Y = 671;
				}
			}
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				if (_olafBox_Y == 671)
				{
					_olafBox_Y = 734;
				}
			}
		}
		else
		{
			if (!_itemTrade)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) || KEYMANAGER->isOnceKeyDown(VK_LEFT))
				{
					if (_itemMove == 0)
					{
						_itemMove++;
						_saveFrameX = _olafItemFrameX[_olafItemNumber];
						_deleteItemFrameX = _olafItemFrameX[_olafItemNumber];
						_olafItemFrameX[_olafItemNumber] = 0;
					}
					else
					{
						_itemMove--;
						_olafItemFrameX[_olafItemNumber] = _saveFrameX;
						_deleteItemFrameX = 7;
					}
				}
			}
		}
	}
}
//에릭이 아이템 충돌했을때 에릭 인벤에 저장
void ui::uiEricItemSave()
{
	if (_itemCollision)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_ericItemFrameX[i] == 0)
			{
				_ericItemFrameX[i] = _setEricFrameX;
				break;
			}
		}
		_itemCollision = false;
	}
}
//벨로그이 아이템 충돌했을때 벨로그 인벤에 저장
void ui::uiBaleogItemSave()
{
	if (_itemCollision)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_baleogItemFrameX[i] == 0)
			{
				_baleogItemFrameX[i] = _setBaleogFrameX;
				break;
			}
		}
		_itemCollision = false;
	}
}
//올라프이 아이템 충돌했을때 올라프 인벤에 저장
void ui::uiOlafItemSave()
{
	if (_itemCollision)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_olafItemFrameX[i] == 0)
			{
				_olafItemFrameX[i] = _setOlafFrameX;
				break;
			}
		}
		_itemCollision = false;
	}

}
//인벤에 있는 아이템 프레임을 이용하여 활성화
void ui::uiItemRender()
{
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 187, 671, _ericItemFrameX[0], _ericItemFrameY[0]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 250, 671, _ericItemFrameX[1], _ericItemFrameY[1]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 187, 734, _ericItemFrameX[2], _ericItemFrameY[2]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 250, 734, _ericItemFrameX[3], _ericItemFrameY[3]);
		
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 470, 671, _baleogItemFrameX[0], _baleogItemFrameY[0]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 533, 671, _baleogItemFrameX[1], _baleogItemFrameY[1]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 470, 734, _baleogItemFrameX[2], _baleogItemFrameY[2]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 533, 734, _baleogItemFrameX[3], _baleogItemFrameY[3]);
		
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 751, 671, _olafItemFrameX[0], _olafItemFrameY[0]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 814, 671, _olafItemFrameX[1], _olafItemFrameY[1]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 751, 734, _olafItemFrameX[2], _olafItemFrameY[2]);
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 814, 734, _olafItemFrameX[3], _olafItemFrameY[3]);
		
		IMAGEMANAGER->findImage("uiItem")->frameRender(getMemDC(), 880, 671, _deleteItemFrameX, 0);

}
//각 바이킹 아이템 넘버 꼴보기 싫음
void ui::vikingItemNumber(int choice)
{
	if (choice == 0)
	{
		if (_ericBox_X == 187 && _ericBox_Y == 671)
		{
			_ericItemNumber = 0;
		}
		else if (_ericBox_X == 250 && _ericBox_Y == 671)
		{
			_ericItemNumber = 1;
		}
		else if (_ericBox_X == 187 && _ericBox_Y == 734)
		{
			_ericItemNumber = 2;
		}
		else if (_ericBox_X == 250 && _ericBox_Y == 734)
		{
			_ericItemNumber = 3;
		}
	}
	else if (choice == 1)
	{
		if (_baleogBox_X == 470 && _baleogBox_Y == 671)
		{
			_baleogItemNumber = 0;
		}
		else if (_baleogBox_X == 533 && _baleogBox_Y == 671)
		{
			_baleogItemNumber = 1;
		}
		else if (_baleogBox_X == 470 && _baleogBox_Y == 734)
		{
			_baleogItemNumber = 2;
		}
		else if (_baleogBox_X == 533 && _baleogBox_Y == 734)
		{
			_baleogItemNumber = 3;
		}
	}
	else if (choice == 2)
	{
		if (_olafBox_X == 751 && _olafBox_Y == 671)
		{
			_olafItemNumber = 0;
		}
		else if (_olafBox_X == 814 && _olafBox_Y == 671)
		{
			_olafItemNumber = 1;
		}
		else if (_olafBox_X == 751 && _olafBox_Y == 734)
		{
			_olafItemNumber = 2;
		}
		else if (_olafBox_X == 814 && _olafBox_Y == 734)
		{
			_olafItemNumber = 3;
		}
	}
}
//바이킹 레드키 아이템 사용
void ui::itemUse(bool redKey, int choice)
{
	if (choice == 0)
	{
		if (redKey)
		{
			if (_ericItemNumber == 0 && _ericItemFrameX[0] == 1)
			{
				_ericItemFrameX[0] = 0;
				_itemUse = true;
			}
			else if (_ericItemNumber == 1 && _ericItemFrameX[1] == 1)
			{
				_ericItemFrameX[1] = 0;
				_itemUse = true;
			}
			else if (_ericItemNumber == 2 && _ericItemFrameX[2] == 1)
			{
				_ericItemFrameX[2] = 0;
				_itemUse = true;
			}
			else if (_ericItemNumber == 3 && _ericItemFrameX[3] == 1)
			{
				_ericItemFrameX[3] = 0;
				_itemUse = true;
			}
		}
	}
	else if (choice == 1)
	{
		if (redKey)
		{
			if (_baleogItemNumber == 0 && _baleogItemFrameX[0] == 1)
			{
				_baleogItemFrameX[0] = 0;
				_itemUse = true;
			}
			else if (_baleogItemNumber == 1 && _baleogItemFrameX[1] == 1)
			{
				_baleogItemFrameX[1] = 0;
				_itemUse = true;
			}
			else if (_baleogItemNumber == 2 && _baleogItemFrameX[2] == 1)
			{
				_baleogItemFrameX[2] = 0;
				_itemUse = true;
			}
			else if (_baleogItemNumber == 3 && _baleogItemFrameX[3] == 1)
			{
				_baleogItemFrameX[3] = 0;
				_itemUse = true;
			}
		}
	}
	else if (choice == 2)
	{
		if (redKey)
		{
			if (_olafItemNumber == 0 && _olafItemFrameX[0] == 1)
			{
				_olafItemFrameX[0] = 0;
				_itemUse = true;
			}
			else if (_olafItemNumber == 1 && _olafItemFrameX[1] == 1)
			{
				_olafItemFrameX[1] = 0;
				_itemUse = true;
			}
			else if (_olafItemNumber == 2 && _olafItemFrameX[2] == 1)
			{
				_olafItemFrameX[2] = 0;
				_itemUse = true;
			}
			else if (_olafItemNumber == 3 && _olafItemFrameX[3] == 1)
			{
				_olafItemFrameX[3] = 0;
				_itemUse = true;
			}
		}
	}
}

void ui::ericItemTrade(int name)
{
	if (_fKeyMod && _choice == 0)
	{
		if (name == BALEOG)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_itemTrade = true;
				if (_itemMove == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						if (_baleogItemFrameX[i] == 0)
						{
							_baleogItemFrameX[i] = _ericItemFrameX[_ericItemNumber];
							_saveTradeFrameX = _ericItemFrameX[_ericItemNumber];
							_ericItemFrameX[_ericItemNumber] = 0;
							break;
						}
					}
					_itemMove++;
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						if (_baleogItemFrameX[i] == _saveTradeFrameX)
						{
							_ericItemFrameX[_ericItemNumber] = _baleogItemFrameX[i];
							_baleogItemFrameX[i] = 0;
							break;
						}
					}
					_itemMove--;
				}
			}
		}
		else if (name == OLAF)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_UP))
			{
				_itemTrade = true;
				if (_itemMove == 0)
				{
					for (int i = 0; i < 4; i++)
					{
						if (_olafItemFrameX[i] == 0)
						{
							_olafItemFrameX[i] = _ericItemFrameX[_ericItemNumber];
							_saveTradeFrameX = _ericItemFrameX[_ericItemNumber];
							_ericItemFrameX[_ericItemNumber] = 0;
							break;
						}
					}
					_itemMove++;
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						if (_olafItemFrameX[i] == _saveTradeFrameX)
						{
							_ericItemFrameX[_ericItemNumber] = _olafItemFrameX[i];
							_olafItemFrameX[i] = 0;
							break;
						}
					}
					_itemMove--;
				}
			}
		}
		if (KEYMANAGER->isOnceKeyDown('F'))
		{
			_fKeyMod = false;
			_itemTrade = false;
			_saveTradeFrameX = 0;
			_itemMove = 0;
		}
		
	}
}

//void ui::baleogItemTrade(int name)
//{
//	if (_fKeyMod && _choice == 1)
//	{
//		if (name == ERIC)
//		{
//			if (KEYMANAGER->isOnceKeyDown(VK_UP))
//			{
//				_itemTrade = true;
//				if (_itemMove == 0)
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_ericItemFrameX[i] == 0)
//						{
//							_ericItemFrameX[i] = _baleogItemFrameX[_baleogItemNumber];
//							_saveTradeFrameX = _baleogItemFrameX[_baleogItemNumber];
//							_baleogItemFrameX[_baleogItemNumber] = 0;
//							break;
//						}
//					}
//					_itemMove++;
//				}
//				else
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_ericItemFrameX[i] == _saveTradeFrameX)
//						{
//							_baleogItemFrameX[_ericItemNumber] = _ericItemFrameX[i];
//							_ericItemFrameX[i] = 0;
//							break;
//						}
//					}
//					_itemMove--;
//				}
//			}
//		}
//		else if (name == OLAF)
//		{
//			if (KEYMANAGER->isOnceKeyDown(VK_UP))
//			{
//				_itemTrade = true;
//				if (_itemMove == 0)
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_olafItemFrameX[i] == 0)
//						{
//							_olafItemFrameX[i] = _baleogItemFrameX[_baleogItemNumber];
//							_saveTradeFrameX = _baleogItemFrameX[_baleogItemNumber];
//							_baleogItemFrameX[_baleogItemNumber] = 0;
//							break;
//						}
//					}
//					_itemMove++;
//				}
//				else
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_olafItemFrameX[i] == _saveTradeFrameX)
//						{
//							_baleogItemFrameX[_olafItemNumber] = _olafItemFrameX[i];
//							_olafItemFrameX[i] = 0;
//							break;
//						}
//					}
//					_itemMove--;
//				}
//			}
//		}
//		if (KEYMANAGER->isOnceKeyDown('F'))
//		{
//			_fKeyMod = false;
//			_itemTrade = false;
//			_saveTradeFrameX = 0;
//			_itemMove = 0;
//		}
//
//	}
//}

//void ui::olafItemTrade(int name)
//{
//	if (_fKeyMod && _choice == 2)
//	{
//		if (name == BALEOG)
//		{
//			if (KEYMANAGER->isOnceKeyDown(VK_UP))
//			{
//				_itemTrade = true;
//				if (_itemMove == 0)
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_baleogItemFrameX[i] == 0)
//						{
//							_baleogItemFrameX[i] = _olafItemFrameX[_olafItemNumber];
//							_saveTradeFrameX = _olafItemFrameX[_olafItemNumber];
//							_olafItemFrameX[_olafItemNumber] = 0;
//							break;
//						}
//					}
//					_itemMove++;
//				}
//				else
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_baleogItemFrameX[i] == _saveTradeFrameX)
//						{
//							_olafItemFrameX[_olafItemNumber] = _baleogItemFrameX[i];
//							_baleogItemFrameX[i] = 0;
//							break;
//						}
//					}
//					_itemMove--;
//				}
//			}
//		}
//		else if (name == ERIC)
//		{
//			if (KEYMANAGER->isOnceKeyDown(VK_UP))
//			{
//				_itemTrade = true;
//				if (_itemMove == 0)
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_ericItemFrameX[i] == 0)
//						{
//							_ericItemFrameX[i] = _olafItemFrameX[_olafItemNumber];
//							_saveTradeFrameX = _olafItemFrameX[_olafItemNumber];
//							_olafItemFrameX[_olafItemNumber] = 0;
//							break;
//						}
//					}
//					_itemMove++;
//				}
//				else
//				{
//					for (int i = 0; i < 4; i++)
//					{
//						if (_ericItemFrameX[i] == _saveTradeFrameX)
//						{
//							_olafItemFrameX[_olafItemNumber] = _ericItemFrameX[i];
//							_ericItemFrameX[i] = 0;
//							break;
//						}
//					}
//					_itemMove--;
//				}
//			}
//		}
//		if (KEYMANAGER->isOnceKeyDown('F'))
//		{
//			_fKeyMod = false;
//			_itemTrade = false;
//			_saveTradeFrameX = 0;
//			_itemMove = 0;
//		}
//
//	}
//}
