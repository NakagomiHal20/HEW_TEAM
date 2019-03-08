//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "life.h"
#include "font.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_HEART		_T("data/TEXTURE/heart.png")
#define TEXTURE_HALF_HEART	_T("data/TEXTURE/half_heart.png")
#define TEXTURE_BLANK_HEART	_T("data/TEXTURE/blank_heart.png")

// ���C�t�̃T�C�Y
#define HEART_SIZE			(50)
#define HEART_SPAN			(5)

// ���C�t�̍��W
#define INIT_POS_X			(100.0f)
#define INIT_POS_Y			(50.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHeart(HEART* heart);			// ���_�̍쐬
void SetVertexHeart(HEART* heart);				// ���_���W�̐ݒ�
void SetColorHeart(HEART* heart);				// ���_�J���[�̐ݒ�
void SetTextureHeart(HEART* heart);				// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMELIFE life;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitLife(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// @�e�N�X�`���̓ǂݍ���
		// ���C�t�R��
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_HEART,
			&life.pTexHeart);

		// �����̃n�[�g
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_HALF_HEART,
			&life.pTexHalfHeart);

		// ��n�[�g
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_BLANK_HEART,
			&life.pTexBlankHeart);
	}

	// �S�̂̕\���ꏊ
	life.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, 0.0f);
	life.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �n�[�g�̕\���ꏊ
	for (int i = 0; i < HEART_MAX; i++)
	{
		life.heart[i].pos = D3DXVECTOR3(life.pos.x + i * (HEART_SIZE + HEART_SPAN), life.pos.y, 0.0f);
		life.heart[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		life.heart[i].drawTex = FULL_HEART;
		MakeVertexHeart(&life.heart[i]);
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitLife(void)
{
	SAFE_RELEASE(life.pTexHeart)
	SAFE_RELEASE(life.pTexHalfHeart)
	SAFE_RELEASE(life.pTexBlankHeart)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife(void)
{
#ifdef _DEBUG
	if(GetKeyboardTrigger(DIK_P))
	{
		GetPlayer(0)->status.HP--;
	}
#endif

	int hp = GetPlayer(0)->status.HP;
	
	// �n�[�g�̕\���ꏊ
	for (int i = 0; i < HEART_MAX; i++)
	{
		// �����n�[�g
		if (hp % 2 && (hp == (i + 1) * 2 - 1))
		{
			life.heart[i].drawTex = HALF_HEART;
			continue;
		}

		// ��̃n�[�g
		if (hp < (i + 1) * 2)
		{
			life.heart[i].drawTex = BLANK_HEART;
		}
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < HEART_MAX; i++)
	{
		// �e�N�X�`���̐ݒ�
		if (life.heart[i].drawTex == FULL_HEART)
		{
			pDevice->SetTexture(0, life.pTexHeart);
		}
		else if (life.heart[i].drawTex == HALF_HEART)
		{
			pDevice->SetTexture(0, life.pTexHalfHeart);
		}
		else if (life.heart[i].drawTex == BLANK_HEART)
		{
			pDevice->SetTexture(0, life.pTexBlankHeart);
		}

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, life.heart[i].vertexWk, sizeof(VERTEX_2D));
	}
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexHeart(HEART *heart)
{
	// ���_���W�̐ݒ�	
	SetVertexHeart(heart);

	// ���_�J���[�̐ݒ�
	SetColorHeart(heart);

	// �e�N�X�`�����W�̐ݒ�
	SetTextureHeart(heart);

	// rhw�̐ݒ�
	heart->vertexWk[0].rhw =
		heart->vertexWk[1].rhw =
		heart->vertexWk[2].rhw =
		heart->vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexHeart(HEART *heart)
{
	heart->vertexWk[0].vtx = D3DXVECTOR3(heart->pos.x, heart->pos.y, heart->pos.z);
	heart->vertexWk[1].vtx = D3DXVECTOR3(heart->pos.x + HEART_SIZE, heart->pos.y, heart->pos.z);
	heart->vertexWk[2].vtx = D3DXVECTOR3(heart->pos.x, heart->pos.y + HEART_SIZE, heart->pos.z);
	heart->vertexWk[3].vtx = D3DXVECTOR3(heart->pos.x + HEART_SIZE, heart->pos.y + HEART_SIZE, heart->pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorHeart(HEART *heart)
{
	heart->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	heart->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	heart->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	heart->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureHeart(HEART *heart)
{
	heart->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	heart->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	heart->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	heart->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

