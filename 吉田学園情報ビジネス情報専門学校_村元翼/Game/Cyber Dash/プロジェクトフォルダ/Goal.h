//===========================================
// �S�[���n�_�̃��f��
// Author:������
//===========================================
#ifndef _GOAL_H_
#define _GOAL_H_
#include "main.h"
#include "model_spawner.h"

//===========================================
// �O���錾
//===========================================
class CUi;

//===========================================
// �N���X�̒�`
//===========================================
class CGoal : public CModel_Spawner
{
public:
	CGoal(OBJTYPE nPriority = OBJTYPE_MODEL);			// �R���X�g���N�^
	~CGoal();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �I�u�W�F�N�g����
	static CGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);

	// �擾�֐�
	D3DXVECTOR3 GetScale() { return m_scale; }

private:
	D3DXVECTOR3 m_scale;			// �X�P�[��
	CUi			*m_pUI;				// �I��UI
	int			m_nFrame;			// �t���[����

};
#endif