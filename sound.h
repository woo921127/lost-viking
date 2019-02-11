#pragma once
#include "gameNode.h"

class sound : public gameNode
{
public:
	sound();
	~sound();

	HRESULT init();
	void release();
	void update();
	void render();
};

