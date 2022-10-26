//===========================================
// �����L���O����
//===========================================
#ifndef _RANKING_H_
#define _RANKING_H_
#include "scene.h"
#include "scene2D.h"
#include "number.h"
#include "score.h"

//================================================
// �O���錾
//================================================
class CScene2D;

//================================================
// �}�N����`
//================================================
#define MAX_RANKING					(5)									// �����L���O�̍ő吔
#define MAX_SCORE_DATA				(MAX_RANKING + 1)					// �X�R�A�f�[�^�̍ő吔
#define RANKING_POLYGON_WIDTH		(30.0f * 1.4f)						// �|���S���̕�
#define RANKING_POLYGON_HEIGHT		(30.0f * 1.2f)						// �|���S���̍���
#define RANKING_RANK_POLYGON_SIZE	(D3DXVECTOR3(225.0f,90.0f,0.0f))	// �����N�̃T�C�Y
#define RANKING_RANK_POLYGON_POS_X	(420.0f)							// �����N��X���W
#define RANKING_MOVE_X				(-30.0f)							// �����L���O�ɏo��UI�̈ړ���
#define RANKING_TEXT				("data/TEXT/ranking.txt")			// �����L���O�e�L�X�g

//================================================
// �N���X�̒�`
//================================================
class CRanking : public CScene
{
public:
	CRanking();											// �R���X�g���N�^
	~CRanking();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �����֐�
	static CRanking *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void DeleteRanking(void);											// �����L���O�폜
	void AscendingSort(void);											// �����\�[�g
	void DescendingSort(void);											// �~���\�[�g

private:
	static CScore *m_apScore[MAX_RANKING];	// �X�R�A�N���X
	CScene2D *m_apScene2D[MAX_RANKING];		// �V�[��2D�N���X
	int m_nScore[MAX_SCORE_DATA];			// �X�R�A
	int m_nNowScore;						// ����̃X�R�A
	D3DCOLORVALUE	m_col;					// �J���[
};

#endif
