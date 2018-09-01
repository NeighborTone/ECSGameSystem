/**
* @file ResourceManager.hpp
* @brief DX���C�u������LoadGraph��LoadSoundMem����擾�����n���h�����Ǘ�����N���X�ł�
* @author tonarinohito
* @date 2018/8/29
*/
#pragma once
#include <DxLib.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <assert.h>
#include "../Utility/Utility.hpp"

class ResourceManager
{
private:
	//!Graphic�n���h�����Ǘ����܂�
	class GraphicManager
	{
	private:
		std::unordered_map<std::string, int> graphs;
		std::unordered_map<std::string, std::pair<int*,size_t>> divGraphs;
	public:
		~GraphicManager()
		{
			for (auto& it : divGraphs)
			{
				Memory::SafeDeleteArray(it.second.first);
			}
			InitGraph();
		}
		/**
		* @brief �摜�����[�h���܂�
		* @param (path) �t�@�C���p�X
		* @param (name) �o�^��
		* @return ����������ture
		* @detail ���ɓo�^�������O�͎g���܂���
		*/
		bool Load(const std::string& path, const std::string& name)
		{
			//���O�̏d���h�~
			if (graphs.count(name))
			{
				WinMessageBox("�O���t�B�b�N�n���h��" + name + "�͂����o�^����Ă��܂�", "Error");
				assert(false);
			}
			graphs[name] = LoadGraph(path.c_str());
			if (graphs[name] == -1) 
			{
				return false; 
			}
			return true;
		}
		/**
		* @brief �����摜�����[�h���܂�
		* @param (path) �t�@�C���p�X
		* @param (name) �o�^��
		* @param (AllNum) �摜�̕�������
		* @param (XNum) �������̐�
		* @param (YNum) �c�����̐�
		* @param (XSize) ���������摜�ꖇ���̉��̃T�C�Y
		* @param (YSize) ���������摜�ꖇ���̏c�̃T�C�Y
		* @return ����������ture
		* @detail ���ɓo�^�������O�͎g���܂���
		*/
		bool LoadDiv(const std::string& path, const std::string& name,
			const int AllNum,
			const int XNum, const int YNum,
			const int XSize, const int YSize)
		{
			//���O�̏d���h�~
			if (divGraphs.count(name))
			{
				WinMessageBox("�O���t�B�b�N�n���h��" + name + "�͂����o�^����Ă��܂�", "Error");
				assert(false);
			}
			divGraphs[name].first = new int[AllNum];
			divGraphs[name].second = (size_t)AllNum;
			int isOk = LoadDivGraph(path.c_str(),AllNum,XNum,YNum,XSize,YSize, divGraphs[name].first);
			if (isOk == -1)
			{
				return false;
			}
			return true;
		}
		/**
		* @brief �������ɓǂݍ��񂾉摜�̃n���h����Ԃ��܂�
		* @param (name) �o�^��
		* @return ����������n���h�����Ԃ�܂�
		* @detail ���݂��Ȃ����O�ɃA�N�Z�X����ƃG���[�ɂȂ�܂�
		*/
		int GetHandle(const std::string& name)
		{
			if (graphs.find(name) == graphs.end())
			{
				WinMessageBox("�o�^��" + name + "�̃O���t�B�b�N�n���h���͑��݂��܂���", "Error");
				assert(false);
			}
			return graphs[name];
		}
		/**
		* @brief �������ɓǂݍ��񂾕����摜�̃n���h����Ԃ��܂�
		* @param (name) �o�^��
		* @param (index) �z��̗v�f��
		* @return ����������n���h�����Ԃ�܂�
		* @detail ���݂��Ȃ����O�ɃA�N�Z�X���邩�������𒴂����l���w�肷��ƃG���[�ɂȂ�܂�
		*/
		int GetDivHandle(const std::string& name,const int index)
		{
			if (divGraphs.find(name) == divGraphs.end())
			{
				WinMessageBox("�o�^��" + name + "�̃O���t�B�b�N�n���h���͑��݂��܂���", "Error");
				assert(false);
			}
			if ((size_t)index >= divGraphs[name].second)
			{
				WinMessageBox("�o�^��" + name + "�̕������𒴂����l�ł�", "Error");
				assert(false);
			}
			return divGraphs[name].first[index];
		}
		/**
		* @brief �������ɓǂݍ��񂾉摜�̃n���h�������݂��邩�Ԃ��܂�
		* @param (name) �o�^��
		* @return �n���h�������݂�����true
		*/
		bool IsExistenceHandle(const std::string& name)
		{
			if (graphs.count(name))
			{
				return true;
			}
			return false;
		}
		/**
		* @brief �������ɓǂݍ��񂾕����摜�̃n���h�������݂��邩�Ԃ��܂�
		* @param (name) �o�^��
		* @return �n���h�������݂�����true
		*/
		bool IsExistenceDivHandle(const std::string& name)
		{
			if (divGraphs.count(name))
			{
				return true;
			}
			return false;
		}
		/**
		* @brief �������ɓǂݍ��񂾉摜���\�[�X��������܂�
		* @param (name) �o�^��
		* @return �o�^�������݂��Ȃ��ꍇ�����N���܂���
		*/
		void ReMoveDivGraph(const std::string& name)
		{
			if (divGraphs.find(name) == divGraphs.end() || !divGraphs[name].first)
			{
				return;
			}
			DeleteGraph(*divGraphs[name].first);
			Memory::SafeDeleteArray(divGraphs[name].first);
			divGraphs.erase(name);
		}
		/**
		* @brief �������ɓǂݍ��񂾕����摜���\�[�X��������܂�
		* @param (name) �o�^��
		* @return �o�^�������݂��Ȃ��ꍇ�����N���܂���
		*/
		void ReMoveGraph(const std::string& name)
		{
			if (graphs.find(name) == graphs.end() || !graphs[name])
			{
				return;
			}
			DeleteGraph(graphs[name]);
			graphs.erase(name);
		}
	};

	//!�T�E���h�n���h�����Ǘ����܂�
	class SoundManager
	{
	private:
		std::unordered_map<std::string, int> sounds;
	public:
		~SoundManager()
		{
			InitSoundMem();
		}
		/**
		* @brief �T�E���h�����[�h���܂�
		* @param (path) �t�@�C���p�X
		* @param (name) �o�^��
		* @return ����������ture
		* @detail ���ɓo�^�������O�͎g���܂���
		*/
		bool Load(const std::string& path, const std::string& name)
		{
			//���O�̏d���h�~
			if (sounds.count(name))
			{
				WinMessageBox("�T�E���h�n���h��" + name + "�͂����o�^����Ă��܂�", "Error");
				assert(false);
			}
			sounds[name] = LoadSoundMem(path.c_str());
			if (sounds[name] == -1)
			{
				return false;
			}
			return true;
		}
		/**
		* @brief �������ɓǂݍ��񂾃T�E���h�n���h����Ԃ��܂�
		* @param (name) �o�^��
		* @return ����������n���h�����Ԃ�܂�
		* @detail ���݂��Ȃ����O�ɃA�N�Z�X����ƃG���[�ɂȂ�܂�
		*/
		int GetHandle(const std::string& name)
		{
			if (sounds.find(name) == sounds.end())
			{
				WinMessageBox("�o�^��" + name + "�̃T�E���h�n���h���͑��݂��܂���", "Error");
				assert(false);
			}
			return sounds[name];
		}
		/**
		* @brief �������ɓǂݍ��񂾃T�E���h�n���h�������݂��邩�Ԃ��܂�
		* @param (name) �o�^��
		* @return �n���h�������݂�����true
		*/
		bool IsExistenceHandle(const std::string& name)
		{
			if (sounds.count(name))
			{
				return true;
			}
			return false;
		}
		/**
		* @brief �������ɓǂݍ��񂾃T�E���h���\�[�X��������܂�
		* @param (name) �o�^��
		* @return �o�^�������݂��Ȃ��ꍇ�����N���܂���
		*/
		void ReMove(const std::string& name)
		{
			if (sounds.find(name) == sounds.end() || !sounds[name])
			{
				return;
			}
			DeleteSoundMem(sounds[name]);
			sounds.erase(name);
		}
	};

public:
	//!GraphicManager���擾���܂�
	static GraphicManager& GetGraph()
	{
		static std::unique_ptr<GraphicManager> pGraph = 
			std::make_unique<GraphicManager>();
		return *pGraph;
	}
	//!SoundManager���擾���܂�
	static SoundManager& GetSound()
	{
		static std::unique_ptr<SoundManager> pSound =
			std::make_unique<SoundManager>();
		return *pSound;
	}
};