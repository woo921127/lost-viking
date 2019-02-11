#pragma once
#include "gameNode.h"

class gameOver : public gameNode
{
	int _frameX;
	float _oldTime;
	image* _over;

public:
	gameOver();
	~gameOver();

	HRESULT init();
	void release();
	void update();
	void render();

};

