//=============================================================================
//
// �v���[���[���� [enemy.cpp]
// Author : ���@���C��
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "debugproc.h"
#include "math.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT	 MakeVertexEnemy(void);
void	 SetVertexEnemy(void);
void	 SetTextureEnemy(int no, int cntPattern);
void	 SetEnemy(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY	 enemy[ENEMY_MAX];
LPDIRECT3DTEXTURE9 enemyTexture[2];//���ENEMY
//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);	// �v���C���[�O�Ԃ̃A�h���X���擾����	
	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_ENEMY1,	// �t�@�C���̖��O
			&enemyTexture[0]);
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_ENEMY2,	// �t�@�C���̖��O
			&enemyTexture[1]);
	}
	for (int i = 0; i < ENEMY_MAX; i++) 
	{
		// �v���C���[�̏���������	   
		enemy->pos = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X,
								 SCREEN_CENTER_Y/4 + rand() % SCREEN_CENTER_Y,
								 0.0f);
		enemy->use = false;
		enemy->countAnim = 0;
		enemy->patternAnim = 0;
		enemy->direction = 1;
		enemy->moving_cooldown = 0;
		enemy->speed = 3;
		enemy->status.ATK = 5;
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y);
		enemy->radius = D3DXVec2Length(&temp);
		enemy->baseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);	// �v���C���[�̊p�x��������
		enemy->type = rand()%2;
		// ���_���̍쐬
		MakeVertexEnemy();
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	SAFE_RELEASE(enemyTexture[0]);
	SAFE_RELEASE(enemyTexture[1]);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	ENEMY *enemy = GetEnemy(0);
	// �A�j���[�V����	
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy->use)
		{
			enemy->countAnim++;
			if (enemy->moving_cooldown > 0)
			{
				enemy->patternAnim = (enemy->countAnim) % ANIM_PATTERN_NUM;
				// �e�N�X�`�����W��ݒ�
				SetTextureEnemy(enemy->direction, enemy->patternAnim);
				if (enemy->patternAnim == 1 || enemy->patternAnim == 6)
					enemy->moving_cooldown--;
			}
			enemy->pos.x--;
			if (enemy->pos.x < 0)
				enemy->use = false;
			enemy->moving_cooldown = 1;
			//// �ړ���̍��W�Œ��_��ݒ�
			SetVertexEnemy();
		}
		else
		{
			SetEnemy();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use)					// �g�p���Ă����ԂȂ�X�V����
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, enemyTexture[enemy->type]);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BULLET, enemy->vtx, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);
	// ���_���W�̐ݒ�
	SetVertexEnemy();
	// rhw�̐ݒ�
	enemy->vtx[0].rhw =
	enemy->vtx[1].rhw =
	enemy->vtx[2].rhw =
	enemy->vtx[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	enemy->vtx[0].diffuse = 
	enemy->vtx[1].diffuse = 
	enemy->vtx[2].diffuse = 
	enemy->vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	enemy->vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemy->vtx[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	enemy->vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	enemy->vtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	// �e�N�X�`���̐ݒ�
	SetTextureEnemy(enemy->direction, enemy->patternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy(void)
{
	ENEMY *enemy = GetEnemy(0);
	// ���_���W�̐ݒ�
	enemy->vtx[0].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y, 0);
	enemy->vtx[1].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y, 0);
	enemy->vtx[2].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, 0);
	enemy->vtx[3].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, 0);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEnemy(int dir, int cntPattern)
{
	ENEMY *enemy = GetEnemy(0);
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;
		
	enemy->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	enemy->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	enemy->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	enemy->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);	
}

void SetEnemy(void)
{
	ENEMY *enemy = GetEnemy(0);			  
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == false)			// ���g�p��Ԃ̃o���b�g��������
		{	
			enemy->use = true;				// �g�p��Ԃ֕ύX����
			enemy->pos = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X,
									 SCREEN_CENTER_Y/4 + rand() % SCREEN_CENTER_Y,
									 0.0f);	// ���W���Z�b�g
			enemy->patternAnim = 0;
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

/*******************************************************************************
�֐���:	ENEMY *GetMapAdr( int pno )
����:	int pno : �v���C���[�ԍ�
�߂�l:	ENEMY *
����:	�v���C���[�̃A�h���X���擾����
*******************************************************************************/
ENEMY *GetEnemy(int pno)
{
	return &enemy[pno];
}