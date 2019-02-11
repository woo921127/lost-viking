#include "stdafx.h"
#include "item.h"
#include "playerManager.h"


item::item()
{
}


item::~item()
{
}

HRESULT item::init()
{

	return S_OK;
}

void item::release()
{
}

void item::update()
{
	removeItem();

	//�������� �����ǥ�� �׸��� ���ؼ� ī�޶� �޾ƿ��� �׷��ش�
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		_viItem->viewX = _viItem->x - _playerManager->getCamera()->getCameraX();
		_viItem->viewY = _viItem->y - _playerManager->getCamera()->getCameraY();

		_viItem->rc = RectMake(_viItem->viewX, _viItem->viewY, _viItem->image->getFrameWidth(), _viItem->image->getFrameHeight());
	}

}

void item::render()
{
	//�� ��ǥ�� ���� 
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		_viItem->image->frameRender(getMemDC(), _viItem->viewX, _viItem->viewY, _viItem->frameX, _viItem->frameY);
		//Rectangle(getMemDC(), _viItem->rc);
	}


}

void item::createItem(const char* imageName,float x, float y, int frameX, int frameY)
{
	tagItem item;
	ZeroMemory(&item, sizeof(item));
	item.image = IMAGEMANAGER->findImage(imageName);
	//�츮 �������̹����� �� ������ó�� ������ �����Ƿ� �������� �޴´�.
	item.frameX = frameX;
	item.frameY = frameY;
	item.x = x;
	item.y = y;
	item.viewX = x;
	item.viewY = y;
	item.isLive = true;
	item.rc = RectMake(x, y, item.image->getFrameWidth(), item.image->getFrameHeight());

	_vItem.push_back(item);
}

void item::removeItem()
{
	//�������� isLive �� false�� �Ǹ� �̷������Ѵ�
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); )
	{
		if ((*_viItem).isLive == false)
		{
			_viItem = _vItem.erase(_viItem);
		}
		else
		{
			++_viItem;
		}
	}
}
