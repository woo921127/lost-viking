#pragma once
#include "singletonBase.h"
#include <map> //�ڵ����� ���ִ°�
#include <unordered_map> //�׳� �����ϴ°�

#include "inc/fmod.hpp"	//fmod.hpp���� ��Ŭ���
#pragma comment(lib, "lib/fmodex64_vc.lib")	//���̺귯�� ���� ���

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
	void stop(string keyName); //����
	void pause(string keyName); //�Ͻ�����
	void resume(string keyName); //�ٽ����

	float getVolume(string keyName); //�Ҹ� ��������
	void setVolume(string keyName, float volume); //�Ҹ�����

	unsigned int getPosition(string keyName); //���� ����� ���� ��ġ
	void setPosition(string keyName, int ms); //���� ����� ��ġ ����
	unsigned int getLength(string keyName); //���� ���� ��ü ����

	//���� �̺�Ʈ ó����
	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);


};

