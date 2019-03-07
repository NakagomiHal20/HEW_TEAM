//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "score.h"
#include "font.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_SCORE				_T("data/TEXTURE/number.png")
#define TEXTURE_PATTERN_DIVIDE_X	(10)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)

// ���̃T�C�Y
#define DIGIT_WIDTH					(35)
#define DIGIT_HEIGHT				(50)	

// �X�R�A���W
#define SCORE_POS_X					(950)		// 1���ځi�E�[�j�̌��̍��W�ƂȂ�
#define SCORE_POS_Y					(50.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexScore(DIGIT *digit);					// ���_�̍쐬
void SetVertexScore(DIGIT *digit);						// ���_���W�̐ݒ�
void SetColorScore(DIGIT *digit);						// ���_�J���[�̐ݒ�
void SetTextureScore(DIGIT *digit, int cntPattern);		// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMESCORE score;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(int type)
{
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_SCORE,
			&score.pTexture);
	}

	// �X�R�A������
	score.pos = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);
	score.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	score.value = 0;

	// �e��������
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		score.digit[i].number = 0;
		// �E������ׂĂ���
		score.digit[i].pos.x = score.pos.x - i * DIGIT_WIDTH;
		score.digit[i].pos.y = score.pos.y = score.pos.y;
		MakeVertexScore(&score.digit[i]);
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	SAFE_RELEASE(score.pTexture)
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_O))
	{
		AddScore(100);
	}
#endif

	// �X�R�A�e���̐������Z�o
	int wk = score.value;
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		score.digit[i].number = wk % 10;
		wk /= 10;

		//�e�N�X�`�����W�̐ݒ�
		SetTextureScore(&score.digit[i], score.digit[i].number);
	}

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, score.pTexture );

	// �|���S���̕`��
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score.digit[i].vertexWk, sizeof(VERTEX_2D));
	}
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexScore(DIGIT *digit)
{
	// ���_���W�̐ݒ�	
	SetVertexScore(digit);

	// ���_�J���[�̐ݒ�
	SetColorScore(digit);

	// �e�N�X�`�����W�̐ݒ�
	SetTextureScore(digit, digit->number);

	// rhw�̐ݒ�
	digit->vertexWk[0].rhw =
		digit->vertexWk[1].rhw =
		digit->vertexWk[2].rhw =
		digit->vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexScore(DIGIT *digit)
{
	digit->vertexWk[0].vtx = D3DXVECTOR3(digit->pos.x, digit->pos.y, digit->pos.z);
	digit->vertexWk[1].vtx = D3DXVECTOR3(digit->pos.x + DIGIT_WIDTH, digit->pos.y, digit->pos.z);
	digit->vertexWk[2].vtx = D3DXVECTOR3(digit->pos.x, digit->pos.y + DIGIT_HEIGHT, digit->pos.z);
	digit->vertexWk[3].vtx = D3DXVECTOR3(digit->pos.x + DIGIT_WIDTH, digit->pos.y + DIGIT_HEIGHT, digit->pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorScore(DIGIT *digit)
{
	digit->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	digit->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	digit->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	digit->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureScore(DIGIT *digit, int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;		//�e�N�X�`���p�^�[����X��(U��)�����ɉ����ڂ��J�E���g
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;		//�e�N�X�`���p�^�[����Y��(V��)�����ɉ����ڂ��J�E���g
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;		//�P�p�^�[����U�l
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;		//�P�p�^�[����V�l

	digit->vertexWk[0].tex = D3DXVECTOR2((float)(x) * sizeX, (float)(y) * sizeY);
	digit->vertexWk[1].tex = D3DXVECTOR2((float)(x) * sizeX + sizeX, (float)(y) * sizeY);
	digit->vertexWk[2].tex = D3DXVECTOR2((float)(x) * sizeX, (float)(y) * sizeY + sizeY);
	digit->vertexWk[3].tex = D3DXVECTOR2((float)(x) * sizeX + sizeX, (float)(y) * sizeY + sizeY);
}


//=============================================================================
// �X�R�A�����Z
//=============================================================================
void AddScore(int addValue)
{
	score.value += addValue;
}
