//==================================
// ���b�V���t�B�[���h�̍쐬
// Author: ������
//==================================
#ifndef _MESHFIELD_H
#define _MESHFIELD_H
#include "main.h"
#include "scene.h"
#include "Scene3D.h"
#include "mesh.h"

//==============================================
// �}�N����`
//==============================================
#define MESH_VTX (4)

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CMeshField : public CScene, public CMesh
{
public:
	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CMeshField(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshField();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical);	// ���b�V���t�B�[���h�̐���
	void CollisionManager(void);																				// �����蔻��Ǘ��֐�
	bool LineCollisionMesh(CScene *pScene, const int *pnVtx);													// ���ƃ��b�V���̓����蔻��
	bool ShadowCollisionMesh(CScene *pScene, const int *pnVtx);													// �e�Ƃ̓����蔻��
	void ProcessByObjtype(CScene *pScene,D3DXVECTOR3 &pos);														// �^�C�v�ʏ���
};
#endif