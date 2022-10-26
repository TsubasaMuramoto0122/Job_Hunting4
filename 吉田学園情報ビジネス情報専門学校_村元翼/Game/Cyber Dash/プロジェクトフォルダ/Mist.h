//========================================
// �~�X�g�̐���
// Author:������
//========================================
#ifndef _MIST_H_
#define _MIST_H_
#include "main.h"
#include "scene.h"
#include <random>

//========================================
// �}�N����`
//========================================
#define DEFAULT_MAX_FRAME (60)		// �f�t�H���g�̍ő�t���[���� 
#define DEFAULT_MAX_COUNT (10)		// �f�t�H���g�̍ő�J�E���g��

//========================================
// �O���錾
//========================================

//========================================
// �N���X�̒�`
//========================================
class CMist : public CScene
{
public:
	CMist(int fMaxFrame,
		  int fMaxCount,
		  OBJTYPE nPriority = OBJTYPE_POLYGON2D);				// �R���X�g���N�^

	~CMist();													// �f�X�g���N�^
	
	HRESULT Init(D3DXVECTOR3 RangeMin, D3DXVECTOR3 RangeMax);	// ������
	void Uninit(void);											// �I��
	void Update(void);											// �X�V
	void Draw(void);											// �`��

	// �����֐�
	static CMist *Create
	(
		D3DXVECTOR3 RangeMin,
		D3DXVECTOR3 RangeMax,
		int nMaxFrame = DEFAULT_MAX_FRAME,
		int nMaxCount = DEFAULT_MAX_COUNT,
		bool bInfinite = false
	);

	// �ݒ�֐�
	void SetInfinite(bool bSwitch) { m_bInfinite = bSwitch; }

private:
	D3DXVECTOR3 m_RangeMin;			// �͈͍ŏ��l
	D3DXVECTOR3 m_RangeMax;			// �͈͍ő�l
	int			m_nFrame;			// �t���[��
	int			m_nCount;			// �J�E���g
	const int	m_nMaxFrame;		// �t���[���ő�l
	const int	m_nMaxCount;		// �J�E���g�ő�l
	bool		m_bInfinite;		// �����ɐ������邩
	random_device m_Randseed;		// ����������
};

#endif _MIST_H_