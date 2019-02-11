#include "stdafx.h"
#include "camera.h"


camera::camera()
{
}


camera::~camera()
{
}

HRESULT camera::init()
{
	_mapimage = IMAGEMANAGER->findImage("backGround");

	_camera.x = 0;
	_camera.y = 0;
	_time = 1.5f;

	_change = false;

	return S_OK;
}

void camera::release()
{
}

void camera::update(float playerX, float playerY)
{
	if (!_change)
	{
		//4모서리에 있을때
		if (playerX < WINSIZEX / 2 && playerY < WINSIZEY / 2)
		{
			_camera.x = 0;
			_camera.y = 0;
		}
		else if (playerX > 3000 - WINSIZEX / 2 && playerY < WINSIZEY / 2)
		{
			_camera.x = 3000 - WINSIZEX;
			_camera.y = 0;
		}
		else if (playerX < WINSIZEX / 2 && playerY > 2240 - WINSIZEY / 2)
		{
			_camera.x = 0;
			_camera.y = 2240 - WINSIZEY;
		}
		else if (playerX > 3000 - WINSIZEX / 2 && playerY > 2240 - WINSIZEY / 2)
		{
			_camera.x = 3000 - WINSIZEX;
			_camera.y = 2240 - WINSIZEY;
		}
		//x좌표예외처리
		else if (playerX < WINSIZEX / 2)
		{
			_camera.x = 0;
			_camera.y = playerY - WINSIZEY / 2;
		}
		else if (playerX > 3000 - WINSIZEX / 2)
		{
			_camera.x = 3000 - WINSIZEX;
			_camera.y = playerY - WINSIZEY / 2;
		}
		//y좌표예외처리
		else if (playerY < WINSIZEY / 2)
		{
			_camera.x = playerX - WINSIZEX / 2;
			_camera.y = 0;
		}
		else if (playerY > 2240 - WINSIZEY / 2)
		{
			_camera.x = playerX - WINSIZEX / 2;
			_camera.y = 2240 - WINSIZEY;
		}
		//기본상태
		else
		{
			_camera.x = playerX - WINSIZEX / 2;
			_camera.y = playerY - WINSIZEY / 2;
		}
	}
	else
	{
		changeMoving();
	}
}

void camera::render()
{
	//char str[100];
	//sprintf_s(str, "카메라 x : %f", _camera.x);
	//TextOut(getMemDC(), 500, 20, str, strlen(str));
	//sprintf_s(str, "카메라 y : %f", _camera.y);
	//TextOut(getMemDC(), 500, 40, str, strlen(str));
}

void camera::cameraChange(float newX, float newY)
{
	_change = true;

	_new = cameraPos(newX, newY);

	_orizin.x = _camera.x;
	_orizin.y = _camera.y;

	_Distance = getDistance(_orizin.x, _orizin.y, _new.x, _new.y);
	_angle = getAngle(_orizin.x, _orizin.y, _new.x, _new.y);
	_worldTime = TIMEMANAGER->getWorldTime();
	_time = CAMERA_CHANGING_SPEED;
}

void camera::changeMoving()
{
	if (_change == false) return;

	float elapsedTime = TIMEMANAGER->getElpasedTime();

	if (_Distance == 0)
	{
		_time = 0.001f;
	}
	float moveSpeed = (elapsedTime / _time) * _Distance;

	if (moveSpeed != 0)
	{
		_camera.x += cosf(_angle) * moveSpeed;
		_camera.y += -sinf(_angle) * moveSpeed;
	}

	if (_time + _worldTime <= TIMEMANAGER->getWorldTime())
	{
		_worldTime = TIMEMANAGER->getWorldTime();

		_camera.x = _new.x;
		_camera.y = _new.y;

		_change = false;
	}
}

POINTFLOAT camera::cameraPos(float x, float y)
{
	//4모서리에 있을때
	if (x < WINSIZEX / 2 && y < WINSIZEY / 2)
	{
		_point.x = 0;
		_point.y = 0;

		return _point;
	}
	else if (x > 3000 - WINSIZEX / 2 && y < WINSIZEY / 2)
	{
		_point.x = 3000 - WINSIZEX;
		_point.y = 0;

		return _point;
	}
	else if (x < WINSIZEX / 2 && y > 2240 - WINSIZEY / 2)
	{
		_point.x = 0;
		_point.y = 2240 - WINSIZEY;

		return _point;
	}
	else if (x > 3000 - WINSIZEX / 2 && y > 2240 - WINSIZEY / 2)
	{
		_point.x = 3000 - WINSIZEX;
		_point.y = 2240 - WINSIZEY;

		return _point;
	}
	//x좌표예외처리
	else if (x < WINSIZEX / 2)
	{
		_point.x = 0;
		_point.y = y - WINSIZEY / 2;

		return _point;
	}
	else if (x > 3000 - WINSIZEX / 2)
	{
		_point.x = 3000 - WINSIZEX;
		_point.y = y - WINSIZEY / 2;

		return _point;
	}
	//y좌표예외처리
	else if (y < WINSIZEY / 2)
	{
		_point.x = x - WINSIZEX / 2;
		_point.y = 0;

		return _point;
	}
	else if (y > 2240 - WINSIZEY / 2)
	{
		_point.x = x - WINSIZEX / 2;
		_point.y = 2240 - WINSIZEY;

		return _point;
	}
	//기본상태
	else
	{
		_point.x = x - WINSIZEX / 2;
		_point.y = y - WINSIZEY / 2;

		return _point;
	}
}


