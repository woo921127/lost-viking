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

	//아이템을 상대좌표로 그리기 위해서 카메라를 받아온후 그려준다
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		_viItem->viewX = _viItem->x - _playerManager->getCamera()->getCameraX();
		_viItem->viewY = _viItem->y - _playerManager->getCamera()->getCameraY();

		_viItem->rc = RectMake(_viItem->viewX, _viItem->viewY, _viItem->image->getFrameWidth(), _viItem->image->getFrameHeight());
	}

}

void item::render()
{
	//뷰 좌표로 랜더 
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
	//우리 아이템이미지는 다 프레임처럼 합쳐져 있으므로 프레임을 받는다.
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
	//아이템의 isLive 가 false가 되면 이레이즈한다
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
