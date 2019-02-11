#pragma once
#include "gameNode.h"
#include "playerManager.h"
#include "item.h"
#include "ui.h"

enum ITEM
{
	REDKEY,
	REDLOCK,
	GOLDKEY,
	GOLDLOCK,
	MEAT,
	BANANA
};


class stage1 : public gameNode
{
private:

	playerManager* _playerManager;

	RECT temp;

	//아이템
	item* _redKey;
	item* _redLock;
	item* _banana;
	item* _meat;

	//오브젝트
	item* _dari;
	item* _bubble;

	//UI
	ui* _ui;

	bool _isPixel;
	bool _uiChange;

	float _bubbleTime;
	float _worldTime;
	int _bubbleFrameTime;
	int _bubbleRemoveTime;

	int _currentFrameX;

	float _bottomTum;

	bool _redKeyOn;
	float _oldTime;
	int _dariCount;
	float _dariTum;
	float _dariRightTum;

public:
	stage1();
	~stage1();

	HRESULT init();
	void release();
	void update();
	void render();

	void viewPixel();
	
	void createBubble();

	void itemCollision();

	void dariOn();

	void characterCollision();

	playerManager* getPlayerManager() { return _playerManager; }

};

