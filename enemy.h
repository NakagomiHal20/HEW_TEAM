//=============================================================================
//
// �|���S������ [enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

#define TEXTURE_GAME_ENEMY1		_T("data/TEXTURE/enemy1.png")
#define TEXTURE_GAME_ENEMY2		_T("data/TEXTURE/enemy2.png")
#define TEXTURE_ENEMY_SIZE_X	(140) // �e�N�X�`���T�C�Y
#define TEXTURE_ENEMY_SIZE_Y	(200) // ����

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define ENEMY_MAX	(10)			// �v���C���[�̐�
/*******************************************************************************
* �\���̒�`
********************************************************************************/
struct	ENEMY				// �L�����N�^�[�̊�{�\����
{
	CHARACTER				status;			// ���ʃp�����[�^�[
	bool					use;			// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;			// �|���S���̈ړ���
	int						patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	float					countAnim;		// �A�j���[�V�����J�E���g	
	int						type;
	VERTEX_2D				vtx[NUM_VERTEX];// ���_���i�[���[�N
	float					radius;			// �|���S���̊p�x	
	float					baseAngle;
	int						direction = -1;
	int						moving_cooldown = -1;
	int						speed;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int type);
void	UninitEnemy(void);
void	UpdateEnemy(void);
void	DrawEnemy(void);
ENEMY   *GetEnemy(int pno);	// �v���C���[��(�A�h���X)���擾
#endif