//===========================================
// ���f����z�u���邽�߂̏���
//===========================================
#ifndef _MODEL_SPAWNER_H
#define _MODEL_SPAWNER_H

//===========================================
// �C���N���[�h�t�@�C��
//===========================================
#include "main.h"
#include "scene.h"

//===========================================
// �O���錾
//===========================================
class CModel;

//===========================================
// �N���X�̒�`
//===========================================
class CModel_Spawner : public CScene
{
public:
	CModel_Spawner(OBJTYPE nPriority = OBJTYPE_MODEL);	// �R���X�g���N�^
	~CModel_Spawner();									// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// ����
	static CModel_Spawner *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);//rot�͊p�x������

	//-----------------------------------------------
	// �ݒ�E�擾����
	//-----------------------------------------------
	CModel *GetModel(void)			{ return m_pModel; }	// ���f�����̎擾
	void SetRot(float fRot)			{ m_rot.y = fRot; }		// Y����]���̐ݒ�
	void SetSRot(D3DXVECTOR3 rot)	{ m_rot = rot; }		// ��]���̐ݒ�
	void SetSPos(D3DXVECTOR3 pos)	{ m_pos = pos; }		// �ʒu���̐ݒ�

private:
	float m_fRot;									// ��]
	D3DXVECTOR3				m_pos;					// ���f���̈ʒu�i�I�t�Z�b�g�j
	D3DXVECTOR3				m_rot;					// ����
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXMATERIAL			*m_pMat;				// �}�e���A���ւ̃|�C���^
	CModel					*m_pModel;				// ���f���N���X
};
#endif
