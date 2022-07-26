//=============================================================================
// コントロール処理 [control_fade.h]
//=============================================================================
#ifndef _CONTROL_FADE_H_
#define _CONTROL_FADE_H_
#include "main.h"
#include "control.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene;

//*****************************************************************************
// コントロールクラス
//*****************************************************************************
class CControlFade : public CControl
{
public:
	CControlFade();							// コンストラクタ
	~CControlFade();						// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(CScene *pObject);			// 更新処理
	static CControlFade *Create();			// 生成処理
};

#endif