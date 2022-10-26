//=========================================
// UI�쐬
// Author:������
//=========================================
#ifndef _UI_H_
#define _UI_H_
#include "scene.h"
#include "scene2D.h"
#include "control.h"

//=========================================
// �N���X����
//=========================================
class CUi : public CScene2D
{
public:

	//------------------------------
	// UI�^�C�v�̍\����
	//------------------------------
	typedef enum
	{
		UI_TYPE_NONE = 0,
		UI_TYPE_BLINK,
		UI_TYPE_START,
		UI_TYPE_ZOOM,
		UI_TYPE_MAX

	} UI_TYPE;

	CUi(OBJTYPE nPriority = OBJTYPE_UI);				// �R���X�g���N�^
	~CUi();												// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �I�u�W�F�N�g�����֐�
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type = UI_TYPE_NONE);

	// �擾�֐�
	UI_TYPE GetUiType(void);
	CControl *GetControl() { return m_pControl; }

private:
	CControl					*m_pControl;	// �R���g���[���N���X
	UI_TYPE						m_UiType;		// UI�^�C�v
	D3DXVECTOR3					m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3					m_size;			// �T�C�Y
	D3DXCOLOR					m_col;			// �J���[
	bool						m_bRelease;		// �I������

};
#endif