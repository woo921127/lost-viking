#pragma once
#include "gameNode.h"

class jump : public gameNode
{
private:
	float* _eric_X;
	float* _eric_Y;

	float _gravity, _jumpPower;
	float* _start_X; 
	float* _start_Y;

	bool _isJump;

public:
	jump();
	~jump();

	HRESULT init();
	void release();
	void update();
	void render();

	void Jumping(float* x, float* y, float* startX, float* startY, float jumpPower, float gravity);

};

