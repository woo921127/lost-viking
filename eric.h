#pragma once
#include "gameNode.h"

enum ERICSTATE
{
	ERIC_RIGHT_STOP,
	ERIC_LEFT_STOP,
	ERIC_RIGHT_MOVE,
	ERIC_LEFT_MOVE,
	ERIC_UP_MOVE,
	ERIC_DOWN_MOVE,
	ERIC_RIGHT_JUMP,
	ERIC_LEFT_JUMP,
	ERIC_RIGHT_DASH,
	ERIC_LEFT_DASH,
	ERIC_RIGHT_WALL_PUSH,
	ERIC_LEFT_WALL_PUSH,
	ERIC_RIGHT_DEAD,
	ERIC_LEFT_DEAD,
	ERIC_RIGHT_BUTT, //������ ��ġ��
	ERIC_LEFT_BUTT, //���� ��ġ��
	ERIC_FALL, //����
	ERIC_RIGHT_FALL_DOWN, //�����ϰ� �������� ��

};
class eric : public gameNode
{
private:
	image* _ericImage;
	RECT _eric_rc;
	float _eric_X, _eric_Y;

	float _acceleration; //�뽬�Ҷ� ���ӵ�
	float _speed;
	animation* _ericMotion; //���� ���
	ERICSTATE _ericState; //��������
	bool _isLadderCollision; //��ٸ� �浹�ߴ�
	bool _isJump;
	float _motionTime; //���Ÿ��
	float _motionWorldTime;

public:
	eric();
	~eric();

	HRESULT init();
	void release();
	void update(float viewX, float viewY, float* x, float* y);
	void render(float viewX, float viewY);

	void keySetting();
	void jumpKeySetting();
	void dashKeySetting();

	static void rightDash(void* obj);
	static void leftDash(void* obj);
	static void rightJump(void* obj);
	static void leftJump(void* obj);
	static void rightButt(void* obj);
	static void leftButt(void* obj);
	static void rightCrash(void* obj);

	ERICSTATE getEricState() {return _ericState;}
	void setEricState(ERICSTATE state) {_ericState = state;}

	animation* getEricMotion() {return _ericMotion;}
	void setEricMotion(animation* motion) {_ericMotion = motion;}

	float getplayerX() { return _eric_X; }
	float getplayerY() { return _eric_Y; }

	float getSpeed() { return _speed; }

	void setIsJumpMotion(bool jump) {_isJump = jump;}

	void setLadderCollision(bool collision) {_isLadderCollision = collision;}
};

