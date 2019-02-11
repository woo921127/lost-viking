#pragma once
#include "gameNode.h"
#include "button.h"
#include "action.h"

class selectScene : public gameNode
{
private:
	button* _btnSceneChange;

	button* _btnAngleUp;
	button* _btnAngleDown;

	action* _action;
	image* _fairy;

public:
	selectScene();
	~selectScene();

	HRESULT init();
	void release();
	void update();
	void render();

	//콜백에 사용될 씬변경용 함쑤
	static void cbSceneChange();

	static void cbAngleUp();
	static void cbAngleDown();
};

