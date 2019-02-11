#pragma once
#include "gameNode.h"
#include <vector>

struct tagItem
{
	image* image;
	float x;
	float y;
	float viewX;
	float viewY;
	RECT rc;
	bool isLive;
	int frameX;
	int frameY;
};

class playerManager;

class item : public gameNode
{
private:
	//아이템 클래스 안에 벡터 생성
	vector<tagItem>				_vItem;
	vector<tagItem>::iterator	_viItem;

	playerManager* _playerManager;

public:
	item();
	~item();

	HRESULT init();
	void release();
	void update();
	void render();

	void createItem(const char* imageName,float x, float y,int frameX,int frameY);
	void removeItem();

	vector<tagItem> getVItem() { return _vItem; }
	vector<tagItem>::iterator getViItem() { return _viItem; }
	//벡터가 아이템 클래스에 있으므로 직접 연결하기위해 주소로 변경할수 있게 하기 위해 어드레스 설정
	vector<tagItem>* getVItemAddress() { return &_vItem; }

	//카메라 좌표를 받아오기 위해 한쪽에서만 연결
	void getPlayerManagerLinkAddress(playerManager* playerManager) { _playerManager = playerManager; }
};

