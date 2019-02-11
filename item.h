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
	//������ Ŭ���� �ȿ� ���� ����
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
	//���Ͱ� ������ Ŭ������ �����Ƿ� ���� �����ϱ����� �ּҷ� �����Ҽ� �ְ� �ϱ� ���� ��巹�� ����
	vector<tagItem>* getVItemAddress() { return &_vItem; }

	//ī�޶� ��ǥ�� �޾ƿ��� ���� ���ʿ����� ����
	void getPlayerManagerLinkAddress(playerManager* playerManager) { _playerManager = playerManager; }
};

