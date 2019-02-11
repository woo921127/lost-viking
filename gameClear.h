#pragma once
#include "gameNode.h"

class gameClear : public gameNode
{
	image* _bg;
	float _worldTime;
	float _nextTime;
	int picture;


public:
	gameClear();
	~gameClear();

	HRESULT init();
	void release();
	void update();
	void render();
};

