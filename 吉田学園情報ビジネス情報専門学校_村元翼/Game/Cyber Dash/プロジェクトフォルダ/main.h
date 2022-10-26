//=============================================================================
//
// ���C������ [main.h]
// Author : ������
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "d3dx9.h"											// �`��ɕK�v
#define DIRECTINPUT_VERSION (0x0800)						// �r���h���̌x���Ώ��p�}�N��
#include <dinput.h>											// DirectInput�ɕK�v
#include <XAudio2.h>										// �T�E���h�Đ��ɕK�v
#include <stdio.h>
#include <vector>
#include <map>
#include <math.h>
//#include <stdlib.h>
//#include <string.h>

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment (lib,"d3d9.lib")							// �`��ɕK�v
#pragma comment (lib,"d3dx9.lib")							// [d3d9.lib]�̊g�����C�u����
#pragma comment (lib,"dxguid.lib")							// DirectX�R���|�[�l���g�g�p�ɕK�v
#pragma comment (lib,"dinput8.lib")							// ���͏����ɕK�v
#pragma comment (lib,"winmm.lib")							// FPS�\���ɕK�v
#pragma comment (lib,"xinput.lib")							// XInput���͏����ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH		(1280)													// �E�B���h�E�̕�(16:9)
#define SCREEN_HEIGHT		(720)													// �E�B���h�E�̍���(16:9)
#define FULLSCREEN_WIDTH	(1920)													// �t���X�N���[����
#define FULLSCREEN_HEIGHT	(1080)													// �t���X�N���[������
#define SCREEN_CENTER		(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0.0f))	// �X�N���[���̐^��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#if _DEBUG
int GetFPS();
#endif
#endif //_MAIN_H_

