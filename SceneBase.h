/*
	@file	SceneBase.h
	@brief	シーン管理のスーパークラス
	@date	2017/02/18
	@author	仁科香苗
*/
#pragma once

class SceneBase
{
public:
	virtual ~SceneBase() {};

	virtual void Init() = 0;		//初期化
	virtual SceneBase* Update(SceneBase* scene) = 0;		//更新
	virtual void Render() = 0;		//描画
	virtual void Destroy() = 0;		//解放
};
