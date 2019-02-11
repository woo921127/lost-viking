#pragma once
#include "singletonBase.h"
#include <map> //자동정렬 해주는것
#include <unordered_map> //그냥 정렬하는것

#include "inc/fmod.hpp"	//fmod.hpp파일 인클루드
#pragma comment(lib, "lib/fmodex64_vc.lib")	//라이브러리 파일 등록

using namespace FMOD;

#define SOUNDBUFFER 30
#define EXTRACHANNELBUFFER 5

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager : public singletonBase<soundManager>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundsIter;
	typedef map<string, Channel**> arrChannels;
	typedef map<string, Channel**>::iterator arrChannelsIter;

private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;

public:
	soundManager();
	~soundManager();

	HRESULT init();
	void release();
	void update();

	void addSound(string keyName, string soundName, bool bgm, bool loop);
	void play(string keyName, float volume = 1.0f); // 0.0 ~ 1.0f -> 0 ~ 255
	void stop(string keyName); //정지
	void pause(string keyName); //일시정지
	void resume(string keyName); //다시재생

	float getVolume(string keyName); //소리 가져오기
	void setVolume(string keyName, float volume); //소리조절

	unsigned int getPosition(string keyName); //사운드 재생중 현재 위치
	void setPosition(string keyName, int ms); //사운드 재생중 위치 변경
	unsigned int getLength(string keyName); //사운드 파일 전체 길이

	//사운드 이벤트 처리용
	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);


};

