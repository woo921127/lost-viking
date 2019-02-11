#pragma once
#include "gameNode.h"

class menu : public gameNode
{
private:
	int _menuSelect_X, _menuSelect_Y;		//�޴� ���� ������ ��ġ��
	int _imageAlphaValue;					//������ ���İ� ���� 

	bool _padeIn, _padeOut;					//���̵� ��, ���̵� �ƿ� ȿ�� 
	bool _menuGameStart;					//�޴� ���� ���� �Ұ�
	bool _opening;
public:
	menu();
	~menu();

	HRESULT init();
	void release();
	void update();
	void render();

	bool getMenuQuit() { return _menuGameStart; }
	void padeIn();
	void padeOut();
};

