//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "title.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_TITLE			_T("data/TEXTURE/title.png")

// �^�C�g���̃T�C�Y
#define TITLE_SIZE_X			(SCREEN_WIDTH)
#define TITLE_SIZE_Y			(SCREEN_HEIGHT)	

// �^�C�g���̍��W
#define INIT_POS_X				(0.0f)
#define MOVE_SPEED_X			(1.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(void);				// ���_�̍쐬
void SetVertexTitle(void);					// ���_���W�̐ݒ�
void SetColorTitle(void);					// ���_�J���[�̐ݒ�
void SetTextureTitle(void);					// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMETITLE title;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_TITLE,
			&title.pTexture);
	}

	title.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	title.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexTitle();

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	SAFE_RELEASE(title.pTexture)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	if (IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(0, BUTTON_X)
		|| GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		SetStage(GAME);
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, title.pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, title.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// ���_���W�̐ݒ�	
	SetVertexTitle();

	// ���_�J���[�̐ݒ�
	SetColorTitle();

	// �e�N�X�`�����W�̐ݒ�
	SetTextureTitle();

	// rhw�̐ݒ�
	title.vertexWk[0].rhw =
		title.vertexWk[1].rhw =
		title.vertexWk[2].rhw =
		title.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexTitle(void)
{
	title.vertexWk[0].vtx = D3DXVECTOR3(title.pos.x, title.pos.y, title.pos.z);
	title.vertexWk[1].vtx = D3DXVECTOR3(title.pos.x + TITLE_SIZE_X, title.pos.y, title.pos.z);
	title.vertexWk[2].vtx = D3DXVECTOR3(title.pos.x, title.pos.y + TITLE_SIZE_Y, title.pos.z);
	title.vertexWk[3].vtx = D3DXVECTOR3(title.pos.x + TITLE_SIZE_X, title.pos.y + TITLE_SIZE_Y, title.pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorTitle(void)
{
	title.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	title.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	title.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	title.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTitle(void)
{
	title.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	title.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	title.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	title.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
