#include "stdafx.h"
#include "olaf.h"


olaf::olaf()
{
}


olaf::~olaf()
{
}

HRESULT olaf::init()
{
	_olafimg = IMAGEMANAGER->addFrameImage("olaf", "image\\olaf.bmp", 0, 0, 1280, 2560, 8, 16, true, RGB(255, 0, 255));

	_olafDirection = OLAF_DIRECTION_RIGHT_STOP;

	_olaf_x = 100;
	_olaf_y = 100;

	_speed = 0;
	_olaf_rc = RectMakeCenter(_olaf_x, _olaf_y, _olafimg->getFrameWidth(), _olafimg->getFrameHeight());


	int rightStop[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafRightStop", "olaf", rightStop, 1, 6, true);
	int rightShieldup[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafRightShield", "olaf", rightShieldup, 1, 6, true);
	int leftStop[] = { 2 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafLeftStop", "olaf", leftStop, 1, 6, true);
	int leftShieldup[] = { 3 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafLeftShield", "olaf", leftShieldup, 1, 6, true);
	int rightMove[] = { 8, 9,10,11,12,13,14,15 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafRightMove", "olaf", rightMove, 8, 6, true);
	int leftMove[] = { 23,22,21,20,19,18,17,16 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafLeftMove", "olaf", leftMove, 8, 6, true);
	int rightShieldUpMove[] = { 24,25,26,27,28,29,30,31 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafRightShieldUpMove", "olaf", rightShieldUpMove, 8, 6, true);
	int leftShieldUpMove[] = { 39,38,37,36,35,34,33,32 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafLeftShieldUpMove", "olaf", leftShieldUpMove, 8, 6, true);
	int upMove[] = { 40,41,42,43 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "upMove", "olaf", upMove, 4, 6, true);

	int downMove[] = { 43, 42, 41, 40 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "downMove", "olaf", downMove, 4, 6, true);

	int ShieldUpGravity[] = { 51,50,49,48 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafShieldUpRightGravity", "olaf", ShieldUpGravity, 4, 6, true);

	int ShieldDownGravity[] = { 52,53 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafShieldDownRightGravity", "olaf", ShieldDownGravity, 2, 6, true);

	int ShieldUpLeftGravity[] = { 58,59,60,61 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafShieldUpLeftGravity", "olaf", ShieldUpLeftGravity, 4, 6, true);

	int ShieldDownLeftGravity[] = { 56,57 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafShieldDownLeftGravity", "olaf", ShieldDownLeftGravity, 2, 6, true);

	int rightPush[] = { 64,65,66,67 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafRightPush", "olaf", rightPush, 4, 6, true);
	int leftPush[] = { 68,69,70,71 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "olafLeftPush", "olaf", leftPush, 4, 6, true);

	int rightDead[] = { 112, 113, 114, 115, 116, 117, 118, 119 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "rightDead", "olaf", rightDead, 8, 6, false);

	int leftDead[] = { 127, 126, 125, 124, 123, 122, 121, 120 };
	KEYANIMANAGER->addArrayFrameAnimation("olafName", "leftDead", "olaf", leftDead, 8, 6, false);

	_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightStop");


	num = 0;
	_changeMode = false;

	return S_OK;
}

void olaf::release()
{
}

void olaf::update(float viewX, float viewY, float* x, float* y)
{

	_olaf_x = viewX;
	_olaf_y = viewY;

	//f (_olafDirection == OLAF_DIRECTION_RIGHT_MOVE)
	//
	//	*x += _speed ;
	//	
	//
	//lse if (_olafDirection == OLAF_DIRECTION_LEFT_MOVE)
	//
	//	*y -= _speed ;
	//	
	//

	olafMovement();

	KEYANIMANAGER->update();
}

void olaf::render(float viewX, float viewY)
{
	_olafimg->aniRender(getMemDC(), viewX + 20, viewY - 10, _olafMotion);

	//char str[128];
	//sprintf_s(str, "상태확인 : %d    확인용 : %f", _olafDirection, _olaf_x);
	//TextOut(getMemDC(), 200, 40, str, strlen(str));
}

void olaf::olafMovement()
{
	//==========================SHILED CHANGE==========================//
	//==========================실드가 앞에 있을때 ====================//
	if (_changeMode == false)
	{
		if (_olafDirection == OLAF_DIRECTION_RIGHT_STOP)
		{
			if (KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))
			{
				_olafDirection = OLAF_DIRECTION_RIGHT_SHIELD;
				_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightShield");
				_olafMotion->start();
				_changeMode = true;
			}
		}
		else if (_olafDirection == OLAF_DIRECTION_LEFT_STOP)
		{
			if (KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))
			{
				_olafDirection = OLAF_DIRECTION_LEFT_SHIELD;
				_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftShield");
				_olafMotion->start();
				_changeMode = true;
			}
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_olafDirection = OLAF_DIRECTION_RIGHT_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightMove");
			_olafMotion->start();
		}
		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && ((KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))))
		{
			_olafDirection = OLAF_DIRECTION_RIGHT_SHIELD_UP_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightShieldUpMove");
			_olafMotion->start();
			_changeMode = true;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _olafDirection == OLAF_DIRECTION_RIGHT_MOVE)
		{
			_olafDirection = OLAF_DIRECTION_RIGHT_STOP;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightStop");
			_olafMotion->start();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_olafDirection = OLAF_DIRECTION_LEFT_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftMove");
			_olafMotion->start();
		}
		else if (KEYMANAGER->isStayKeyDown(VK_LEFT) && ((KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))))
		{
			_olafDirection = OLAF_DIRECTION_LEFT_SHIELD_UP_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftShieldUpMove");
			_olafMotion->start();
			_changeMode = true;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && _olafDirection == OLAF_DIRECTION_LEFT_MOVE)
		{
			_olafDirection = OLAF_DIRECTION_LEFT_STOP;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftStop");
			_olafMotion->start();
		}

	}


	//=============실드가 머리 위에 있을때======================//
	else if (_changeMode == true)
	{
		if (_olafDirection == OLAF_DIRECTION_RIGHT_SHIELD)
		{

			if (KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))
			{
				_olafDirection = OLAF_DIRECTION_RIGHT_STOP;
				_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightStop");
				_olafMotion->start();
				_changeMode = false;
			}
		}
		else if (_olafDirection == OLAF_DIRECTION_LEFT_SHIELD)
		{
			if (KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))
			{
				_olafDirection = OLAF_DIRECTION_LEFT_STOP;
				_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftStop");
				_olafMotion->start();
				_changeMode = false;
			}
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_olafDirection = OLAF_DIRECTION_RIGHT_SHIELD_UP_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightShieldUpMove");
			_olafMotion->start();
		}
		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && ((KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))))
		{
			_olafDirection = OLAF_DIRECTION_RIGHT_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightMove");
			_olafMotion->start();
			_changeMode = false;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _olafDirection == OLAF_DIRECTION_RIGHT_SHIELD_UP_MOVE)
		{
			_olafDirection = OLAF_DIRECTION_RIGHT_SHIELD;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafRightShield");
			_olafMotion->start();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_olafDirection = OLAF_DIRECTION_LEFT_SHIELD_UP_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftShieldUpMove");
			_olafMotion->start();
		}
		else if (KEYMANAGER->isStayKeyDown(VK_LEFT) && ((KEYMANAGER->isOnceKeyDown('F') || KEYMANAGER->isOnceKeyDown('D'))))
		{
			_olafDirection = OLAF_DIRECTION_LEFT_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftMove");
			_olafMotion->start();
			_changeMode = false;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && _olafDirection == OLAF_DIRECTION_LEFT_SHIELD_UP_MOVE)
		{
			_olafDirection = OLAF_DIRECTION_LEFT_SHIELD;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "olafLeftShield");
			_olafMotion->start();
		}
	}
	//사다리 충돌했을경우 오르락 내리락 하기위함.
	if (_isLadderCollision == true)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_olafDirection = OLAF_DIRECTION_UP_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "upMove");
			_olafMotion->start();
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_UP))
		{
			_olafMotion->pause();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_olafDirection = OLAF_DIRECTION_DOWN_MOVE;
			_olafMotion = KEYANIMANAGER->findAnimation("olafName", "downMove");
			_olafMotion->start();
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
		{
			_olafMotion->pause();
		}
	}

	//=================================================================//

}
