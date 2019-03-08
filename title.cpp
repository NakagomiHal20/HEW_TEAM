//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "quadrangle.h"
#include "player.h"
#include "title.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`���ꏊ
#define TEXTURE_TITLE			_T("data/TEXTURE/title.png")
#define TEXTURE_MENU_START		_T("data/TEXTURE/start.png")
#define TEXTURE_MENU_EXIT		_T("data/TEXTURE/exit.png")
#define TEXTURE_CURSOR			_T("data/TEXTURE/cursor.png")

// �^�C�g���̃T�C�Y
#define TITLE_SIZE_X			(SCREEN_WIDTH)
#define TITLE_SIZE_Y			(SCREEN_HEIGHT)	

// �^�C�g���̍��W
#define INIT_POS_X				(0.0f)
#define MOVE_SPEED_X			(1.0f)

// ���j���\�̎��
enum MENU_TYPE
{
	MENU_START,					// �Q�[���X�^�[�g
	MENU_EXIT,					// �Q�[���I��
	MENU_TYPE_MAX				// ���j���[�̐�
};

// ���j���[
#define MENU_SIZE_X				(250.0f)
#define MENU_SIZE_Y				(150.0f)
#define START_POS_X				(200.0f)
#define EXIT_POS_X				(500.0f)
#define MENU_POS_Y				(450.0f)

// �J�[�\��
#define CURSOR_SIZE_X			(45.0f)
#define CURSOR_SIZE_Y			(80.0f)
#define CURSOR_POS_X			(500.0f)
#define CURSOR_POS_Y			(530.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(void);					// ���_�̍쐬
void SetVertexTitle(void);						// ���_���W�̐ݒ�
void SetColorTitle(void);						// ���_�J���[�̐ݒ�
void SetTextureTitle(void);						// �e�N�X�`�����W�̐ݒ�


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMETITLE title;
int	selectNo;	

// @QUADRANGLE�\���̂Ŏ蔲������
QUADRANGLE menu[MENU_TYPE_MAX];					// ���j���[
LPDIRECT3DTEXTURE9 menuTex[MENU_TYPE_MAX];		// ���j���\�e�N�X�`��
QUADRANGLE cursor;								// �J�[�\��
LPDIRECT3DTEXTURE9 cursorTex;					// �J�[�\���e�N�X�`��


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

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_MENU_START,
			&menuTex[MENU_START]);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_MENU_EXIT,
			&menuTex[MENU_EXIT]);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_CURSOR,
			&cursorTex);
	}

	// BG������
	title.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	title.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MakeVertexTitle();

	// ���j���[
	InitQuad(&menu[MENU_START], MENU_SIZE_X, MENU_SIZE_Y);
	InitQuad(&menu[MENU_EXIT], MENU_SIZE_X, MENU_SIZE_Y);
	menu[MENU_START].transform.pos = D3DXVECTOR3(START_POS_X, MENU_POS_Y, 0.0f);
	menu[MENU_EXIT].transform.pos = D3DXVECTOR3(EXIT_POS_X, MENU_POS_Y, 0.0f);
	SetVertexQuad(&menu[MENU_START]);
	SetVertexQuad(&menu[MENU_EXIT]);

	// �J�[�\��
	InitQuad(&cursor, CURSOR_SIZE_X, CURSOR_SIZE_Y);
	cursor.transform.pos = D3DXVECTOR3(CURSOR_POS_X, CURSOR_POS_Y, 0.0f);
	SetVertexQuad(&cursor);

	selectNo = MENU_START;

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
	// BGM�Đ�
	PlayGameSound(BGM_TITLE, CONTINUE_SOUND, LOOP);

	// �I��
	if (IsButtonTriggered(0, BUTTON_RIGHT) || GetKeyboardTrigger(DIK_RIGHT)
		|| IsButtonTriggered(0, BUTTON_LEFT) || GetKeyboardTrigger(DIK_LEFT))
	{
		selectNo = selectNo ? MENU_START : MENU_EXIT;
		PlayGameSound(SE_MENU_CURSOR, INIT_SOUND, NONE);	// �J�[�\����
	}

	// ����
	if (IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(0, BUTTON_X)
		|| GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		StopAllSound(INIT_SOUND);							// ����S�Ď~�߂�
		PlayGameSound(SE_MENU_DECISION, INIT_SOUND, NONE);	// ���艹
		if (selectNo == MENU_START) SetStage(GAME);			// �Q�[���ɑJ��
		if (selectNo == MENU_EXIT)	SetStage(EXIT);			// �Q�[���I��
	}

	// �J�[�\���̏ꏊ
	selectNo ? cursor.transform.pos.x = EXIT_POS_X : cursor.transform.pos.x = START_POS_X;
	SetVertexQuad(&cursor);

	// ���j���[
	for (int i = 0; i < MENU_TYPE_MAX; i++)
	{
		menu[i].radius = menu[i].originalRadius;
	}
	menu[selectNo].radius = menu[selectNo].radius * 1.2f;
	SetVertexQuad(&menu[MENU_START]);
	SetVertexQuad(&menu[MENU_EXIT]);
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �^�C�g��BG
	pDevice->SetTexture(0, title.pTexture);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, title.vertexWk, sizeof(VERTEX_2D));

	// ���j���[
	for (int i = 0; i < MENU_TYPE_MAX; i++)
	{
		pDevice->SetTexture(0, menuTex[i]);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, menu[i].vertexWk, sizeof(VERTEX_2D));
	}

	// �J�[�\��
	pDevice->SetTexture(0, cursorTex);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, cursor.vertexWk, sizeof(VERTEX_2D));
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
