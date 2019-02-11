#include "stdafx.h"
#include "selectScene.h"


selectScene::selectScene()
{
}


selectScene::~selectScene()
{
}

HRESULT selectScene::init()
{
	//�Լ������ͷ� ����ų���� ������ �Լ�����ó��()�� ������ �ʴ´�
	_btnSceneChange = new button;
	_btnSceneChange->init("��ư", WINSIZEX / 2, WINSIZEY / 2,
		PointMake(0, 1), PointMake(0, 0), cbSceneChange);

	_btnAngleDown = new button;
	_btnAngleDown->init("��ư", WINSIZEX / 2 - 200, WINSIZEY / 2 + 200,
		PointMake(0, 1), PointMake(0, 0), cbAngleDown);

	_btnAngleUp = new button;
	_btnAngleUp->init("��ư", WINSIZEX / 2 + 200, WINSIZEY / 2 + 200,
		PointMake(0, 1), PointMake(0, 0), cbAngleUp);


	//======================================================
	_action = new action;
	_action->init();

	IMAGEMANAGER->addImage("����", "����.bmp", 50, 50, false, RGB(0, 0, 0));

	_fairy = IMAGEMANAGER->findImage("����");
	_fairy->SetX(WINSIZEX / 2);
	_fairy->SetY(WINSIZEY / 2);

	_action->moveTo(_fairy, WINSIZEX - 50, WINSIZEY - 50, 2.0f);

	return S_OK;
}

void selectScene::release()
{
}

void selectScene::update()
{
	_btnSceneChange->update();
	_btnAngleDown->update();
	_btnAngleUp->update();
	_action->update();
}

void selectScene::render()
{
	//_btnSceneChange->render();
	//
	//_btnAngleDown->render();
	//_btnAngleUp->render();

	_fairy->render(getMemDC(), _fairy->GetX(), _fairy->GetY());

	char str[128];
	sprintf_s(str, "����Ʈ ��!");
	TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2 - 200, str, strlen(str));
}

void selectScene::cbSceneChange()
{
	SCENEMANAGER->changeScene("��Ÿ��");
}

void selectScene::cbAngleUp()
{
	DATABASE->setElementDataAngle("battle", DATABASE->getElementData("battle")->angle += 0.2f);
}

void selectScene::cbAngleDown()
{
	DATABASE->setElementDataAngle("battle", DATABASE->getElementData("battle")->angle -= 0.2f);
}
