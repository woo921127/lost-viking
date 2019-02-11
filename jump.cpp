#include "stdafx.h"
#include "jump.h"

jump::jump()
{
}


jump::~jump()
{
}

HRESULT jump::init()
{
	_jumpPower = _gravity = _isJump = 0;

	return S_OK;
}

void jump::release()
{
}

void jump::update()
{
	if(_isJump == false) return;

	*_eric_Y -= _jumpPower;
	_jumpPower -= _gravity;

	if (*_start_Y < *_eric_Y)
	{
		_isJump = false;
		*_eric_Y = *_start_Y;
	}
}

void jump::render()
{
}

void jump::Jumping(float* x, float* y, float* startX, float* startY, float jumpPower, float gravity)
{
	if(_isJump == true) return;

	_isJump = true;

	_eric_X = x;
	_eric_Y = y;

	_start_X = startX;
	_start_Y = startY;

	_jumpPower = jumpPower;
	_gravity = gravity;
}
