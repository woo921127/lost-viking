#include "stdafx.h"
#include "starScene.h"
#include "battle.h"

starScene::starScene()
{
}


starScene::~starScene()
{
}

HRESULT starScene::init()
{
	_battle = new battle;
	_battle->init("battle", WINSIZEX / 2, WINSIZEY / 2);

	return S_OK;
}

void starScene::release()
{
}

void starScene::update()
{
	_battle->update();
}

void starScene::render()
{
	_battle->render();

	char str[128];
	sprintf_s(str, "½ºÅ¸ ¾À!");
	TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2 + 200, str, strlen(str));
}
