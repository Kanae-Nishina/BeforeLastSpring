/*
	@file			GameObject.h
	@brief		ゲームオブジェクト
	@date		2017/02/22
	@author	仁科香苗
*/
#pragma once

class GameObject
{
public:
	virtual ~GameObject() {};
	virtual void Init() = 0;				//初期化
	virtual void Update() = 0;		//更新
	virtual void Render() = 0;		//描画
	virtual void Destroy() = 0;		//解放
};