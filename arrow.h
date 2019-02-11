#pragma once
#include "gameNode.h"
#include "animation.h"

enum ARROWSTATE
{
	ARROW_LEFT_FIRE,
	ARROW_RIGHT_FIRE,
};

struct tagArrow
{
	image* arrowImage;
	float x, y;
	RECT arrowRc;
	float speed;
	float angle;
	float fireX, fireY;
	float radius;
	float viewX, viewY;
};

class arrow : public gameNode
{
private:
	vector<tagArrow>			_vArrow;
	vector<tagArrow>::iterator  _viArrow;

	ARROWSTATE _arrowState;
	const char* _imageName;
	float _range;
	int _arrowMax;

	animation* _arrowAni;

public:
	arrow();
	~arrow();

	HRESULT init(const char* imageName, int arrowMax, float range);
	void release();
	void update();
	void render(float viewX, float viewY);

	void arrowFire(float x, float y, float speed, float angle);
	void arrowMove();

	ARROWSTATE getArrowState() { return _arrowState; }
	void setArrowState(ARROWSTATE state) { _arrowState = state; }

	vector<tagArrow> getVArrow() { return _vArrow; }
	vector<tagArrow>* getVArrowAddress() { return &_vArrow; }

	float getArrowX()
	{
		for (int i = 0; i < _vArrow.size(); i++)
		{
			return _vArrow[i].x;
		}
	}

	void setArrowX(float x)
	{
		for (int i = 0; i < _vArrow.size(); i++)
		{
			_vArrow[i].x = x;
		}
	}

};

