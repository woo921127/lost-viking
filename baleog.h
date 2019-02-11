#pragma once
#include "gameNode.h"

struct baleogPlayer
{
	float x, y;
	image* baleogImage;
	image* arrowImage;
	RECT baleogRc;
	float speed;
};


enum BALEOGSTATE //���α� ����
{
	BALEOG_RIGHT_STOP, //������ �⺻����
	BALEOG_LEFT_STOP, //���� �⺻����
	BALEOG_RIGHT_MOVE, //������ ������
	BALEOG_LEFT_MOVE, //���� ������
	BALEOG_UP_MOVE, //��ٸ� Ż��� ���� ������
	BALEOG_DOWN_MOVE, //��ٸ� Ż��� �Ʒ��� ������
	BALEOG_RIGHT_SWORD_ATTACK_ONE, //������ ù��° ���� �˰���
	BALEOG_LEFT_SWORD_ATTACK_ONE, //���� ù��° ���� �˰���
	BALEOG_RIGHT_SWORD_ATTACK_TWO, //������ �ι�° ���� �˰���
	BALEOG_LEFT_SWORD_ATTACK_TWO, //���� �ι�° ���� �˰���
	BALEOG_RIGHT_ARROW_ATTACK, //������ ȭ�����
	BALEOG_LEFT_ARROW_ATTACK, //���� ȭ�����
	BALEOG_RIGHT_WALL_PUSH,
	BALEOG_LEFT_WALL_PUSH,
	BALEOG_RIGHT_DEAD,
	BALEOG_LEFT_DEAD,
};

enum BALEOGINFO // ���α� ������
{
	BALEOG_X = WINSIZEX / 2,
	BALEOG_Y = WINSIZEY / 2,
	BALEOG_SPEED = 0,
};

class baleog : public gameNode
{
private:
	BALEOGSTATE _baleogState;
	baleogPlayer _baleogPlayer;
	
	animation* _baleogMotion; //���α� ��� ����� �� �ִϸ��̼�

	int _rndAttack;

	bool _isLadderColision; //��ٸ� �浹�ߴ�
	bool _arrowFireStop;
	bool _isFire;

public:
	baleog();
	~baleog();

	HRESULT init();
	void release();
	void update(float viewX, float viewY, float* x, float* y);
	void render(float viewX, float viewY);

	void keySetting(); //Ű����
	void arrowAttack(); //ȭ�����
	void swordAttack(); //�˰���

	//�ݹ��Լ� �ҷ��� �Լ�
	static void rightFire(void* obj);
	static void leftFire(void* obj);

public:
	//���α� ���� ������ ������
	BALEOGSTATE getBaleogState() { return _baleogState; }
	void setBaleogState(BALEOGSTATE state) { _baleogState = state; }

	//���α� �ִϸ��̼� ������ ������
	animation* getBaleogMotion() {return _baleogMotion;}
	void setBaleogMotion(animation* motion) {_baleogMotion = motion;}

	//��ٸ� �浹�ߴ��� ������ ������
	bool getLadderCollision() {return _isLadderColision;}
	void setLadderCollision(bool collision) {_isLadderColision = collision;}

	float getBaleogX() {return _baleogPlayer.x;}
	float getBaleogY() {return _baleogPlayer.y;}

	void setBaleogX(float x) {_baleogPlayer.x = x;}
	void setBaleogY(float y) {_baleogPlayer.y = y;}

	float getSpeed() { return _baleogPlayer.speed; }

	void setIsFire(bool fire) { _isFire = fire; }
};

