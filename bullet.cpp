/*******************************************************************************
//
// �o���b�g(�U��)���� [bullet.cpp]
// Author : HAL���� 2�N���Q�[���w�� GP11B341 16 ���@���C��
//
********************************************************************************/
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "sound.h"
#include "input.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(int no);
void	SetTextureBullet(int no, int cntPattern);
void	SetVertexBullet (int no);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		bullet_texture = NULL;			// �e�N�X�`���ւ̃|���S��
BULLET					bulletWk[BULLET_MAX];			// �o���b�g�\����
LPDIRECTSOUNDBUFFER8	g_pSE;							// SE�p�o�b�t�@
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = &bulletWk[0];		// �o���b�g�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);		// �v���C���[�̃|�C���^�[��������
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		// �e�N�X�`���[�̏��������s���H
		if (type == 0)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�̃|�C���^
				TEXTURE_GAME_BULLET,					// �t�@�C���̖��O
				&bullet->texture);						// �ǂݍ��ރ������̃|�C���^			
		}
		bullet->use = false;									// ���g�p�i���˂���Ă��Ȃ��e�j
		bullet->pos = D3DXVECTOR3(-600.0f, -600.0f, 0.0f);		// ���W�f�[�^��������		
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_BULLET_SIZE_X / 2, TEXTURE_BULLET_SIZE_Y / 2);
		bullet->radius = D3DXVec2Length(&temp);
		MakeVertexBullet(i);									// ���_���̍쐬
	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	SAFE_RELEASE(bullet_texture);
	SAFE_RELEASE(g_pSE);
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = &bulletWk[0];			// �o���b�g�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);			// �o���b�g�̃|�C���^�[��������
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{		
		if (bullet->use == true)			
		{
			// �o���b�g�̈ړ�����
			if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT))
			{
				bullet->pos.x -= player->speed;
			}
			if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))
			{
				bullet->pos.x += player->speed;
			}
			bullet->pos.y -= BULLET_SPEED;		
			// ��ʊO�܂Ői�񂾁H
			if (bullet->pos.y < player->pos.y || bullet->pos.y > SCREEN_HEIGHT
				|| bullet->pos.x < -TEXTURE_BULLET_SIZE_X || bullet->pos.x > SCREEN_WIDTH)
			{
				bullet->use = false;			
				bullet->pos.x = -100.0f;
			}
			SetVertexBullet(i);
		}		
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = bulletWk;				// �o���b�g�̃|�C���^�[��������
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);	
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{		
		if (bullet->use)					// �g�p���Ă����ԂȂ�X�V����
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, bullet->texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BULLET, bullet->vertexWk, sizeof(VERTEX_2D));
		}
	}	
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBullet(int no)
{	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		
	BULLET *bullet = &bulletWk[no];
	// ���_���W�̐ݒ�
	SetVertexBullet(no);
	// rhw�̐ݒ�
	bullet->vertexWk[0].rhw =
	bullet->vertexWk[1].rhw =
	bullet->vertexWk[2].rhw =
	bullet->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	bullet->vertexWk[0].diffuse = 
	bullet->vertexWk[1].diffuse = 
	bullet->vertexWk[2].diffuse = 
	bullet->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);	
	// �e�N�X�`�����W�̐ݒ�
	bullet->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	bullet->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET, 0.0f);
	bullet->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET);
	bullet->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET);
	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBullet(int no)
{	
	BULLET *bullet = &bulletWk[no];			// �o���b�g�̃|�C���^�[��������
	// ���_���W�̐ݒ�	
	bullet->vertexWk[0].vtx = D3DXVECTOR3(bullet->pos.x, bullet->pos.y, 0);
	bullet->vertexWk[1].vtx = D3DXVECTOR3(bullet->pos.x + TEXTURE_BULLET_SIZE_X, bullet->pos.y, 0);
	bullet->vertexWk[2].vtx = D3DXVECTOR3(bullet->pos.x, bullet->pos.y + TEXTURE_BULLET_SIZE_Y, 0);
	bullet->vertexWk[3].vtx = D3DXVECTOR3(bullet->pos.x + TEXTURE_BULLET_SIZE_X, bullet->pos.y + TEXTURE_BULLET_SIZE_Y, 0);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBullet(int no, int cntPattern)
{	
	BULLET *bullet = &bulletWk[no];			// �o���b�g�̃|�C���^�[��������	
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_BULLET;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
	// ���_���W�̐ݒ�
	bullet->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	bullet->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	bullet->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	bullet->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetBullet(D3DXVECTOR3 pos,int atk, int dir)
{
	BULLET *bullet = &bulletWk[0];			// �o���b�g�̃|�C���^�[��������
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == false)			// ���g�p��Ԃ̃o���b�g��������
		{
			bullet->use = true;				// �g�p��Ԃ֕ύX����
			bullet->pos = pos;				// ���W���Z�b�g
			bullet->atk = atk;
			bullet->direction = dir;
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

/*******************************************************************************
�֐���:	BULLET *GetBulletAdr( int pno )
����:	int pno : Bullet�ԍ�
�߂�l:	BULLET *
����:	Bullet�̃A�h���X���擾����
*******************************************************************************/
BULLET *GetBullet(int pno)
{
	return &bulletWk[pno];
}