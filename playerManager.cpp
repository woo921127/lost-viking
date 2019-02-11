#include "stdafx.h"
#include "playerManager.h"
#include "item.h"

playerManager::playerManager()
{
}


playerManager::~playerManager()
{
}

HRESULT playerManager::init()
{
	_eric = new eric;
	_eric->init();
	_baleog = new baleog;
	_baleog->init();
	_olaf = new olaf;
	_olaf->init();

	//벨로그 화살부분
	IMAGEMANAGER->addFrameImage("화살", "image/bullet.bmp", 85, 20, 5, 2, true, RGB(255, 0, 255));
	_arrow = new arrow;
	_arrow->init("화살", 0, 600);
	_isArrowFireStop = false;

	for (int i = 0; i < 3; ++i)
	{
		_x[i] = 30 + 120 * i;
		_y[i] = 75;
		_viewX[i] = _x[i];
		_viewY[i] = _y[i];
	}

	for (int i = 0; i < 3; ++i)
	{
		_rc[i] = RectMake(_viewX[i], _viewY[i], 100, 100);
	}

	//사다리 부분
	_ladder[0].x = 920;
	_ladder[0].y = 1350;
	_ladder[0].viewX = _ladder[0].x;
	_ladder[0].viewY = _ladder[0].y;
	_ladder[0].rc = RectMake(_ladder[0].viewX, _ladder[0].viewY, 30, 285);

	_ladder[1].x = 2000;
	_ladder[1].y = 1150;
	_ladder[1].viewX = _ladder[1].x;
	_ladder[1].viewY = _ladder[1].y;
	_ladder[1].rc = RectMake(_ladder[1].viewX, _ladder[1].viewY, 30, 680);

	_ladder[2].x = 2140;
	_ladder[2].y = 1805;
	_ladder[2].viewX = _ladder[2].x;
	_ladder[2].viewY = _ladder[2].y;
	_ladder[2].rc = RectMake(_ladder[2].viewX, _ladder[2].viewY, 30, 295);

	_ladder[3].x = 2515;
	_ladder[3].y = 1520;
	_ladder[3].viewX = _ladder[3].x;
	_ladder[3].viewY = _ladder[3].y;
	_ladder[3].rc = RectMake(_ladder[3].viewX, _ladder[3].viewY, 30, 255);

	_ladder[4].x = 2890;
	_ladder[4].y = 1285;
	_ladder[4].viewX = _ladder[4].x;
	_ladder[4].viewY = _ladder[4].y;
	_ladder[4].rc = RectMake(_ladder[4].viewX, _ladder[4].viewY, 30, 495);

	//데드존
	_deadZone[0].x = 840;
	_deadZone[0].y = 940;
	_deadZone[0].viewX = _deadZone[0].x;
	_deadZone[0].viewY = _deadZone[0].y;
	_deadZone[0].rc = RectMake(_deadZone[0].viewX, _deadZone[0].viewY, 285, 85);

	_deadZone[1].x = 1025;
	_deadZone[1].y = 1740;
	_deadZone[1].viewX = _deadZone[1].x;
	_deadZone[1].viewY = _deadZone[1].y;
	_deadZone[1].rc = RectMake(_deadZone[1].viewX, _deadZone[1].viewY, 200, 80);

	_deadZone[2].x = 1690;
	_deadZone[2].y = 2180;
	_deadZone[2].viewX = _deadZone[2].x;
	_deadZone[2].viewY = _deadZone[2].y;
	_deadZone[2].rc = RectMake(_deadZone[2].viewX, _deadZone[2].viewY, 140, 60);
	
	_isDebug = false;

	for (int i = 0; i < 3; ++i)
	{
		//센터
		_prove_X[i] = (_rc[i].right - _rc[i].left) / 2 + _rc[i].left;
		_prove_Y[i] = _rc[i].bottom;
	}

	_camerc_x = 100;
	_camerc_y = 100;
	_cameview_x = _camerc_x;
	_cameview_y = _camerc_y;

	_camerc = RectMake(_cameview_x, _cameview_y, 50, 50);

	_jumpPower = 8.f;
	_gravity = 0.3;
	_isJump = false;
	_jumpNum = 1;
	_choice = ERIC;

	_ericSound = _baleogSound =  _olafSound = 0;

	_camera = new camera;
	_camera->init();

	_isCameraMode = false;
	_isLadderCollision[3] = false;
	_isButt = false;
	_buttAngle = (PI / 180) * 150;
	_isFallDown = false;

	_deadTime = 0;
	_moveWorldTime = TIMEMANAGER->getWorldTime();
	_moveTime = 0.45f;

	_soundOldtime = GetTickCount();

	return S_OK;
}

void playerManager::release()
{
	SAFE_DELETE(_camera);
	SAFE_DELETE(_eric);
	SAFE_DELETE(_baleog);
	SAFE_DELETE(_olaf);
	SAFE_DELETE(_arrow);
}

void playerManager::update()
{
	//벨로그 화살
	_arrow->update();
	for (int i = 0; i < _arrow->getVArrow().size(); i++)
	{
		(*_arrow->getVArrowAddress())[i].viewX = (*_arrow->getVArrowAddress())[i].x - _camera->getCameraX();
		(*_arrow->getVArrowAddress())[i].viewY = (*_arrow->getVArrowAddress())[i].y - _camera->getCameraY();
	}

	//박치기하면 밀려남.
	if (_eric->getEricState() == ERIC_RIGHT_BUTT || _eric->getEricState() == ERIC_LEFT_BUTT)
	{
		_x[ERIC] += cosf(_buttAngle) * 0.3f;
		_y[ERIC] += -sinf(_buttAngle) * 0.3f;
	}

	//에릭의 점프
	jumpGravity(_choice);
	//사다리 충돌
	ladderCollision();
	//데드존 충돌
	deadZoneCollision();
	//픽셀충돌
	pixelCollisionEmerald();
	pixelCollisionGreen();
	pixelCollisionYellow();
	pixelCollisionRed();
	
	//캐릭터 무브
	characterMove();
	//캐릭터 고르기
	characterChoice();
	//카메라 체인지
	characterChange();
	
	//캐릭터박스 보이기용
	if (KEYMANAGER->isOnceKeyDown(VK_NUMPAD1))
	{
		if (_isDebug)
		{
			_isDebug = false;
		}
		else
		{
			_isDebug = true;
		}
	}

	//가상좌표 갱신
	for (int i = 0; i < 3; ++i)
	{
		_viewX[i] = _x[i] - _camera->getCameraX();
		_viewY[i] = _y[i] - _camera->getCameraY();
	}

	//사다리 갱신
	for (int i = 0; i < 5; i++)
	{
		_ladder[i].viewX = _ladder[i].x - _camera->getCameraX();
		_ladder[i].viewY = _ladder[i].y - _camera->getCameraY();
	}
	_ladder[0].rc = RectMake(_ladder[0].viewX, _ladder[0].viewY, 30, 285);
	_ladder[1].rc = RectMake(_ladder[1].viewX, _ladder[1].viewY, 30, 670);
	_ladder[2].rc = RectMake(_ladder[2].viewX, _ladder[2].viewY, 30, 300);
	_ladder[3].rc = RectMake(_ladder[3].viewX, _ladder[3].viewY, 30, 260);
	_ladder[4].rc = RectMake(_ladder[4].viewX, _ladder[4].viewY, 30, 490);

	//데드존 갱신
	for (int i = 0; i < 3; i++)
	{
		_deadZone[i].viewX = _deadZone[i].x - _camera->getCameraX();
		_deadZone[i].viewY = _deadZone[i].y - _camera->getCameraY();
	}
	_deadZone[0].rc = RectMake(_deadZone[0].viewX, _deadZone[0].viewY, 285, 85);
	_deadZone[1].rc = RectMake(_deadZone[1].viewX, _deadZone[1].viewY, 200, 80);
	_deadZone[2].rc = RectMake(_deadZone[2].viewX, _deadZone[2].viewY, 140, 60);

	//갱신
	for (int i = 0; i < 3; ++i)
	{
		_rc[i] = RectMake(_viewX[i], _viewY[i], 100, 100);
		_prove_X[i] = _x[i] + 50;
		_prove_Y[i] = _y[i] + 100;
	}

	if (_choice == ERIC)
	{
		_eric->update(_viewX[ERIC] - 40, _viewY[ERIC] - 60, &_x[ERIC], &_y[ERIC]);
	}
	else if (_choice == BALEOG)
	{
		_baleog->update(_viewX[BALEOG] + 50, _viewY[BALEOG] + 50, &_x[BALEOG], &_y[BALEOG]);
	}

	else if(_choice == OLAF)
	{
		_olaf->update(_viewX[OLAF] + 50, _viewY[OLAF] + 50, &_x[OLAF], &_y[OLAF]);
	}

	_cameview_x = _camerc_x - _camera->getCameraX();
	_cameview_y = _camerc_y - _camera->getCameraY();

	_camerc = RectMake(_cameview_x, _cameview_y, 50, 50);

	if (KEYMANAGER->isStayKeyDown('L'))
	{
		_camerc_x += 15;
	}
	if (KEYMANAGER->isStayKeyDown('J'))
	{
		_camerc_x -= 15;
	}
	if (KEYMANAGER->isStayKeyDown('K'))
	{
		_camerc_y += 15;
	}
	if (KEYMANAGER->isStayKeyDown('I'))
	{
		_camerc_y -= 15;
	}

	ericwallcheck();
	baleogwallcheck();
	olafwallcheck();
	olafShieldUpMove();
	olafSHieldDownMove();
	SideIn();

	if (_olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_UP_GRAVITY || _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_UP_GRAVITY)
	{
		_y[OLAF] += 1;
		_gravityStop[OLAF] = true;
	}

}

void playerManager::render()
{
	_eric->render(_viewX[ERIC] - 40, _viewY[ERIC] - 60);
	_baleog->render(_viewX[BALEOG] - 40, _viewY[BALEOG] - 50);
	_olaf->render(_viewX[OLAF] - 50, _viewY[OLAF] - 50);

	//사다리 렉트
	//for (int i = 0; i < 5; i++)
	//{
	//	Rectangle(getMemDC(), _ladder[i].rc);
	//}
	//데드존 렉트
	//for (int i = 0; i < 3; i++)
	//{
	//	Rectangle(getMemDC(), _deadZone[i].rc);
	//}
	for (int i = 0; i < _arrow->getVArrow().size(); i++)
	{
		_arrow->render((*_arrow->getVArrowAddress())[i].viewX, (*_arrow->getVArrowAddress())[i].viewY);
	}

	if (_isDebug)
	{
		for (int i = 0; i < 3; ++i)
		{
			Rectangle(getMemDC(), _rc[i]);
		}
	}

	//Rectangle(getMemDC(), _camerc);
	//_camera->render();
	//char str[100];
	//sprintf_s(str, "_x : %d", _x[0]);
	//TextOut(getMemDC(), 300, 100, str, strlen(str));

	//sprintf_s(str, "_prove_X : %f", _prove_X[0]);
	//TextOut(getMemDC(), 300, 120, str, strlen(str));
	//
	//sprintf_s(str, "_prove_Y : %f", _prove_Y[0]);
	//TextOut(getMemDC(), 300, 140, str, strlen(str));
	//
	//sprintf_s(str, "_isLadderCollision : %d", _isLadderCollision);
	//TextOut(getMemDC(), 300, 160, str, strlen(str));
	//sprintf_s(str, "_gravityStop : %d", _gravityStop);
	//TextOut(getMemDC(), 300, 180, str, strlen(str));
	//sprintf_s(str, "_deadTime : %d", _deadTime);
	//TextOut(getMemDC(), 300, 200, str, strlen(str));

	//sprintf_s(str, "_isDead[ERIC] : %d", _isDead[ERIC]);
	//TextOut(getMemDC(), 300, 220, str, strlen(str));

	//sprintf_s(str, "_isButt : %d", _isButt);
	//TextOut(getMemDC(), 300, 240, str, strlen(str));

	//sprintf_s(str, "_buttAngle : %f", _buttAngle);
	//TextOut(getMemDC(), 300, 260, str, strlen(str));

	//sprintf_s(str, "_isLadderCollision: %d", _isLadderCollision);
	//TextOut(getMemDC(), 300, 280, str, strlen(str));
}

void playerManager::characterChoice()
{
	if (_choice == ERIC)
	{
		_camera->update(_x[ERIC], _y[ERIC]);
	}
	else if (_choice == BALEOG)
	{
		_camera->update(_x[BALEOG], _y[BALEOG]);
	}
	else if (_choice == OLAF)
	{
		_camera->update(_x[OLAF], _y[OLAF]);
	}

	//카메라모드 활성화
	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		if (!_isCameraMode)
		{
			_isCameraMode = true;
		}
		else if (_isCameraMode)
		{
			_isCameraMode = false;
		}
	}
	if (_isCameraMode)
	{
		_camera->update(_camerc_x, _camerc_y);
	}
}

void playerManager::characterMove()
{
	if (_camera->getChange() == false)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _baleog->getBaleogState() != BALEOG_LEFT_SWORD_ATTACK_ONE && _baleog->getBaleogState() != BALEOG_LEFT_SWORD_ATTACK_TWO && _baleog->getBaleogState() != BALEOG_RIGHT_SWORD_ATTACK_ONE && _baleog->getBaleogState() != BALEOG_RIGHT_SWORD_ATTACK_TWO && _baleog->getBaleogState() != BALEOG_LEFT_ARROW_ATTACK && _baleog->getBaleogState() != BALEOG_RIGHT_ARROW_ATTACK && _eric->getEricState() != ERIC_RIGHT_BUTT && _eric->getEricState() != ERIC_LEFT_BUTT && _eric->getEricState() != ERIC_RIGHT_FALL_DOWN && _eric->getEricState() != ERIC_FALL)
		{
			if (_moveTime + _moveWorldTime <= TIMEMANAGER->getWorldTime())
			{
				_moveWorldTime = TIMEMANAGER->getWorldTime();
				if (_eric->getEricState() != ERIC_RIGHT_JUMP && _eric->getEricState() != ERIC_LEFT_JUMP)
				{
					SOUNDMANAGER->play("viking_Movement", 0.3f);
				}
			}
			if (_choice == ERIC && _eric->getEricState() != ERIC_RIGHT_DEAD && _eric->getEricState() != ERIC_LEFT_DEAD)
			{
				_x[ERIC] -= _eric->getSpeed();
				_gravityStop[ERIC] = false;
				_isLadderCollision[ERIC] = false;
			}
			else if (_choice == BALEOG && _baleog->getBaleogState() != BALEOG_RIGHT_DEAD && _baleog->getBaleogState() != BALEOG_LEFT_DEAD)
			{
				_x[BALEOG] -= _baleog->getSpeed() + 5;
				_gravityStop[BALEOG] = false;
				_isLadderCollision[BALEOG] = false;
			}
			else if (_choice == OLAF && _olaf->getOlafDirection() != OLAF_DIRECTION_RIGHT_DEAD && _olaf->getOlafDirection() != OLAF_DIRECTION_LEFT_DEAD)
			{
				_x[OLAF] -= _olaf->getSpeed() + 5;
				_gravityStop[OLAF] = false;
				_isLadderCollision[OLAF] = false;
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _baleog->getBaleogState() != BALEOG_RIGHT_SWORD_ATTACK_ONE && _baleog->getBaleogState() != BALEOG_RIGHT_SWORD_ATTACK_TWO && _baleog->getBaleogState() != BALEOG_LEFT_SWORD_ATTACK_ONE && _baleog->getBaleogState() != BALEOG_LEFT_SWORD_ATTACK_TWO && _baleog->getBaleogState() != BALEOG_RIGHT_ARROW_ATTACK && _baleog->getBaleogState() != BALEOG_LEFT_ARROW_ATTACK && _eric->getEricState() != ERIC_RIGHT_BUTT && _eric->getEricState() != ERIC_LEFT_BUTT && _eric->getEricState() != ERIC_RIGHT_FALL_DOWN && _eric->getEricState() != ERIC_FALL)
		{
			if (_moveTime + _moveWorldTime <= TIMEMANAGER->getWorldTime())
			{
				_moveWorldTime = TIMEMANAGER->getWorldTime();
				if (_eric->getEricState() != ERIC_RIGHT_JUMP && _eric->getEricState() != ERIC_LEFT_JUMP)
				{
					SOUNDMANAGER->play("viking_Movement", 0.3f);
				}
			}
			if (_choice == ERIC && _eric->getEricState() != ERIC_RIGHT_DEAD && _eric->getEricState() != ERIC_LEFT_DEAD)
			{
				_x[ERIC] += _eric->getSpeed();
				_gravityStop[ERIC] = false;
				_isLadderCollision[ERIC] = false;
			}
			else if (_choice == BALEOG && _baleog->getBaleogState() != BALEOG_RIGHT_DEAD && _baleog->getBaleogState() != BALEOG_LEFT_DEAD)
			{
				_x[BALEOG] += _baleog->getSpeed() + 5;
				_gravityStop[BALEOG] = false;
				_isLadderCollision[BALEOG] = false;
			
			}
			else if (_choice == OLAF && _olaf->getOlafDirection() != OLAF_DIRECTION_RIGHT_DEAD && _olaf->getOlafDirection() != OLAF_DIRECTION_LEFT_DEAD)
			{
				_x[OLAF] += _olaf->getSpeed() + 5;
				_gravityStop[OLAF] = false;
				_isLadderCollision[OLAF] = false;
			}
		}
		//벨로그 화살쏘는부분
		if (_choice == BALEOG)
		{
			baleogArrow();
		}
	}
	//카메라 체인지 도중이면 다른애들 움직여도 멈추게함.
	else if (_camera->getChange() == true)
	{
		if (_choice == ERIC)
		{
			if (_baleog->getBaleogState() == BALEOG_RIGHT_MOVE)
			{
				_baleog->setBaleogState(BALEOG_RIGHT_STOP);
				_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "rightStop"));
				_baleog->getBaleogMotion()->start();
			}
			else if (_baleog->getBaleogState() == BALEOG_LEFT_MOVE)
			{
				_baleog->setBaleogState(BALEOG_LEFT_STOP);
				_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "leftStop"));
				_baleog->getBaleogMotion()->start();
			}
			if (_olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_MOVE)
			{
				_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_STOP);
				_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafRightStop"));
				_olaf->getOlafMotion()->start();
			}
			else if (_olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_MOVE)
			{
				_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_STOP);
				_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafLeftStop"));
				_olaf->getOlafMotion()->start();
			}
		}
		else if (_choice == BALEOG)
		{
			if (_eric->getEricState() == ERIC_RIGHT_MOVE)
			{
				_eric->setEricState(ERIC_RIGHT_STOP);
				_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightStop"));
				_eric->getEricMotion()->start();
			}
			else if (_eric->getEricState() == ERIC_LEFT_MOVE)
			{
				_eric->setEricState(ERIC_LEFT_STOP);
				_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftStop"));
				_eric->getEricMotion()->start();
			}
			if (_olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_MOVE)
			{
				_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_STOP);
				_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafRightStop"));
				_olaf->getOlafMotion()->start();
			}
			else if (_olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_MOVE)
			{
				_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_STOP);
				_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafLeftStop"));
				_olaf->getOlafMotion()->start();
			}
		}
		else if (_choice == OLAF)
		{
			if (_eric->getEricState() == ERIC_RIGHT_MOVE)
			{
				_eric->setEricState(ERIC_RIGHT_STOP);
				_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightStop"));
				_eric->getEricMotion()->start();
			}
			else if (_eric->getEricState() == ERIC_LEFT_MOVE)
			{
				_eric->setEricState(ERIC_LEFT_STOP);
				_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftStop"));
				_eric->getEricMotion()->start();
			}
			if (_baleog->getBaleogState() == BALEOG_RIGHT_MOVE)
			{
				_baleog->setBaleogState(BALEOG_RIGHT_STOP);
				_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "rightStop"));
				_baleog->getBaleogMotion()->start();
			}
			else if (_baleog->getBaleogState() == BALEOG_LEFT_MOVE)
			{
				_baleog->setBaleogState(BALEOG_LEFT_STOP);
				_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "leftStop"));
				_baleog->getBaleogMotion()->start();
			}
		}
	}
}

void playerManager::characterChange()
{
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		if (_choice == ERIC )
		{
			_baleogSound = RND->getFromIntTo(0, 5);
			switch (_baleogSound)
			{
				case 0:
					SOUNDMANAGER->play("baleog");
					break;
				case 1:
					SOUNDMANAGER->play("baleog_2");
					break;
				case 2:
					SOUNDMANAGER->play("baleog_3");
					break;
				case 3:
					SOUNDMANAGER->play("baleog_4");
					break;
				case 4:
					SOUNDMANAGER->play("baleog_5");
					break;
			}
			if (_isDead[BALEOG] == false)
			{
				_choice = BALEOG;
				_camera->cameraChange(_x[BALEOG], _y[BALEOG]);
				SOUNDMANAGER->play("UI_EricPic");
			}
			else if(_isDead[OLAF] == false)
			{
				_choice = OLAF;
				_camera->cameraChange(_x[OLAF], _y[OLAF]);
				SOUNDMANAGER->play("UI_BaleogPic");
			}
		}
		else if (_choice == BALEOG)
		{
			_olafSound = RND->getFromIntTo(0, 5);
			switch (_olafSound)
			{
			case 0:
				SOUNDMANAGER->play("olaf");
				break;
			case 1:
				SOUNDMANAGER->play("olaf_2");
				break;
			case 2:
				SOUNDMANAGER->play("olaf_3");
				break;
			case 3:
				SOUNDMANAGER->play("olaf_4");
				break;
			case 4:
				SOUNDMANAGER->play("olaf_5");
				break;
			}
			if (_isDead[OLAF] == false)
			{
				_choice = OLAF;
				_camera->cameraChange(_x[OLAF], _y[OLAF]);
				SOUNDMANAGER->play("UI_BaleogPic");
			}
			else if(_isDead[ERIC] == false)
			{
				_choice = ERIC;
				_camera->cameraChange(_x[ERIC], _y[ERIC]);
				SOUNDMANAGER->play("UI_OlafPic");
			}
		}
		else if (_choice == OLAF)
		{
			_ericSound = RND->getFromIntTo(0, 5);
			switch (_ericSound)
			{
			case 0:
				SOUNDMANAGER->play("eric");
				break;
			case 1:
				SOUNDMANAGER->play("eric_2");
				break;
			case 2:
				SOUNDMANAGER->play("eric_3");
				break;
			case 3:
				SOUNDMANAGER->play("eric_4");
				break;
			case 4:
				SOUNDMANAGER->play("eric_5");
				break;
			}
			if (_isDead[ERIC] == false)
			{
				_choice = ERIC;
				_camera->cameraChange(_x[ERIC], _y[ERIC]);
				SOUNDMANAGER->play("UI_OlafPic");
			}
			else if(_isDead[BALEOG] == false)
			{
				_choice = BALEOG;
				_camera->cameraChange(_x[BALEOG], _y[BALEOG]);
				SOUNDMANAGER->play("UI_BaleogPic");
			}
		}
	}
}

void playerManager::ladderCollision()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{	
			//사다리 범위 안에 들어오면
			if (_rc[i].left < _ladder[j].rc.right - 50 && _rc[i].right > _ladder[j].rc.left + 50 && _rc[i].top < _ladder[j].rc.bottom && _rc[i].bottom > _ladder[j].rc.top)
			{
				//낙하 부분 삭제
				//if (_rc[i].top > _ladder[j].rc.top && _rc[i].bottom < _ladder[j].rc.top + (_ladder[j].rc.bottom - _ladder[j].rc.top) / 2 && _rc[i].left <= _ladder[j].rc.right && _rc[i].right >= _ladder[j].rc.left)
				//{
				//	if (_eric->getEricState() != ERIC_UP_MOVE && _eric->getEricState() != ERIC_DOWN_MOVE && _eric->getEricState() != ERIC_RIGHT_STOP && _eric->getEricState() != ERIC_LEFT_STOP && _eric->getEricState() != ERIC_RIGHT_FALL_DOWN)
				//	{
				//		_eric->setEricState(ERIC_FALL);
				//		_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "fall"));
				//		_eric->getEricMotion()->start();
				//		_isFallDown = true;
				//	}
				//}
				_ladderChoice = j; //몇번째 사다리 충돌했는지 저장할 변수
				//사다리 충돌
				if (_camera->getChange() == false)
				{
					if (KEYMANAGER->isStayKeyDown(VK_UP))
					{
						if (_choice == ERIC)
						{
							_eric->setLadderCollision(true);
							_y[ERIC] -= _eric->getSpeed() + 5;
							_isLadderCollision[ERIC] = true;
							_gravityStop[ERIC] = true; //중력 멈추고
						}
						else if (_choice == BALEOG)
						{
							_baleog->setLadderCollision(true);
							_y[BALEOG] -= _baleog->getSpeed() + 5;
							_isLadderCollision[BALEOG] = true;
							_gravityStop[BALEOG] = true; //중력 멈추고
						}
						else if (_choice == OLAF)
						{
							_olaf->setLadderCollision(true);
							_y[OLAF] -= _olaf->getSpeed() + 5;
							_isLadderCollision[OLAF] = true;
							_gravityStop[OLAF] = true; //중력 멈추고
						}
					}

					if (_rc[i].bottom < _ladder[j].rc.bottom)
					{
						if (KEYMANAGER->isStayKeyDown(VK_DOWN))
						{
							if (_choice == ERIC)
							{
								_eric->setLadderCollision(true);
								_y[ERIC] += _eric->getSpeed() + 5;
								_isLadderCollision[ERIC] = true;
								_gravityStop[ERIC] = true; //중력 멈추고
							}
							else if (_choice == BALEOG)
							{
								_baleog->setLadderCollision(true);
								_y[BALEOG] += _baleog->getSpeed() + 5;
								_isLadderCollision[BALEOG] = true;
								_gravityStop[BALEOG] = true; //중력 멈추고
							}
							else if (_choice == OLAF)
							{
								_olaf->setLadderCollision(true);
								_y[OLAF] += _olaf->getSpeed() + 5;
								_isLadderCollision[OLAF] = true;
								_gravityStop[OLAF] = true; //중력 멈추고
							}
						}
					}
				}
			}
			else if	(!(_rc[_choice].left < _ladder[_ladderChoice].rc.right - 50 && _rc[_choice].right > _ladder[_ladderChoice].rc.left + 50 && _rc[_choice].top < _ladder[_ladderChoice].rc.bottom && _rc[_choice].bottom > _ladder[_ladderChoice].rc.top))
			//else if (!IntersectRect(&temp, &_rc[_choice], &_ladder[_ladderChoice].rc))
			{
				if (_camera->getChange() == false)
				{
					if(_choice == ERIC)
					{
						_isLadderCollision[ERIC] = false;
						_gravityStop[ERIC] = false;
						_eric->setLadderCollision(false);
					}
					else if (_choice == BALEOG)
					{
						_isLadderCollision[BALEOG] = false;
						_gravityStop[BALEOG] = false;
						_baleog->setLadderCollision(false);
					}
					else if (_choice == BALEOG)
					{
						_isLadderCollision[OLAF] = false;
						_gravityStop[OLAF] = false;
						_olaf->setLadderCollision(false);
					}
			
				}
			}

		}
	}
}

void playerManager::deadZoneCollision()
{
	if (_deadTime > 70)
	{
		if (_isDead[ERIC] == false || _isDead[BALEOG] == false || _isDead[OLAF] == false)
		{
			if (_choice == ERIC)
			{
				_deadTime = 0;
				_choice = BALEOG;
				_camera->cameraChange(_x[BALEOG], _y[BALEOG]);
				SOUNDMANAGER->play("UI_EricPic");
				_x[ERIC] = 0;
				_y[ERIC] = 0;

			}
			else if (_choice == BALEOG)
			{
				_deadTime = 0;
				_choice = OLAF;
				_camera->cameraChange(_x[OLAF], _y[OLAF]);
				SOUNDMANAGER->play("UI_BaleogPic");
				_x[BALEOG] = 0;
				_y[BALEOG] = 0;

			}
			else if (_choice == OLAF)
			{
				_deadTime = 0;
				_choice = ERIC;
				_camera->cameraChange(_x[ERIC], _y[ERIC]);
				SOUNDMANAGER->play("UI_OlafPic");
				_x[OLAF] = 0;
				_y[OLAF] = 0;

			}
		}
	}
	
	RECT temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (IntersectRect(&temp, &_rc[i], &_deadZone[j].rc))
			{
				if (_choice == ERIC)
				{
					if (_deadZone[j].rc.top < _rc[_choice].bottom)
					{
						_deadTime++;
						_isDead[ERIC] = true;
						_deadTum = _rc[_choice].bottom - _deadZone[j].rc.top;
						_y[ERIC] = _y[ERIC] - _deadTum;

						if (_eric->getEricState() == ERIC_RIGHT_MOVE || _eric->getEricState() == ERIC_RIGHT_STOP || _eric->getEricState() == ERIC_RIGHT_JUMP || _eric->getEricState() == ERIC_LEFT_JUMP)
						{
							_eric->setEricState(ERIC_RIGHT_DEAD);
							_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightDead"));
							_eric->getEricMotion()->start();
	
						}
						else if (_eric->getEricState() == ERIC_LEFT_MOVE || _eric->getEricState() == ERIC_LEFT_STOP || _eric->getEricState() == ERIC_RIGHT_JUMP || _eric->getEricState() == ERIC_LEFT_JUMP)
						{
							_eric->setEricState(ERIC_LEFT_DEAD);
							_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftDead"));
							_eric->getEricMotion()->start();

						}
					}
				}
				else if (_choice == BALEOG)
				{
					if (_deadZone[j].rc.top < _rc[_choice].bottom)
					{
						_deadTime++;
						_isDead[BALEOG] = true;
						_deadTum = _rc[_choice].bottom - _deadZone[j].rc.top;
						_y[BALEOG] = _y[BALEOG] - _deadTum;

						if (_baleog->getBaleogState() == BALEOG_RIGHT_MOVE || _baleog->getBaleogState() == BALEOG_RIGHT_STOP)
						{
							_baleog->setBaleogState(BALEOG_RIGHT_DEAD);
							_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "rightDead"));
							_baleog->getBaleogMotion()->start();

						}
						else if (_baleog->getBaleogState() == BALEOG_LEFT_MOVE || _baleog->getBaleogState() == BALEOG_LEFT_STOP)
						{
							_baleog->setBaleogState(BALEOG_LEFT_DEAD);
							_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "leftDead"));
							_baleog->getBaleogMotion()->start();

						}
					}
				}
				else if (_choice == OLAF)
				{
					if (_deadZone[j].rc.top < _rc[_choice].bottom)
					{
						_deadTime++;
						_isDead[OLAF] = true;
						_deadTum = _rc[_choice].bottom - _deadZone[j].rc.top;
						_y[OLAF] = _y[OLAF] - _deadTum;

						if (_olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_MOVE || _olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_STOP || _olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_UP_GRAVITY || _olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_DOWN_GRAVITY || _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_UP_GRAVITY || _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_DOWN_GRAVITY)
						{
							_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_DEAD);
							_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "rightDead"));
							_olaf->getOlafMotion()->start();

						}
						else if (_olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_MOVE || _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_STOP || _olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_UP_GRAVITY || _olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_DOWN_GRAVITY || _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_UP_GRAVITY || _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_DOWN_GRAVITY)
						{
							_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_DEAD);
							_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "leftDead"));
							_olaf->getOlafMotion()->start();

						}
					}
				}
			}
		}
	}
}

void playerManager::pixelCollisionGreen()
{
	//위로 올라서게
	if (!_isJump)
	{
		if (_isLadderCollision[ERIC] == false)
		{
			//에릭
			for (int i = _prove_Y[ERIC] - 10; i < _prove_Y[ERIC] + 10; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), _prove_X[ERIC], i);

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if (r == 0 && g == 255 && b == 0)
				{
					bottomcheck = true;
					if (_isButt == true)
					{
						_y[ERIC] = i - 150;
						_isButt = false;
						break;
					}
					else if (_isButt == false)
					{
						_y[ERIC] = i - 100;
					}
					_jumpNum = 1;
					_eric->setIsJumpMotion(false); //픽셀충돌하면 점프모션 꺼라
					//낙하하고 떨어질때 충돌 삭제
					//if (_isFallDown == true && _eric->getEricState() == ERIC_FALL)
					//{
					//	_eric->setEricState(ERIC_RIGHT_FALL_DOWN);
					//	_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightFallDown"));
					//	_eric->getEricMotion()->start();
					//	_isFallDown = false;
					//}
					break;
				}
				else
					bottomcheck = false;


			}
		}
		if (_isLadderCollision[BALEOG] == false)
		{
			//벨로그
			for (int i = _prove_Y[BALEOG] - 10; i < _prove_Y[BALEOG] + 10; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), _prove_X[BALEOG], i);

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if (r == 0 && g == 255 && b == 0)
				{
					bottomcheck_1 = true;
					_y[BALEOG] = i - 100;

					break;
				}
				else
					bottomcheck_1 = false;
			}
		}
		if (_isLadderCollision[OLAF] == false)
		{
			//올라프
			for (int i = _prove_Y[OLAF] - 10; i < _prove_Y[OLAF] + 10; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), _prove_X[OLAF], i);

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if (r == 0 && g == 255 && b == 0)
				{
					_y[OLAF] = i - 100;

					bottomcheck_2 = true;
					break;
				}
				else bottomcheck_2 = false;
			}
		}
	}
}

void playerManager::pixelCollisionYellow()
{
	//왼쪽벽
	//if (select == 1)
	{
		//에릭
		for (int i = _prove_X[ERIC] - 30; i > _prove_X[ERIC] - 50; --i)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), i, _prove_Y[ERIC] - 50);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 255 && g == 255 && b == 0)
			{
				_wallcheck++;
				_x[ERIC] = i;
				leftcheck = true;

				if (_eric->getEricState() == ERIC_LEFT_DASH)
				{
					_buttAngle = (PI / 180) * 60;
					_eric->setEricState(ERIC_LEFT_BUTT);
					_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftButt"));
					_eric->getEricMotion()->start();
					_isButt = true;
				}
				break;
			}
			else leftcheck = false;		
		}
	}
	//else if (select == 2)
	{
		//벨로그
		for (int i = _prove_X[BALEOG] - 30; i > _prove_X[BALEOG] - 50; --i)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), i, _prove_Y[BALEOG] - 50);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 255 && g == 255 && b == 0)
			{
				leftcheck_1 = true;
				_x[BALEOG] = i;

				break;
			}
			else
				leftcheck_1 = false;
		}
	}
	//else if (select == 3)
	{
		//올라프
		for (int i = _prove_X[OLAF] - 30; i > _prove_X[OLAF] - 50; --i)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), i, _prove_Y[OLAF] - 50);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 255 && g == 255 && b == 0)
			{
				_x[OLAF] = i;

				leftcheck_2 = true;

				break;
			}
			else
				leftcheck_2 = false;
		}
	}
}

void playerManager::pixelCollisionEmerald()
{
	//오른쪽벽
	//if (select == 1)
	{
		//에릭
		for (int i = _prove_X[ERIC] + 30; i < _prove_X[ERIC] + 50; ++i)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), i, _prove_Y[ERIC] - 50);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 0 && g == 255 && b == 255)
			{
				_x[ERIC] = i - 100;

				rightcheck = true;

				if (_eric->getEricState() == ERIC_RIGHT_DASH)
				{
					_buttAngle = (PI / 180) * 150;
					_eric->setEricState(ERIC_RIGHT_BUTT);
					_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightButt"));
					_eric->getEricMotion()->start();
					_isButt = true;
					
				}
				break;
			}
			else rightcheck = false;
		}
	}
	//else if (select == 2)
	{
		//벨로그
		for (int i = _prove_X[BALEOG] + 30; i < _prove_X[BALEOG] + 50; ++i)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), i, _prove_Y[BALEOG] - 50);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 0 && g == 255 && b == 255)
			{
				_x[BALEOG] = i - 100;

				rightcheck_1 = true;

				break;
			}
			else
				rightcheck_1 = false;
		}
	}
	//else if (select == 3)
	{
		//올라프
		for (int i = _prove_X[OLAF] + 30; i < _prove_X[OLAF] + 50; ++i)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), i, _prove_Y[OLAF] - 50);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 0 && g == 255 && b == 255)
			{
				_x[OLAF] = i - 100;

				rightcheck_2 = true;
				break;
			}
			else
				rightcheck_2 = false;
		}
	}
}

void playerManager::pixelCollisionRed()
{
	for (int i = _prove_Y[ERIC] - 95; i > _prove_Y[ERIC] - 100; i--)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("pixel")->getMemDC(), _prove_X[ERIC], i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
	
		if (r == 255 && g == 0 && b == 0)
		{
			_y[ERIC] = i;
			_y[ERIC] += 5.0f;
			_isJump = false;
		}
	}
}

void playerManager::jumpGravity(int select)
{
	if (_isGravity)
	{
		if (_choice == ERIC)
		{
			if (_gravityStop[ERIC] == false)
			{
				_y[ERIC] += 7.f;
				_jumpCount = 0;
			}
		}
		else if (_choice == BALEOG)
		{
			if (_gravityStop[BALEOG] == false)
			{
				_y[BALEOG] += 7.f;
			}
		}
		else if (_choice == OLAF)
		{
			if (_gravityStop[OLAF] == false)
			{
				_y[OLAF] += 7.f;
			}
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		if (_isJump)
		{
			_jumpCount++;
			_y[select] -= _jumpPower;
			_jumpPower -= _gravity;
			_isGravity = false;

			if (_jumpCount > 50)
			{
				_isJump = false;
			}
		}
		else
		{
			_isGravity = true;
		}


		if (select == ERIC)
		{
			if (_jumpNum > 0)
			{
				if (KEYMANAGER->isOnceKeyDown('F') && _camera->getChange() == false)
				{
					_jumpNum--;
					_jumpPower = 6.f;
					_gravity = 0.05f;
					_isJump = true;

					_eric->jumpKeySetting();
				}
			}
		}
	}
}

void playerManager::baleogArrow()
{
	if (_arrow->getVArrow().size() == 0)
	{
		if (KEYMANAGER->isOnceKeyDown('S'))
		{
			_baleog->setIsFire(true);
		}
		else if (KEYMANAGER->isOnceKeyUp('S'))
		{
			_baleog->setIsFire(false);
			_baleog->getBaleogMotion()->resume();
			_isArrowFireStop = false;
		}

		if (KEYMANAGER->isStayKeyDown('S'))
		{
			_isArrowFireStop = true;
			if (_baleog->getBaleogMotion()->getFramePos().x == 750 && _baleog->getBaleogState() == BALEOG_RIGHT_ARROW_ATTACK)
			{
				_baleog->getBaleogMotion()->pause();
			}
			else if (_baleog->getBaleogMotion()->getFramePos().x == 300 && _baleog->getBaleogState() == BALEOG_LEFT_ARROW_ATTACK)
			{
				_baleog->getBaleogMotion()->pause();
			}
		}
	}
	if (_isArrowFireStop == false)
	{
		if (_baleog->getBaleogState() == BALEOG_RIGHT_ARROW_ATTACK)
		{
			if (_baleog->getBaleogMotion()->getFramePos().x == 750)
			{
				SOUNDMANAGER->play("baleog_Arrow");
				_arrow->arrowFire(_x[BALEOG], _y[BALEOG] + 50, 10, PI2);
				_arrow->setArrowState(ARROW_RIGHT_FIRE);
			}
		}
		else if (_baleog->getBaleogState() == BALEOG_LEFT_ARROW_ATTACK)
		{
			if (_baleog->getBaleogMotion()->getFramePos().x == 300)
			{
				SOUNDMANAGER->play("baleog_Arrow");
				_arrow->arrowFire(_x[BALEOG], _y[BALEOG] + 50, 10, PI);
				_arrow->setArrowState(ARROW_LEFT_FIRE);
			}
		}
	}
}

void playerManager::ericwallcheck()
{
	//왼쪽 벽
	if (bottomcheck == true && rightcheck == true)
	{
		_wallcheck++;

		if (_wallcheck < 5 && _eric->getEricState() == ERIC_RIGHT_MOVE)
		{
			_eric->setEricState(ERIC_RIGHT_WALL_PUSH);
			_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightPush"));
			_eric->getEricMotion()->start();
		}
		else if ((KEYMANAGER->isOnceKeyUp(VK_RIGHT)))
		{
			_eric->setEricState(ERIC_RIGHT_STOP);
			_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightStop"));
			_eric->getEricMotion()->start();

		}
	}
	else
		_wallcheck = 0;
	//=============================================//
	//오른벽
	if (bottomcheck == true && leftcheck == true)
	{


		if (_wallcheck < 5 && _eric->getEricState() == ERIC_LEFT_MOVE)
		{
			_eric->setEricState(ERIC_LEFT_WALL_PUSH);
			_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftPush"));
			_eric->getEricMotion()->start();
		}
		else if ((KEYMANAGER->isOnceKeyUp(VK_LEFT)))
		{
			_eric->setEricState(ERIC_LEFT_STOP);
			_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftStop"));
			_eric->getEricMotion()->start();

		}
	}
	else
		_wallcheck = 0;


	//=============================================//
	//오른쪽 미는모션중에 점프할때
	if (_eric->getEricState() == ERIC_RIGHT_WALL_PUSH)
	{
		if (_isJump == true)
		{
			checkjumpcount++;

			if (checkjumpcount < 5)
			{
				_eric->setEricState(ERIC_RIGHT_JUMP);
				_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "rightJump"));
				_eric->getEricMotion()->start();
			}
		}
	}
	else
		checkjumpcount = 0;
	//===============================================//
	//왼쪽 미는모션중에 점프할때
	if (_eric->getEricState() == ERIC_LEFT_WALL_PUSH)
	{
		if (_isJump == true)
		{
			checkjumpcount++;

			if (checkjumpcount < 5)
			{
				_eric->setEricState(ERIC_LEFT_JUMP);
				_eric->setEricMotion(KEYANIMANAGER->findAnimation("ericName", "leftJump"));
				_eric->getEricMotion()->start();
			}
		}
	}
	else
		checkjumpcount = 0;
}

void playerManager::baleogwallcheck()
{
	if (bottomcheck_1 == true && rightcheck_1 == true)
	{
		_wallcheck++;

		if (_wallcheck < 5 && _baleog->getBaleogState() == BALEOG_RIGHT_MOVE)
		{
			_baleog->setBaleogState(BALEOG_RIGHT_WALL_PUSH);
			_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "rightPush"));
			_baleog->getBaleogMotion()->start();
		}
		else if ((KEYMANAGER->isOnceKeyUp(VK_RIGHT)))
		{
			_baleog->setBaleogState(BALEOG_RIGHT_STOP);
			_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "rightStop"));
			_baleog->getBaleogMotion()->start();
		}
	}
	else
		_wallcheck = 0;
	//=============================================//
	//오른벽
	if (bottomcheck_1 == true && leftcheck_1 == true)
	{
		_wallcheck++;

		if (_wallcheck < 5 && _baleog->getBaleogState() == BALEOG_LEFT_MOVE)
		{
			_baleog->setBaleogState(BALEOG_LEFT_WALL_PUSH);
			_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "leftPush"));
			_baleog->getBaleogMotion()->start();
		}
		else if ((KEYMANAGER->isOnceKeyUp(VK_RIGHT)))
		{
			_baleog->setBaleogState(BALEOG_LEFT_STOP);
			_baleog->setBaleogMotion(KEYANIMANAGER->findAnimation("벨로그캐릭터", "leftStop"));
			_baleog->getBaleogMotion()->start();
		}
	}
	else
		_wallcheck = 0;
}

void playerManager::olafwallcheck()
{
	if (bottomcheck_2 == true && rightcheck_2 == true)
	{
		_wallcheck++;

		if (_wallcheck < 5 && _baleog->getBaleogState() == BALEOG_RIGHT_MOVE)
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_WALL_PUSH);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafRightPush"));
			_olaf->getOlafMotion()->start();
		}
		else if ((KEYMANAGER->isOnceKeyUp(VK_RIGHT)))
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_STOP);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafRightStop"));
			_olaf->getOlafMotion()->start();
		}
	}
	else
		_wallcheck = 0;
	//=============================================//
	//오른벽
	if (bottomcheck_2 == true && leftcheck_2 == true)
	{
		_wallcheck++;

		if (_wallcheck < 5 && _baleog->getBaleogState() == BALEOG_LEFT_MOVE)
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_WALL_PUSH);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafLeftPush"));
			_olaf->getOlafMotion()->start();
		}
		else if ((KEYMANAGER->isOnceKeyUp(VK_LEFT)))
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_STOP);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafLeftStop"));
			_olaf->getOlafMotion()->start();
		}
	}
	else
		_wallcheck = 0;
}

void playerManager::olafShieldUpMove()
{
	if (bottomcheck_2 == false && _olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_UP_MOVE)
	{
		_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_SHIELD_UP_GRAVITY);
		_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafShieldUpRightGravity"));
		_olaf->getOlafMotion()->start();
	}
	else if (_olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_UP_GRAVITY)
	{
		if (bottomcheck_2 == true)
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_SHIELD);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafRightShield"));
			_olaf->getOlafMotion()->start();
		}
	}
	if (bottomcheck_2 == false && _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_UP_MOVE)
	{
		_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_SHIELD_UP_GRAVITY);
		_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafShieldUpLeftGravity"));
		_olaf->getOlafMotion()->start();
	}
	else if (_olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_UP_GRAVITY)
	{
		if (bottomcheck_2 == true)
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_SHIELD);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafLeftShield"));
			_olaf->getOlafMotion()->start();
		}
	}

}

void playerManager::olafSHieldDownMove()
{
	if (bottomcheck_2 == false && _olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_MOVE)
	{

		_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_SHIELD_DOWN_GRAVITY);
		_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafShieldDownRightGravity"));
		_olaf->getOlafMotion()->start();

	}
	else if (_olaf->getOlafDirection() == OLAF_DIRECTION_RIGHT_SHIELD_DOWN_GRAVITY)
	{
		if (bottomcheck_2 == true)
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_RIGHT_STOP);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafRightStop"));
			_olaf->getOlafMotion()->start();
		}
	}
	if (bottomcheck_2 == false && _olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_MOVE)
	{

		_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_SHIELD_DOWN_GRAVITY);
		_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafShieldDownLeftGravity"));
		_olaf->getOlafMotion()->start();

	}
	else if (_olaf->getOlafDirection() == OLAF_DIRECTION_LEFT_SHIELD_DOWN_GRAVITY)
	{
		if (bottomcheck_2 == true)
		{
			_olaf->setOlafDirection(OLAF_DIRECTION_LEFT_STOP);
			_olaf->setOlafMotion(KEYANIMANAGER->findAnimation("olafName", "olafLeftStop"));
			_olaf->getOlafMotion()->start();
		}
	}
}
void playerManager::SideIn()
{
	if (_x[ERIC] < 0)
	{
		_x[ERIC] = 0;
	}
	else if (_x[ERIC] > 2900)
	{
		_x[ERIC] = 2900;
	}

	if (_x[OLAF] < 0)
	{
		_x[OLAF] = 0;
	}
	else if (_x[OLAF] > 2900)
	{
		_x[OLAF] = 2900;
	}

	if (_x[BALEOG] < 0)
	{
		_x[BALEOG] = 0;
	}
	else if (_x[BALEOG] > 2900)
	{
		_x[BALEOG] = 2900;
	}

}
