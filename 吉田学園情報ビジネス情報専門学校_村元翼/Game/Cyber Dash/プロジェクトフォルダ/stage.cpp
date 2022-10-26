//========================================
// �X�e�[�W
// Author:������
//========================================
#include "stage.h"
#include "model.h"
#include "texture.h"
#include "model_spawner.h"
#include "player.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "LoadX.h"
#include "manager.h"
#include "meshsphere.h"
#include "Goal.h"
#include "Obstacle.h"
#include "Gate.h"
#include "JumpCircle.h"

//========================================
// �}�N����`
//========================================
#define PLAYER_POS (D3DXVECTOR3(-1000.0f, 50.0f, 0.0f))
#define DEBUG_POS (D3DXVECTOR3(16600.0f, 50.0f, 0.0f))

//========================================
// �ÓI�����o�ϐ��̏�����
//========================================
D3DXVECTOR3 CStage::m_StartPos	= { 0.0f,0.0f,0.0f };
D3DXVECTOR3 CStage::m_GoalPos	= { 0.0f,0.0f,0.0f };
CGoal	*CStage::m_pGoal		= nullptr;
CPlayer *CStage::m_pPlayer		= nullptr;

//========================================
// �R���X�g���N�^
//========================================
CStage::CStage()
{
	m_StartPos = PLAYER_POS;
}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
CStage::~CStage()
{
}

//------------------------------------
// �X�e�[�W������
//------------------------------------
HRESULT CStage::Init(void)
{
	return S_OK;
}

//--------------------------------------------
// �I��
//--------------------------------------------
void CStage::Uninit(void)
{
	if (m_pGoal)
	{
		m_pGoal->Uninit();
		m_pGoal = nullptr;
	}

	if (m_pPlayer)
	{
		m_pPlayer = nullptr;
	}
}

//--------------------------------------------
// �X�V
//--------------------------------------------
void CStage::Update(void)
{

}

//------------------------------------
// �X�e�[�W�̓ǂݍ���
//------------------------------------
void CStage::SetStage(const char *aStageFileName)
{
	// �t�@�C���ǂݍ���
	FILE *pFile;
	// NULL�`�F�b�N
	if (pFile = fopen(aStageFileName, "r"))
	{
		// �ǂݍ��݂Ɏg���ϐ�
		char aStr[6][255] = { NULL };		

		// �擾�Ɏg�����[�J���ϐ�
		int nFunc;							// ���f���ɕt����@�\
		int nTexType, nMeshType, nObjType;	// ���b�V���ƃe�N�X�`���ƃI�u�W�F�N�g�̃^�C�v
		int nBlockX, nBlockY;				// ���������E�c������
		string sType;						// ���f���̖��O(ModelLoad000.txt�ŕt�������O)
		D3DXVECTOR3 pos, rot, scale, size;	// �ʒu�E��]�E�X�P�[���E�T�C�Y
		bool bInSide = false;				// �����ɕ`�悷�邩�̔���

		CLoadX *pLoadX = CManager::GetInstance()->GetLoadX();

		// END_SCRIPT���Ă΂��܂Ń��[�v����
		// �P�P���ǂݍ���
		while(fscanf(pFile, "%s", &aStr[0]))
		{
			//--------------------------------------------------------------
			// ���b�V���ݒu����
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "MESH_SET") == 0)
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
				if (strcmp(aStr[1], "END_MESH_SET") == 0)	// ���b�V���̐���
				{
					switch (nMeshType)
					{
					case CScene::MESH_FIELD:
						CMeshField::Create(pos, D3DXVECTOR3(size.x, 0.0f, size.y), rot, nBlockX, nBlockY);
						break;

					case CScene::MESH_WALL:
						CMeshField::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), rot, nBlockX, nBlockY);
						break;

					case CScene::MESH_CYLINDER:
						CMeshCylinder::Create(pos, size, rot, nBlockX, nBlockY,CMeshCylinder::CYLINDER_TYPE::STAND);
						break;

					case CScene::MESH_SPHERE:
						CMeshSphere::Create(pos, size, rot, nBlockX, nBlockY,bInSide,CScene::OBJTYPE_MESH);
						break;

					default:
						break;
					}
					break;
				}
			}

			//--------------------------------------------------------------
			// �I�u�W�F�N�g�ݒu����
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "MODEL_SET") == 0)
			{
				if (pLoadX->GetNumAll() > 0)	// �T�C�Y�`�F�b�N
				{
					fscanf(pFile, "%s", &aStr[1]);					// ������ǂݍ���
					if (strcmp(aStr[1], "FUNC") == 0)				// ���f���@�\�擾
					{
						fscanf(pFile, "%*s%d", &nFunc);
					}
					if (strcmp(aStr[1], "TYPE") == 0)				// �^�C�v�擾
					{
						fscanf(pFile, "%*s%s", sType.c_str());
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
					if (strcmp(aStr[1], "END_MODEL_SET") == 0)		// �I�u�W�F�N�g�̐���
					{
						int nNumX = pLoadX->GetNum(sType.c_str());

						D3DXVECTOR3 rad;
						rad.x = D3DXToRadian(rot.x);
						rad.y = D3DXToRadian(rot.y);
						rad.z = D3DXToRadian(rot.z);

						switch (static_cast<LOADMODEL>(nFunc))
						{
						case LOADMODEL::NORMAL:			// �ʏ탂�f��
							CModel_Spawner::Create(	pos, rad, scale, nNumX);
							break;

						case LOADMODEL::GOAL:			// �S�[�����f��
							if (!m_pGoal)
							{
								m_pGoal = CGoal::Create(pos, rad, scale, nNumX);
								m_GoalPos = pos;
							}
							break;

						case LOADMODEL::NEEDLE:			// �j(��Q��)���f��
							CObstacle::Create(pos, rad, scale, nNumX,CObstacle::OBSTACLE::NEEDLE);
							break;

						case LOADMODEL::OBSCUBE:		// �L���[�u(��Q��)���f��
							CObstacle::Create(pos, rad, scale, nNumX, CObstacle::OBSTACLE::CUBE);
							break;

						case LOADMODEL::GATE01:			// �Q�[�g1���f��
							CGate::Create(pos, rad, scale, nNumX, CGate::GATE_TYPE::A);
							break;

						case LOADMODEL::GATE02:			// �Q�[�g2���f��
							CGate::Create(pos, rad, scale, nNumX, CGate::GATE_TYPE::B);
							break;

						case LOADMODEL::GATE03:			// �Q�[�g3���f��
							CGate::Create(pos, rad, scale, nNumX, CGate::GATE_TYPE::C);
							break;

						case LOADMODEL::GATE04:			// �Q�[�g4���f��
							CGate::Create(pos, rad, scale, nNumX, CGate::GATE_TYPE::D);
							break;

						default :						// �ʏ탂�f��
							CModel_Spawner::Create(pos, rad, scale, nNumX);
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
			
			//--------------------------------------------------------------
			// �v���C���[�ݒu����
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "PLAYER_SET") == 0)
			{
				if (pLoadX->GetNumAll() > 0)	// �T�C�Y�`�F�b�N
				{
					fscanf(pFile, "%s", &aStr[1]);							// ������ǂݍ���
					if (strcmp(aStr[1], "END_PLAYER_SET") == 0)				// �I�u�W�F�N�g�̐���
					{
						// �v���C���[�̓ǂݍ���
						m_pPlayer = CPlayer::Create(m_StartPos, { 0.0f,0.0f,0.0f }, CPlayer::PLAYER_STATE::CUBE);
						break;
					}
				}

				else
				{
					printf("�ǂݍ��ݍς݂�X�t�@�C�������݂��܂���");
				}
			}

			//--------------------------------------------------------------
			// �W�����v�T�[�N����������
			//--------------------------------------------------------------
			while (strcmp(aStr[0], "OBJECT_SET") == 0)
			{
				fscanf(pFile, "%s", &aStr[1]);			// ������ǂݍ���
				if (strcmp(aStr[1], "TYPE") == 0)		// �I�u�W�F�N�g�^�C�v�擾
				{
					fscanf(pFile, "%*s%d", &nObjType);
				}

				if (strcmp(aStr[1], "POS") == 0)		// �ʒu�擾
				{
					fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
				}

				if (strcmp(aStr[1], "SIZE") == 0)		// �T�C�Y�擾
				{
					fscanf(pFile, "%*s%f %f %f", &size.x, &size.y, &size.z);
				}

				if (strcmp(aStr[1], "END_OBJECT_SET") == 0)
				{
					CJumpCircle::Create(pos, size, static_cast<CJumpCircle::CIRCLE_TYPE>(nObjType));	// �W�����v�T�[�N���̐���
					break;
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