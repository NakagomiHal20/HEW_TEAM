//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef ___SOUND_H___
#define ___SOUND_H___


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>


//*****************************************************************************
// �}�N���E�񋓌^��`
//*****************************************************************************
enum SOUND_NAME	//�T�E���h�e�[�u�� --------------------------------------------
{	// SE
	SE_ATTACK,
	SE_GAME_CLEAR,
	SE_GAME_OVER,
	SE_ITEM_GET,
	SE_DAMAGE_PLAYER,
	SE_DAMAGE_TARGET,
	SE_WALK,

	// BGM
	BGM_TITLE,
	BGM_GAME,
	BGM_MUTEKI,
//-----------------------------------------------------------------------------
	SOUND_MAX			// �T�E���h�̐�
};


enum SOUND_PLAY_TYPE
{	// �Đ��^�C�v
	INIT_SOUND,			// �ŏ�����Đ�
	CONTINUE_SOUND,		// �p�����čĐ�
	TYPE_MAX
};

enum SOUND_PLAY_MODE
{	// �Đ��p�t���O
	NONE,				// ��x�̂ݍĐ�
	LOOP,				// ���[�v�Đ�
	FLAG_MAX
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );		// �T�E���h�̏���������
void					UninitSound();				// �T�E���h�̏I������
LPDIRECTSOUNDBUFFER8	LoadSound(int no);			// �T�E���h�̃��[�h

// �����ƂɍĐ�����
void PlayGameSound(SOUND_NAME no, SOUND_PLAY_TYPE type, SOUND_PLAY_MODE mode);

// �����~�߂�
void StopSound(SOUND_NAME no, SOUND_PLAY_TYPE type);

// ����S�Ď~�߂�
void StopAllSound(SOUND_PLAY_TYPE type);

// �Đ������ǂ������ׂ�
bool IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );

// �T�E���h���擾
LPDIRECTSOUNDBUFFER8 GetSound(SOUND_NAME sound_name);

// �T�E���h�����Z�b�g
void ResetSound(SOUND_NAME sound_name);

#endif