//===========================================
// �W�����v�T�[�N���̍쐬
// Author;������
//===========================================
#include "JumpCircle.h"
#include "texture.h"
#include "effect.h"
#include "player.h"
#include "InputManager.h"

//===========================================
// �}�N����`
//===========================================
#define JUMP_POW	(15.0f)											// �W�����v��
#define YELLOW		(D3DXCOLOR({ 1.0f,1.0f,1.0f,1.0f }))			// ���F
#define RED			(D3DXCOLOR({ 1.0f,0.0f,0.0f,1.0f }))			// �ԐF
#define GREEN		(D3DXCOLOR({ 0.0f,1.0f,0.0f,1.0f }))			// �ΐF
#define BLACK		(D3DXCOLOR({ 0.0f,0.0f,0.0f,1.0f }))			// ���F

//===========================================
// �ÓI�����o�ϐ��̏�����
//===========================================
float CJumpCircle::m_fJumpPower = JUMP_POW;		// �W�����v�͂�����

//===========================================
// �R���X�g���N�^
//===========================================
CJumpCircle::CJumpCircle(OBJTYPE nPriority) : CBillboard(nPriority)
{

}

//===========================================
// �f�X�g���N�^
//===========================================
CJumpCircle::~CJumpCircle()
{

}

//===========================================
// ����
//===========================================
CJumpCircle *CJumpCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CIRCLE_TYPE type)
{
	CJumpCircle *pJumpCircle = nullptr;
	if (pJumpCircle = new CJumpCircle)
	{
		pJumpCircle->m_type = type;		// �^�C�v
		pJumpCircle->Init(pos, size);	// ������
	}

	return pJumpCircle;
}

//===========================================
// ������
//===========================================
HRESULT CJumpCircle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �r���{�[�h������
	CBillboard::Init(pos, size);
	BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_JUMPCIRCLE"));

	// �^�C�v�ʂɃJ���[��ύX
	switch (m_type)
	{
	case CIRCLE_TYPE::JUMP:
		m_col = YELLOW;
		break;
			
	case CIRCLE_TYPE::QUICK_REV:
		m_col = RED;
		break;

	case CIRCLE_TYPE::SLOW_REV:
		m_col = GREEN;
		break;

	case CIRCLE_TYPE::FALL:
		m_col = BLACK;
		break;
	}

	return S_OK;
}

//===========================================
// �I��
//===========================================
void CJumpCircle::Uninit(void)
{
	CBillboard::Uninit();
}

//===========================================
// �X�V
//===========================================
void CJumpCircle::Update(void)
{
	CBillboard::Update();

	// �G�t�F�N�g����
	CEffect::Create(m_pos, m_size / 2, { 1.0f,1.0f,0.0f,1.0f }, 0.5f, 3);

	// �~�̓����蔻��
	CircleCollision();
}

//===========================================
// �`��
//===========================================
void CJumpCircle::Draw(void)
{
	CBillboard::Draw();
}

//===========================================
// �����蔻��(�~�Ɖ~)
//===========================================
void CJumpCircle::CircleCollision(void)
{
	CScene *pScene = CScene::GetScene(OBJTYPE_PLAYER);

	while (pScene)
	{
		CScene *pSceneNext = CScene::GetSceneNext(pScene);

		D3DXVECTOR3 pos = m_pos;			// ���_
		float Rad = m_size.y ;				// ���a

		D3DXVECTOR3 pos2 = pScene->GetPos();
		float Rad2 = pScene->GetSize().y;
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �~�̒��ɂ��邩�𔻒肷��
		if (powf(Rad + Rad2, 2) >= powf(pos.x - pos2.x, 2) + powf(pos.y - pos2.y, 2))
		{
			if (!pPlayer->GetLock())
			{
				pPlayer->SetLock(true);
			}

			// �W�����v�{�^��������
			if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_SPACE))
			{
				// �W�����v�����Ƃ��̃G�t�F�N�g���o��
				for (int nCnt = 0; nCnt < 5; nCnt++)
				{
					CEffect::Create(m_pos, m_size * 3, { 1.0f,1.0f,1.0f,1.0f }, 1.0f, 3, "TEX_TYPE_JUMPSTAR");
				}

				// ���]����
				if (pPlayer->GetReverse())
				{
					m_fJumpPower = -JUMP_POW;
				}
				else
				{
					m_fJumpPower = JUMP_POW;
				}

				// �^�C�v�ʃW�����v����
				switch (m_type)
				{
				case CIRCLE_TYPE::JUMP:
					Jump(pPlayer);
					break;

				case CIRCLE_TYPE::QUICK_REV:
					Reverse(pPlayer);
					break;

				case CIRCLE_TYPE::SLOW_REV:
					SlowRev(pPlayer);
					break;

				case CIRCLE_TYPE::FALL:
					Fall(pPlayer);
					break;
				}
			}
		}

		else
		{
			if (pPlayer->GetLock())
			{
				pPlayer->SetLock(false);
			}
		}

		pScene = pSceneNext;
	}
}

//===========================================
// �W�����v�̏���
//===========================================
void CJumpCircle::Jump(CPlayer *pPlayer)
{
	// �v���C���[�̏�Ԃ��擾
	CPlayer::PLAYER_STATE State = pPlayer->GetState();
	switch (State)
	{
	case CPlayer::PLAYER_STATE::BALL:
		pPlayer->JumpAction(m_fJumpPower);
		break;

	case CPlayer::PLAYER_STATE::CUBE:
		pPlayer->JumpAction(m_fJumpPower);
		break;

	case CPlayer::PLAYER_STATE::AIRSHIP:
		pPlayer->SetSpeed({ pPlayer->GetSpeed().x,50.0f,0.0f });
		break;

	case CPlayer::PLAYER_STATE::UFO:
		pPlayer->JumpAction(m_fJumpPower);
		break;
	}

}

//===========================================
// �d�͔��]
//===========================================
void CJumpCircle::Reverse(CPlayer *pPlayer)
{
	pPlayer->SetReverse();
	pPlayer->JumpAction(m_fJumpPower);
}

//===========================================
// �������d�͔��]
//===========================================
void CJumpCircle::SlowRev(CPlayer *pPlayer)
{
	pPlayer->SetReverse();
	pPlayer->JumpAction(-m_fJumpPower/2);

}

//===========================================
// �}�ɗ�����
//===========================================
void CJumpCircle::Fall(CPlayer *pPlayer)
{
	pPlayer->JumpAction(-m_fJumpPower);
}