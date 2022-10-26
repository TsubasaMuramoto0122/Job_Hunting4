//===========================================================
// �T�E���h���� [sound.h]
// Author : ������
//===========================================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include "main.h"

//===========================================================
// �T�E���h�N���X
//===========================================================
class CSound
{
public:
	CSound();
	~CSound();

	//=================================
	// �T�E���h�t�@�C��
	//=================================
	typedef enum
	{
		//--------------------------
		// �a�f�l
		//--------------------------
		SOUND_LABEL_BGM_TITLE = 0,			// �^�C�g����BGM
		SOUND_LABEL_BGM_GAME,				// �Q�[����BGM
		SOUND_LABEL_BGM_RESULT,				// ���U���g��BGM

		//--------------------------
		// �r�d
		//--------------------------
		SOUND_LABEL_SE_DECIDE,				// ���艹
		SOUND_LABEL_SE_JUMP,				// �W�����v��
		SOUND_LABEL_SE_START,				// �X�^�[�g��
		SOUND_LABEL_SE_IGNITION,			// ���Ή�
		SOUND_LABEL_SE_EXPLOSION,			// ������
		SOUND_LABEL_SE_DESTROY,				// �j��
		SOUND_LABEL_SE_CURSOR,				// �J�[�\����
		SOUND_LABEL_SE_CANCEL,				// �L�����Z����
		SOUND_LABEL_SE_TIMEUP,				// �^�C���A�b�v��
		SOUND_LABEL_SE_COUNT,				// �J�E���g��
		SOUND_LABEL_SE_GAMESTART,			// �Q�[���J�n��
		SOUND_LABEL_SE_CARRYUP,				// �����グ�鉹
		SOUND_LABEL_SE_THROW,				// �����鉹
		SOUND_LABEL_SE_FALL,				// �����鉹
		SOUND_LABEL_MAX

	} SOUND_LABEL;

	//=============================================================================
	// ���\�b�h
	//=============================================================================
	HRESULT InitSound(void);																				// ������			
	void	UninitSound(void);																				// �I��
	HRESULT PlaySound(SOUND_LABEL label);																	// �Đ�
	void	StopSound(SOUND_LABEL label);																	// ��~
	void	StopSound(void);																				// ��~
	void	ControllVoice(SOUND_LABEL label, float fVolume) { m_apSourceVoice[label]->SetVolume(fVolume); }	// ���ʒ���

private:
	//=============================================================================
	// �p�����[�^�\���̒�`
	//=============================================================================
	typedef struct
	{
		char *filename;		// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} PARAM;

	// �e���f�ނ̃p�����[�^
	const static PARAM m_aParam[SOUND_LABEL_MAX];

	//=============================================================================
	// ���\�b�h
	//=============================================================================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//=============================================================================
	// �����o�ϐ�
	//=============================================================================
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

};
#endif
