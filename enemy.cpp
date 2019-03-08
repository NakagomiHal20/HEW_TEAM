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

#include "time.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT	 MakeVertexEnemy(int no);
void	 SetVertexEnemy(int no);
void	 SetTextureEnemy(int no, int cntPattern);
void	 SetEnemy(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY	 enemy[ENEMY_MAX];
LPDIRECT3DTEXTURE9 enemyTexture[2];//���ENEMY
int		 enemy_count,enemy_rate;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	srand(time(NULL));
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
	for (int i = 0; i < ENEMY_MAX; i++,enemy++) 
	{
		// �v���C���[�̏���������	   
		enemy->pos = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X,
								 SCREEN_CENTER_Y/4 + rand() % SCREEN_CENTER_Y,
								 0.0f);
		enemy->use = FALSE;
		enemy->countAnim = 0;
		enemy->patternAnim = 0;
		enemy->moving_cooldown = 0;
		enemy->speed = 1.0f;
		enemy->status.ATK = 5;
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y);
		enemy->radius = D3DXVec2Length(&temp);
		enemy->baseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);	// �v���C���[�̊p�x��������
		enemy->type = rand()%2;
		// ���_���̍쐬
		MakeVertexEnemy(i);
	}
	enemy_count = 0;
	enemy_rate = 100;
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
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use)
		{
			enemy->countAnim += enemy->speed * 0.3f;
			//enemy->moving_cooldown = 1;
			////enemy->countAnim ++;
			//if (enemy->moving_cooldown > 0)
			//{
				enemy->patternAnim = (int)(enemy->countAnim) % ANIM_PATTERN_NUM;
				// �e�N�X�`�����W��ݒ�
				SetTextureEnemy(i, enemy->patternAnim);			
			//}
			enemy->pos.x += enemy->direction*enemy->speed * 3.0f;
			//// �ړ���̍��W�Œ��_��ݒ�
			SetVertexEnemy(i);
			if (enemy->pos.x < 0 || enemy->pos.x > SCREEN_WIDTH)
				enemy->use = false;
		}
		else
		{
			enemy_count++;
			if (enemy_count>enemy_rate&& !enemy->use)			
			SetEnemy();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(int pno)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(pno);
	//for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	//{
		if (enemy->use)					// �g�p���Ă����ԂȂ�X�V����
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, enemyTexture[enemy->type]);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BULLET, enemy->vtx, sizeof(VERTEX_2D));
		}
	//}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEnemy(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(no);
	// ���_���W�̐ݒ�
	SetVertexEnemy(no);
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
	SetTextureEnemy(no, enemy->patternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy(int no)
{
	ENEMY *enemy = GetEnemy(no);
	// ���_���W�̐ݒ�
	enemy->vtx[0].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y, 0);
	enemy->vtx[1].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y, 0);
	enemy->vtx[2].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, 0);
	enemy->vtx[3].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, 0);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEnemy(int no, int cntPattern)
{
	ENEMY *enemy = GetEnemy(no);
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;
		
	if (enemy->direction < 0)
	{
		enemy->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		enemy->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		enemy->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		enemy->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else
	{
		enemy->vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		enemy->vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		enemy->vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		enemy->vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
}
//=============================================================================
// �G�̔��ːݒ�
//=============================================================================
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
			enemy->speed = 1.0f;
			enemy->direction = -1;
			enemy->type = rand() % 2;
			enemy_count = 0;
			enemy_rate = 60+rand()%41;
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

/************************thsths*******************************************************
�֐���:	ENEMY *GetMapAdr( int pno )
����:	int pno : �v���C���[�ԍ�
�߂�l:	ENEMY *
����:	�v���C���[�̃A�h���X���擾����
*******************************************************************************/
ENEMY *GetEnemy(int pno)
{
	return &enemy[pno];
}