//======================================================
// X�t�@�C���̓ǂݍ���
// Author:������
//======================================================
#include "LoadX.h"
#include "manager.h"
#include "renderer.h"

//======================================================
// �ÓI�����o�ϐ��̏�����
//======================================================
vector<CLoadX::XData>	CLoadX::m_vDataX;	// �x�N�^�[X�t�@�C���f�[�^�\����
map<string, int>		CLoadX::m_NameX;	// ������A�z�N���X
int						CLoadX::m_nNum = 0; // X�t�@�C���̐�

//======================================================
// �R���X�g���N�^
//======================================================
CLoadX::CLoadX()
{

}

//======================================================
// �f�X�g���N�^
//======================================================
CLoadX::~CLoadX()
{

}

//======================================================
// ������
//======================================================
HRESULT CLoadX::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	FILE *pFile = nullptr;
	if (pFile = fopen("data/TEXT/ModelLoad000.txt", "r"))	// �t�@�C�����J��
	{
		char cStr[128];
		while (fgets(cStr, 128, pFile))					// ��s���ǂݍ���
		{
			fscanf(pFile, "%s", cStr);					// �������ۑ�
			if(strncmp(cStr, "NUM_MODEL",10) == 0)
			{
				fscanf(pFile, "%*s%d", &m_nNum);		// ���f������ۑ�
				m_vDataX.resize(m_nNum);				// �z��(�v�f)���̊m��

				int nCnt = 0;							// �J�E���g�p�ϐ�
				while (fgets(cStr, 128, pFile))			// ��s���ǂݍ���
				{
					fscanf(pFile, "%s", cStr);			// �������ۑ�
					if (strcmp(cStr, "PAS") == 0)
					{
						char aPas[64];
						fscanf(pFile, "%*s%s", &aPas[0]);		// �t�@�C���p�X�擾
						m_vDataX[nCnt].m_XFilePas += aPas;		// �t�@�C���p�X�i�[
					}

					else if (strcmp(cStr, "NAME") == 0)
					{
						char aName[64];
						fscanf(pFile, "%*s%s", &aName[0]);		// �t�@�C���l�[���擾
						m_NameX[&aName[0]] = nCnt;				// �t�@�C���l�[���Ɣԍ������т���
						nCnt++;
					}

					// �S�Ẵ��f����ǂݍ���
					else if (strcmp(cStr, "END_SCRIPT") == 0)
					{
						// X�t�@�C���̓ǂݍ���
						for (int nMod = 0; nMod < m_nNum; nMod++)
						{
							D3DXLoadMeshFromX
							(
								m_vDataX[nMod].m_XFilePas.c_str(),
								D3DXMESH_SYSTEMMEM, pDevice,
								NULL,
								&m_vDataX[nMod].m_pBuffMat,
								NULL,
								&m_vDataX[nMod].m_nNumMat,
								&m_vDataX[nMod].m_pMesh
							);

							// �}�e���A�����ɑ΂���|�C���^���擾
							m_vDataX[nMod].m_pMat = (D3DXMATERIAL*)m_vDataX[nMod].m_pBuffMat->GetBufferPointer();

							// �}�e���A���ǂݍ���
							for (int nMat = 0; nMat < (int)m_vDataX[nMod].m_nNumMat; nMat++)
							{
								LPDIRECT3DTEXTURE9 pTexBuffer = nullptr;	// ���[�J���e�N�X�`���\����

								if (m_vDataX[nMod].m_pMat[nMat].pTextureFilename)
								{
									// �e�N�X�`���̓ǂݍ���
									D3DXCreateTextureFromFile
									(
										pDevice,
										m_vDataX[nMod].m_pMat[nMat].pTextureFilename,
										&pTexBuffer
									);
								}
								m_vDataX[nMod].m_pTex.emplace_back(pTexBuffer);	// �e�N�X�`���f�[�^�̒ǉ�
							}
						}
					}
				}
			}
		}
	}
	else
	{
		printf("�t�@�C���ǂݍ��݂Ɏ��s");
	}

	fclose(pFile);	// �t�@�C�������

	return S_OK;
}

//======================================================
// �I��
//======================================================
void CLoadX::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nNum; nCnt++)
	{
		// ���b�V���̔j��
		if (m_vDataX[nCnt].m_pMesh)
		{
			m_vDataX[nCnt].m_pMesh->Release();
			m_vDataX[nCnt].m_pMesh = nullptr;
		}

		// �}�e���A���̔j��
		if (m_vDataX[nCnt].m_pBuffMat)
		{
			m_vDataX[nCnt].m_pBuffMat->Release();
			m_vDataX[nCnt].m_pBuffMat = nullptr;
		}

		// �e�N�X�`���̔j��
		for (int nTex = 0; nTex < (int)m_vDataX[nCnt].m_nNumMat; nTex++)
		{
			if (m_vDataX[nCnt].m_pTex[nTex])
			{
				m_vDataX[nCnt].m_pTex[nTex]->Release();
				m_vDataX[nCnt].m_pTex[nTex] = nullptr;
			}
		}
	}
}

//======================================================
// X�t�@�C���̎擾
//======================================================
CLoadX::XData CLoadX::GetXData(string xType)
{
	// ������ɍ�����int�^���l���z��ɓ���
	return m_vDataX[m_NameX[xType]];
}

