#include "stdafx.h"
#include "eric.h"


eric::eric()
{
}


eric::~eric()
{
}

HRESULT eric::init()
{
	_ericImage = IMAGEMANAGER->addFrameImage("eric", "image/eric.bmp", 0, 0, 1760, 2170, 11, 14, true, RGB(255, 0, 255));

	_eric_X = WINSIZEX / 2;
	_eric_Y = WINSIZEY / 2;

	_ericState = ERIC_RIGHT_STOP;

	_eric_rc = RectMakeCenter(_eric_X, _eric_Y, _ericImage->getFrameWidth(), _ericImage->getFrameHeight());

	int rightStop[] = { 0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightStop", "eric", rightStop, 2, 4, true);

	int leftStop[] = { 2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "leftStop", "eric", leftStop, 2, 4, true);

	int rightMove[] = { 11,12,13,14,15,16,17,18 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightMove", "eric", rightMove, 8, 10, true);

	int leftMove[] = { 29,28,27,26,25,24,23,22 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "leftMove", "eric", leftMove, 8, 10, true);

	int rightDashArr[] = { 33, 34, 35, 36, 37, 38, 39, 40, 39, 38, 39, 40 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightDash", "eric", rightDashArr, 12, 10, false, rightDash, this);

	int leftDashArr[] = { 51, 50, 49, 48, 47, 46, 45, 44, 45, 46, 45, 44 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "leftDash", "eric", leftDashArr, 12, 10, false, leftDash, this);

	int rightJumpArr[] = { 77, 78, 79, 80 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightJump", "eric", rightJumpArr, 4, 5, false, rightJump, this);

	int leftJumpArr[] = { 84, 83, 82, 81 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "leftJump", "eric", leftJumpArr, 4, 5, false, leftJump, this);

	int upMove[] = { 88, 89, 90, 91 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "upMove", "eric", upMove, 4, 8, true);

	int downMove[] = { 91, 90, 89, 88 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "downMove", "eric", downMove, 4, 8, true);

	int rightPush[] = { 121, 122, 123, 124, 125 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightPush", "eric", rightPush, 5, 8, true);

	int leftPush[] = { 130, 129, 128, 127, 126 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "leftPush", "eric", leftPush, 5, 8, true);

	int rightDead[] = {132, 133, 134, 135, 136, 137, 138, 139};
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightDead", "eric", rightDead, 8, 6, false);

	int leftDead[] = {149, 148, 147, 146, 145, 144, 143, 142};
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "leftDead", "eric", leftDead, 8, 6, false);

	int right_Butt[] = {55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65};
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightButt", "eric", right_Butt, 11, 4, false, rightButt, this);

	int left_Butt[] = {76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66};
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "leftButt", "eric", left_Butt, 11, 4, false, leftButt, this);

	int fall[] = {10};
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "fall", "eric", fall, 1, 1, false);

	int rightFallDown[] = { 57, 58, 59, 60, 61, 62, 63, 64, 65 };
	KEYANIMANAGER->addArrayFrameAnimation("ericName", "rightFallDown", "eric", rightFallDown, 9, 4, false, rightCrash, this);

	_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightStop");

	_speed = 0;
	_acceleration = 0;
	_isLadderCollision = false;
	_isJump = false;

	_motionTime = 1.0f;
	_motionWorldTime = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void eric::release()
{

}

void eric::update(float viewX, float viewY, float* x, float* y)
{
	_eric_X = viewX;
	_eric_Y = viewY;

	keySetting();
	dashKeySetting();

	if (_isJump == false)
	{
		if (_ericState != ERIC_RIGHT_MOVE && (_ericState == ERIC_RIGHT_STOP || _ericState == ERIC_RIGHT_JUMP))
		{
			if (_motionTime + _motionWorldTime <= TIMEMANAGER->getWorldTime())
			{
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightStop");
				_motionWorldTime = TIMEMANAGER->getWorldTime();
			}
		}
		if (_ericState != ERIC_LEFT_MOVE && (_ericState == ERIC_LEFT_STOP || _ericState == ERIC_LEFT_JUMP))
		{
			if (_motionTime + _motionWorldTime <= TIMEMANAGER->getWorldTime())
			{
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftStop");
				_motionWorldTime = TIMEMANAGER->getWorldTime();
			}
		}
	}

	//가속도 주기위함
	if (_ericState == ERIC_RIGHT_MOVE)
	{
		if (_speed < 4)
		{
			_acceleration = 0.09f;
			*x += _speed;
			_speed += _acceleration;
		}
		else if (_speed >= 4)
		{
			_speed = 7;
		}
	}
	//가속도 주기위함
	else if (_ericState == ERIC_LEFT_MOVE)
	{
		if (_speed < 4)
		{
			_acceleration = 0.09f;
			*x -= _speed;
			_speed += _acceleration;
		}
		else if (_speed >= 4)
		{
			_speed = 7;
		}
	}

	switch (_ericState)
	{
	case ERIC_RIGHT_DASH:
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_ericState = ERIC_LEFT_MOVE;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftMove");
			_ericMotion->start();
		}
		break;
	case ERIC_LEFT_DASH:
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_ericState = ERIC_RIGHT_MOVE;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightMove");
			_ericMotion->start();
		}
		break;
	}

	//대쉬나 점프할때 오른쪽이나 왼쪽 꾹눌러도 무브상태로 하기위함.
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && (_ericState == ERIC_RIGHT_STOP || _ericState == ERIC_LEFT_STOP))
	{
		_ericState = ERIC_RIGHT_MOVE;
		_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightMove");
		_ericMotion->start();
	}
	else if (KEYMANAGER->isStayKeyDown(VK_LEFT) && (_ericState == ERIC_LEFT_STOP || _ericState == ERIC_RIGHT_STOP))
	{
		_ericState = ERIC_LEFT_MOVE;
		_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftMove");
		_ericMotion->start();
	}

	KEYANIMANAGER->update();
}

void eric::render(float viewX, float viewY)
{
	//char str[128];
	//sprintf_s(str, "에릭좌표 : %f ", _eric_X);
	//TextOut(getMemDC(), 200, 100, str, strlen(str));

	//sprintf_s(str, "사다리충돌 : %d ", _isLadderCollision);
	//TextOut(getMemDC(), 200, 120, str, strlen(str));

	//sprintf_s(str, "에릭 상태 : %d ", _ericState);
	//TextOut(getMemDC(), 600, 140, str, strlen(str));

	//sprintf_s(str, "에릭 가속도 : %f ", _acceleration);
	//TextOut(getMemDC(), 200, 610, str, strlen(str));

	//sprintf_s(str, "에릭 속도 : %f ", _speed);
	//TextOut(getMemDC(), 200, 630, str, strlen(str));

	_ericImage->aniRender(getMemDC(), viewX + 10, viewY, _ericMotion);
}
void eric::keySetting()
{
	if (_ericState != ERIC_RIGHT_DEAD && _ericState != ERIC_LEFT_DEAD)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && _ericState != ERIC_RIGHT_DASH && _ericState != ERIC_LEFT_DASH && _ericState != ERIC_RIGHT_BUTT && _ericState != ERIC_LEFT_BUTT && _ericState != ERIC_RIGHT_FALL_DOWN && _ericState != ERIC_FALL)
		{
			_speed = 1;
			_ericState = ERIC_RIGHT_MOVE;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightMove");
			_ericMotion->start();
			//점프상태에서 오른쪽키 눌러도 점프모션 나오기 위함.
			if (_isJump == true && _ericState == ERIC_RIGHT_MOVE)
			{
				_ericState = ERIC_RIGHT_JUMP;
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightJump");
				_ericMotion->start();
			}
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _ericState != ERIC_LEFT_MOVE && _ericState != ERIC_LEFT_DASH && _ericState != ERIC_RIGHT_DASH && _ericState != ERIC_UP_MOVE && _ericState != ERIC_DOWN_MOVE && _ericState != ERIC_RIGHT_JUMP && _ericState != ERIC_LEFT_JUMP && _ericState != ERIC_RIGHT_BUTT && _ericState != ERIC_LEFT_BUTT && _ericState != ERIC_FALL && _ericState != ERIC_RIGHT_FALL_DOWN)
		{
			_speed = 1;
			_ericState = ERIC_RIGHT_STOP;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightStop");
			_ericMotion->start();
			//점프상태에서 오른쪽키를 떼도 점프모션 나오기 위함.
			if (_isJump == true)
			{
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightJump");
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_LEFT) && _ericState != ERIC_LEFT_DASH && _ericState != ERIC_RIGHT_DASH && _ericState != ERIC_RIGHT_BUTT && _ericState != ERIC_LEFT_BUTT && _ericState != ERIC_RIGHT_FALL_DOWN && _ericState != ERIC_FALL)
		{
			_speed = 1;
			_ericState = ERIC_LEFT_MOVE;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftMove");
			_ericMotion->start();

			//점프상태에서 왼쪽키 눌러도 점프모션 나오기 위함.
			if (_isJump == true && _ericState == ERIC_LEFT_MOVE)
			{
				_ericState = ERIC_LEFT_JUMP;
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftJump");
				_ericMotion->start();
			}
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && _ericState != ERIC_RIGHT_MOVE && _ericState != ERIC_RIGHT_DASH && _ericState != ERIC_LEFT_DASH && _ericState != ERIC_UP_MOVE && _ericState != ERIC_DOWN_MOVE && _ericState != ERIC_RIGHT_JUMP && _ericState != ERIC_LEFT_JUMP && _ericState != ERIC_LEFT_BUTT && _ericState != ERIC_RIGHT_BUTT && _ericState != ERIC_FALL && _ericState != ERIC_RIGHT_FALL_DOWN)
		{
			_speed = 1;
			_ericState = ERIC_LEFT_STOP;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftStop");
			_ericMotion->start();
			//점프상태에서 왼쪽키를 떼도 점프모션 나오기 위함.
			if (_isJump == true)
			{
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftJump");
			}
		}
	}

	//사다리 충돌했을경우만 위아래 움직이게함.
	if (_isLadderCollision == true)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && _ericState != ERIC_FALL && _ericState != ERIC_RIGHT_FALL_DOWN)
		{
			_ericState = ERIC_UP_MOVE;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "upMove");
			_ericMotion->start();
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_UP) && _ericState != ERIC_RIGHT_MOVE && _ericState != ERIC_LEFT_MOVE && _ericState != ERIC_FALL && _ericState != ERIC_RIGHT_FALL_DOWN)
		{
			_ericMotion->pause();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _ericState != ERIC_FALL && _ericState != ERIC_RIGHT_FALL_DOWN)
		{
			_ericState = ERIC_DOWN_MOVE;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "downMove");
			_ericMotion->start();
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_DOWN) && _ericState != ERIC_RIGHT_MOVE && _ericState != ERIC_LEFT_MOVE && _ericState != ERIC_FALL && _ericState != ERIC_RIGHT_FALL_DOWN)
		{
			_ericMotion->pause();
		}
	}
}
void eric::jumpKeySetting()
{
	if (!(_ericState == ERIC_RIGHT_DASH) && !(_ericState == ERIC_LEFT_DASH))
	{
		SOUNDMANAGER->play("eric_Jump");
		_isJump = true;
		_motionTime = 0.7f;

		if (_ericState == ERIC_RIGHT_STOP || _ericState == ERIC_RIGHT_MOVE)
		{
			_ericState = ERIC_RIGHT_JUMP;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightJump");
			_ericMotion->start();
		}
		else if (_ericState == ERIC_LEFT_STOP || _ericState == ERIC_LEFT_MOVE)
		{
			_ericState = ERIC_LEFT_JUMP;
			_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftJump");
			_ericMotion->start();
		}
	}
}
void eric::dashKeySetting()
{
	if (KEYMANAGER->isOnceKeyDown('D'))
	{
		if (_speed >= 7)
		{
			SOUNDMANAGER->play("eric_Walk");
			if (_ericState == ERIC_RIGHT_STOP || _ericState == ERIC_RIGHT_MOVE)
			{
				_ericState = ERIC_RIGHT_DASH;
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "rightDash");
				_ericMotion->start();
			}

			else if (_ericState == ERIC_LEFT_STOP || _ericState == ERIC_LEFT_MOVE)
			{
				_ericState = ERIC_LEFT_DASH;
				_ericMotion = KEYANIMANAGER->findAnimation("ericName", "leftDash");
				_ericMotion->start();
			}
		}
	}

}
void eric::rightDash(void* obj)
{
	eric* e = (eric*)obj;
	if (e->getEricState() == ERIC_RIGHT_DASH)
	{
		e->setEricState(ERIC_RIGHT_STOP);
		e->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightStop"));
		e->getEricMotion()->start();
	}
}

void eric::leftDash(void* obj)
{
	eric* e = (eric*)obj;
	if (e->getEricState() == ERIC_LEFT_DASH)
	{
		e->setEricState(ERIC_LEFT_STOP);
		e->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftStop"));
		e->getEricMotion()->start();
	}
}

void eric::rightJump(void* obj)
{
	eric* e = (eric*)obj;
	if (e->getEricState() == ERIC_RIGHT_JUMP)
	{
		e->setEricState(ERIC_RIGHT_STOP);
		e->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightStop"));
		e->getEricMotion()->start();
	}
}

void eric::leftJump(void* obj)
{
	eric* e = (eric*)obj;
	if (e->getEricState() == ERIC_LEFT_JUMP)
	{
		e->setEricState(ERIC_LEFT_STOP);
		e->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftStop"));
		e->getEricMotion()->start();
	}
}

void eric::rightButt(void * obj)
{
	eric* e = (eric*)obj;
	if (e->getEricState() == ERIC_RIGHT_BUTT)
	{
		e->setEricState(ERIC_RIGHT_STOP);
		e->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightStop"));
		e->getEricMotion()->start();
	}
}

void eric::leftButt(void * obj)
{
	eric* e = (eric*)obj;
	if (e->getEricState() == ERIC_LEFT_BUTT)
	{
		e->setEricState(ERIC_LEFT_STOP);
		e->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftStop"));
		e->getEricMotion()->start();
	}
}

void eric::rightCrash(void * obj)
{
	eric* e = (eric*)obj;
	if (e->getEricState() == ERIC_RIGHT_FALL_DOWN)
	{
		e->setEricState(ERIC_RIGHT_STOP);
		e->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightStop"));
		e->getEricMotion()->start();
	}
}
