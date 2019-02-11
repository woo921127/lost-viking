#pragma once
#include "gameNode.h"

enum OLAFDIRECTION
{
	OLAF_DIRECTION_RIGHT_STOP,
	OLAF_DIRECTION_LEFT_STOP,
	OLAF_DIRECTION_RIGHT_SHIELD,
	OLAF_DIRECTION_LEFT_SHIELD,
	OLAF_DIRECTION_RIGHT_MOVE,
	OLAF_DIRECTION_LEFT_MOVE,
	OLAF_DIRECTION_UP_MOVE,
	OLAF_DIRECTION_DOWN_MOVE,
	OLAF_DIRECTION_RIGHT_SHIELD_UP_MOVE,
	OLAF_DIRECTION_LEFT_SHIELD_UP_MOVE,
	OLAF_DIRECTION_RIGHT_SHIELD_UP_GRAVITY,
	OLAF_DIRECTION_RIGHT_SHIELD_DOWN_GRAVITY,
	OLAF_DIRECTION_LEFT_SHIELD_UP_GRAVITY,
	OLAF_DIRECTION_LEFT_SHIELD_DOWN_GRAVITY,
	OLAF_DIRECTION_RIGHT_WALL_PUSH,
	OLAF_DIRECTION_LEFT_WALL_PUSH,
	OLAF_DIRECTION_RIGHT_DEAD,
	OLAF_DIRECTION_LEFT_DEAD,
};



class olaf : public gameNode
{
private:

	OLAFDIRECTION _olafDirection;

	image* _olafimg;
	float _olaf_x, _olaf_y;
	RECT _olaf_rc;
	float _speed;

	bool _changeMode;
	int num;
	animation* _olafMotion;

	RECT _wall;
	float _Wax, _Way;

	bool _isLadderCollision; //사다리 충돌했니
	POINTFLOAT _cameraPos;


public:

	olaf();
	~olaf();

	HRESULT init();
	void release();
	void update(float viewX, float viewY, float* x, float* y);
	void render(float viewX, float viewY);

	OLAFDIRECTION getOlafDirection() { return _olafDirection; }
	void setOlafDirection(OLAFDIRECTION direction) { _olafDirection = direction; }

	animation* getOlafMotion() { return _olafMotion; }
	void setOlafMotion(animation* ani) { _olafMotion = ani; }

	//inline int getolafX() { return _olaf_x; }
	//void setolafX(int olafx) { _olaf_x = olafx; }
	//
	//inline int getolafY() { return _olaf_y; }
	//void setolafY(int olafy) { _olaf_y = olafy; }

	float getplayerX() { return _olaf_x; }
	float getplayerY() { return _olaf_y; }
	float getSpeed() { return _speed; }

	void setLadderCollision(bool collision) { _isLadderCollision = collision; }

	void olafMovement();
};

