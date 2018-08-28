#pragma once
#include <DxLib.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <assert.h>
#include "../Utility/Utility.hpp"

//DX���C�u������LoadGraph��LoadSoundMen����擾�����n���h�����Ǘ�����N���X
//�Ƃ肠���������Ɏ���
class ResourceManager
{
private:
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
		int GetHandle(const std::string& name)
		{
			if (graphs.find(name) == graphs.end())
			{
				WinMessageBox("�o�^��" + name + "�̃O���t�B�b�N�n���h���͑��݂��܂���", "Error");
				assert(false);
			}
			return graphs[name];
		}
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
		bool IsExistenceHandle(const std::string& name)
		{
			if (graphs.count(name))
			{
				return true;
			}
			return false;
		}
		bool IsExistenceDivHandle(const std::string& name)
		{
			if (divGraphs.count(name))
			{
				return true;
			}
			return false;
		}
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
	class SoundManager
	{
	private:

	public:

	};

public:
	static GraphicManager& GetGraph()
	{
		static std::unique_ptr<GraphicManager> pGraph = 
			std::make_unique<GraphicManager>();
		return *pGraph;
	}
};