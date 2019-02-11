#pragma once
#include "gameNode.h"

class menu : public gameNode
{
private:
	int _menuSelect_X, _menuSelect_Y;		//메뉴 선택 아이콘 위치값
	int _imageAlphaValue;					//오프닝 알파값 변수 

	bool _padeIn, _padeOut;					//페이드 인, 페이드 아웃 효과 
	bool _menuGameStart;					//메뉴 게임 시작 불값
	bool _opening;
public:
	menu();
	~menu();

	HRESULT init();
	void release();
	void update();
	void render();

	bool getMenuQuit() { return _menuGameStart; }
	void padeIn();
	void padeOut();
};

