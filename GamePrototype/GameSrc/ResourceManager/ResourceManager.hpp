/**
* @file ResourceManager.hpp
* @brief DXライブラリのLoadGraphやLoadSoundMemから取得したハンドルを管理するクラスです
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
	//!Graphicハンドルを管理します
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
		* @brief 画像をロードします
		* @param (path) ファイルパス
		* @param (name) 登録名
		* @return 成功したらture
		* @detail 既に登録した名前は使えません
		*/
		bool Load(const std::string& path, const std::string& name)
		{
			//名前の重複防止
			if (graphs.count(name))
			{
				WinMessageBox("グラフィックハンドル" + name + "はもう登録されています", "Error");
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
		* @brief 分割画像をロードします
		* @param (path) ファイルパス
		* @param (name) 登録名
		* @param (AllNum) 画像の分割総数
		* @param (XNum) 横方向の数
		* @param (YNum) 縦方向の数
		* @param (XSize) 分割した画像一枚分の横のサイズ
		* @param (YSize) 分割した画像一枚分の縦のサイズ
		* @return 成功したらture
		* @detail 既に登録した名前は使えません
		*/
		bool LoadDiv(const std::string& path, const std::string& name,
			const int AllNum,
			const int XNum, const int YNum,
			const int XSize, const int YSize)
		{
			//名前の重複防止
			if (divGraphs.count(name))
			{
				WinMessageBox("グラフィックハンドル" + name + "はもう登録されています", "Error");
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
		* @brief メモリに読み込んだ画像のハンドルを返します
		* @param (name) 登録名
		* @return 成功したらハンドルが返ります
		* @detail 存在しない名前にアクセスするとエラーになります
		*/
		int GetHandle(const std::string& name)
		{
			if (graphs.find(name) == graphs.end())
			{
				WinMessageBox("登録名" + name + "のグラフィックハンドルは存在しません", "Error");
				assert(false);
			}
			return graphs[name];
		}
		/**
		* @brief メモリに読み込んだ分割画像のハンドルを返します
		* @param (name) 登録名
		* @param (index) 配列の要素数
		* @return 成功したらハンドルが返ります
		* @detail 存在しない名前にアクセスするか分割数を超えた値を指定するとエラーになります
		*/
		int GetDivHandle(const std::string& name,const int index)
		{
			if (divGraphs.find(name) == divGraphs.end())
			{
				WinMessageBox("登録名" + name + "のグラフィックハンドルは存在しません", "Error");
				assert(false);
			}
			if ((size_t)index >= divGraphs[name].second)
			{
				WinMessageBox("登録名" + name + "の分割数を超えた値です", "Error");
				assert(false);
			}
			return divGraphs[name].first[index];
		}
		/**
		* @brief メモリに読み込んだ画像のハンドルが存在するか返します
		* @param (name) 登録名
		* @return ハンドルが存在したらtrue
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
		* @brief メモリに読み込んだ分割画像のハンドルが存在するか返します
		* @param (name) 登録名
		* @return ハンドルが存在したらtrue
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
		* @brief メモリに読み込んだ画像リソースを解放します
		* @param (name) 登録名
		* @return 登録名が存在しない場合何も起きません
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
		* @brief メモリに読み込んだ分割画像リソースを解放します
		* @param (name) 登録名
		* @return 登録名が存在しない場合何も起きません
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

	//!サウンドハンドルを管理します
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
		* @brief サウンドをロードします
		* @param (path) ファイルパス
		* @param (name) 登録名
		* @return 成功したらture
		* @detail 既に登録した名前は使えません
		*/
		bool Load(const std::string& path, const std::string& name)
		{
			//名前の重複防止
			if (sounds.count(name))
			{
				WinMessageBox("サウンドハンドル" + name + "はもう登録されています", "Error");
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
		* @brief メモリに読み込んだサウンドハンドルを返します
		* @param (name) 登録名
		* @return 成功したらハンドルが返ります
		* @detail 存在しない名前にアクセスするとエラーになります
		*/
		int GetHandle(const std::string& name)
		{
			if (sounds.find(name) == sounds.end())
			{
				WinMessageBox("登録名" + name + "のサウンドハンドルは存在しません", "Error");
				assert(false);
			}
			return sounds[name];
		}
		/**
		* @brief メモリに読み込んだサウンドハンドルが存在するか返します
		* @param (name) 登録名
		* @return ハンドルが存在したらtrue
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
		* @brief メモリに読み込んだサウンドリソースを解放します
		* @param (name) 登録名
		* @return 登録名が存在しない場合何も起きません
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
	//!GraphicManagerを取得します
	static GraphicManager& GetGraph()
	{
		static std::unique_ptr<GraphicManager> pGraph = 
			std::make_unique<GraphicManager>();
		return *pGraph;
	}
	//!SoundManagerを取得します
	static SoundManager& GetSound()
	{
		static std::unique_ptr<SoundManager> pSound =
			std::make_unique<SoundManager>();
		return *pSound;
	}
};