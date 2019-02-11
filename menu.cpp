#include "stdafx.h"
#include "menu.h"


menu::menu()
{
}


menu::~menu()
{
}

HRESULT menu::init()
{
	IMAGEMANAGER->addImage("������", "image/theLostVikings.bmp", 1000, 609, true, RGB(255, 0 ,255));
	IMAGEMANAGER->addImage("�޴�", "image/menu.bmp", 1000, 609, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�޴�����", "image/menuselect.bmp", 50, 50, true, RGB(255, 0, 255));

	_menuSelect_X = 320;
	_menuSelect_Y = 390;
	_imageAlphaValue = 0;

	_menuGameStart = false;
	_opening = true;					
	_padeIn = true;
	_padeOut = false;

	//SOUNDMANAGER->play("opening_bgm");

	return S_OK;
}

void menu::release()
{
}

void menu::update()
{
	padeIn();
	padeOut();
	
	if (!_opening)						//������ ����� �޴����� Ű�Է�
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP) || KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			if (_menuSelect_Y == 455) _menuSelect_Y = 390;
			else _menuSelect_Y = 455;
			SOUNDMANAGER->play("menu_SeletSound");
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			if (_menuSelect_Y == 390)
			{
				_padeOut = true;
				SOUNDMANAGER->stop("opening_bgm");
				SOUNDMANAGER->play("viking_GameStart");
				
			}
			else PostQuitMessage(0);
		}
	}
	else								//������ ���ͽ� ���̵�ƿ� ����
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) && !_padeIn)
		{
			_padeOut = true;
			
		}
	}
}

void menu::render()
{
	if (_opening)					
	{
		IMAGEMANAGER->findImage("������")->alphaRender(getMemDC(), _imageAlphaValue);
	}
	else if(!_opening)
	{
		IMAGEMANAGER->findImage("�޴�")->alphaRender(getMemDC(), _imageAlphaValue);
		IMAGEMANAGER->findImage("�޴�����")->alphaRender(getMemDC(), _menuSelect_X, _menuSelect_Y, _imageAlphaValue);
	}


}

void menu::padeIn()
{
	if (_padeIn)						//���̵� �� ����
	{
		if (_imageAlphaValue <= 254)
		{
			_imageAlphaValue += 2;
			if (_imageAlphaValue >= 253)
			{
				_imageAlphaValue = 255;
			}
		}
		else							//������ fasle
		{
			_padeIn = false;
		}
	}
}

void menu::padeOut()
{
	if (_padeOut)						//���� ����
	{
		if (_imageAlphaValue > 0)
		{
			_imageAlphaValue -= 1;
			if (_imageAlphaValue < 3)
			{
				_imageAlphaValue = 0;
			}
		}
		else							//���̵� �ƿ��� ������ �����׵� ����
		{
			_padeOut = false;
			if (!_opening)
			{
				_menuGameStart = true;
				SOUNDMANAGER->play("stage1_bgm");
				return;
			}
			_opening = false;
			if (!_opening) _imageAlphaValue = 255;

		}
	}
}
