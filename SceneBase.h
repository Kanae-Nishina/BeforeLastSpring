/*
	@file	SceneBase.h
	@brief	�V�[���Ǘ��̊��N���X
	@date	2017/02/21
	@author	�m�ȍ��c
*/
#pragma once

#if 0 �G�������̂ł����������
class SceneBase
{
public:
	virtual ~SceneBase() {};
	virtual void Init() = 0;							//������
	virtual SceneBase* Update(SceneBase* scene) = 0;	//�X�V
	virtual void Render() = 0;							//�`��
	virtual void Destroy() = 0;							//���
};

#endif