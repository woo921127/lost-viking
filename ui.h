#pragma once
#include "gameNode.h"

class ui : public gameNode
{
private:
	int _ericCurrentX, _ericCurrentY;				//에릭 프로필 프레임 X,Y
	int _baleogCurrentX, _baleogCurrentY;			//벨로그 프로필 프레임 X, Y
	int _olafCurrentX, _olafCurrentY;				//올라프 프로필 프레임 X, Y
							
	int _choice;
	int _ericLife, _baleogLife, _olafLife;			//바이킹 Life
	int _ericBox_X, _ericBox_Y;						//에릭 박스 커서 X,Y값
	int	_baleogBox_X, _baleogBox_Y;					//벨로그 박스 커서 X,Y값
	int	_olafBox_X, _olafBox_Y;						//올라프 박스 커서 X,Y값

	int _ericItemFrameX[4], _ericItemFrameY[4];
	int _baleogItemFrameX[4], _baleogItemFrameY[4];
	int _olafItemFrameX[4], _olafItemFrameY[4];
	int _deleteItemFrameX;
	int _saveFrameX, _itemMove, _saveTradeFrameX;
	int _setEricFrameX, _setBaleogFrameX, _setOlafFrameX;

	bool _ericCursor, _baleogCursor, _olafCursor;	//커서 깜빡임
	bool _fKeyMod;
	bool _uiItemBling;
	bool _itemCollision;
	bool _itemUse;
	bool _itemTrade;
	bool _ericDead, _baleogDead, _olafDead;

	int _ericItemNumber, _baleogItemNumber, _olafItemNumber;

	int _cursorOldTime;									//커서 깜빡일때 시간

public:
	ui();
	~ui();

	HRESULT init();
	void release();
	void update(int choice, bool uiChange);
	void render();
	void profileUpdate(int choice);					//프로필 이미지 변경할 때
	void profileDead();

	void keyControl(int choice);
	void uiEricItemSave();
	void uiBaleogItemSave();
	void uiOlafItemSave();
	void uiItemRender();
	void vikingItemNumber(int choice);
	void itemUse(bool redKey, int choice);
	void ericItemTrade(int name);
	//void baleogItemTrade(int name);
	//void olafItemTrade(int name);


	int getEricLife() { return _ericLife; }				// 35씩 뺄때마다 life 하나씩 줄어듬~
	int getBaleogLife() { return _baleogLife; }
	int getOlafLife() { return _olafLife; }
	bool getItemUse() { return _itemUse; }

	int getEricItemNumber() { return _ericItemNumber;  }
	int getBaleogItemNumber() { return _baleogItemNumber; }
	int getOlafItemNumber() { return _olafItemNumber; }

	void setEricFrameX(int currentFrameX) { _setEricFrameX = currentFrameX;}
	void setBaleogFrameX(int currentFrameX) { _setBaleogFrameX = currentFrameX; }
	void setOlafFrameX(int currentFrameX) { _setOlafFrameX = currentFrameX; }
	void setItemCollision(bool itemCOllision) { _itemCollision = itemCOllision; }
	void setEricDead(bool dead) { _ericDead = dead; }
	void setBaleogDead(bool dead) { _baleogDead = dead; }
	void setOlafDead(bool dead) { _olafDead = dead; }
};

