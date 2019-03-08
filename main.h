//=============================================================================
//
// ���C������ [main.h]
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"

// �x���΍�
#define DIRECTINPUT_VERSION (0x0800)

#include "dinput.h"
#include <tchar.h>
#include "mmsystem.h"


//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif


//*****************************************************************************
// �}�N���E�񋓌^��`
//*****************************************************************************
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	NUM_VERTEX				(4)					// ���_��
#define	NUM_POLYGON				(2)					// �|���S����

// ��ʃT�C�Y
#define SCREEN_WIDTH			(1024)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT			(576)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X			(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y			(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

// �v���O�����S�ʂŎg�p����\�L
#define FIRSTINIT				(0)					// ���񏉊���
#define REINIT					(1)					// �ď�����
#define	FPS						(60)				// FPS
#define SCREEN_OUT				(-10000.0f)			// ��ʊO


//-----------------------------------------------------------------------------
// ����������}�N��
//-----------------------------------------------------------------------------
#define SAFE_RELEASE(p) if (p) { p->Release(); p = NULL; }
#define SAFE_DELETE_ARRAY(a) { delete [] (a); (a) = NULL; }
#define SAFE_DELETE(p) if(p){ delete[] p; p=NULL; }
#define SAFE_DESTROY(x) { if(x) { (x)->Destroy(); (x) = NULL; } }
//-----------------------------------------------------------------------------
// �����v�Z
//-----------------------------------------------------------------------------
// �u�b�v�u���v�u���v�� �t���[����
#define		MSECOND(_msec)		((_msec) * 0.001f * FPS)
#define		SECOND(_sec)		((_sec) * FPS)
#define		MINUTE(_min)		(SECOND((_min) * 60))
#define		HOUR(_hour)			(MINUTE((_hour) * 60))

// �u�~���v�u�Z���`�v�u�L���v�� ���[�g�� 
#define		MM(_mm)				(CM((_mm) * 0.1f))
#define		CM(_cm)				(M((_cm) * 0.01f))
#define		M(_m)				(_m)
#define		KM(_km)				(M((_km) * 1000.0f))

// ���x
#define		M_S(_speed)			(M(_speed) / SECOND(1.0f))
#define		KM_H(_speed)		(KM(_speed) / HOUR(1.0f))

// �����x
#define		M_S2(_acc)			(M(_acc) / (SECOND(1.0f) * SECOND(1.0f)))

// �W���d�͉����x (�������Ȃ̂Ł|)
#define		GRAVITY				(M_S2(-9.8f))


//-----------------------------------------------------------------------------
// �X�e�[�W�J��
//-----------------------------------------------------------------------------
enum STAGE
{
	SPLASH,						// �X�v���b�V��
	TITLE,						// �^�C�g��
	TUTORIAL,					// �`���[�g���A��
	GAME,						// �Q�[��
	PAUSE,						// �|�[�Y���
	RESULT,						// ���U���g
	EXIT						// �I��
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
struct  VERTEX_2D
{
	D3DXVECTOR3		vtx;		// ���_���W
	float			rhw;		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR		diffuse;	// ���ˌ�
	D3DXVECTOR2		tex;		// �e�N�X�`�����W
};


// TRANSFORM�\���̂��`
struct  TRANSFORM
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ��]
	D3DXVECTOR3		scl;		// �傫��
};


// �Q�[���i�s�f�[�^�Z�b�g
struct GAMEDATA
{
	BOOL			isGameClear;	// �Q�[���̃��U���g// TRUE:GameClear FALSE:GameOver
};


struct CHARACTER 
{
	char name[32];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int LUCK;
	int EXP;
	int LV;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �f�o�C�X�擾
LPDIRECT3DDEVICE9 GetDevice(void);

// �X�e�[�W���Z�b�g
void SetStage(STAGE set);

// FPS�擾
int GetFPS(void);

// ���ݎ����擾
DWORD GetTime(void);

// �Q�[���i�s�f�[�^�Z�b�g���擾
GAMEDATA *GetGameData(void);


#endif