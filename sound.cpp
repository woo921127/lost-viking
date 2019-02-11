#include "stdafx.h"
#include "sound.h"


sound::sound()
{
}


sound::~sound()
{
}

HRESULT sound::init()
{
	SOUNDMANAGER->addSound("baleog", "sound/baleog.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_2", "sound/baleog_2.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_3", "sound/baleog_3.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_4", "sound/baleog_4.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_5", "sound/baleog_5.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_Arrow", "sound/baleog_Arrow.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_ArrowPull", "sound/baleog_ArrowPull.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_Attack", "sound/baleog_Attack.mp3", false, false);
	SOUNDMANAGER->addSound("baleog_Attack2", "sound/baleog_Attack2.mp3", false, false);
	SOUNDMANAGER->addSound("eric", "sound/eric.mp3", false, false);
	SOUNDMANAGER->addSound("eric_2", "sound/eric_2.mp3", false, false);
	SOUNDMANAGER->addSound("eric_3", "sound/eric_3.mp3", false, false);
	SOUNDMANAGER->addSound("eric_4", "sound/eric_4.mp3", false, false);
	SOUNDMANAGER->addSound("eric_5", "sound/eric_5.mp3", false, false);
	SOUNDMANAGER->addSound("eric_Jump", "sound/eric_Jump.mp3", false, false);
	SOUNDMANAGER->addSound("menu_SeletSound", "sound/menu_SeletSound.mp3", false, false);
	SOUNDMANAGER->addSound("olaf", "sound/olaf.mp3", false, false);
	SOUNDMANAGER->addSound("olaf_2", "sound/olaf_2.mp3", false, false);
	SOUNDMANAGER->addSound("olaf_3", "sound/olaf_3.mp3", false, false);
	SOUNDMANAGER->addSound("olaf_4", "sound/olaf_4.mp3", false, false);
	SOUNDMANAGER->addSound("olaf_5", "sound/olaf_5.mp3", false, false);
	SOUNDMANAGER->addSound("opening_bgm", "sound/opening_bgm.mp3", true, true);
	SOUNDMANAGER->addSound("stage1_bgm", "sound/stage1_bgm.mp3", true, true);
	SOUNDMANAGER->addSound("UI_BaleogPic", "sound/UI_BaleogPic.mp3", false, false);
	SOUNDMANAGER->addSound("UI_EricPic", "sound/UI_EricPic.mp3", false, false);
	SOUNDMANAGER->addSound("UI_OlafPic", "sound/UI_OlafPic.mp3", false, false);
	SOUNDMANAGER->addSound("viking_GameStart", "sound/viking_GameStart.mp3", false, false);
	SOUNDMANAGER->addSound("viking_Movement", "sound/viking_Movement.mp3", false, false);
	SOUNDMANAGER->addSound("eric_Walk", "sound/eric_Walk.mp3", false, false);
	SOUNDMANAGER->addSound("ending", "sound/ending.mp3", true, true);
	
	
	return S_OK;
}

void sound::release()
{
}

void sound::update()
{
}

void sound::render()
{
}
