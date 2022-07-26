//===========================================
// ���e�X�|�i�[�̍쐬
// Author : ������
//===========================================
#ifndef _BOMBSPAWNER_H_
#define _BOMBSPAWNER_H_
#include "main.h"
#include "scene.h"

//===========================================
// �}�N����`
//===========================================

//===========================================
// �O���錾
//===========================================
class CModel;
class CBomb;

//===========================================
// �N���X��`
//===========================================
class CBombSpawner : public CScene
{
public:
	//---------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//---------------------------------
	CBombSpawner(OBJTYPE nPriority = OBJTYPE_MODEL);
	~CBombSpawner();

	//---------------------------------
	// ���\�b�h
	//---------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	static CBombSpawner *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);	// �I�u�W�F�N�g����

private:
	CModel			*m_pModel;			// ���f���N���X
	CBomb			*m_pBomb;			// ���e�N���X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_scale;			// �X�P�[��
	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	int				m_nSpawnTime;		// �X�|�[���^�C��
	bool			m_bIsThere;			// �����ɑ��݂��Ă��邩
};
#endif