#include "stdafx.h"
#include "bulletPool.h"

Bullet::Bullet()
	: _bulletImage(NULL), _isActive(false), _radius(10.0f)
{
}

Bullet::~Bullet()
{
}

void Bullet::init(image * image, float radius, float range)
{
	_bulletImage = image;
	_radius = radius;
	_range = range;
	_isActive = false;
}

void Bullet::release()
{
}

void Bullet::update()
{
	bulletMove();
}

void Bullet::render()
{
	_bulletImage->render(getMemDC(), _rc.left, _rc.top);
}

void Bullet::bulletFire(float x, float y, float angle, float speed)
{
	_x = _fireX = x;
	_y = _fireY = y;
	_angle = angle;
	_speed = speed;

	_rc = RectMakeCenter(_x, _y, _bulletImage->GetWidth(), _bulletImage->GetHeight());
	
	_isActive = true;
}

void Bullet::bulletMove()
{
	_x += cosf(_angle) * _speed;
	_y += -sinf(_angle) * _speed;

	_rc = RectMakeCenter(_x, _y,
		_bulletImage->GetWidth(),
		_bulletImage->GetHeight()
	);

	if (_range < getDistance(_x, _y, _fireX, _fireY) ) _isActive = false;
}


//=========================
//## Pool~~~ ##
//=========================


BulletPool::BulletPool()
{
}

BulletPool::~BulletPool()
{
}

void BulletPool::init(const char * imageName, int createBullet, float range, float radius)
{
	_imageName = imageName;
	_defaultRadius = radius;
	_defaultRange = range;

	//벡터 싸이즈 정해줌
	_disableBullets.reserve(createBullet);
	_enableBullets.reserve(createBullet);

	image* tempImage = IMAGEMANAGER->findImage(imageName);

	//오브젝트 풀에 싸이즈 잡아주고
	for (int i = 0; i < createBullet; ++i)
	{
		_disableBullets.push_back(new Bullet);
		_disableBullets[i]->init(tempImage, _defaultRadius, _defaultRange);
	}

}

void BulletPool::release()
{
	for (int i = 0; i < _disableBullets.size(); ++i) SAFE_DELETE(_disableBullets[i]);
	for (int i = 0; i < _enableBullets.size(); ++i)  SAFE_DELETE(_enableBullets[i]);

	_disableBullets.clear();
	_enableBullets.clear();
}

void BulletPool::update()
{
	for (int i = 0; i < _enableBullets.size(); ++i)
	{
		//뿌려진 총알이라면
		if (_enableBullets[i]->getActive())
		{
			//움직여주고
			_enableBullets[i]->update();
		}
		else
		{
			//사용할 필요 없는 총알은 다시 풀로~
			Bullet* tempBullet = _enableBullets[i];

			_enableBullets.erase(_enableBullets.begin() + i--);

			_disableBullets.push_back(tempBullet);
		}
	}
}

void BulletPool::render()
{
	//발사된 애만 그린다
	for (int i = 0; i < _enableBullets.size(); ++i)
	{
		_enableBullets[i]->render();
	}

}

void BulletPool::fire(float x, float y, float angle, float speed)
{
	Bullet* tempBullet = NULL;

	//풀에 담긴 애가 없다면
	if (_disableBullets.size() == 0)
	{
		tempBullet = new Bullet;
		image* tempImage = IMAGEMANAGER->findImage(_imageName);

		tempBullet->init(tempImage, _defaultRadius, _defaultRange);
	}
	else
	{
		tempBullet = _disableBullets.back();
		_disableBullets.pop_back();
	}

	//메모리풀에서 꺼내서 쏨
	tempBullet->bulletFire(x, y, angle, speed);

	_enableBullets.push_back(tempBullet);
}

