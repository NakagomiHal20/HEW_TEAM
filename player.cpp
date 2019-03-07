//=============================================================================
//
// �v���[���[���� [player.cpp]
// Author : ���@���C��
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT	 MakeVertexPlayer(void);
void	 SetVertexPlayer(void);
void	 SetTexturePlayer(int dir,int cntPattern);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PLAYER	 player[PLAYER_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);	// �v���C���[�O�Ԃ̃A�h���X���擾����	
	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�̃|�C���^
								  TEXTURE_GAME_PLAYER,	// �t�@�C���̖��O
								  &player->texture[0]);

		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�̃|�C���^
								  TEXTURE_GAME_PLAYER2,	// �t�@�C���̖��O
								  &player->texture[1]);
	}
	// �v���C���[�̏���������	   
	player->pos = D3DXVECTOR3(TEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
	player->countAnim = 0;
	player->patternAnim = 0;
	player->direction = 1;
	player->moving_cooldown = 0;
	player->speed = 1.0f;
	player->status.ATK = 5;
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_Y);
	player->radius = D3DXVec2Length(&temp);
	player->baseAngle = atan2f(TEXTURE_PLAYER_SIZE_Y, TEXTURE_PLAYER_SIZE_X);	// �v���C���[�̊p�x��������
	// ���_���̍쐬
	MakeVertexPlayer();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *player = GetPlayer(0);	
	SAFE_RELEASE(player->texture[0]);
	SAFE_RELEASE(player->texture[1]);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{			
	PLAYER *player = GetPlayer(0);
	// �A�j���[�V����	
	player->countAnim+= player->speed*0.1f;
	if (player->moving_cooldown > 0)
	{
		player->patternAnim = (int)(player->countAnim) % ANIM_PATTERN_NUM;
		// �e�N�X�`�����W��ݒ�
		SetTexturePlayer(player->direction,player->patternAnim);
		if (player->patternAnim == 1 || player->patternAnim == 6)
			player->moving_cooldown--;
	}
	// ���͑Ή�
	if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN))
	{
		player->moving_cooldown = 1;
		player->pos.y += player->speed;
	}
	if (GetKeyboardPress(DIK_UP) ||	IsButtonPressed(0, BUTTON_UP))
	{
		player->moving_cooldown = 1;
		player->pos.y -= player->speed;
	}
	if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT)) 
	{
		player->moving_cooldown = 1;
		player->direction = -1;
		player->pos.x -= player->speed;
	}
	if (GetKeyboardPress(DIK_RIGHT)	|| IsButtonPressed(0, BUTTON_RIGHT))
	{
		player->moving_cooldown = 1;
		player->direction = 1;
		player->pos.x += player->speed;
	}
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_A))
	{
		D3DXVECTOR3 player_centre;
		if (player->direction == -1)
			player_centre.x = player->pos.x;
		else
			player_centre.x = player->pos.x + TEXTURE_PLAYER_SIZE_X;
		player_centre.y = player->pos.y + TEXTURE_PLAYER_SIZE_Y;
		SetBullet(player_centre,player->status.ATK, player->direction);
	}
	if (player->pos.x < 0)
	{
		player->pos.x = 0;
	}
	else if (player->pos.x > SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X)
	{
		//�E�ɃX�N�[��
		player->pos.x = SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X;
	}
	else if (player->pos.y < SCREEN_HEIGHT /4)
	{
		player->pos.y = SCREEN_HEIGHT / 4;
	}
	else if (player->pos.y > SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y)
	{
		player->pos.y = SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y;
	}

	// �������ǉ����� �S�[���������ĂȂ��Ƃ��͂��̏���
	// �S�[���������Ă���Ƃ��͉�ʒ[�܂ňړ��ł���炵��
	if (player->pos.x > RIGHT_SCROLL_LINE_X)
	{
		player->pos.x = RIGHT_SCROLL_LINE_X;
	}
	else if (player->pos.x < LEFT_SCROLL_LINE_X)
	{
		player->pos.x = LEFT_SCROLL_LINE_X;
	}

	// �ړ���̍��W�Œ��_��ݒ�
	SetVertexPlayer();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	BULLET *bullet = GetBullet(0);
	if (bullet->use)
		pDevice->SetTexture(0, player->texture[1]);
	else
		pDevice->SetTexture( 0, player->texture[0]);
	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vtx, sizeof(VERTEX_2D));	
	PrintDebugProc(1,"Dir:%d", player->direction);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	// ���_���W�̐ݒ�
	SetVertexPlayer();
	// rhw�̐ݒ�
	player->vtx[0].rhw =
	player->vtx[1].rhw =
	player->vtx[2].rhw =
	player->vtx[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	player->vtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	player->vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	player->vtx[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	player->vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	player->vtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	// �e�N�X�`���̐ݒ�
	SetTexturePlayer(player->direction,player->patternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	// ���_���W�̐ݒ�
	player->vtx[0].vtx = D3DXVECTOR3(player->pos.x,	player->pos.y,0);
	player->vtx[1].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X,	player->pos.y,0);
	player->vtx[2].vtx = D3DXVECTOR3(player->pos.x,	player->pos.y + TEXTURE_PLAYER_SIZE_Y,0);
	player->vtx[3].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X,	player->pos.y + TEXTURE_PLAYER_SIZE_Y,0);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePlayer(int dir, int cntPattern )
{
	PLAYER *player = GetPlayer(0);
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;
	
	if (dir == 1)
	{
		player->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		player->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		player->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		player->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else
	{
		player->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		player->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		player->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		player->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
}
/*******************************************************************************
�֐���:	PLAYER *GetMapAdr( int pno )
����:	int pno : �v���C���[�ԍ�
�߂�l:	PLAYER *
����:	�v���C���[�̃A�h���X���擾����
*******************************************************************************/
PLAYER *GetPlayer(int pno)
{
	return &player[pno];
}