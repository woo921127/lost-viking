#include "stdafx.h"
#include "baleog.h"


baleog::baleog()
{
}


baleog::~baleog()
{
}

HRESULT baleog::init()
{
	_baleogPlayer.baleogImage = IMAGEMANAGER->addFrameImage("���α�", "image/Baleog.bmp", 0, 0, 1200, 2250, 8, 15, true, RGB(255, 0, 255));
	_baleogPlayer.x = BALEOG_X;
	_baleogPlayer.y = BALEOG_Y;
	_baleogPlayer.speed = BALEOG_SPEED;
	_baleogPlayer.baleogRc = RectMakeCenter(_baleogPlayer.x, _baleogPlayer.y, _baleogPlayer.baleogImage->getFrameWidth(), _baleogPlayer.baleogImage->getFrameHeight());

	_baleogState = BALEOG_RIGHT_STOP; //ó���� ������ �⺻���·�

	int rightStop[] = { 0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "rightStop", "���α�", rightStop, 2, 4, true);

	int leftStop[] = { 2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "leftStop", "���α�", leftStop, 2, 4, true);

	int rightMove[] = { 8, 9, 10, 11, 12, 13, 14, 15 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "rightMove", "���α�", rightMove, 8, 10, true);

	int leftMove[] = { 23, 22, 21, 20, 19, 18, 17, 16 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "leftMove", "���α�", leftMove, 8, 10, true);

	int rightArrowAttack[] = { 24, 25, 26, 27, 28, 29, 30, 31 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "rightArrowAttack", "���α�", rightArrowAttack, 8, 10, false, rightFire, this);

	int leftArrowAttack[] = { 39, 38, 37, 36, 35, 34, 33, 32 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "leftArrowAttack", "���α�", leftArrowAttack, 8, 10, false, leftFire, this);

	int rightSwordAttackOne[] = { 40, 41, 42, 43 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "rightSwordAttackOne", "���α�", rightSwordAttackOne, 4, 10, false, rightFire, this);

	int rightSwordAttackTwo[] = { 44, 45, 46, 47 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "rightSwordAttackTwo", "���α�", rightSwordAttackTwo, 4, 10, false, rightFire, this);

	int leftSwordAttackOne[] = { 55, 54, 53, 52 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "leftSwordAttackOne", "���α�", leftSwordAttackOne, 4, 10, false, leftFire, this);

	int leftSwordAttackTwo[] = { 51, 50, 49, 48 };
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "leftSwordAttackTwo", "���α�", leftSwordAttackTwo, 4, 10, false, leftFire, this);

	int upMove[] = {56, 57, 58, 59};
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "upMove", "���α�", upMove, 4, 10, true);

	int downMove[] = {59, 58, 57, 56};
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "downMove", "���α�", downMove, 4, 10, true);

	int rightPush[] = {64, 65, 66, 67};
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "rightPush", "���α�", rightPush, 4, 8, true);

	int leftPush[] = {71, 70, 69, 68};
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "leftPush", "���α�", leftPush, 4, 8, true);

	int rightDead[] = {104, 105, 106, 107, 108, 109, 110, 111};
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "rightDead", "���α�", rightDead, 8, 6, false);

	int leftDead[] = {118, 117, 116, 115, 114, 113, 112, 111};
	KEYANIMANAGER->addArrayFrameAnimation("���α�ĳ����", "leftDead", "���α�", leftDead, 8, 6, false);

	_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "rightStop");

	IMAGEMANAGER->addFrameImage("ȭ��", "image/bullet.bmp", 85, 20, 5, 2, true, RGB(255, 0, 255));

	_rndAttack = RND->getInt(2);

	_arrowFireStop = false;
	_isLadderColision = false;
	_isFire = false;

	return S_OK;
}

void baleog::release()
{

}

void baleog::update(float viewX, float viewY, float* x, float* y)
{
	_baleogPlayer.x = viewX;
	_baleogPlayer.y = viewY;

	//Ű����
	keySetting();

	//ȭ�����
	arrowAttack();

	//�˰���
	swordAttack();


	//�˰����̳� ȭ������ϰ� ������Ű�� ����Ű�� �ڴ����� ������·� �ϱ�����.
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && (_baleogState == BALEOG_RIGHT_STOP || _baleogState == BALEOG_LEFT_STOP))
	{
		_baleogState = BALEOG_RIGHT_MOVE;
		_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "rightMove");
		_baleogMotion->start();
	}
	else if (KEYMANAGER->isStayKeyDown(VK_LEFT) && (_baleogState == BALEOG_LEFT_STOP || _baleogState == BALEOG_RIGHT_STOP))
	{
		_baleogState = BALEOG_LEFT_MOVE;
		_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "leftMove");
		_baleogMotion->start();
	}

	_baleogPlayer.baleogRc = RectMakeCenter(_baleogPlayer.x, _baleogPlayer.y, _baleogPlayer.baleogImage->getFrameWidth(), _baleogPlayer.baleogImage->getFrameHeight());

	KEYANIMANAGER->update();

}

void baleog::render(float viewX, float viewY)
{
	//char str[128];
	//sprintf_s(str, "���α� ���� : %d", _baleogState);
	//TextOut(getMemDC(), 100, 100, str, strlen(str));

	//sprintf_s(str, "���α� ���ǵ� : %d", _baleogPlayer.speed);
	//TextOut(getMemDC(), 100, 120, str, strlen(str));

	//sprintf_s(str, "�������̹��� x��ǥ : %d", _baleogMotion->getFramePos().x);
	//TextOut(getMemDC(), 100, 140, str, strlen(str));

	_baleogPlayer.baleogImage->aniRender(getMemDC(), viewX + 10, viewY, _baleogMotion);
}

void baleog::keySetting()
{
	if (_baleogState != BALEOG_RIGHT_DEAD && _baleogState != BALEOG_LEFT_DEAD)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && _baleogState != BALEOG_LEFT_ARROW_ATTACK && _baleogState != BALEOG_RIGHT_ARROW_ATTACK && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_ONE && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_TWO && _baleogState != BALEOG_LEFT_SWORD_ATTACK_ONE && _baleogState != BALEOG_LEFT_SWORD_ATTACK_TWO)
		{
			_baleogState = BALEOG_RIGHT_MOVE; //������ ������ ���·� ��ȯ
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "rightMove"); //��� �ٲ��ְ�
			_baleogMotion->start(); //��� ����
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _baleogState != BALEOG_LEFT_MOVE && _baleogState != BALEOG_LEFT_ARROW_ATTACK && _baleogState != BALEOG_RIGHT_ARROW_ATTACK && _baleogState != BALEOG_LEFT_SWORD_ATTACK_ONE && _baleogState != BALEOG_LEFT_SWORD_ATTACK_TWO && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_ONE && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_TWO && _baleogState != BALEOG_UP_MOVE && _baleogState != BALEOG_DOWN_MOVE)
		{
			_baleogState = BALEOG_RIGHT_STOP;
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "rightStop");
			_baleogMotion->start();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_LEFT) && _baleogState != BALEOG_LEFT_ARROW_ATTACK && _baleogState != BALEOG_RIGHT_ARROW_ATTACK && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_ONE && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_TWO && _baleogState != BALEOG_LEFT_SWORD_ATTACK_ONE && _baleogState != BALEOG_LEFT_SWORD_ATTACK_TWO)
		{
			_baleogState = BALEOG_LEFT_MOVE;
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "leftMove");
			_baleogMotion->start();
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && _baleogState != BALEOG_RIGHT_MOVE && _baleogState != BALEOG_LEFT_ARROW_ATTACK && _baleogState != BALEOG_RIGHT_ARROW_ATTACK && _baleogState != BALEOG_LEFT_SWORD_ATTACK_ONE && _baleogState != BALEOG_LEFT_SWORD_ATTACK_TWO && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_ONE && _baleogState != BALEOG_RIGHT_SWORD_ATTACK_TWO && _baleogState != BALEOG_UP_MOVE && _baleogState != BALEOG_DOWN_MOVE)
		{
			_baleogState = BALEOG_LEFT_STOP;
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "leftStop");
			_baleogMotion->start();
		}
	}

	//��ٸ� �浹������츸 ������ ������ �ϱ�����.
	if (_isLadderColision == true)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_baleogState = BALEOG_UP_MOVE;
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "upMove");
			_baleogMotion->start();
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_UP) && _baleogState != BALEOG_RIGHT_MOVE && _baleogState != BALEOG_LEFT_MOVE)
		{
			_baleogMotion->pause();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_baleogState = BALEOG_DOWN_MOVE;
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "downMove");
			_baleogMotion->start();
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_DOWN) && _baleogState != BALEOG_RIGHT_MOVE && _baleogState != BALEOG_LEFT_MOVE)
		{
			_baleogMotion->pause();
		}
	}
}

void baleog::arrowAttack()
{
	//ȭ�����
	if (_isFire == true && (_baleogState == BALEOG_LEFT_STOP || _baleogState == BALEOG_LEFT_MOVE || _baleogState == BALEOG_RIGHT_STOP || _baleogState == BALEOG_RIGHT_MOVE))
	{
		SOUNDMANAGER->play("baleog_ArrowPull");
		if (_baleogState == BALEOG_RIGHT_STOP || _baleogState == BALEOG_RIGHT_MOVE)
		{
			_baleogState = BALEOG_RIGHT_ARROW_ATTACK;
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "rightArrowAttack");
			_baleogMotion->start();
		}
		else if (_baleogState == BALEOG_LEFT_STOP || _baleogState == BALEOG_LEFT_MOVE)
		{
			_baleogState = BALEOG_LEFT_ARROW_ATTACK;
			_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "leftArrowAttack");
			_baleogMotion->start();
		}
	}
}

void baleog::swordAttack()
{
	//�˰���
	if(KEYMANAGER->isOnceKeyDown('D'))
	{
		 //�������ݰ� �ޱ�����
		if (_baleogState == BALEOG_RIGHT_STOP || _baleogState == BALEOG_RIGHT_MOVE)
		{
			_rndAttack = RND->getInt(2);
			if (_rndAttack == 0)
			{
				_baleogState = BALEOG_RIGHT_SWORD_ATTACK_ONE;
				_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "rightSwordAttackOne");
				_baleogMotion->start();
				SOUNDMANAGER->play("baleog_Attack");
			}
			else if (_rndAttack == 1)
			{
				_baleogState = BALEOG_RIGHT_SWORD_ATTACK_TWO;
				_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "rightSwordAttackTwo");
				_baleogMotion->start();
				SOUNDMANAGER->play("baleog_Attack2");
			}
		}
		else if (_baleogState == BALEOG_LEFT_STOP || _baleogState == BALEOG_LEFT_MOVE)
		{
			if (_rndAttack == 0)
			{
				_baleogState = BALEOG_LEFT_SWORD_ATTACK_ONE;
				_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "leftSwordAttackOne");
				_baleogMotion->start();
				SOUNDMANAGER->play("baleog_Attack");
			}
			else if (_rndAttack == 1)
			{
				_baleogState = BALEOG_LEFT_SWORD_ATTACK_TWO;
				_baleogMotion = KEYANIMANAGER->findAnimation("���α�ĳ����", "leftSwordAttackTwo");
				_baleogMotion->start();
				SOUNDMANAGER->play("baleog_Attack2");
			}
		}
	}
}

void baleog::rightFire(void* obj)
{
	baleog* baleogPlayer = (baleog*)obj;
	baleogPlayer->setBaleogState(BALEOG_RIGHT_STOP);
	baleogPlayer->setBaleogMotion(KEYANIMANAGER->findAnimation("���α�ĳ����", "rightStop"));
	baleogPlayer->getBaleogMotion()->start();
}

void baleog::leftFire(void * obj)
{
	baleog* baleogPlayer = (baleog*)obj;
	baleogPlayer->setBaleogState(BALEOG_LEFT_STOP);
	baleogPlayer->setBaleogMotion(KEYANIMANAGER->findAnimation("���α�ĳ����", "leftStop"));
	baleogPlayer->getBaleogMotion()->start();
}


