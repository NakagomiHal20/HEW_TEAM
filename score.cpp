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
//#define TEXTURE_SCORE		_T("data/TEXTURE/score.png")

// �X�R�A�̃T�C�Y
#define SCORE_SIZE_X		(SCREEN_WIDTH * 2)
#define SCORE_SIZE_Y		(SCREEN_HEIGHT / 5)	

// �X�R�A���W
#define SCORE_POS_X			(800)
#define SCORE_POS_Y			(100)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexScore(void);					// ���_�̍쐬
void SetVertexScore(void);						// ���_���W�̐ݒ�
void SetColorScore(void);						// ���_�J���[�̐ݒ�
void SetTextureScore(void);						// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMESCORE score;
static TCHAR text[256];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(int type)
{
	if (type == 0)
	{
		//// �e�N�X�`���̓ǂݍ���
		//LPDIRECT3DDEVICE9 pDevice = GetDevice();
		//D3DXCreateTextureFromFile(pDevice,
		//	TEXTURE_SCORE,
		//	&score.pTexture);
	}

	score.pos = D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f);
	score.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexScore();

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


	// �����̕\��
	wsprintf(text, _T("%d\n"), score.value);

	SetVertexScore();
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �����̕\��
	LPD3DXFONT font = GetFont(IMPACT);
	RECT rect = { SCORE_POS_X, SCORE_POS_Y, SCREEN_WIDTH, SCREEN_HEIGHT };
	font->DrawText(NULL, text, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	//// ���_�t�H�[�}�b�g�̐ݒ�
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// �e�N�X�`���̐ݒ�
	//pDevice->SetTexture( 0, score.pTexture );

	//// �|���S���̕`��
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score.vertexWk, sizeof(VERTEX_2D));
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexScore(void)
{
	// ���_���W�̐ݒ�	
	SetVertexScore();

	// ���_�J���[�̐ݒ�
	SetColorScore();

	// �e�N�X�`�����W�̐ݒ�
	SetTextureScore();

	// rhw�̐ݒ�
	score.vertexWk[0].rhw =
		score.vertexWk[1].rhw =
		score.vertexWk[2].rhw =
		score.vertexWk[3].rhw = 1.0f;

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexScore(void)
{
	score.vertexWk[0].vtx = D3DXVECTOR3(score.pos.x, score.pos.y, score.pos.z);
	score.vertexWk[1].vtx = D3DXVECTOR3(score.pos.x + SCORE_SIZE_X, score.pos.y, score.pos.z);
	score.vertexWk[2].vtx = D3DXVECTOR3(score.pos.x, score.pos.y + SCORE_SIZE_Y, score.pos.z);
	score.vertexWk[3].vtx = D3DXVECTOR3(score.pos.x + SCORE_SIZE_X, score.pos.y + SCORE_SIZE_Y, score.pos.z);
}


//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorScore(void)
{
	score.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureScore(void)
{
	score.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	score.vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	score.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	score.vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// �X�R�A�����Z
//=============================================================================
void AddScore(int addValue)
{
	score.value += addValue;
}
