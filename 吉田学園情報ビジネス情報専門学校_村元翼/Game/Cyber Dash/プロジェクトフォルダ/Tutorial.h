//=====================================
// �`���[�g���A��
// Author:������
//=====================================
#include "main.h"
#include "player.h"

//=====================================
// �}�N����`
//=====================================
#define TYPE_MAX		(4)
#define EXP_FONTSIZE	(64)

//=====================================
// �O���錾
//=====================================
class CStage;
class CStringManager;

//=====================================
// �N���X��`
//=====================================
class CTutorial
{
public:
	// �L�����N�^�[�����\����
	typedef enum
	{
		EXP_01 = 0,
		EXP_02,
		EXP_03,
		EXP_04,
		EXP_MAX
	}CHARA_EXP;


	CTutorial();					// �R���X�g���N�^
	~CTutorial();					// �f�X�g���N�^
	HRESULT Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��

	void ExpManager(void);
	void ExpChange(int nExp);
	void KeyExpCreate(CStringManager **ppKeyExp, const char *cStr, const int& nFontSize = EXP_FONTSIZE);	// �L�[���������񐶐��֐�

private:
	CStringManager	*m_pExpStr;				// ������Ǘ��N���X
	CPlayer			*m_pPlayer[TYPE_MAX];	// �v���C���[�N���X
	CStage			*m_pStage;				// �X�e�[�W�N���X
	CHARA_EXP		m_Exp;					// �ǂ̃v���C���[�̐��������Ă��邩�̔ԍ� 
};