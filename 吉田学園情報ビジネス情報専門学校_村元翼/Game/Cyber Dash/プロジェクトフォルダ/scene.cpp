//========================================================================
// scene.cpp�̍쐬
// Author:������
//========================================================================
#include "scene.h"
#include "renderer.h"
#include "manager.h"

//========================================================================
// �ÓI�����o�ϐ��̏�����
//========================================================================
CScene *CScene::m_pCur[OBJTYPE_MAX] = {};		// �Ō�̃I�u�W�F�N�g
CScene *CScene::m_pTop[OBJTYPE_MAX] = {};		// �ŏ��̃I�u�W�F�N�g
int CScene::m_nNumAll[OBJTYPE_MAX] = {};		// �e��I�u�W�F�N�g��

//========================================================================
// �R���X�g���N�^
//========================================================================
CScene::CScene()
{

}

//========================================================================
// �I�[�o�[���[�h�R���X�g���N�^
//========================================================================
CScene::CScene(OBJTYPE nPriority)
{
	// 1�T��
	if (!m_pTop[nPriority] && !m_pCur[nPriority])
	{
		m_pTop[nPriority] = this;	// this�͍ŏ��̃I�u�W�F�N�g�ł���
		m_pCur[nPriority] = this;	// this�͍Ō�̃I�u�W�F�N�g�ł���
		m_pNext = nullptr;
		m_pPrev = nullptr;
	}

	// 2�T�ڈȍ~
	else
	{
		m_pCur[nPriority]->m_pNext = this;		// ���݂̃I�u�W�F�N�g(A)���猩�����̃I�u�W�F�N�g(B)�͎����ł���
		this->m_pPrev = m_pCur[nPriority];		// �����̈�O(B����݂�A)��m_pCur�ł���
		m_pCur[nPriority] = this;				// ���݂�A��B�ł���
		this->m_pNext = nullptr;				// ���͖���
	}

	m_nNumAll[nPriority]++;						// �����J�E���g
	m_nPriority = nPriority;					// �D�揇�ʂ̒l����
	m_objType = nPriority;						// �I�u�W�F�N�g�^�C�v�̑��

	//------------------------------------------
	// �ϐ�������
	//------------------------------------------
	m_pos		= { 0.0f,0.0f,0.0f };			// �ʒu
	m_posOld	= { 0.0f,0.0f,0.0f };			// �Ō�ɍX�V�����ʒu
	m_size		= { 0.0f,0.0f,0.0f };			// �傫��
	m_col		= { 0.0f,0.0f,0.0f,0.0f };		// �J���[
	m_Tex		= { 0.0f,0.0f };				// UV���W
	m_bDeath	= false;						// ���S����

}

//========================================================================
// �f�X�g���N�^(�ʂ̃I�u�W�F�N�g�̔j��)
//========================================================================
CScene::~CScene()
{
	// �������I�u�W�F�N�g���ŏ����Ō�̃I�u�W�F�N�g��������(�I�u�W�F�N�g��1�݂̂�������)
	if (this == m_pTop[m_nPriority] && this == m_pCur[m_nPriority])
	{
		// �S�ċ�ɂ���
		m_pTop[m_nPriority] = nullptr;
		m_pCur[m_nPriority] = nullptr;
		m_pNext = nullptr;
		m_pPrev = nullptr;
	}

	else if (this == m_pTop[m_nPriority])
	{
		m_pNext->m_pPrev = nullptr;		// ���̃I�u�W�F�N�g�����O�̃I�u�W�F�N�g������ɂ���
		m_pTop[m_nPriority] = m_pNext;	// �ŏ��̃I�u�W�F�N�g�������̃I�u�W�F�N�g�ɂ��炷

	}
	else if (this == m_pCur[m_nPriority])
	{
		m_pPrev->m_pNext = nullptr;
		m_pCur[m_nPriority] = m_pPrev;

	}
	else
	{
		
		m_pPrev->m_pNext = m_pNext;	// �����̑O�̃I�u�W�F�N�g�������Ă��鎟�̃I�u�W�F�N�g���������̎��̃I�u�W�F�N�g���ɂ���
		m_pNext->m_pPrev = m_pPrev;	// �����̎��̃I�u�W�F�N�g�������Ă���O�̃I�u�W�F�N�g���������̑O�̃I�u�W�F�N�g���ɂ���
	}

	// �I�u�W�F�N�g���̃J�E���g�����炷
	m_nNumAll[m_nPriority]--;
}


//========================================================================
// �S�ẴI�u�W�F�N�g�j��
//========================================================================
void CScene::ReleaseAll()
{	
	CScene *pScene = nullptr;		// �����I�u�W�F�N�g
	CScene *pSaveScene = nullptr;	// �ێ��p�I�u�W�F�N�g

	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		// �t�F�[�h�̂ݑS���[�h�Ŏg���̂ŏ����Ȃ�
		if (nObj != OBJTYPE_FADE)
		{
			pScene = m_pTop[nObj];

			// �Ō�̃I�u�W�F�N�g�̎���Next��NULL�łȂ�������
			// �������[�v
			while (pScene)
			{
				// �g�b�v�̎��̃I�u�W�F�N�g���i�[
				pSaveScene = pScene->m_pNext;

				// �g�b�v�̏�������
				pScene->Uninit();
				if (pScene->m_bDeath)
				{
					delete pScene;
					pScene = nullptr;
				}

				//�g�b�v�̏������̃I�u�W�F�N�g�ɂ���
				pScene = pSaveScene;
			}
		}
	}
}

//========================================================================
// �S�ẴI�u�W�F�N�g�X�V
//========================================================================
void CScene::UpdateAll()
{
	CScene *pNextUpdate = nullptr;									// �X�V�����p�I�u�W�F�N�g

		for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
		{
			if (!CManager::GetInstance()->GetStop() && !CManager::GetInstance()->GetPause())					// �|�[�Y��Ԃ�false�Ȃ�
			{
				if (m_pTop[nObj])
				{
					pNextUpdate = m_pTop[nObj];						// ���ɍX�V����I�u�W�F�N�g��ۑ�����ϐ�

					while (pNextUpdate)
					{
						pNextUpdate->Update();
						pNextUpdate = pNextUpdate->m_pNext;
					}
				}
			}

			// �|�[�Y���ɍX�V����I�u�W�F�N�g
			else
			{
				if (nObj == OBJTYPE_PAUSE || nObj == OBJTYPE_UI || nObj == OBJTYPE_FADE)	// �I�u�W�F�N�g���t�F�[�horUIor�|�[�Y�Ȃ�
				{
					if (m_pTop[nObj])
					{
						CScene *m_pUpdate = m_pTop[nObj];					// ���ɍX�V����I�u�W�F�N�g�̊i�[

						while (m_pUpdate)									// m_pUpdate��NULL������܂�
						{
							if (!m_pUpdate->m_bDeath)
							{
								m_pUpdate->Update();
							}

							m_pUpdate = m_pUpdate->m_pNext;
						}
					}
				}
			}
		}

	CScene *pSave = nullptr;	//	�ۑ��p�I�u�W�F�N�g

		// �I�u�W�F�N�g�����S���Ă��邩�̔���
		for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
		{
			if (m_pTop[nObj])
			{
				pNextUpdate = m_pTop[nObj];

				while (pNextUpdate)
				{
					// ���̃f�[�^��ۑ�
					pSave = pNextUpdate->m_pNext;

					// ���S����
					if (pNextUpdate->m_bDeath)
					{
						delete pNextUpdate;
						pNextUpdate = nullptr;
					}

					// ���̃I�u�W�F�N�g�ɂ���
					pNextUpdate = pSave;
				}
			}
		}
}

//========================================================================
// �S�Ă̕`��
//========================================================================
void CScene::DrawAll()
{
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		if (m_pTop[nObj])
		{
			// ���ɕ`�悷��I�u�W�F�N�g�̊i�[
			CScene *pNextDraw = m_pTop[nObj];

			while (pNextDraw)
			{
				if (!pNextDraw->m_bDeath)
				{
					pNextDraw->Draw();
				}
				pNextDraw = pNextDraw->m_pNext;
			}
		}
	}
}

//========================================================================
// ���g�̔j��
//========================================================================
void CScene::Release()
{
	// ���S�t���O��true�ɂ���
	m_bDeath = true;
}