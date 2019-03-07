//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "road.h"
#include "timer.h"
#include "result.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_GAME_CLEAR		_T("data/TEXTURE/game_clear.png")
#define TEXTURE_GAME_OVER		_T("data/TEXTURE/game_over.png")

// ���U���g�̃T�C�Y
#define RESULT_SIZE_X			(SCREEN_WIDTH)
#define RESULT_SIZE_Y			(SCREEN_HEIGHT)	

// ���U���g�̍��W
#define INIT_POS_X				(0.0f)
#define MOVE_SPEED_X			(1.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(void);	// ���_�̍쐬
void SetVertexResult(void);		// ���_���W�̐ݒ�
void SetColorResult(void);		// ���_�J���[�̐ݒ�
void SetTextureResult(void);	// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMERESULT	result;
BOOL		isSoundPlay;	// ���Đ����邩

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_GAME_CLEAR,
			&result.pTexture[GAME_CLEAR]);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_GAME_OVER,
			&result.pTexture[GAME_OVER]);
	}

	isSoundPlay = TRUE;
	result.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	result.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexResult();

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	SAFE_RELEASE(result.pTexture[GAME_CLEAR])
	SAFE_RELEASE(result.pTexture[GAME_OVER])
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	GAMEDATA *gameData = GetGameData();

	// �T�E���h
	if (isSoundPlay)
	{
		if (gameData->isGameClear)
		{
			PlayGameSound(SE_GAME_CLEAR, INIT_SOUND, NONE);
		}
		else
		{
			PlayGameSound(SE_GAME_OVER, INIT_SOUND, NONE);
		}
		isSoundPlay = FALSE;
	}

	// �X�e�[�W�J��
	if (IsButtonTriggered(0, BUTTON_A) || GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		GetGameData()->isGameClear = FALSE;
		InitPlayer(REINIT);
		InitEnemy(REINIT);
		InitBg(REINIT);
		InitRoad(REINIT);
		InitTimer(REINIT);
		SetStage(TITLE);
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	if (GetGameData()->isGameClear)
	{
		pDevice->SetTexture(0, result.pTexture[GAME_CLEAR]);
	}
	else
	{
		pDevice->SetTexture(0, result.pTexture[GAME_OVER]);
	}

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, result.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(void)
{
	// ���_���W�̐ݒ�	
	SetVertexResult();

	// ���_�J���[�̐ݒ�
	SetColorResult();

	// �e�N�X�`�����W�̐ݒ�
	SetTextureResult();

	// rhw�̐ݒ�
	result.vertexWk[0].rhw =
		result.vertexWk[1].rhw =
		result.vertexWk[2].rhw =
		result.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexResult(void)
{
	result.vertexWk[0].vtx = D3DXVECTOR3(result.pos.x, result.pos.y, result.pos.z);
	result.vertexWk[1].vtx = D3DXVECTOR3(result.pos.x + RESULT_SIZE_X, result.pos.y, result.pos.z);
	result.vertexWk[2].vtx = D3DXVECTOR3(result.pos.x, result.pos.y + RESULT_SIZE_Y, result.pos.z);
	result.vertexWk[3].vtx = D3DXVECTOR3(result.pos.x + RESULT_SIZE_X, result.pos.y + RESULT_SIZE_Y, result.pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorResult(void)
{
	result.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureResult(void)
{
	result.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	result.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	result.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	result.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
