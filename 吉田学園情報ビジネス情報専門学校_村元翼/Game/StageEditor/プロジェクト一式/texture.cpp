//=============================================================================
// �e�N�X�`������
// Author:������
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//================================================
// �ÓI�����o�ϐ��錾
//================================================
std::vector<LPDIRECT3DTEXTURE9> CTexture::m_apTexture = {};	// �e�N�X�`���ւ̃|�C���^
std::vector<std::string> CTexture::m_aPas;					// �x�N�^�[������N���X
map<string, int> CTexture::m_texType;						// ������A�z�N���X
int CTexture::m_nNumTex = 0;								// �e�N�X�`���̐�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{
	m_texType.clear();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// ������
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���擾����

	// �t�@�C���ǂݍ���
	FILE *pFile;
	if (pFile = fopen("data/TEXTURE/SCRIPT/texture.txt", "r"))
	{
		char cString[128];
		// ��s���ۑ�
		while (fgets(cString, 128, pFile) != NULL)
		{
			// �������ۑ�
			fscanf(pFile, "%s", cString);
			// ������̒���TEX_NUM����������
			if (strncmp("TEX_NUM", cString, 8) == 0)
			{
				//�e�N�X�`���ő吔�ǂݍ���
				fscanf(pFile, "%*s%d", &m_nNumTex);

				int nNum = 0;
				// ��s���ۑ�
				while (fgets(cString, 128, pFile) != NULL)
				{
					// �������ۑ�
					fscanf(pFile, "%s", cString);
					//������̒���PAS����������
					if (strncmp("PAS", cString, 4) == 0)
					{
						// �p�X�̎擾
						char cString[128];
						fscanf(pFile, "%*s%s", &cString[0]);
						m_aPas.push_back(&cString[0]);

						// ���O�̎擾
						char cName[1][128];
						fscanf(pFile, "%*s%*s%s", cName[0]);

						// ���O�Ɛ��̊��蓖��
						m_texType[cName[0]] = nNum;
						nNum++;
					}
				}
			}
		}
	}
	else
	{
		printf("�t�@�C�����J���܂���ł���\n");
	}
	fclose(pFile);

	for (int nCntTex = 0; nCntTex < m_nNumTex; nCntTex++)
	{
		// �f�[�^�ǉ��p�e�N�X�`���|�C���^
		LPDIRECT3DTEXTURE9 pTexBuffer = nullptr;
		//�e�N�X�`���̐���
		D3DXCreateTextureFromFile
		(
			pDevice,
			m_aPas[nCntTex].c_str(),
			&pTexBuffer
		);

		// �f�[�^�̒ǉ�
		m_apTexture.push_back(pTexBuffer);
	}
}

//=============================================================================
// �I��
//=============================================================================
void CTexture::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nNumTex; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCnt])
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// �e�N�X�`�����蓖��
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(std::string texType)
{
	return m_apTexture[m_texType[texType]];
}