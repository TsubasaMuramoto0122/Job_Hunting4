//=====================================================
// �t�F�[�h�̏���
// Author;������
//=====================================================
#ifndef _FADE_H_
#define _FADE_H_

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "texture.h"
#include "manager.h"
#include "scene.h"

//=====================================================
// �}�N����`
//=====================================================
#define FADE_IN_SPEED (0.05f)	// �t�F�[�h�C�����x
#define FADE_OUT_SPEED (0.01f)	// �t�F�[�h�A�E�g���x

//=====================================================
// �O���錾
//=====================================================
class CBg;

//=====================================================
// �t�F�[�h�N���X��`
//=====================================================
class CFade : public CScene
{
public:
	//----------------------------------
	// �t�F�[�h�̏�ԗ񋓌^�\����
	//----------------------------------
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX,
	} FADE;

	CFade(OBJTYPE nPriority = OBJTYPE_FADE);			// �R���X�g���N�^
	~CFade();											// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �ݒ�E�擾�֐�
	static void SetFade(CManager::MODE modeNext);						
	FADE GetFade(void);			

	// �I�u�W�F�N�g��������
	static CFade *Create(CManager::MODE modeNext,
						 D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
						 D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f));						

private:
	static CBg					*m_pBg;			// �w�i�I�u�W�F�N�g
	static FADE					m_fade;			// �t�F�[�h�̏��
	static CManager::MODE		m_modeNext;		// ���̉�ʁi���[�h�j
	static float				m_colorAlpha;	// �t�F�[�h�̐F

};
#endif

