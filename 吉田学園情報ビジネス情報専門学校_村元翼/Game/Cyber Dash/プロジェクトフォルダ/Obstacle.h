//===========================================
// ��Q���̏���
// Author:������
//===========================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include "main.h"
#include "model_spawner.h"

//===========================================
// �O���錾
//===========================================
class CUi;

//===========================================
// �N���X�̒�`
//===========================================
class CObstacle : public CModel_Spawner
{
public:
	//-----------------------------
	// ��Q���̎�ޕ���
	//-----------------------------
	enum class OBSTACLE
	{
		NEEDLE = 0,
		CUBE,
		MAX
	};

	CObstacle(OBJTYPE nPriority = OBJTYPE_MODEL);		// �R���X�g���N�^
	~CObstacle();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �I�u�W�F�N�g����
	static CObstacle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, OBSTACLE obstype);
	// �v���C���[����
	void DeletePlayer(void);

private:
	CUi			*m_pUI;			// �c�@�\���pUI
	int			m_nFrame;		// �t���[���v���p
	bool		m_bDoOnce;		// ��x�̂ݒʂ鏈���Ɏg���ϐ�
	bool		m_bRetry;		// ���g���C���Ɏg���ϐ�
	OBSTACLE	m_ObsType;		// ��Q���̎��
};
#endif
