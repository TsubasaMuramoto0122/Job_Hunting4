//===============================================
// �t�F�[�h�p�|���S��
// Author:������
//===============================================
#include "main.h"
#include "scene2D.h"

//===============================================
// �}�N����`
//===============================================
#define INIT_FADE_SIZE (1.5f)	// �����T�C�Y���w�肵�Ȃ������Ƃ��̏����t�F�[�h�T�C�Y

//===============================================
// �N���X�̒�`
//===============================================
class CFadePolygon : public CScene2D
{
public:

	CFadePolygon(float fFadeSize,OBJTYPE nPriority = OBJTYPE_POLYGON2D);	// �R���X�g���N�^
	~CFadePolygon();														// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);						// ������
	void Uninit(void);														// �I��
	void Update(void);														// �X�V
	void Draw(void);														// �`��

	// �I�u�W�F�N�g�����֐�
	static CFadePolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,float fInitFadeSize = INIT_FADE_SIZE);

	// �t�F�[�h�֐�
	void Fade(float& fAlpha);

private:
	const static float	m_fFadeInSpeed;		// �t�F�[�h�C�����x
	const static float	m_fFadeOutSpeed;	// �t�F�[�h�A�E�g���x
	const float			m_fInitFadeSize;	// �����̃t�F�[�h�T�C�Y

	CScene2D		*m_pScene2D;			// 2D�|���S���N���X
	D3DXVECTOR3		m_pos;					// �ʒu
	D3DXVECTOR3		m_size;					// �傫��
	float			m_fAlpha;				// �����x(���l)
	float			m_fFadeSize;			// �t�F�[�h�T�C�Y
	bool			m_bFadeIn;				// �t�F�[�h�C�����s�����̔���

};