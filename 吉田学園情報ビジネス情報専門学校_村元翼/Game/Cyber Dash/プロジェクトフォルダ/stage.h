//========================================
// �X�e�[�W�ǂݍ���
// Author:������
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"

//========================================
// �}�N����`
//========================================

//========================================
// �O���錾
//========================================
class CModel_Spawner;
class CPlayer;
class CMeshField;
class CMeshCylinder;
class CMeshSphere;
class CModel_Spawner;
class CGoal;
class CObstacle;
class CTarget;

//========================================
// �X�e�[�W�N���X
//========================================
class CStage
{
public:
	//------------------------------------
	// �ǂݍ��ރ��f���̎��
	//------------------------------------
	enum class LOADMODEL
	{
		NORMAL = 0,		// �ʏ�
		GOAL,			// �S�[��
		NEEDLE,			// ��Q���p�j
		OBSCUBE,		// ��Q���p�L���[�u
		GATE01,			// �Q�[�g1
		GATE02,			// �Q�[�g2
		GATE03,			// �Q�[�g3
		GATE04,			// �Q�[�g4
		MAX
	};

	//------------------------------------
	// �R���X�g���N�^�A�f�X�g���N�^
	//------------------------------------
	CStage();
	~CStage();

	//------------------------------------
	// �v���g�^�C�v�錾
	//------------------------------------
	HRESULT Init(void);											// ������
	void Uninit(void);											// �I��
	void Update(void);											// �X�V
	void SetStage(const char *aStageFileName);					// ���f���e�L�X�g��ǂݍ��ފ֐�

	static CPlayer		*GetPlayer()		{ return m_pPlayer; }		// �v���C���[�̎擾
	static CGoal		*GetGoal()			{ return m_pGoal; }			// �S�[���̎擾
	static D3DXVECTOR3	GetStartPos()		{ return m_StartPos; }		// �X�^�[�g�ʒu�̎擾
	static D3DXVECTOR3	GetGoalPos()		{ return m_GoalPos; }		// �S�[���ʒu�̎擾

private:
	vector<CObstacle>	m_pObstacle;
	static CPlayer		*m_pPlayer;
	static CGoal		*m_pGoal;
	static D3DXVECTOR3	m_StartPos;
	static D3DXVECTOR3	m_GoalPos;
};
#endif