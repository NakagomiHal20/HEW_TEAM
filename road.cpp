//=============================================================================
//
// ���i�w�i�j���� [road.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "road.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_ROAD		_T("data/TEXTURE/road.png")

// ���i�w�i�j�̃T�C�Y
#define ROAD_SIZE_X			(SCREEN_WIDTH * 2)
#define ROAD_SIZE_Y			(SCREEN_HEIGHT / 5)	

// ���i�w�i�j�̍��W
#define INIT_POS_X			(0.0f)
#define INIT_POS_Y			(SCREEN_HEIGHT * 0.8f)
#define MOVE_SPEED_X		(2.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexRoad(void);					// ���_�̍쐬
void SetVertexRoad(void);						// ���_���W�̐ݒ�
void SetColorRoad(void);						// ���_�J���[�̐ݒ�
void SetTextureRoad(void);						// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMEROAD road;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitRoad(int type)
{
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_ROAD,
			&road.pTexture);
	}

	road.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, 0.0f);
	road.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexRoad();

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitRoad(void)
{
	SAFE_RELEASE(road.pTexture)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateRoad(void)
{
	if (IsButtonPressed(0, BUTTON_RIGHT) || GetKeyboardPress(DIK_D))
	{
		road.pos.x -= MOVE_SPEED_X;
	}
	else if (IsButtonPressed(0, BUTTON_LEFT) || GetKeyboardPress(DIK_A))
	{
		road.pos.x += MOVE_SPEED_X;
	}

	if (road.pos.x < -ROAD_SIZE_X / 2)
	{
		road.pos.x = INIT_POS_X;
	}
	if (road.pos.x > INIT_POS_X)
	{
		road.pos.x = -ROAD_SIZE_X / 2;
	}

	SetVertexRoad();
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawRoad(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, road.pTexture );

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, road.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexRoad(void)
{
	// ���_���W�̐ݒ�	
	SetVertexRoad();

	// ���_�J���[�̐ݒ�
	SetColorRoad();

	// �e�N�X�`�����W�̐ݒ�
	SetTextureRoad();

	// rhw�̐ݒ�
	road.vertexWk[0].rhw =
		road.vertexWk[1].rhw =
		road.vertexWk[2].rhw =
		road.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexRoad(void)
{
	road.vertexWk[0].vtx = D3DXVECTOR3(road.pos.x, road.pos.y, road.pos.z);
	road.vertexWk[1].vtx = D3DXVECTOR3(road.pos.x + ROAD_SIZE_X, road.pos.y, road.pos.z);
	road.vertexWk[2].vtx = D3DXVECTOR3(road.pos.x, road.pos.y + ROAD_SIZE_Y, road.pos.z);
	road.vertexWk[3].vtx = D3DXVECTOR3(road.pos.x + ROAD_SIZE_X, road.pos.y + ROAD_SIZE_Y, road.pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorRoad(void)
{
	road.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	road.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	road.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	road.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureRoad(void)
{
	road.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	road.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	road.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	road.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
