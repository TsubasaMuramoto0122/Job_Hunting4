//========================================
// �X�e�[�W�ǂݍ���
// Author:������
//========================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"

//========================================
// �O���錾
//========================================
class CModel_Spawner;
class CPlayer;

//========================================
// �X�e�[�W�N���X�̒�`
//========================================
class CStage
{
public:
	enum ModelType
	{
		TypeBomb = 6,
		TypeBombspawner = 2,
		TypeTargetBag = 3,
		TypeTargetWoodBox = 4,

	};
	CStage();				// �R���X�g���N�^
	~CStage();				// �f�X�g���N�^

	HRESULT Init(void);								// ������
	void Uninit(void);								// �I��
	void Update(void);								// �X�V
	void SetStage(const char *aStageFileName);		// ���f���e�L�X�g��ǂݍ��ފ֐�
};
#endif