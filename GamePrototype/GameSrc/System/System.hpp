#pragma once
#include <DxLib.h>
#include <memory>
#include <assert.h>
#include "../Utility/FPS.hpp"
#include "../Utility/Utility.hpp"
//Dxlib�̏������ƃV�X�e���̃`�F�b�N���s���܂�
class System
{
private:
	void SystemInit()
	{
		//�E�B���h�E���m���A�N�e�B�u���͎��s���Ȃ�
		SetAlwaysRunFlag(false);
		//���O����
		SetOutApplicationLogValidFlag(false);
		//�c����ێ�
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
		//�E�C���h�E�^�C�g����ύX
		SetMainWindowText("Game");
		//��ʃT�C�Y�ύX
		SetGraphMode(SCREEN_WIDIH, SCREEN_HEIGHT, 32);
		//�E�B���h�E���[�h�ύX
		ChangeWindowMode(WinMessageBox::IsFullScreen());
		//������
		DxLib_Init();
		assert(DxLib_IsInit());
		//����ʐݒ�
		SetDrawScreen(DX_SCREEN_BACK);
	}
	bool Processloop()
	{
		if (ScreenFlip() != 0) return false;
		if (ProcessMessage() != 0) return false;
		if (ClearDrawScreen() != 0) return false;
		return true;
	}
public:
	static FPS& GetFPS()
	{
		static std::unique_ptr<FPS> instance = std::make_unique<FPS>();
		return *instance;
	}
	static constexpr int
		SCREEN_WIDIH = 1280,
		SCREEN_HEIGHT = 720;
	System()
	{
		SystemInit();
	}

	~System()
	{
		DxLib_End();
	}

	bool IsOk()
	{
		GetFPS().UpDate();
		return Processloop();
	}
};