/*
	@file			GameObject.h
	@brief		�Q�[���I�u�W�F�N�g
	@date		2017/02/22
	@author	�m�ȍ��c
*/
#pragma once

class GameObject
{
public:
	virtual ~GameObject() {};
	virtual void Init() = 0;				//������
	virtual void Update() = 0;		//�X�V
	virtual void Render() = 0;		//�`��
	virtual void Destroy() = 0;		//���
};