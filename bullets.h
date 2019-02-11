#pragma once
#include "gameNode.h"
#include <vector>

//총알 구조체
struct tagBullet
{
	image* bulletImage;		//총알이미지
	RECT rc;				//총알렉트
	float x, y;				//총알 좌표
	float angle;			//총알 각도
	float radius;			//총알 반지름
	float speed;			//총알 스피드
	float fireX, fireY;		//총알 발사위치(X,Y)
	bool isFire;
	int count;
};

//누구나 사용가능한 총알
class bullet : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char*					_imageName;
	float						_range;
	int							_bulletMax;
public:
	bullet();
	~bullet();

	HRESULT init(const char* imageName, int bulletMax, float range);
	void release();
	void update();
	void render();

	void bulletFire(float x, float y, float angle, float speed);

	void bulletMove();

	void removeBullet(int arrNum);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }
};


//생성해놓고 발사하는 미쏼~
class missile : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
public:
	missile();
	~missile();

	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();

	//총알 발사함수(나갈좌표X, Y)
	void fire(float x, float y);

	void move();
};
//쏠때 생성해서 발사하는 미쏼~
class spear : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	int _missileMax;

public:
	spear();
	~spear();

	virtual HRESULT init(int missileMax, float range);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y);

	void move();

	void removeSpear(int arrNum);

	vector<tagBullet> getVSpear()			{ return _vBullet; }
	vector<tagBullet>::iterator getVISpear() { return _viBullet; }
};