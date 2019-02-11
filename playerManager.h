#pragma once
#include "gameNode.h"
#include "baleog.h"
#include "olaf.h"
#include "eric.h"
#include "camera.h"
#include "arrow.h"

enum Character
{
	ERIC,
	BALEOG,
	OLAF
};

struct object //��ٸ�
{
	RECT rc;
	float x, y;
	float viewX, viewY;
};


class item;

class playerManager : public gameNode
{
private:
	camera* _camera;

	RECT _rc[3];

	bool _isDebug;

	object _ladder[5]; //��ٸ�
	object _deadZone[3];
	baleog* _baleog;
	eric* _eric;
	olaf* _olaf;
	arrow* _arrow;

	float _x[3];
	float _y[3];
	float _prove_X[3];
	float _prove_Y[3];
	float _viewX[3];
	float _viewY[3];

	RECT _camerc;
	float _camerc_x;
	float _camerc_y;
	float _cameview_x;
	float _cameview_y;

	Character _choice;
	float _gravity;
	float _jumpPower;
	bool _isJump;
	int _jumpCount;
	//���� Ƚ��
	int _jumpNum;

	item* _item;

	bool _isArrowFireStop;

	bool _isGravity;

	bool _isCameraMode;
	bool _isLadderCollision[3];
	bool _gravityStop[3];

	int _ladderChoice;
	int _wallcheck;

	int _ericSound, _baleogSound, _olafSound;
	int _soundOldtime;

	int _deadTime;
	float _deadTum;
	bool _isDead[3]; //3�� ĳ���� �׾���
	float _moveTime, _moveWorldTime;

	bool _isButt; //��ġ���ߴ�
	bool _isFallDown; //�����ϴٰ� ��ġ���ߴ�

	float _buttAngle; //��ġ���Ҷ� �з��� ����

	//����üũ
	bool bottomcheck;
	bool rightcheck;
	bool leftcheck;
	//���α�üũ
	bool bottomcheck_1;
	bool rightcheck_1;
	bool leftcheck_1;
	//�ö���üũ
	bool bottomcheck_2;
	bool rightcheck_2;
	bool leftcheck_2;

	int checkjumpcount;
	int ShieldUpcount;
	int shieldDowncount;
public:
	playerManager();
	~playerManager();

	HRESULT init();
	void release();
	void update();
	void render();

	//ĳ���� ����
	void characterChoice();
	void characterMove();
	void characterChange();

	//��ٸ� �浹
	void ladderCollision();

	//������ �浹
	void deadZoneCollision();

	//�ȼ��浹
	void pixelCollisionGreen();
	void pixelCollisionYellow();
	void pixelCollisionEmerald();
	void pixelCollisionRed();

	//�߷�
	void jumpGravity(int select);

	//���α� ȭ��
	void baleogArrow();

	void ericwallcheck();
	void baleogwallcheck();
	void olafwallcheck();

	void olafShieldUpMove();
	void olafSHieldDownMove();
	void SideIn();
	void getCameraAddressLink(camera* camera) { _camera = camera; }
	void getItemLinkAddress(item* item) { _item = item; }

	float getEricX() { return _x[ERIC]; };
	float getEricY() { return _y[ERIC]; }
	float getBalogX() { return _x[BALEOG]; }
	float getBalogY() { return _y[BALEOG]; }
	float getOlafX() { return _x[OLAF]; }
	float getOlafY() { return _y[OLAF]; }

	void setEricX(float x) { _x[ERIC] = x; }
	void setEricY(float y) { _y[ERIC] = y; }
	void setBalogX(float x) { _x[BALEOG] = x; }
	void setBalogY(float y) { _y[BALEOG] = y; }
	void setOlafX(float x) { _x[OLAF] = x; }
	void setOlafY(float y) { _y[OLAF] = y; }

	bool getIsjump() { return _isJump; }

	camera* getCamera() { return _camera; }

	Character getChoice() {return _choice;}
	void setChoice(Character choice) {_choice = choice;}

	RECT getEricRect() {return _rc[ERIC]; }
	RECT getBaleogRect() { return _rc[BALEOG]; }
	RECT getOlafRect() { return _rc[OLAF]; }

	void setGravity(bool gravity) {_isGravity = gravity;}

	eric* getEric() {return _eric;}
	void setJumpNum(int jumpNum) {_jumpNum = jumpNum;}

	bool getEricDead() {return _isDead[ERIC]; }
	bool getBaleogDead() {return _isDead[BALEOG]; }
	bool getOlafDead() {return _isDead[OLAF]; }
};

