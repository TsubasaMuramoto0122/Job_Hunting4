//==============================================================================
// メッシュスフィアの作成
// Author:村元翼
//==============================================================================
#include "meshsphere.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshSphere::CMeshSphere(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pTexture = nullptr;						// テクスチャポインタ
	m_pVtxBuff = nullptr;						// 頂点バッファポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// スケール

}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshSphere::~CMeshSphere()
{

}

//---------------------------------------------------------------
// インスタンス生成処理
//---------------------------------------------------------------
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical, bool bInsideDraw, CScene::OBJTYPE objtype)
{
	// インスタンス生成
	CMeshSphere *pMeshSphere = nullptr;

	if (pMeshSphere = new CMeshSphere(objtype))
	{
		pMeshSphere->m_rot = rot;					// 回転
		pMeshSphere->m_nLine = nLine;				// 横ポリゴン数
		pMeshSphere->m_nVertical = nVertical;		// 縦ポリゴン数
		pMeshSphere->m_bInsideDraw = bInsideDraw;	// 裏描画にするか
		pMeshSphere->Init(pos, size);				// 初期化
	}
	return pMeshSphere;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CMeshSphere::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 位置・サイズ
	m_pos = pos;
	m_size = size;

	// 位置・サイズ・カラー設定処理
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);
	CScene::SetCol({ 1.0f,1.0f,1.0f,1.0f });

	// 頂点情報を設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * (m_nLine + 1) * (m_nVertical + 1),	// 頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// 頂点インデックスを設定
	pDevice->CreateIndexBuffer
	(
		(sizeof(WORD) * ((m_nVertical - 1) * (m_nLine + 3) + (m_nLine + 1) * (m_nVertical + 1))),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pVtxIndexBuff,
		NULL
	);

	// 3D頂点バッファのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			// フィールド
			if (m_size.y <= 0)
			{
				// 頂点位置
				pVtx[nNum].pos.x = cosf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * (m_size.x / 2.0f);
				pVtx[nNum].pos.y = sinf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * (m_size.x / 2.0f);
				pVtx[nNum].pos.z = m_size.z / 2.0f - (m_size.z / m_nVertical) * nVertical;
			}

			// ウォール
			else if (m_size.y > 0)
			{

				pVtx[nNum].pos.x = cosf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * sinf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_size.x / 2.0f);
				pVtx[nNum].pos.y = cosf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_size.y / 2.0f);
				pVtx[nNum].pos.z = sinf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * sinf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_size.z / 2.0f);
			}

			// 法線
			pVtx[nNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


			//カラー
			pVtx[nNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + (1.0f * nLine), 0.0f + (1.0f * nVertical));

			// 配列の最後に頂点の位置データを追加
			m_vtxLocal.emplace_back(pVtx[nNum].pos);
		}
	}

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	// インデックス情報へのポインタ
	WORD *pIdx;

	// 頂点インデックスバッファのロック
	m_pVtxIndexBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの割り当て
	for (int nCnt1 = 0; nCnt1 < m_nVertical; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nLine + 1; nCnt2++)
		{
			// 番号割り当て
			pIdx[(nCnt2 * 2) + 0 + (m_nLine + 2) * 2 * nCnt1] = (m_nLine + 1) + nCnt2 + (m_nLine + 1) * nCnt1;
			pIdx[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] = nCnt2 + (m_nLine + 1) * nCnt1;
		}
	}
	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		// 縮退ポリゴンの番号割り当て
		pIdx[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = m_nLine + (m_nLine + 1) * nCnt3;
		pIdx[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3;
	}

	// インデックスバッファのアンロック
	m_pVtxIndexBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CMeshSphere::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pVtxIndexBuff)
	{
		m_pVtxIndexBuff->Release();
		m_pVtxIndexBuff = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CMeshSphere::Update(void)
{
	m_size = CScene::GetSize();
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CMeshSphere::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	if (GetObjType() == OBJTYPE_EFFECTMESH)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);										// ライティング有効
		CManager::GetInstance()->GetRenderer()->SetZTest(true, pDevice);					// Ztest
		CManager::GetInstance()->GetRenderer()->SetAddSynthesis(true, pDevice);				// 加算合成
	}

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス
	D3DXMatrixIdentity(&m_mtxWorld);		// ワールドマトリックスの初期化

	// 大きさを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);					// 頂点フォーマットの設定

	pDevice->SetIndices(m_pVtxIndexBuff);			// 頂点インデックスの登録

	pDevice->SetTexture(0, m_pTexture);				// テクスチャの設定

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		((m_nLine + 1) * (m_nVertical + 1)),					// 頂点の数
		0,														// 開始する頂点のインデックス
		2 * m_nLine * m_nVertical + (m_nVertical * 4) - 4		// 描画するプリミティブ数
																// ↑1フィールドに2プリミティブ	↑縮退ポリゴンの分(Z方向に1フィールド伸ばす分4つ生成するから)
	);

	if (GetObjType() == OBJTYPE_EFFECTMESH)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティング有効
		CManager::GetInstance()->GetRenderer()->SetAddSynthesis(false, pDevice);// 加算合成
		CManager::GetInstance()->GetRenderer()->SetZTest(false, pDevice);		// Ztest
	}

	//*****************************************************************************
	// 頂点のワールドマトリックス
	//*****************************************************************************
	D3DXMATRIX mtxParent;	// 親のマトリックス

	// 配列サイズを設定
	m_mtxVec.resize((m_nLine + 1) * (m_nVertical + 1));
	m_vtxWorld.resize((m_nLine + 1) * (m_nVertical + 1));

	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			// 各頂点のワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_mtxVec[nNum]);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_vtxLocal[nNum].x, m_vtxLocal[nNum].y, m_vtxLocal[nNum].z);
			D3DXMatrixMultiply(&m_mtxVec[nNum], &m_mtxVec[nNum], &mtxTrans);

			//*****************************************************************************
			// 親子関係付け処理
			//*****************************************************************************
			// 各頂点の親のマトリックスを設定
			if (m_mtxWorld != nullptr)
			{
				mtxParent = m_mtxWorld;
			}

			else
			{
				// 最新のマトリックスを取得
				pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
			}

			// 算出した各頂点のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply
			(
				&m_mtxVec[nNum],
				&m_mtxVec[nNum],
				&mtxParent
			);
			//****************************************************************************
			// ここまで

			// 各頂点のワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_mtxVec[nNum]);

			// 各頂点のワールド座標の保存
			m_vtxWorld[nNum].x = m_mtxVec[nNum]._41;
			m_vtxWorld[nNum].y = m_mtxVec[nNum]._42;
			m_vtxWorld[nNum].z = m_mtxVec[nNum]._43;
		}
	}
}

//------------------------------------------------------------
// 頂点座標の設定
//------------------------------------------------------------
void CMeshSphere::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CScene::SetPos(pos);
	CScene::SetSize(scale);
	m_pos = pos;
	m_size = scale;
}

//------------------------------------------------------------
//テクスチャの設定
//------------------------------------------------------------
void CMeshSphere::SetTex(int nAnim, int nPartU)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 0.0);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 1.0f);

	m_pVtxBuff->Unlock();

}
void CMeshSphere::SetTex(float fSpeedX, float fSpeedY)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[1].tex = D3DXVECTOR2(1.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fSpeedX, 1.0f + fSpeedY);
	pVtx[3].tex = D3DXVECTOR2(1.0f + fSpeedX, 1.0f + fSpeedY);

	m_pVtxBuff->Unlock();
}

void CMeshSphere::SetCol(D3DXCOLOR col)
{
	CScene::SetCol(col);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			pVtx[nNum].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
		}
	}

	m_pVtxBuff->Unlock();
}