//===========================================
// �Q�[�g�̃��f��
// Author:������
//===========================================
#ifndef _GATE_H_
#define _GATE_H_
#include "main.h"
#include "model_spawner.h"

//===========================================
// �O���錾
//===========================================
class CUi;

//===========================================
// �N���X�̒�`
//===========================================
class CGate : public CModel_Spawner
{
public:
	//------------------------------------------
	// �Q�[�g�̎��
	//------------------------------------------
	enum class GATE_TYPE
	{
		A = 0,
		B,
		C,
		D,
		MAX
	};

	CGate(OBJTYPE nPriority = OBJTYPE_MODEL);			// �R���X�g���N�^
	~CGate();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �I�u�W�F�N�g����
	static CGate *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, GATE_TYPE Type);

	// �擾�֐�
	D3DXVECTOR3 GetScale() { return m_scale; }

private:
	D3DXVECTOR3 m_scale;			// �X�P�[��
	GATE_TYPE	m_Type;				// �Q�[�g�̎��
	bool		m_bDoOnce;			// ��x�̂ݒʂ�ϐ�
};
#endif
