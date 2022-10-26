//===========================================
// ���f����z�u���邽�߂̏���
// Author:������
//===========================================
#ifndef _MODEL_SPAWNER_H_
#define _MODEL_SPAWNER_H_
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

	// �I�u�W�F�N�g����
	static CModel_Spawner *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);//rot�͊p�x������

	//-----------------------------------------------
	// �ݒ�E�擾����
	//-----------------------------------------------
	CModel *GetModel(void)			{ return m_pModel; }	// ���f�����̎擾
	void SetRot(float fRot)			{ m_rot.y = fRot; }		// Y����]���̐ݒ�
	void SetSRot(D3DXVECTOR3 rot)	{ m_rot = rot; }		// ��]���̐ݒ�
	void SetSPos(D3DXVECTOR3 pos)	{ m_pos = pos; }		// �ʒu���̐ݒ�
	void SetBoolRot(bool bRot)		{ m_MoveRot = bRot; }	// ��]���邩�ۂ��̐ݒ�

protected:
	D3DXVECTOR3				m_pos;					// ���f���̈ʒu�i�I�t�Z�b�g�j
	D3DXVECTOR3				m_rot;					// ����
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	CModel					*m_pModel;				// ���f���N���X

private:
	float					m_fRot;					// ��]
	bool					m_MoveRot;				// ��]���郂�f���ɂȂ邩�̔���
	D3DXMATERIAL			*m_pMat;				// �}�e���A���ւ̃|�C���^
};
#endif