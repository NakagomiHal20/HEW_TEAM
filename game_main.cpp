//=============================================================================
// �X�e�[�W���� [GameMain.cpp]
//
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
// �쐬�� : 2018/11/1
//=============================================================================


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "input.h"
#include "sound.h"
#include "debugproc.h"
//#include "game_config.h"
//#include "shadow.h"
//#include "collision.h"
//#include "gameobject.h"
//#include "field.h"
//#include "field_effect.h"
//#include "wall.h"
//#include "model.h"
//#include "character.h"
//#include "orb.h"
//#include "direction_cursor.h"
//#include "effect_locus.h"
//#include "effect_explosion.h"
//#include "effect_wind.h"
//#include "effect_fire.h"
//#include "effect_orb_beacon.h"
//#include "gauge.h"
#include <time.h> 
//#include "score.h"
//#include "number_hp.h"
//#include "face_display.h"
//#include "timer.h"
//#include "item.h"
//#include "map.h" 
//#include "game_bg.h"
//#include "letter_signal.h"
//#include "box.h"
//#include "start_manager.h"
//#include "particle.h"
//#include "score.h"
//#include "title.h"
//#include "result.h" 
#include "game_main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�[���i�s��̊e�t�F�C�Y�̖���
enum PHASE
{
	START,						// �J�n
	FINISH,						// �I��
	PLAY_TIME					// �Q�[����			
};

// �A�C�e���o���p�x�����̍ő�b��
#define ADJUST_ITEM_INTERVAL	(SECOND(10))

// �t�H�O�̍��W
#define START_FOG_POS			(500.0f)
#define END_FOG_POS				(1200.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void StartWork(void);			// START����
BOOL IsVictoryOrDefeat(void);	// �I���Ə��s�̔���
void PreFinishWork(void);		// FINISH�J�ڑO�̏���		
void FinishWork(void);			// FINISH����


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMEDATA	gameData;			// �Q�[���f�[�^�̍쐬


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(int type)
{	
	// �Q�[���f�[�^�̏�����
	gameData.phase = START;
	gameData.winner = -1;
	gameData.loser = -1;
	gameData.winnerUser = -1;
	gameData.winnerModel = -1;
	gameData.isCountTime = TRUE;
	gameData.time = 0;
	gameData.lastTime = 0;
	gameData.remainingTime = GAME_TIME;
	gameData.isTimeUp = FALSE;
	gameData.isSignal = FALSE;
	gameData.startTime = 0;
	gameData.isPrepareFinish = FALSE;
	gameData.finishTime = 0;
	gameData.cntItemLoop = 0;
	gameData.isIce = FALSE;
	gameData.iceTime = 0;
	gameData.isWind = FALSE;
	gameData.windTime = 0;
	gameData.dirWind = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gameData.mapX = MAP_X;
	gameData.mapY = MAP_Y;

#ifdef _DEBUG
	BOOL isCollisionDraw = FALSE;
	BOOL isMapNodeDraw = FALSE;
#endif

	// �Q�[�����W���[���̏�����
	//InitCollisionList(type);	// �Փ˔��胊�X�g�̏���������
	//InitHitList(type);			// �����胊�X�g�̏���������
	//InitGameObject(type);		// �Q�[���I�u�W�F�N�g����������
	//InitBullet(type);			// �o���b�g�̏���������
	//InitShadow(type);			// �e�̏���������
	//InitGameBg(type);			// �Q�[���w�i�̏���������
	//InitField(type);			// �t�B�[���h�̏���������
	//InitFieldEffect(type);		// �t�B�[���h�G�t�F�N�g�̏���������
	//InitBox(type);				// ���̏���������
	//InitModel(type);			// ���f���̏���������
	//InitCharacter(type);		// �L�����N�^�[�̏���������
	//InitOrb(type);				// �I�[�u�̏���������
	//InitItem(type);				// �A�C�e���̏���������
	//InitDirectionCursor(type);	// ���p�J�[�\���̏���������
	//InitWall(type);				// �ǂ̏���������
	//InitLocus(type);			// �O�ՃG�t�F�N�g�̏���������
	//InitExplosion(type);		// �����G�t�F�N�g�̏���������
	//InitFire(type);				// ���G�t�F�N�g�̏���������
	//InitWind(type);				// ���G�t�F�N�g�̏���������
	//InitBeacon(type);			// �I�[�u�r�[�R���̏���������
	//InitGauge(type);			// �Q�[�W�̏���������
	//InitScore(type);			// �X�R�A�̏�����
	//InitNumberHp(type);			// HP�����\���̏���������
	//InitFaceDisplay(type);		// ��\���̏���������
	//InitTimer(type);			// �^�C�}�[�\���̏���������
	//InitLetterSignal(type);		// �V�O�i���̏���������
	//InitMap(type);				// �}�b�v�̏���������

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	//UninitShadow();				// �e�̏I������
	//UninitGameBg();				// �Q�[���w�i�̏I������
	//UninitField();				// �t�B�[���h�̏I������
	//UninitFieldEffect();		// �t�B�[���h�G�t�F�N�g�̏I������
	//UninitWall();				// �ǂ̏I������
	//UninitModel();				// ���f���̏I������
	//UninitCharacter();			// �L�����N�^�[�̏I������
	//UninitOrb();				// �I�[�u�̏I������
	//UninitLocus();				// �O�ՃG�t�F�N�g�̏I������
	//UninitExplosion();			// �����G�t�F�N�g�̏I������
	//UninitFire();				// ���G�t�F�N�g�̏I������
	//UninitWind();				// ���G�t�F�N�g�̏I������
	//UninitBeacon();				// �I�[�u�r�[�R���̏I������
	//UninitDirectionCursor();	// ���p�J�[�\���̏I������
	//UninitGauge();				// �Q�[�W�̏I������
	//UninitScore();				// �X�R�A�̏I������
	//UninitTimer();				// �^�C�}�[�\���̏I������
	//UninitLetterSignal();		// �V�O�i���̏I������
	//UninitItem();				// �A�C�e���̏I������
	//UninitMap();				// �}�b�v�̏I������
	//UninitBullet();				// �o���b�g�̏I������
	//UninitGameObject();			// �Q�[���I�u�W�F�N�g�I������
	//UninitCollisionList();		// �Փ˔��胊�X�g�̏I������
	// ���A���C�j�b�g����������Ȃ�

	//UninitScore();
	//UninitNumberHp();			// HP�����\���̏I������
	//UninitFaceDisplay();		// ��\���̏I������
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	DWORD currentTime = GetTime();			// ���ݎ������擾����
	srand((unsigned)time(NULL));			// �����V�[�h������

	// �o�g��BGM�����[�v�Đ�
	PlayGameSound(BGM_BATTLE_1, CONTINUE, LOOP);

	// �X�e�[�W���Ԍv��
	if (gameData.isCountTime == TRUE)
	{
		// 1/10�b�P�ʂŌo�ߎ������L�^
		if ((currentTime - gameData.lastTime) >= DECI_SECOND)
		{
			gameData.time++;				// 0.�u1�v�b�C���N�������g
			gameData.remainingTime--;		// 0.�u1�v�b�f�N�������g
			gameData.lastTime = currentTime;
		}
	}

	// �X�^�[�g���t�B�j�b�V��
	if (gameData.phase == START)
	{
		StartWork();		// �Q�[���J�n����
	}
	if(gameData.phase == FINISH)
	{
		FinishWork();		// �Q�[���I������
	}

	// �X�V����
	UpdateGameBg();			// �Q�[���w�i�̍X�V����
	UpdateField();			// �t�B�[���h�̍X�V����
	UpdateFieldEffect();	// �t�B�[���h�G�t�F�N�g�̍X�V����
	UpdateShadow();			// �e�̍X�V����
	UpdateCharacter();		// �L�����N�^�[�̍X�V����
	UpdateItem();			// �A�C�e���̍X�V����
	UpdateOrb();			// �I�[�u�̍X�V����
	UpdateLocus();			// �O�ՃG�t�F�N�g�̍X�V����
	UpdateExplosion();		// �����G�t�F�N�g�̍X�V����
	UpdateFire();			// ���G�t�F�N�g�̍X�V����
	UpdateWind();			// ���G�t�F�N�g�̍X�V����
	UpdateBeacon();			// �I�[�u�r�[�R���̍X�V����
	UpdateDirectionCursor();// ���p�J�[�\���̍X�V����
	UpdateBullet();			// �o���b�g�̍X�V����
	UpdateGameObject();		// �Q�[���I�u�W�F�N�g�̍X�V����
	UpdateGauge();			// �Q�[�W�̍X�V����
	UpdateScore();			// �X�R�A�̍X�V����
	UpdateNumberHp();		// HP�����\���̍X�V����
	UpdateFaceDisplay();	// ��\���̍X�V����
	UpdateTimer();			// �^�C�}�[�\���̍X�V����
	UpdateLetterSignal();	// �V�O�i���̍X�V����
	CheckCollision();		// �����蔻��

	// �A�C�e������
	gameData.cntItemLoop++;
	if (gameData.cntItemLoop >SECOND(10) + rand() % ADJUST_ITEM_INTERVAL)
	{
		MAP map = GetMap();
		int itemMapPosX, itemMapPosY;

		do {// ���̏ザ��Ȃ����W���o��܂ŌJ��Ԃ�
			itemMapPosX = rand() % (gameData.mapX);
			itemMapPosY = rand() % (gameData.mapY);

		} while (map[itemMapPosY][itemMapPosX].terrain == OBSTACLE);

		CreateItem(// �A�C�e�����}�b�v���W���ɂɔ���
			D3DXVECTOR3(map[itemMapPosY][itemMapPosX].worldPos.x, 300.0f, map[itemMapPosY][itemMapPosX].worldPos.z),
			ITEM_WIDTH,
			ITEM_HEIGHT);
		gameData.cntItemLoop = 0;
	}

	// �A�C�e������
	if (gameData.isIce)
	{
		if (currentTime > gameData.iceTime
			&&currentTime - gameData.iceTime > ICE_TIME)
		{// �w�莞�Ԍo�߂�����t���O���I�t
			gameData.isIce = FALSE;
		}
	}
	if (gameData.isWind)
	{
		if (currentTime > gameData.windTime
			&&currentTime - gameData.windTime > WIND_TIME)
		{// �w�莞�Ԍo�߂�����t���O���I�t
			gameData.isWind = FALSE;
			StopSound(SE_ITEM_WIND, 0);
		}
	}

	// ���s����
	if(gameData.isPrepareFinish)
	{
		PreFinishWork();
	}
	else if(gameData.isPrepareFinish == FALSE)
	{
		gameData.isPrepareFinish = IsVictoryOrDefeat();
	}

	// �|�[�Y��ʂ̌Ăяo��
	if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START))
	{
		StopAllSound(CONTINUE);
		PlayGameSound(SE_STAGE_PAUSE, FROM_START, NONE);
		SetStage(PAUSE);
	}

	// �}�b�v�m�[�h��`��
	if (IsButtonTriggered(0, BUTTON_M) || GetKeyboardTrigger(DIK_M))
	{
		gameData.isMapNodeDraw = gameData.isMapNodeDraw ? FALSE : TRUE;
	}


#ifdef _DEBUG
	// ���Z�b�g
	if (GetKeyboardTrigger(DIK_DELETE))
	{
		StopAllSound(FROM_START);
		SetStage(TITLE);
	}

	// ���U���g�ɐi��
	if (GetKeyboardTrigger(DIK_R))
	{
		SetStage(RESULT);
	}

	// �����蔻���`��
	if (GetKeyboardTrigger(DIK_T))
	{
		gameData.isCollisionDraw = gameData.isCollisionDraw ? FALSE : TRUE;
	}

	// �ǂ̕`�������
	if (GetKeyboardTrigger(DIK_U))
	{
		WALL *wall = GetWall(0);
		for (int i = 0; i < WALL_MAX; i++, wall++)
		{
			if (wall->stdParam->state & DRAW_PROCESS)
			{
				SetState(&wall->stdParam->state, FALSE, DRAW_PROCESS);
			}
			else
			{
				SetState(&wall->stdParam->state, TRUE, DRAW_PROCESS);
			}
		}
	}

#endif
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 device = GetDevice();

	//�t�H�O�̐ݒ�
	FLOAT StartPos = START_FOG_POS;
	FLOAT EndPos = END_FOG_POS;
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									// �t�H�O�FON
	device->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0xAf, 0xff, 0xff, 0xff));	// �F
	device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);						// ���_���[�h
	device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);						// �e�[�u�����[�h
	device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));					// �J�n�ʒu
	device->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));						// �I���ʒu

	DrawGameBg();			// �Q�[���w�i�̕`�揈��	
	DrawField();			// �t�B�[���h�̕`�揈��
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);									//�t�H�O�FOFF
	DrawFieldEffect();		// �t�B�[���h�G�t�F�N�g�̕`�揈��
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									//�t�H�O�FON
	DrawMap();				// �}�b�v�^�C���̕`��

	DrawWall();				// �ǂ̕`�揈��
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);									//�t�H�O�FOFF
	DrawShadow();			// �e�̕`�揈��
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									//�t�H�O�FON

	DrawBox();				// ���̕`�揈��
	DrawOrb();				// �I�[�u�̕`�揈��
	DrawItem();				// �A�C�e���̕`�揈��
	DrawDirectionCursor();	// ���p�J�[�\���̕`�揈��
	DrawCharacter();		// �L�����N�^�[�̕`�揈��

	// �p�[�e�B�N��
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);									//�t�H�O�FOFF
	DrawLocus();			// �O�ՃG�t�F�N�g�̕`�揈��
	DrawBeacon();			// �I�[�u�r�[�R���̕`�揈��
	DrawExplosion();		// �����G�t�F�N�g�̕`�揈��
	DrawFire();				// ���G�t�F�N�g�̕`�揈��
	DrawWind();				// ���G�t�F�N�g�̕`�揈��
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);									//�t�H�O�FON

	// UI
	DrawGauge();			// �Q�[�W�̕`�揈��
	DrawScore();			// �X�R�A�̕`�揈��
	DrawNumberHp();			// HP�����\���̕`�揈��
	DrawFaceDisplay();		// ��\���̕`�揈��
	DrawTimer();			// �^�C�}�[�\���̕`�揈��
	DrawLetterSignal();		// �V�O�i���̕`�揈��
}


//=============================================================================
// �Q�[���f�[�^�擾
//=============================================================================
GAMEDATA *GetGameData(void)
{
	return	&gameData;
}


//=============================================================================
// START����
//=============================================================================
void StartWork(void)
{
	// �X�^�[�g�������Ԃ��J�E���g
	gameData.startTime++;

	PlayGameSound(SE_BATTLE_START, CONTINUE, NONE);

	if (SECOND(0) < gameData.startTime && gameData.startTime <= SECOND(1))
	{
		if (gameData.isSignal == FALSE)
		{
			SetSignal(SIGNAL_START, TRUE);
			gameData.isSignal = TRUE;
		}
	}
	else if (gameData.startTime > SECOND(3))
	{
		if (gameData.isSignal)
		{
			SetSignal(SIGNAL_START, FALSE);
			gameData.isSignal = FALSE;
		}
		StopSound(SE_BATTLE_START, FROM_START);
		gameData.phase = PLAY_TIME;
	}
}


//=============================================================================
// ���s�̔���
//=============================================================================
BOOL IsVictoryOrDefeat(void)
{
	CHARACTER *character0 = GetCharacter(0);
	CHARACTER *character1 = GetCharacter(1);
	BOOL flag = -1;

	// HP�؂�ɂ�鏟�s
	if (character0->hp == 0 && character1->hp == 0)
	{// ������HP0�̏ꍇ�̓����_���ŏ��s����
		flag = rand() % 2;
	}
	else if (character1->hp == 0)
	{
		flag = 0;
	}
	else if (character0->hp == 0)
	{
		flag = 1;
	}

	// �^�C���A�b�v�ɂ�鏟�s
	if (gameData.remainingTime <= 0)
	{// ����HP�Ń^�C�v�A�b�v�̏ꍇ�̓����_���ŏ��s����
		gameData.isTimeUp = TRUE;	// ���Ԑ؂�t���O���I��

		if (character0->hp == character1->hp)
		{// 
			flag = rand() % 2;
		}
		else if (character0->hp > character1->hp)
		{
			flag = 0;
		}
		else if (character0->hp < character1->hp)
		{
			flag = 1;
		}
	}

	// ���s�̐ݒ�
	if (flag == 0)
	{
		gameData.winner = character0->typeNo;
		gameData.loser = character1->typeNo;
		return TRUE;
	}
	else if (flag == 1)
	{
		gameData.winner = character1->typeNo;
		gameData.loser = character0->typeNo;
		return TRUE;
	}

	return FALSE;
}


//=============================================================================
// FINISH�J�ڑO�̏���
//=============================================================================
void PreFinishWork(void)
{
	if (GetGauge(gameData.loser)->displayLength != 0.0f && gameData.remainingTime > 0) return;

	// �f�[�^�ゾ���łȂ��Q�[�W�̌����ڂƂ��Ă����S��HP���؂ꂽ��
	// �������̓^�C���A�b�v���Ă�����

	// �������[�U�[�������f�������
	CONFIGTABLE config = GetConfigTable();
	for (int i = 0; i < USER_MAX; i++)
	{
		if (config.table[i][CHARACTER_SELECT] == gameData.winner)
		{
			gameData.winnerModel = config.table[i][MODEL_SELECT];
			gameData.winnerUser = i;
		}
	}

	// �s�҂̑���ؒf
	CHARACTER *character = GetCharacter(gameData.loser);
	if ((character->stdParam->state & AIR) == FALSE)
	{
		SetState(&character->stdParam->state, FALSE, UPDATE_PROCESS);
	}
	GetDirectionCursor(gameData.loser)->use = FALSE;

	gameData.isCountTime = FALSE;	// ���Ԍv���I��
	gameData.phase = FINISH;		// �J��
}


//=============================================================================
// FINISH����
//=============================================================================
void FinishWork(void)
{

	// �V�O�i���̐ݒ�
	if (gameData.isTimeUp)
	{
		SetSignal(SIGNAL_TIMEUP, TRUE);
	}
	else
	{
		SetSignal(SIGNAL_FINISH, TRUE);
	}
	gameData.isSignal = TRUE;

	PlayGameSound(SE_BATTLE_FINISH, CONTINUE, NONE);

	// �t�B�j�b�V���������Ԃ��J�E���g
	gameData.finishTime++;

	if (SECOND(3) < gameData.finishTime && gameData.finishTime <= SECOND(5))
	{
		StopSound(SE_BATTLE_FINISH, FROM_START);
	}
	if (gameData.finishTime > SECOND(5))
	{
		StopAllSound(FROM_START);
		SetStage(RESULT);
	}
}


//=============================================================================
// �ړ�����
//=============================================================================
void Move(D3DXVECTOR3 *pos, D3DXVECTOR3 *speed, D3DXVECTOR3 *accelerator, float friction)
{
	// �����x�̉e���𑬓x�ɗ^����
	D3DXVec3Add(speed, speed, accelerator);

	// ���x�ɖ��C�̉e����^����
	D3DXVec3Scale(speed, speed, friction);

	// ���x�̉e�������W�ɗ^����
	D3DXVec3Add(pos, pos, speed);

	return;
}


//=============================================================================
// �x�N�g���̒�����ݒ�
//=============================================================================
D3DXVECTOR3 *SetVecLength(D3DXVECTOR3 *vec, float length)
{
	// �x�N�g���𐳋K��
	D3DXVec3Normalize(vec, vec);

	// �P�ʃx�N�g���ɒ������|���ăX�P�[��
	D3DXVec3Scale(vec, vec, length);

	return vec;
}


//=============================================================================
// �ǂ���x�N�g������
// out : �ǂ���x�N�g���i�߂�l�j
// front : �i�s�x�N�g��
// normal: �Փ˓_�ł̖@���x�N�g��
//=============================================================================
D3DXVECTOR3 *WallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 nor)
{
	float len = D3DXVec3Length(&front);		// �x�N�g���̒�����ۊ� 
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &nor);
	D3DXVec3Normalize(out, &(front - D3DXVec3Dot(&front, &normal) * normal));
	*out *= len;							// ���߂����˒P�ʃx�N�g���ɒ������|����
	return out;
}


//=============================================================================
// ���˃x�N�g������
// out : ���˃x�N�g���i�߂�l�j
// front : �i�s�x�N�g��
// normal: �Փ˓_�ł̖@���x�N�g��
//=============================================================================
D3DXVECTOR3 *ReflectVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 nor)
{
	float len = D3DXVec3Length(&front);		// �x�N�g���̒�����ۊ� 
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal, &nor);
	D3DXVec3Normalize(out, &(front - 2.0f * D3DXVec3Dot(&front, &normal) * normal));
	*out *= len;							// ���߂����˒P�ʃx�N�g���ɒ������|����
	return out;
}


//=============================================================================
// �_���[�W����
//=============================================================================
int Damage(int *hp, int damage)
{
	// HP����_���[�W������
	*hp -= damage;

	// 0�ȉ���HP��0�ɖ߂�
	if (*hp < 0)
	{
		*hp = 0;
	}

	return *hp;
}


//=============================================================================
// ���J�o������
//=============================================================================
int Recovery(int hpMax, int *hp, int recovery)
{
	// HP�𑫂�
	*hp += recovery;

	// �ő�l���z����HP�͍ő�l��
	if (*hp > hpMax)
	{
		*hp = hpMax;
	}

	return *hp;
}
