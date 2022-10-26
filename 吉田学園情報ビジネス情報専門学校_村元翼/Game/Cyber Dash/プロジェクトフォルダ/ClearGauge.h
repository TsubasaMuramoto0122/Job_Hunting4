//============================================================
// �N���A�Q�[�W�̍쐬
// Author:������
//============================================================
#ifndef _CLEARGAUGE_H_
#define _CLEARGAUGE_H_

//============================================================
// �C���N���[�h�t�@�C��
//============================================================
#include "main.h"
#include "scene.h"

//============================================================
// �}�N����`
//============================================================
#define MAX_PERCENT_DIGIT	(3)												// �p�[�Z���g�����̍ő吔
#define GAUGE_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f,0.0f))		// �Q�[�W�̈ʒu
#define GAUGE_SIZE			(D3DXVECTOR3(40.0f,20.0f,0.0f))					// �Q�[�W�̑傫��

//============================================================
// �O���錾
//============================================================
class CNumber;
class CUi;

//============================================================
// �N���X�̐���
//============================================================
class CClearGauge : public CScene
{
public:
	//--------------------------------
	// �\����
	//--------------------------------
	typedef struct
	{
		CUi				*pFrame;							// �Q�[�W�̃t���[��UI
		CUi				*pGauge;							// �Q�[�W��UI
		D3DXVECTOR3		posFrame;							// �Q�[�W�t���[���̈ʒu
		D3DXVECTOR3		posGauge;							// �Q�[�W�̈ʒu
		D3DXVECTOR3		size;								// �Q�[�W�̑傫��
		float			MaxLength;							// �Q�[�W�̍ő�̒���

	}GAUGE_UI_INFO;


	CClearGauge(OBJTYPE nPriority = OBJTYPE_UI);		// �R���X�g���N�^
	~CClearGauge();										// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �I�u�W�F�N�g�̐���
	static CClearGauge *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// �擾�֐�
	static bool GetClear() { return m_bClear; }

	// �p�[�Z���g�v�Z�֐�
	void SetPercent(void);	

private:
	D3DXVECTOR3		m_pos;								// �ʒu
	D3DXVECTOR3		m_size;								// �T�C�Y
	D3DXVECTOR3		m_ClearLength;						// �N���A�܂ł̋���
	int				m_nPercent;							// �N���A�p�[�Z���g�̐���
	CNumber			*m_apPercent[MAX_PERCENT_DIGIT];	// �N���A�p�[�Z���g�̃|���S��
	GAUGE_UI_INFO	m_GaugeUI;							// �Q�[�WUI
	static bool		m_bClear;							// �N���A�܂ŃQ�[�W���B������
};
#endif