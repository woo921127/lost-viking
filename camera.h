#pragma once
#include "gameNode.h"

#define CAMERA_CHANGING_SPEED 1.0f

class camera : public gameNode
{
private:

	image* _mapimage;

	POINTFLOAT _camera;

	bool _change;

	float _Distance;
	float _angle;
	float _speed;
	float _time;
	float _worldTime;

	POINTFLOAT _orizin;
	POINTFLOAT _new;
	POINTFLOAT _point;

public:
	camera();
	~camera();

	HRESULT init();
	void release();
	void update(float playerX, float playerY);
	void render();

	//��������1�� �ű�
	void cameraChange(float newX, float newY);
	void changeMoving();
	POINTFLOAT cameraPos(float x, float y);

	POINTFLOAT getCameraPos() { return _camera; }

	float getCameraX() { return _camera.x; }
	float getCameraY() { return _camera.y; }
	void setCameraX(float x) { _camera.x = x; }
	void setCameraY(float y) { _camera.y = y; }

	//ī�޶� ��ȯ
	bool getChange() { return _change; }
	void setChange(bool v) { _change = v; }
	//�÷��̾�� �Ÿ� 
	void setDistance(float v) { _Distance = v; }
	void setAngle(float v) { _Distance = v; }

};

