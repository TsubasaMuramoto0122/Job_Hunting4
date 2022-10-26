//=========================================================
// �X�e�[�W�����ʂ̏���
// Author:������
//=========================================================
#include "game.h"
#include "fade.h"
#include "manager.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshsphere.h"
#include "timercount.h"
#include "Scene3D.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "LoadX.h"
#include "Ui.h"
#include "mouse.h"
#include "StringManager.h"
#include "model_spawner.h"
#include "model.h"

//=========================================================
// �}�N����`
//=========================================================
#define COUNT_DOWN_FRAME	(60)										// �J�E���g�_�E���̃t���[��
#define SKY_SIZE			(D3DXVECTOR3(10000.0f,10000.0f,10000.0f))	// ��T�C�Y
#define UI_SIZE				(D3DXVECTOR3(1.0f,1.0f,0.0f))				// UI�T�C�Y
#define NUMBER_SIZE			(D3DXVECTOR3(50.0f, 30.0f, 0.0f))			// �X�R�A�T�C�Y
#define DELETE_FRAME		(90)										// ���b�Z�[�W��������܂ł̃t���[��
#define GAME_TIME			(60)										// �Q�[���^�C��
#define FILENAME			("data/SAVE/SaveData000.txt")				// �ǂݍ��݃e�L�X�g
#define PREVIEW_COLOR		(D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))			// �v���r���[�J���[
#define BASE_FIELD_SIZE		(D3DXVECTOR3(50.0f,0.0f,50.0f))				// �����t�B�[���h�T�C�Y
#define MAX_MULTI			(10)										// �ő�{��
#define MAX_DIGIT			(10000)										// �ő包
#define DIGIT_SHIFT_UP		(10)										// ���オ��
#define DIGIT_SHIFT_DOWN	(0.1f)										// ��������

//=========================================================
// �ÓI�����o�ϐ��̏�����
//=========================================================
int CGame::m_nMultiX = 1;
int CGame::m_nMultiZ = 1;
float CGame::m_fEvenX = 0.0f;
float CGame::m_fEvenZ = 0.0f;
bool CGame::m_bInputMode = false;

//=========================================================
// �R���X�g���N�^
//=========================================================
CGame::CGame()
{
	CLoadX *pLoadX = CManager::GetInstance()->GetLoadX();	// ���f��X�t�@�C���N���X�̎擾
	m_nSelectMax = pLoadX->GetNumAll();						// �ő�I��
	m_nSelect = 0;											// �I��ԍ�
	m_nObjType = 0;											// �I�u�W�F�N�g�^�C�v�ԍ�
	m_nPosMode = INPUT_POSX;								// ���͂�����W�̑I��ԍ�
	m_Inputpos = { 0.0f,0.0f,0.0f };						// ���͂�����W		
	m_nFrame = 0;											// �t���[��
	m_pUI = nullptr;										// UI�N���X
	m_pString = nullptr;									// ������N���X
	m_pField = nullptr;										// ���b�V���t�B�[���h�N���X
	m_pModelPrev = nullptr;									// ���f���v���r���[�N���X
	m_pFieldPrev = nullptr;									// ���b�V���t�B�[���h�v���r���[�N���X
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CGame::~CGame()
{

}

//=========================================================
// ������
//=========================================================
HRESULT CGame::Init(void)
{
	// ���b�V���t�B�[���h
	m_pField = CMeshField::Create({ 0.0f,-100.0f,0.0f }, { 10000.0f,0.0f,10000.0f }, { 0.0f,0.0f,0.0f }, 10, 10);
	m_pField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));

	// �v���r���[����
	ObjCreate(m_nObjType);

	return S_OK;
}

//=========================================================
// �I��
//=========================================================
void CGame::Uninit(void)
{
	if (!m_vModel.empty())
	{
		// �i�[����Ă��郂�f���N���X��S�Ĕj��
		for (CModel_Spawner *pDeleteModel : m_vModel)
		{
			if (pDeleteModel)
			{
				pDeleteModel->Uninit();
				pDeleteModel = nullptr;
			}
		}

		// ���f���N���X�R���e�i��j��
		m_vModel.clear();
	}

	if (!m_vMeshField.empty())
	{
		// �R���e�i�Ɋi�[����Ă��郁�b�V���t�B�[���h�N���X��S�Ĕj��
		for (CMeshField *pDeleteField : m_vMeshField)
		{
			if (pDeleteField)
			{
				pDeleteField->Uninit();
				pDeleteField = nullptr;
			}
		}

		// ���b�V���t�B�[���h�N���X�R���e�i��j��
		m_vMeshField.clear();
	}

}

//=========================================================
// �X�V
//=========================================================
void CGame::Update(void)
{
	//-----------------------------------------------
	// �N���X�擾
	//-----------------------------------------------
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CMouse *pMouse = CManager::GetInstance()->GetMouse();
	CCamera *pCamera = CManager::GetInstance()->GetCamera(0);

	// �������̍��W
	D3DXVECTOR3 CreatePos = { pCamera->GetPosR().x + m_fEvenX, pCamera->GetPosR().y,pCamera->GetPosR().z + m_fEvenZ };

	// ���̓��[�h����
	if (!m_bInputMode)
	{
		//-----------------------------------------------
		// �v���r���[�I�u�W�F�N�g�̈ړ�
		//-----------------------------------------------
		switch (m_nObjType)
		{
		case OBJ_MODEL:
			if (m_pModelPrev)
			{
				m_pModelPrev->SetPos(pCamera->GetPosR());
			}
			break;

		case OBJ_MESHFIELD:
			if (m_pFieldPrev)
			{
				m_pFieldPrev->CScene::SetPos(CreatePos);
			}
			break;
		}

		//-----------------------------------------------
		// �I�u�W�F�N�g�̎�ސ؂�ւ�
		//-----------------------------------------------
		if (pKey->GetTrigger(DIK_1) || pKey->GetTrigger(DIK_2))
		{
			// �ϐ����Z�b�g
			m_fEvenX = 0.0f;
			m_fEvenZ = 0.0f;
			m_nMultiX = 1;
			m_nMultiZ = 1;

			ObjDelete();				// �v���r���[�j��

			SelectLoop(m_nObjType, 0, OBJ_MAX - 1, DIK_1, DIK_2);

			ObjCreate(m_nObjType);		// �v���r���[����
		}

		//-----------------------------------------------
		// �I�u�W�F�N�g�^�C�v�ʂ̐؂�ւ�����
		//-----------------------------------------------
		switch (m_nObjType)
		{
			//---------------------------------
			// ���f���؂�ւ�
			//---------------------------------
		case OBJ_MODEL:
			if (pKey->GetTrigger(DIK_C) || pKey->GetTrigger(DIK_Z))
			{
				ObjDelete();			// �I�u�W�F�N�g�j��

				SelectLoop(m_nSelect, 0, m_nSelectMax - 1, DIK_Z, DIK_C);

				ObjCreate(m_nObjType);	// �I�u�W�F�N�g����
			}
			break;

			//---------------------------------
			// ���b�V���t�B�[���h�̃T�C�Y�ύX
			//---------------------------------
		case OBJ_MESHFIELD:
			if (pKey->GetTrigger(DIK_O) || pKey->GetTrigger(DIK_L) || pKey->GetTrigger(DIK_I) || pKey->GetTrigger(DIK_K))
			{
				ObjDelete();			// �I�u�W�F�N�g�j��

				SelectLoop(m_nMultiX, 1, MAX_MULTI, DIK_I, DIK_K);
				SelectLoop(m_nMultiZ, 1, MAX_MULTI, DIK_O, DIK_L);

				ObjCreate(m_nObjType);	// �I�u�W�F�N�g����

				// ���b�V�������Q�{���Ȃ�}�X�ڂɍ��W�����킹��ϐ������
				if (m_pFieldPrev)
				{
					MatchSquare(m_fEvenX, m_nMultiX, BASE_FIELD_SIZE.x / 2);
					MatchSquare(m_fEvenZ, m_nMultiZ, BASE_FIELD_SIZE.z / 2);
				}
			}
			break;
		}

		//-----------------------------------------------
		// ���N���b�NorSPACE�L�[�Ő���
		//-----------------------------------------------
		if (pMouse->GetTrigger(CMouse::MOUSE_LEFT) || pKey->GetTrigger(DIK_SPACE))
		{
			// ���[�J���N���X
			CModel_Spawner	*pModel = nullptr;
			CMeshField		*pField = nullptr;

			// �^�C�v�ʃN���X�I�u�W�F�N�g����
			switch (m_nObjType)
			{
			case OBJ_MODEL:
				pModel = CModel_Spawner::Create(CreatePos, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_nSelect);
				m_vModel.emplace_back(pModel);
				break;

			case OBJ_MESHFIELD:
				pField = CMeshField::Create(CreatePos, { BASE_FIELD_SIZE.x * m_nMultiX,0.0f,BASE_FIELD_SIZE.z * m_nMultiZ }, { 0.0f,0.0f,0.0f }, m_nMultiX, m_nMultiZ);
				m_vMeshField.emplace_back(pField);
				break;
			}
		}

		//-----------------------------------------------
		// BackSpace�ōŌ�ɐ����������f�����폜
		//-----------------------------------------------
		if (pKey->GetTrigger(DIK_BACKSPACE))
		{
			switch (m_nObjType)
			{
			case OBJ_MODEL:
				if (!m_vModel.empty())
				{
					CModel_Spawner *pModelSpn = m_vModel.back();
					if (pModelSpn)
					{
						pModelSpn->Uninit();
						pModelSpn = nullptr;
					}

					// ���f���R���e�i�̍Ō�̗v�f��j��
					m_vModel.pop_back();
				}
				break;

			case OBJ_MESHFIELD:
				if (!m_vMeshField.empty())
				{
					CMeshField *pMeshField = m_vMeshField.back();
					if (pMeshField)
					{
						pMeshField->Uninit();
						pMeshField = nullptr;
					}

					// ���b�V���t�B�[���h�R���e�i�̍Ō�̗v�f��j��
					m_vMeshField.pop_back();
				}
				break;
			}
		}

		//-----------------------------------------------
		// �Z�[�u
		//-----------------------------------------------
		if (pKey->GetTrigger(DIK_F3))
		{
			if (Save(FILENAME))
			{
				// �Z�[�u�������b�Z�[�W�̐���
				SetMessage("�Z�[�u����");
			}
			else
			{
				exit(EXIT_FAILURE);	// �ُ�I��
			}
		}

		//-----------------------------------------------
		// ���[�h
		//-----------------------------------------------
		if (pKey->GetTrigger(DIK_F4))
		{
			if (Load(FILENAME))
			{
				// ���[�h�������b�Z�[�W�̐���
				SetMessage("���[�h����");
			}
		}

		// ���b�Z�[�W�\�����̏���
		if (m_pString)
		{
			m_nFrame++;	// �t���[���J�E���g

			// ���b�Z�[�W�폜
			if (m_nFrame > DELETE_FRAME)
			{
				m_pString->Uninit();
				delete m_pString;
				m_pString = nullptr;

				m_nFrame = 0;
			}
		}
	}

	// ���̓��[�h�Ȃ�
	else
	{
		SelectLoop(m_nPosMode, 0, INPUT_MAX - 1, DIK_LEFT, DIK_RIGHT);

		// ���[�h�ʂɍ��W���͂��s��
		switch (m_nPosMode)
		{
		case INPUT_POSX:
			InputPos(m_Inputpos.x);
			break;

		case INPUT_POSY:
			InputPos(m_Inputpos.y);
			break;

		case INPUT_POSZ:
			InputPos(m_Inputpos.z);
			break;
		}
	}

	if (pKey->GetTrigger(DIK_F1))
	{
		// ���b�Z�[�W��\��
		if (!m_pString)
		{
			m_pString = CStringManager::Create({ SCREEN_WIDTH / 3,SCREEN_HEIGHT / 2, 0.0f }, 64, 64, "���W����͂��Ă�������", "�l�r�S�V�b�N");

			// ���͈ʒu���Z�b�g
			m_Inputpos = { 0.0f,0.0f,0.0f };
		}
		else
		{
			m_pString->Uninit();
			delete m_pString;
			m_pString = nullptr;
		}

		// ���W���̓��[�h��؂�ւ���
		m_bInputMode = !m_bInputMode;
	}
}

//=========================================================
// �`��
//=========================================================
void CGame::Draw(void)
{

}

//=========================================================
// ���b�Z�[�W�̐ݒ�(����)
//=========================================================
void CGame::SetMessage(const char* sMessage)
{
	// ���b�Z�[�W�����ɂ���Ȃ��ɔj��
	if (m_pString)
	{
		m_pString->Uninit();
		delete m_pString;
		m_pString = nullptr;
	}

	if (!m_pString)
	{
		m_pString = CStringManager::Create({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0.0f }, 64, 64, sMessage, "�l�r�S�V�b�N");
	}
}

//=========================================================
// �}�X�ڂɍ��킹��֐�
//=========================================================
void CGame::MatchSquare(float &fEven,const int &nMulti, const float &fShift)
{
	// �|����ϐ���2�{���Ȃ�
	if (nMulti % 2 == 0)
	{
		fEven = fShift;
	}

	// �����łȂ��Ȃ�
	else
	{
		fEven = 0.0f;
	}
}

//=========================================================
// �I�����̐؂�ւ�(���[�v)
//=========================================================
void CGame::SelectLoop(int &nNow, const int &nMin, const int &nMax, const int &nKey1, const int nKey2)
{
	// �L�[�{�[�h�N���X�擾
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

	// �߂�
	if (pKey->GetTrigger(nKey1))
	{
		nNow--;
		if (nNow < nMin)
		{
			nNow = nMax;
		}
	}

	// �i�߂�
	else if (pKey->GetTrigger(nKey2))
	{
		nNow++;
		if (nNow > nMax)
		{
			nNow = nMin;
		}
	}
}

//=========================================================
// ���W�w��
//=========================================================
void CGame::InputPos(float &fpos)
{
	// �L�[�{�[�h�N���X�擾
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

	int nNum = 0;
	if (pKey->GetTrigger(DIK_1) || pKey->GetTrigger(DIK_2) || pKey->GetTrigger(DIK_3) || pKey->GetTrigger(DIK_4) || pKey->GetTrigger(DIK_5)||
		pKey->GetTrigger(DIK_6) || pKey->GetTrigger(DIK_7) || pKey->GetTrigger(DIK_8) || pKey->GetTrigger(DIK_9) || pKey->GetTrigger(DIK_0))
	{
		if (pKey->GetTrigger(DIK_1))
		{
			nNum = 1;
		}
		else if (pKey->GetTrigger(DIK_2))
		{
			nNum = 2;
		}
		else if (pKey->GetTrigger(DIK_3))
		{
			nNum = 3;
		}
		else if (pKey->GetTrigger(DIK_4))
		{
			nNum = 4;
		}
		else if (pKey->GetTrigger(DIK_5))
		{
			nNum = 5;
		}
		else if (pKey->GetTrigger(DIK_6))
		{
			nNum = 6;
		}
		else if (pKey->GetTrigger(DIK_7))
		{
			nNum = 7;
		}
		else if (pKey->GetTrigger(DIK_8))
		{
			nNum = 8;
		}
		else if (pKey->GetTrigger(DIK_9))
		{
			nNum = 9;
		}
		else if (pKey->GetTrigger(DIK_0))
		{
			nNum = 0;
		}

		if (fpos < MAX_DIGIT)
		{
			fpos *= DIGIT_SHIFT_UP;	// �������炷
			fpos += nNum;			// �ŏ����ɓ��͂�������������
		}
	}

	// �ŏ��������
	if (pKey->GetTrigger(DIK_BACKSPACE))
	{
		if (fpos > 0.0f)
		{
			fpos *= DIGIT_SHIFT_DOWN;
			fpos = floorf(fpos);	// �����_�ȉ��؂�̂�
		}
	}

	// ���͂������W�ɃJ�����̈ʒu�����킹��
	if (pKey->GetTrigger(DIK_RETURN))
	{
		CManager::GetInstance()->GetCamera(0)->SetPosR(m_Inputpos);

		// ���b�Z�[�W�j��
		if (m_pString)
		{
			m_pString->Uninit();
			delete m_pString;
			m_pString = nullptr;
		}

		// ���̓��[�h���I������
		m_bInputMode = false;
	}
}

//=========================================================
// UI�����֐�
//=========================================================
void CGame::SetUiCreate(const char* TexStr, float fSizeMulti)
{
	// �e�N�X�`���N���X�擾
	CTexture *pTex = CManager::GetInstance()->GetTexture();

	// UI�폜
	SetUiDelete();

	// UI����
	if (!m_pUI)
	{
		m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * fSizeMulti,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
		m_pUI->BindTexture(pTex->GetTexture(TexStr));
	}
}

//=========================================================
// UI�폜�֐�
//=========================================================
void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		m_pUI = nullptr;
	}
}

//=========================================================
// �I�u�W�F�N�g�폜
//=========================================================
void CGame::ObjDelete(void)
{
	if (m_pFieldPrev)
	{
		m_pFieldPrev->Uninit();
		m_pFieldPrev = nullptr;
	}

	if (m_pModelPrev)
	{
		m_pModelPrev->Uninit();
		m_pModelPrev = nullptr;
	}
}

//=========================================================
// �I�u�W�F�N�g����
//=========================================================
void CGame::ObjCreate(int nObjtype)
{
	// �J�����擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera(0);

	// �^�C�v�ʃI�u�W�F�N�g����
	switch (nObjtype)
	{
	case OBJ_MODEL:
		if (!m_pModelPrev)
		{
			m_pModelPrev = CModel_Spawner::Create({ pCamera->GetPosR() }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_nSelect);
			m_pModelPrev->SetCol(PREVIEW_COLOR);
		}
		break;

	case OBJ_MESHFIELD:
		if (!m_pFieldPrev)
		{
			m_pFieldPrev = CMeshField::Create({ pCamera->GetPosR() }, { BASE_FIELD_SIZE.x * m_nMultiX,0.0f,BASE_FIELD_SIZE.z * m_nMultiZ }, { 0.0f,0.0f,0.0f }, m_nMultiX, m_nMultiZ);
			m_pFieldPrev->SetCol(PREVIEW_COLOR);
		}
		break;
	}
}


//=========================================================
// �Z�[�u����
//=========================================================
bool CGame::Save(const char* sSaveName)
{
	// �t�@�C���\����
	FILE *pFile = nullptr;

	// �������݃��[�h�Ńt�@�C�����J��
	if (pFile = fopen(sSaveName, "w"))
	{
		fprintf(pFile, "//=============================================//\n");
		fprintf(pFile, "// �X�e�[�W�f�[�^[SaveData000.txt]\n");
		fprintf(pFile, "// Author : ������\n");
		fprintf(pFile, "//=============================================//\n\n");

		// ���f���̔z�u
		if (!m_vModel.empty())
		{
			fprintf(pFile, "//======================================//\n");
			fprintf(pFile, "//			 ���f���z�u���				//\n");
			fprintf(pFile, "//======================================//\n");

			// ���f���̐��������[�v����
			for (CModel_Spawner *pModel : m_vModel)
			{
				fprintf(pFile, "MODELSET\n");
				fprintf(pFile, "	TYPE = %d\n", pModel->GetModel()->GetXType());
				fprintf(pFile, "	POS = %.2f %.2f %.2f\n", pModel->GetPos().x, pModel->GetPos().y, pModel->GetPos().z);
				fprintf(pFile, "	ROT = %.2f %.2f %.2f\n", pModel->GetRot().x, pModel->GetRot().y, pModel->GetRot().z);
				fprintf(pFile, "END_MODELSET\n\n");
			}
		}

		// ���b�V���t�B�[���h�̔z�u
		if (!m_vMeshField.empty())
		{
			fprintf(pFile, "//======================================//\n");
			fprintf(pFile, "//			 ���f���z�u���				//\n");
			fprintf(pFile, "//======================================//\n");

			// ���b�V���t�B�[���h�̐��������[�v����
			for (CMeshField *pMeshField : m_vMeshField)
			{
				fprintf(pFile, "FIELDSET\n");
				fprintf(pFile, "	POS = %.2f %.2f %.2f\n", pMeshField->GetPos().x, pMeshField->GetPos().y, pMeshField->GetPos().z);
				fprintf(pFile, "	ROT = %.2f %.2f %.2f\n", pMeshField->GetRot().x, pMeshField->GetRot().y, pMeshField->GetRot().z);
				fprintf(pFile, "	SIZE = %.2f %.2f %.2f\n", pMeshField->GetSize().x, pMeshField->GetSize().y, pMeshField->GetSize().z);
				fprintf(pFile, "	BLOCK = %d %d\n", pMeshField->GetLine(), pMeshField->GetVertical());
				fprintf(pFile, "END_FIELDSET\n\n");
			}
		}

		fprintf(pFile, "END_SCRIPT");
	}

	// �Z�[�u�̎��s
	else
	{
		return false;
	}

	// �t�@�C�������
	fclose(pFile);

	return true;
}

//=========================================================
// ���[�h����
//=========================================================
bool CGame::Load(const char* sLoadName)
{
	// �t�@�C���\����
	FILE *pFile;

	// �ǂݍ��݃��[�h�Ńt�@�C�����J��
	if (pFile = fopen(sLoadName, "r"))
	{
		// �R���e�i�N���X��j��
		if (!m_vModel.empty() || !m_vMeshField.empty())
		{
			// �I��
			Uninit();
		}

		// NULL�`�F�b�N
		if (m_vModel.empty() || m_vMeshField.empty())
		{
			// �ǂݍ��݂Ɏg���ϐ�
			char aStr[2][64] = { NULL,NULL };

			D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };	// �ʒu
			D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };	// ��]
			D3DXVECTOR3 size = { 0.0f,0.0f,0.0f };	// �T�C�Y
			int nType = 0;							// �^�C�v
			int nLine = 0;							// ��������
			int nVertical = 0;						// �c������

			while (fscanf(pFile, "%s", &aStr[0]))
			{
				// ���f���̓ǂݍ���
				while (strcmp(aStr[0], "MODELSET") == 0)
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
					if (strcmp(aStr[1], "END_MODELSET") == 0)		// �I�u�W�F�N�g�̐���
					{
						CModel_Spawner *pModel = nullptr;
						pModel = CModel_Spawner::Create(pos, rot, { 1.0f,1.0f,1.0f }, nType);
						m_vModel.emplace_back(pModel);
						break;
					}
				}


				// �t�B�[���h�̓ǂݍ���
				while (strcmp(aStr[0], "FIELDSET") == 0)
				{
					fscanf(pFile, "%s", &aStr[1]);					// ������ǂݍ���

					if (strcmp(aStr[1], "POS") == 0)				// �ʒu�擾
					{
						fscanf(pFile, "%*s%f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr[1], "ROT") == 0)				// ��]�擾
					{
						fscanf(pFile, "%*s%f %f %f", &rot.x, &rot.y, &rot.z);
					}
					if (strcmp(aStr[1], "SIZE") == 0)				// �T�C�Y�擾
					{
						fscanf(pFile, "%*s%f %f %f", &size.x, &size.y, &size.z);
					}
					if (strcmp(aStr[1], "BLOCK") == 0)				// ���b�V�����擾
					{
						fscanf(pFile, "%*s%d %d", &nLine, &nVertical);
					}
					if (strcmp(aStr[1], "END_FIELDSET") == 0)		// �I�u�W�F�N�g�̐���
					{
						CMeshField *pField = nullptr;
						pField = CMeshField::Create(pos, size, rot, nLine, nVertical);
						m_vMeshField.emplace_back(pField);
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

	}

	// ���[�h���s
	else
	{
		printf("�t�@�C�����J���܂���");
		return false;
	}

	return true;
}