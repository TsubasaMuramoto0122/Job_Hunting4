//==================================
// ���b�V���t�B�[���h�̍쐬
// Author: ������
//==================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_
#include "main.h"
#include "scene.h"
#include "mesh.h"
#include "Scene3D.h"

//------------------------------------------------------------------------------
// �N���X�̒�`(���d��`)
//------------------------------------------------------------------------------
class CMeshCylinder : public CScene ,public CMesh
{
public:
	//--------------------------------------------
	// ���̃^�C�v�\����
	//--------------------------------------------
	enum class CYLINDER_TYPE
	{
		STAND = 0,
		STAND_INSIDE,
		LIE,
		LIE_INSIDE,
		MAX
	};

	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CMeshCylinder(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshCylinder();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// �I�u�W�F�N�g����
	static CMeshCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical, CYLINDER_TYPE type);	// �V�[��3D�̐���

private:
	CYLINDER_TYPE m_Type;		// �����b�V���̃^�C�v
};


#endif


