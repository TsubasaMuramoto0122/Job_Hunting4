//================================================
// �����L���O����
// Author:������
//================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "renderer.h"
#include "score.h"
#include "manager.h"
#include "ranking.h"
#include "playdata.h"
#include "number.h"

//================================================
// �ÓI�����o�ϐ��錾
//================================================
CScore *CRanking::m_apScore[MAX_RANKING] = {};

//================================================
// �R���X�g���N�^
//================================================
CRanking::CRanking()
{
	// �e��ϐ�������
	for (int nCnt = 0; nCnt < MAX_SCORE_DATA; nCnt++)
	{
		m_nScore[nCnt] = 0;				// �X�R�A�̏�����
	}
	m_nNowScore = 0;					// ���݂̃X�R�A
	m_col = { 0.0f,0.0f,0.0f,0.0f };	// �J���[

}

//================================================
// �f�X�g���N�^
//================================================
CRanking::~CRanking()
{

}

//================================================
// ����������
//================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTexture *pTex = CManager::GetInstance()->GetTexture();
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// �X�R�A�̐���
		m_apScore[nCnt] = CScore::Create
		(
			D3DXVECTOR3
			(
				pos.x,
				pos.y + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)) * nCnt + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)),
				0.0f
			),
			size
		);

		// �����N����
		m_apScene2D[nCnt] = CScene2D::Create
		(
			D3DXVECTOR3
			(
				RANKING_RANK_POLYGON_POS_X,
				pos.y + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)) * nCnt + (float)(SCREEN_HEIGHT / (MAX_RANKING + 1)),
				0.0f
			),
			RANKING_RANK_POLYGON_SIZE
		);

		// ���ʃe�N�X�`��
		switch (nCnt)
		{
		case 0:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_1ST"));
			break;
		case 1:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_2ND"));
			break;
		case 2:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_3RD"));
			break;
		case 3:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_4TH"));
			break;
		case 4:
			m_apScene2D[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_5TH"));
			break;
		}
	}

	// ���[�h����
	FILE *pFile;
	if (pFile = fopen(RANKING_TEXT, "r"))	// �t�@�C�����J��
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fscanf(pFile, "%d", &m_nScore[nCnt]);
		}
	}
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}
	fclose(pFile);	// �t�@�C�������

	// �v���C�f�[�^�̎擾
	CPlayData *pPlayData = nullptr;
	pPlayData = CManager::GetInstance()->GetPlayData();

	// �v���C�f�[�^�ɕۑ����Ă��鍡��̃X�R�A��ϐ��ɕۑ�
	m_nScore[MAX_SCORE_DATA - 1] = pPlayData->GetScore();
	m_nNowScore = pPlayData->GetScore();

	// �\�[�g
	for (int nCnt = 0; nCnt < MAX_SCORE_DATA; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < MAX_SCORE_DATA - 1; nCnt2++)
		{
			// �����ŕ��ׂ�
			if ((m_nScore[nCnt2] < m_nScore[nCnt2 + 1] && m_nScore[nCnt2 + 1] != 0) || m_nScore[nCnt2] == 0)
			{
				int nScore = m_nScore[nCnt2];
				m_nScore[nCnt2] = m_nScore[nCnt2 + 1];
				m_nScore[nCnt2 + 1] = nScore;
			}
		}
	}

	// �X�R�A�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_apScore[nCnt]->SetScore(m_nScore[nCnt]);
	}

	// �Z�[�u����
	if (pFile = fopen(RANKING_TEXT, "w"))	// �t�@�C�����J��
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fprintf(pFile, "%d\n", m_apScore[nCnt]->GetScore());
		}
	}
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}

	fclose(pFile);	// �t�@�C�������

	//�J���[�̐ݒ�
	m_col = { 255.0f ,0.0f,0.0f,255.0f };

	return S_OK;
}

//================================================
// �I������
//================================================
void CRanking::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// �V�[��2D�̔j��
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = nullptr;
		}

		// �X�R�A�̔j��
		if (m_apScore[nCnt])
		{
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = nullptr;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//================================================
// �X�V����
//================================================
void CRanking::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// �X�R�A�̈ʒu�擾����
		D3DXVECTOR3 posScore = m_apScore[nCnt]->GetPos();

		if (posScore.x > SCREEN_WIDTH / 2.0f)
		{
			if (nCnt > 0)
			{
				// �X�R�A�̈ʒu�擾����
				D3DXVECTOR3 posScorePrev = m_apScore[nCnt - 1]->GetPos();
				if (posScorePrev.x <= SCREEN_WIDTH / 2.0f)
				{
					posScore.x += RANKING_MOVE_X;
				}
			}
			else
			{
				posScore.x += RANKING_MOVE_X;
			}
			if (posScore.x <= SCREEN_WIDTH / 2.0f)
			{
				posScore.x = SCREEN_WIDTH / 2.0f;
			}

			// �ʒu�ݒ�
			//m_apScore[nCnt]->SetScorePos(posScore);
		}
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (m_nNowScore != 0 && m_nScore[nCnt] == m_nNowScore)
		{
			//----------------------------------
			// �_�ŏ���
			//----------------------------------
			m_col.a -= 255;
			if (m_col.r == 255)
			{
				m_col.r = 0;
				m_col.g = 255;
				m_col.b = 0;
			}
			else if (m_col.g == 255)
			{
				m_col.r = 0;
				m_col.g = 0;
				m_col.b = 255;
			}
			else if (m_col.b == 255)
			{
				m_col.r = 255;
				m_col.g = 0;
				m_col.b = 0;
			}

			if (m_col.a < 0)
			{
				m_col.a = 255;
			}

			for (int nCntNum = 0; nCntNum < MAX_SCORE; nCntNum++)
			{
				// �i���o�[���擾���ăJ���[��ݒ�
				m_apScore[nCnt]->GetNumber(nCntNum)->SetCol(m_col);
			}
			break;
		}
	}
}

//================================================
// �`�揈��
//================================================
void CRanking::Draw(void)
{

}

//================================================
// ��������
//================================================
CRanking* CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �C���X�^���X�̐���
	CRanking *pRanking = nullptr;
	if (!pRanking)
	{
		pRanking = new CRanking;
		if (pRanking)
		{
			pRanking->Init(pos, size);	// ������
		}
	}
	return pRanking;
}

//================================================
// �����L���O�폜
//================================================
void CRanking::DeleteRanking(void)
{
	// �Z�[�u����
	FILE *pFile;
	if (pFile = fopen(RANKING_TEXT, "w"))	// �t�@�C�����J��
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			m_apScore[nCnt]->SetScore(0);							// �X�R�A��0�ɂ���
			fprintf(pFile, "%d\n", m_apScore[nCnt]->GetScore());	// 0�ɂ����X�R�A��ۑ�
		}
	}
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}

	fclose(pFile);		// �t�@�C�������
}