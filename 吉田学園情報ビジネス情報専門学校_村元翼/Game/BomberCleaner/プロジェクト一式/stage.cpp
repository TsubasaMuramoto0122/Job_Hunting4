//========================================
// �X�e�[�W�̍쐬
// Author:������
//========================================
#include "stage.h"
#include "model.h"
#include "texture.h"
#include "model_spawner.h"
#include "BombSpawner.h"
#include "Bomb.h"
#include "player.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "LoadX.h"
#include "manager.h"
#include "meshsphere.h"
#include "Target.h"

//========================================
// �R���X�g���N�^
//========================================
CStage::CStage()
{
}

//========================================
// �f�X�g���N�^
//========================================
CStage::~CStage()
{
}

//========================================
// �X�e�[�W������
//========================================
HRESULT CStage::Init(void)
{
	return S_OK;
}

//========================================
// �I��
//========================================
void CStage::Uninit(void)
{

}

//========================================
// �X�V
//========================================
void CStage::Update(void)
{

}

//========================================
// �X�e�[�W�̓ǂݍ���
//========================================
void CStage::SetStage(const char *aStageFileName)
{
	// �t�@�C���ǂݍ���
	FILE *pFile;

	// NULL�`�F�b�N
	if (pFile = fopen(aStageFileName, "r"))
	{
		char aStr[6][255] = { NULL };		// �ǂݍ��݂Ɏg���ϐ�

		// ���[�J�����
		int nType = 0;
		int nFunc = 0;
		int nTexType = 0;
		int nMeshType = 0;
		int nBlockX = 0;
		int nBlockY = 0;
		bool bInSide = false;
		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 size = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 scale = { 1.0f,1.0f,1.0f };

		// END_SCRIPT���Ă΂��܂Ń��[�v����
		// �P�P���ǂݍ���
		while(fscanf(pFile, "%s", &aStr[0]))
		{
			//=====================================================
			// �I�u�W�F�N�g�ݒu����
			//=====================================================
			while (strcmp(aStr[0], "MODELSET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// �T�C�Y�`�F�b�N
				{
					fscanf(pFile, "%s", &aStr[1]);					// ������ǂݍ���

					if (strcmp(aStr[1], "TYPE") == 0)				// �^�C�v�擾
					{
						fscanf(pFile, "%*s%d", &nType);
					}
					if (strcmp(aStr[1], "POS") == 0)				// �ʒu�擾
					{
						fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr[1], "ROT") == 0)				// ��]�擾
					{
						fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(aStr[1], "SCALE") == 0)				// �X�P�[���擾
					{
						fscanf(pFile, "%*s%f %f %f", &scale.x, &scale.y, &scale.z);
					}
					if (strcmp(aStr[1], "END_MODELSET") == 0)		// �I�u�W�F�N�g�̐���
					{
						// �^�C�v�ʐ���
						switch (nType)
						{
						case TypeBomb:
							CBomb::Create(pos, rot, scale, nType);
							break;

						case TypeBombspawner:
							CBombSpawner::Create(pos, rot, scale, nType);
							break;

						case TypeTargetBag:
							CTarget::Create(pos, rot, scale, nType);
							break;

						case TypeTargetWoodBox:
							CTarget::Create(pos, rot, scale, nType);
							break;

						default:
							CModel_Spawner::Create(pos, rot, scale, nType);
							break;
						}				
						break;
					}
				}

				else
				{
					printf("�ǂݍ��ݍς݂�X�t�@�C�������݂��܂���");
				}
			}

			//=====================================================
			// ���b�V���ݒu����
			//=====================================================
			while (strcmp(aStr[0], "FIELDSET") == 0)
			{
				fscanf(pFile, "%s", &aStr[1]);				// ������ǂݍ���
				if (strcmp(aStr[1], "MESH_TYPE") == 0)		// ���b�V���^�C�v�擾
				{
					fscanf(pFile, "%*s%d", &nMeshType);
				}
				if (strcmp(aStr[1], "TEX_TYPE") == 0)		// �e�N�X�`���^�C�v�擾
				{
					fscanf(pFile, "%*s%d", &nTexType);
				}
				if (strcmp(aStr[1], "POS") == 0)			// �ʒu�擾
				{
					fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(aStr[1], "ROT") == 0)			// ��]�擾
				{
					fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
				}
				if (strcmp(aStr[1], "SIZE") == 0)			// �T�C�Y�擾
				{
					fscanf(pFile, "%*s%f %f %f", &size.x, &size.y, &size.z);
				}
				if (strcmp(aStr[1], "BLOCK") == 0)			// �������擾
				{
					fscanf(pFile, "%*s%d %d", &nBlockX, &nBlockY);
				}
				if (strcmp(aStr[1], "SIDE") == 0)			// �`��T�C�h�擾
				{
					fscanf(pFile, "%*s%d", (int*)&bInSide);
				}
				if (strcmp(aStr[1], "END_FIELDSET") == 0)	// ���b�V���̐���
				{
					// �^�C�v�ʐ���
					CMeshField::Create(pos, D3DXVECTOR3(size.x, 0.0f, size.y), rot, nBlockX, nBlockY);
					break;
				}
			}

			//=====================================================
			// ���b�V���ݒu����
			//=====================================================
			while (strcmp(aStr[0], "WALLSET") == 0)
			{
				// ���[�J�����b�V�����
				int nTexType = 0;
				int nMeshType = 0;
				int nBlockX = 0;
				int nBlockY = 0;
				bool bInSide = false;
				D3DXVECTOR3 pos, rot, size;

				fscanf(pFile, "%s", &aStr[1]);				// ������ǂݍ���
				if (strcmp(aStr[1], "MESH_TYPE") == 0)		// ���b�V���^�C�v�擾
				{
					fscanf(pFile, "%*s%d", &nMeshType);
				}
				if (strcmp(aStr[1], "TEX_TYPE") == 0)		// �e�N�X�`���^�C�v�擾
				{
					fscanf(pFile, "%*s%d", &nTexType);
				}
				if (strcmp(aStr[1], "POS") == 0)			// �ʒu�擾
				{
					fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(aStr[1], "ROT") == 0)			// ��]�擾
				{
					fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
				}
				if (strcmp(aStr[1], "SIZE") == 0)			// �T�C�Y�擾
				{
					fscanf(pFile, "%*s%f %f %f", &size.x, &size.y, &size.z);
				}
				if (strcmp(aStr[1], "BLOCK") == 0)			// �������擾
				{
					fscanf(pFile, "%*s%d %d", &nBlockX, &nBlockY);
				}
				if (strcmp(aStr[1], "SIDE") == 0)			// �`��T�C�h�擾
				{
					fscanf(pFile, "%*s%d", (int*)&bInSide);
				}
				if (strcmp(aStr[1], "END_WALLSET") == 0)	// ���b�V���̐���
				{
					// �^�C�v�ʐ���
					CMeshField::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), rot, nBlockX, nBlockY);
					break;
				}
			}
			
			while (strcmp(aStr[0], "PLAYER_SET") == 0)
			{
				if (CManager::GetInstance()->GetLoadX()->GetNumAll() > 0)	// �T�C�Y�`�F�b�N
				{
					fscanf(pFile, "%s", &aStr[1]);				// ������ǂݍ���
					if (strcmp(aStr[1], "POS") == 0)			// �ʒu�擾
					{
						fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr[1], "ROT") == 0)			// ��]�擾
					{
						fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(aStr[1], "END_PLAYER_SET") == 0)		// �I�u�W�F�N�g�̐���
					{
						// �v���C���[�̓ǂݍ���
						CPlayer::Create(pos, rot, 0);
						break;
					}
				}

				else
				{
					printf("�ǂݍ��ݍς݂�X�t�@�C�������݂��܂���");
				}
			}
			
			// �ǂݍ��݂Ɣz�u�I��
			if (strcmp(aStr[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}

	else
	{
		// �G���[
		perror("�t�@�C�����J���܂���ł���");
	}

	// �t�@�C�������
	fclose(pFile);
}