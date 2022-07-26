//=============================================================================
//
// メイン処理 [main.h]
// Author : 村元翼
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <Windows.h>
#include "d3dx9.h"											// 描画に必要
#define DIRECTINPUT_VERSION (0x0800)						// ビルド時の警告対処用マクロ
#include <dinput.h>											// DirectInputに必要
#include <XAudio2.h>										// サウンド再生に必要
#include <stdio.h>
#include <vector>
#include <map>
#include <math.h>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment (lib,"d3d9.lib")							// 描画に必要
#pragma comment (lib,"d3dx9.lib")							// [d3d9.lib]の拡張ライブラリ
#pragma comment (lib,"dxguid.lib")							// DirectXコンポーネント使用に必要
#pragma comment (lib,"dinput8.lib")							// 入力処理に必要
#pragma comment (lib,"winmm.lib")							// FPS表示に必要
#pragma comment (lib,"xinput.lib")							// XInput入力処理に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define SCREEN_WIDTH		(1280)													// ウィンドウの幅(16:9)
//#define SCREEN_HEIGHT		(720)													// ウィンドウの高さ(16:9)
#define SCREEN_WIDTH	(1920)													// フルスクリーン幅
#define SCREEN_HEIGHT	(1080)													// フルスクリーン高さ
#define SCREEN_CENTER		(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0.0f))	// スクリーンの真ん中

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int GetFPS();

#endif

