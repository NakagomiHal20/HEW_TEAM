//=============================================================================
// �T�E���h���� [sound.cpp]
//
// Author : GP11B341 24�@�����a�P
// �쐬�� : 2018/11/1
//=============================================================================


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "sound.h"


//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )


//*****************************************************************************
// �T�E���h�t�@�C���̃p�X
//*****************************************************************************
// �T�E���h�t�@�C���̃p�X�ih�̒ʂ��i���o�[�ƍ��킹�邱�Ɓj
const TCHAR* c_soundFilename[] = {
	// SE
	_T("data/SOUND/SE/Attack.wav"),
	_T("data/SOUND/SE/gameclear.wav"),
	_T("data/SOUND/SE/gameover.wav"),
	_T("data/SOUND/SE/item.wav"),
	_T("data/SOUND/SE/PlayerDamage.wav"),
	_T("data/SOUND/SE/TargetDamage.wav"),
	_T("data/SOUND/SE/walk.wav"),
	_T("data/SOUND/SE/se_menu_decision.wav"),
	_T("data/SOUND/SE/se_menu_cursor.wav"),


	// BGM
	_T("data/SOUND/BGM/title.wav"),
	_T("data/SOUND/BGM/gamegamen.wav"),
	_T("data/SOUND/BGM/muteki.wav"),
};


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IDirectSound8 *directSound = NULL;			// �T�E���h�C���^�[�t�F�[�X
LPDIRECTSOUNDBUFFER8 sound[SOUND_MAX];		// �T�E���h�p�o�b�t�@
//LPDIRECTSOUNDBUFFER8 dualSound[DUPLICATED_SOUND_MAX];



//=============================================================================
// ����������
//=============================================================================
// hWnd:�E�B���h�E�n���h��
HRESULT InitSound( HWND hWnd )
{
	// DirectSound�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &directSound, NULL)))
		return E_FAIL;
	
	// �������x���ݒ� 
	if (FAILED(directSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	// ���[�h
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		sound[i] = LoadSound(i);
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �T�E���h�p�o�b�t�@�̉��
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		SAFE_RELEASE(sound[i])
	}

	// ���T�E���h�p�o�b�t�@�����ɍs��
	if( directSound != NULL )
	{
		directSound->Release();
		directSound = NULL;
	}
}


//=============================================================================
// �T�E���h�̃��[�h
//=============================================================================
// no:�T�E���h�i���o�[�i�w�b�_�ɒ�`���ꂽ�񋓌^�萔�j
LPDIRECTSOUNDBUFFER8 LoadSound(int no)
{
	// MMIO = �}���`���f�B�A���o�́A�̗��B
	LPDIRECTSOUNDBUFFER  BaseBuffer = NULL;	// �ȃf�[�^�̑����o�b�t�@
	LPDIRECTSOUNDBUFFER8 buffer = NULL;		// �ȃf�[�^�̃o�b�t�@
	DSBUFFERDESC buff;							// �o�b�t�@�ݒ�\����

	HMMIO hMmio = NULL;							// MMIO�n���h��
	MMIOINFO mmioInfo;							// �}���`���f�B�A�f�[�^�\����

	MMRESULT mmRes;								// MM�������ʊi�[�ϐ�
	MMCKINFO riffChunk, formatChunk, dataChunk;	// �`�����N�f�[�^�\����
	DWORD size;									// �f�[�^�T�C�Y�i�[�p
	WAVEFORMATEX pcm;							// �ȃf�[�^�t�H�[�}�b�g�\����

	LPVOID Block;								// �Z�J���_���o�b�t�@�̃f�[�^�������ݐ�A�h���X
	DWORD  dwSize;								// �Z�J���_���o�b�t�@�̃T�C�Y

	// 1.�n���h�������ƂɃt�@�C�����J��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if(!hMmio)
		return NULL;

	// 2.�t�@�C����͇@ RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);			// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.�t�@�C����͇A �t�H�[�}�b�g�`�����N����
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);				// �����������Ƃɓǂݍ���
	
	if(size != formatChunk.cksize) 		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0); 

	// 4.�t�@�C����͇B �f�[�^�`�����N����
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);		// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR) 		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.�f�[�^�ǂݍ���		
	char *data = new char[dataChunk.cksize];								// �K�v�ȑ傫���̗̈���m�ۂ���
	size = mmioRead(hMmio, (HPSTR)data, dataChunk.cksize);					// �f�[�^��ǂݍ���
	
	if(size != dataChunk.cksize)  		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		delete[] data;
		return NULL;
	}

	// 6.�Ȃ�ǂݍ��ށu�Z�J���_���o�b�t�@�v��p��
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));								// �܂�������
	buff.dwSize = sizeof(DSBUFFERDESC);										// ��������e��ݒ�
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// �����o�b�t�@�����
	if (FAILED(directSound->CreateSoundBuffer(&buff, &BaseBuffer, NULL)))
		return NULL;

	// �T�E���h�o�b�t�@�����o��
	if (FAILED(BaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&buffer)))
		return NULL;

	// ���o�����瑍���o�b�t�@���̂Ă�
	BaseBuffer->Release();

	// 7.���ꂽ�Z�J���_���o�b�t�@�Ƀf�[�^��]��
	// �܂��͏������݂ł���悤�o�b�t�@�����b�N
	if (FAILED(buffer->Lock(0, 0, &Block, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(Block, data, dwSize);	// �f�[�^�]��
	delete[] data;					// ���̓ǂݍ��ݗ̈������

	// �o�b�t�@���b�N����
	buffer->Unlock(Block, dwSize, 0, 0);

	// �Z�J���_���o�b�t�@��Ԃ��Ă悤�₭����...
	return buffer;
}


//=============================================================================
// ����炷
//-----------------------------------------------------------------------------
// �߂�l�Fvoid
// ����  �FSOUND_NAMME     no    �Đ�����T�E���h�̔ԍ�
//         SOUND_PLAY_TYPE type  INIT_SOUND�c�c�ŏ�����炷�@CONTINUE_SOUND�c�c�p�����Ė炷
//	       SOUND_PLAY_MODE flag  NONE�c�c�P��Đ����ďI���@LOOP�c�c���[�v�Đ�
//=============================================================================
void PlayGameSound(SOUND_NAME no, SOUND_PLAY_TYPE type, SOUND_PLAY_MODE mode)
{	
	// type��INITIAL�ɂ���Ɗ֐����ĂԂ��ƂɍĐ��ʒu���擪�ɂ��ǂ�̂�
	// BGM��A���Ŕ���������ʉ���CONTINUE�ɂ���
	if (type == INIT_SOUND)
	{
		// �Đ��ʒu��擪�ɖ߂�
		// �����T�E���h�𖳉����ԂȂ��A���Đ����邽��
		sound[no]->SetCurrentPosition(0);
	}

	sound[no]->Play(0, 0, mode);
}



//=============================================================================
// �����~�߂�
//-----------------------------------------------------------------------------
// �߂�l�Fvoid
// ����  �FSOUND_NAMME     no    ��~����T�E���h�̔ԍ�
//         SOUND_PLAY_TYPE type  INIT_SOUND�c�c��~��Đ��ʒu�����Z�b�g�@CONTINUE_SOUND�c�c��~��Đ��ʒu�͂��̂܂�
//=============================================================================
void StopSound(SOUND_NAME no, SOUND_PLAY_TYPE type)
{
	DWORD status;

	sound[no]->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )	// ���Ă�����
	{
		sound[no]->Stop();				// �Ӗ��I�ɂ�Pause�ɂȂ�B
	}

	if (type == INIT_SOUND)
	{
		// �Đ��ʒu��擪�ɖ߂�
		sound[no]->SetCurrentPosition(0);
	}
}


//=============================================================================
// ����S�Ď~�߂�
//-----------------------------------------------------------------------------
// �߂�l�Fvoid
// ����  �FSOUND_PLAY_TYPE type  INIT_SOUND�c�c��~��Đ��ʒu�����Z�b�g�@CONTINUE_SOUND�c�c��~��Đ��ʒu�͂��̂܂�         
//=============================================================================
void StopAllSound(SOUND_PLAY_TYPE type)
{
	DWORD status;
	
	for (int i = 0; i < SOUND_MAX; i++)
	{
		sound[i]->GetStatus(&status);

		if (status & DSBSTATUS_PLAYING)	// ���Ă�����
		{
			sound[i]->Stop();			// �Ӗ��I�ɂ�Pause�ɂȂ�B
		}

		if (type == INIT_SOUND)
		{
			// ���̌�ɂ�������s���Ȃ��ƍĐ��ʒu������������Ȃ�
			// �Đ��ʒu��擪�ɖ߂�
			sound[i]->SetCurrentPosition(0);
		}
	}
}


//=============================================================================
// �Đ������ǂ������ׂ�
//=============================================================================
// buffer:����炵�����T�E���h�f�[�^�̃Z�J���_���o�b�t�@
bool IsPlaying(LPDIRECTSOUNDBUFFER8 buffer)
{
	DWORD status;

	buffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )
	{
		return true;
	}
	return false;
}


//=============================================================================
// �T�E���h�����Z�b�g
//=============================================================================
void ResetSound(SOUND_NAME sound_name)
{
	LPDIRECTSOUNDBUFFER8 buffer = GetSound(sound_name);
	// �Đ��ʒu��擪�ɖ߂�
	// �����T�E���h�𖳉����ԂȂ��A���Đ����邽��
	buffer->SetCurrentPosition(0);
}


//=============================================================================
// �T�E���h���擾
//-----------------------------------------------------------------------------
// �߂�l�FLPDIRECTSOUNDBUFFER8�@ �@�T�E���h�o�b�t�@�ւ̃|�C���^
// ����  �FSOUND_NAMME  soundName �@�擾����T�E���h�̔ԍ�
//=============================================================================
LPDIRECTSOUNDBUFFER8 GetSound(SOUND_NAME soundName)
{
	return	sound[soundName];
}


