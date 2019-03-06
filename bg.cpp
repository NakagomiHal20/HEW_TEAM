//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "bg.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_BG			_T("data/TEXTURE/bg.png")

// �w�i�̃T�C�Y
#define BG_SIZE_X			(SCREEN_WIDTH * 2)
#define BG_SIZE_Y			(SCREEN_HEIGHT)	

// �w�i�̍��W
#define INIT_POS_X			(0.0f)
#define MOVE_SPEED_X		(1.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBg(void);					// ���_�̍쐬
void SetVertexBg(void);						// ���_���W�̐ݒ�
void SetColorBg(void);						// ���_�J���[�̐ݒ�
void SetTextureBg(void);					// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMEBG bg;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBg(int type)
{
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_BG,
			&bg.pTexture);
	}

	bg.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bg.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexBg();

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitBg(void)
{
	SAFE_RELEASE(bg.pTexture)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateBg(void)
{
	// �X�N���[��
	if (IsButtonPressed(0, BUTTON_RIGHT) || GetKeyboardPress(DIK_RIGHT))
	{
		if (GetPlayer(0)->pos.x >= RIGHT_SCROLL_LINE_X)
		{
			bg.pos.x -= MOVE_SPEED_X;
		}
	}
	else if (IsButtonPressed(0, BUTTON_LEFT) || GetKeyboardPress(DIK_LEFT))
	{
		if (GetPlayer(0)->pos.x <= LEFT_SCROLL_LINE_X)
		{
			bg.pos.x += MOVE_SPEED_X;
		}
	}

	// ���[�v
	if (bg.pos.x < -BG_SIZE_X / 2)
	{
		bg.pos.x = INIT_POS_X;
	}
	if (bg.pos.x > INIT_POS_X)
	{
		bg.pos.x = -BG_SIZE_X / 2;
	}

	SetVertexBg();
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, bg.pTexture );

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bg.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBg(void)
{
	// ���_���W�̐ݒ�	
	SetVertexBg();

	// ���_�J���[�̐ݒ�
	SetColorBg();

	// �e�N�X�`�����W�̐ݒ�
	SetTextureBg();

	// rhw�̐ݒ�
	bg.vertexWk[0].rhw =
		bg.vertexWk[1].rhw =
		bg.vertexWk[2].rhw =
		bg.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBg(void)
{
	bg.vertexWk[0].vtx = D3DXVECTOR3(bg.pos.x, bg.pos.y, bg.pos.z);
	bg.vertexWk[1].vtx = D3DXVECTOR3(bg.pos.x + BG_SIZE_X, bg.pos.y, bg.pos.z);
	bg.vertexWk[2].vtx = D3DXVECTOR3(bg.pos.x, bg.pos.y + BG_SIZE_Y, bg.pos.z);
	bg.vertexWk[3].vtx = D3DXVECTOR3(bg.pos.x + BG_SIZE_X, bg.pos.y + BG_SIZE_Y, bg.pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorBg(void)
{
	bg.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bg.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bg.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bg.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBg(void)
{
	bg.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	bg.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	bg.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	bg.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
