//=============================================================================
// �X�v���b�V�� [splash.cpp]
//
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
// �쐬�� : 2018/1/16
//=============================================================================


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "sound.h"
#include "input.h"
#include "quadrangle.h"
#include "splash.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`��
#define TEXTURE_TEAM_LOGO			("data/TEXTURE/logo.png")

// �e�N�X�`���̃T�C�Y
#define TEAM_LOGO_SIZE_X			(480)						// �T�C�Y�iX�j
#define TEAM_LOGO_SIZE_Y			(200)						// �T�C�Y�iY�j

// �^�C���e�[�u���i1/1000�b�j
#define 	SEQUENCE_1				(0			+ 1000)			// �E�F�C�g
#define		SEQUENCE_2				(SEQUENCE_1	+ 1000)			// �t�F�[�h�C��
#define		SEQUENCE_3				(SEQUENCE_2	+ 2000)			// �\����
#define		WAIT_PRE_TITLE			(SEQUENCE_3	+ 1000)			// �t�F�[�h�A�E�g
#define		END						(WAIT_PRE_TITLE + 500)		// �E�F�C�g	


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
QUADRANGLE			splash;										// �`�[�����S
QUADRANGLE			bg;											// �w�i
LPDIRECT3DTEXTURE9	texSplash = NULL;							// �e�N�X�`���ւ̃|�C���^


//=============================================================================
// ����������
//=============================================================================
HRESULT InitSplash(BOOL type)
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	// ���S
	InitQuad(&splash, TEAM_LOGO_SIZE_X, TEAM_LOGO_SIZE_Y);
	splash.transform.pos = D3DXVECTOR3((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 0.0f);
	SetColorQuad(&bg, D3DCOLOR_RGBA(255, 255, 255, 0));
	SetVertexQuad(&splash);

	// �w�i
	InitQuad(&bg, SCREEN_WIDTH, SCREEN_HEIGHT);
	bg.transform.pos = D3DXVECTOR3((float)SCREEN_CENTER_X, (float)SCREEN_CENTER_Y, 0.0f);
	SetColorQuad(&bg, D3DCOLOR_RGBA(255, 255, 255, 255));
	SetVertexQuad(&bg);

	// 1��ڂ̏��������̂ݎ��s
	if (type != REINIT)
	{// �V�[���J�ڂȂǂɂ��ď��������͍s��Ȃ����������ɏ���

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(device,		// �f�o�C�X�̃|�C���^
			TEXTURE_TEAM_LOGO,					// �t�@�C���̖��O
			&texSplash);						// �ǂݍ��ރ������̃|�C���^
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitSplash(void)
{
		SAFE_RELEASE(texSplash)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateSplash(void)
{
	// @������
	DWORD currentTime = GetTime();			// ���ݎ������擾����
	static	DWORD lastTime = currentTime;	// ���Ԍv���p
	static	DWORD time = 0;					// ���Ԍv���p
	static  int valueFade = 0;

	// @���Ԍv��
	time += currentTime - lastTime;			// �o�ߎ��ԁi�~���b�j���L�^
	lastTime = currentTime;					// ���t���[���ł̎������L�^

	// @�X�L�b�v
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE)
		|| IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(0, BUTTON_X))
	{
		time = END;
	}

	// @�X�v���b�V��
	if (0 < time && time < SEQUENCE_1)
	{// �\���܂ł̃E�F�C�g

	}
	if (SEQUENCE_1 < time && time < SEQUENCE_2)
	{// �t�F�[�h�C��
		valueFade += 255 / (int)MSECOND(SEQUENCE_2 - SEQUENCE_1);
		if (valueFade > 255) { valueFade = 255; }
	}
	if (SEQUENCE_2 < time && time < SEQUENCE_3)
	{// �\����
		valueFade = 255;
	}
	if (SEQUENCE_3 < time && time < WAIT_PRE_TITLE)
	{// �t�F�[�h�A�E�g
		valueFade -= 255 / (int)MSECOND(WAIT_PRE_TITLE - SEQUENCE_3);
		if (valueFade < 0) { valueFade = 0; }
	}
	if (WAIT_PRE_TITLE < time && time < END)
	{// �E�F�C�g
		valueFade = 0;
	}
	if (END < time)
	{// �I������
		time = 0;
		valueFade = 0;
		SetStage(TITLE);
	}

	// ���l�̔��f
	SetColorQuad(&splash, D3DCOLOR_RGBA(255, 255, 255, valueFade));
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawSplash(void)
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	device->SetFVF(FVF_VERTEX_2D);

	// @�w�i
	// �e�N�X�`���̐ݒ�
	device->SetTexture(0, NULL);

	// �|���S���̕`��
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bg.vertexWk, sizeof(VERTEX_2D));
	
	// @���S
	// �e�N�X�`���̐ݒ�
	device->SetTexture(0, texSplash);

	// �|���S���̕`��
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, splash.vertexWk, sizeof(VERTEX_2D));
}



