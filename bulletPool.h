#pragma once
#include "gameNode.h"
#include <vector>


class Bullet : public gameNode
{
private:
	image* _bulletImage;		//총알이미지
	RECT _rc;					//총알렉트
	float _x, _y;				//총알 좌표
	float _fireX, _fireY;		//총알 발사위치(X,Y)
	float _angle;				//총알 각도
	float _radius;				//총알 반지름
	float _speed;				//총알 스피드
	float _range;				//총알 최대 사거리
	
	bool _isActive;
public:
	Bullet();
	~Bullet();

	void init(image* image, float radius, float range);
	void release();
	void update();
	void render();

	void bulletFire(float x, float y, float angle, float speed);

	void bulletMove();

public:

	//활동중인 총알 여부
	void setActive(bool isActive)	{ _isActive = isActive; }
	bool getActive() const			{ return _isActive; }

	RECT getRC() const				{ return _rc; }
};

//뷸렛 풀~
class BulletPool
{
private:
	float _defaultRange;
	float _defaultRadius;
	const char* _imageName;

	vector<Bullet*> _disableBullets;//안쓰는거
	vector<Bullet*> _enableBullets;//쓰는거
public:
	BulletPool();
	~BulletPool();

	void init(const char* imageName, int createBullet = 16000, float range = 1200.0f, float radius = 10.0f);
	void release();
	void update();
	void render();

	void fire(float x, float y, float angle, float speed);


public:
	//발사된 총알 벡터
	vector<Bullet*> getActiveBullets()const { return _enableBullets; }
};
