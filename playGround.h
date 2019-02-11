#pragma once
#include "gameNode.h"
#include "stage1.h"
#include "menu.h"
#include "sound.h"
#include "gameOver.h"
#include "gameClear.h"

//�����е� 
//0 �ȼ����̱�
//1 ��Ʈ���̱�
//2
//3
//4
//5
//6
//7
//8
//9



class playGround : public gameNode
{
private:
	menu* _menu;
	stage1* _stage1;
	gameOver* _gameOver;
	gameClear* _gameClear;
	sound* _sound;

	int _gameOverCount;
	bool _isGameOver;

	RECT _end;
	RECT temp;
	bool _isGameClear;
	bool _isMenu;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	

	playGround();
	~playGround();
};

