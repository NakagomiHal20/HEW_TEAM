//=============================================================================
//
// �X�e�[�W���� [GameMain.h]
//
//=============================================================================
#ifndef _GAME_MAIN_H_
#define _GAME_MAIN_H_


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// �}�N���E�񋓌^��`
//*****************************************************************************

// ������@�̃r���h�X�C�b�`
// OPERATIN_1, OPERTION_2�ǂ��炩
#define OPERATION_1

// �I�u�W�F�N�g�J�e�S��
enum OBJECT_CATERGORY {
	// �啪��
	STD_GAMEOBJECT = 1,
	STD_UI,

	// ������
	BULLET_KIND,					// STD_GAMEOBJECT

	// ������	
	ORB_TYPE,						// BULLET_KIND
	JIN_TYPE,						// BULLET_KIND
	ITEM_TYPE,						// BULLET_KIND
	CHARACTER_TYPE,
	BOX_TYPE,
	DIRECTIONCURSOR_TYPE,
	WALL_TYPE,
	RAY_TYPE
};

// �Q�[����{�ݒ�
#define GAME_TIME					(1800)					// ��������
#define MAP_X						(18)					// �}�b�vX���W�̐�
#define MAP_Y						(10)					// �}�b�vY���W�̐�

// ����
#define GROUND_POS_Y				(1.0f)					// �n��Y���W
#define GROUND_HITPOS_Y				(0.0f)					// ���n����Y���W
#define VALUE_GROUND_FRICTION		(0.8f)					// �n�㖀�C�W��
#define VALUE_LANDING_FRICTION		(0.2f)					// ���n�����C�W��
#define ORBLANDING_FRICTION			(0.7f)					// ���n�����C�W��
#define ITEM_LANDING_FRICTION		(0.7f)					// �A�C�e�����n�����C�W��
#define ICE_FRICTION				(0.995f)				// �A�C�X���ʎ��̖��C�W��
#define MAKE_ICE_ACCELERATOR		(0.05f)					// �|���ăA�C�X���ʎ��̉����x		

// �_���[�W
#define EXPLOSION_DAMAGE			(20)


//*****************************************************************************
// �񋓌^��`�@�������蔻��֘A�ȊO
//*****************************************************************************
/* GAMEDATA�\���̂̃����o�ϐ��Ɏg�p���� */
// �X�e�[�W�ԍ�
enum STAGE_NO
{
	STAGE_1,				// �X�e�[�W 1
	STAGE_2,				// �X�e�[�W 2
	STAGE_3,				// �X�e�[�W 3
	STAGE_MAX				// �X�e�[�W�̐�
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// GAMEDATA�\����
typedef struct {

	// �Q�[���i�s�t�F�C�Y
	int	phase;

	// ���s
	int winner;							// �o�g���̏��҃L�����N�^�[
	int loser;							// �o�g���̔s�҃L�����N�^�[
	int winnerUser;						// �o�g���̏��҃��[�U�[
	int winnerModel;					// �o�g���̏��҃��f��

	// ����
	int time;							// �o�ߎ��Ԃ��i�[
	DWORD lastTime;						// ���Ԍv�Z�Ɏg�p�i1/10�b�O�̎������i�[����j
	int remainingTime;					// �c�莞�Ԃ��i�[
	BOOL isCountTime;					// TRUE�����Ԍv������ FALSE�����Ԍv�����Ȃ�
	BOOL isTimeUp;						// ���Ԑ؂ꂩ
	BOOL isSignal;						// �V�O�i���\�����邩
	DWORD startTime;					// ���Ԍv�Z�Ɏg�p
	BOOL isPrepareFinish;				// �X�e�[�W�I�������ɓ�������
	DWORD finishTime;				// ���Ԍv�Z�Ɏg�p

	// �A�C�e��
	int	cntItemLoop = 0;				// �A�C�e�����o�ꂷ��܂ł̌o�߃t���[����
	BOOL isIce;							// �A�C�X���ʒ���
	DWORD iceTime;						// �A�C�X�J�n����
	BOOL isWind;						// �E�C���h���ʒ���
	DWORD windTime;						// �E�C���h�J�n����
	D3DXVECTOR3 dirWind;				// �E�C���h�x�N�g��

	// �}�b�v
	int mapX;							// �}�b�vX���W�̐�
	int mapY;							// �}�b�vY���W�̐�
	BOOL isCollisionDraw;				// �����蔻���`�悷�邩
	BOOL isMapNodeDraw;					// �}�b�v�m�[�h��`�悷�邩

}GAMEDATA;


//*****************************************************************************
// �`��\�[�g�֘A��`
//*****************************************************************************


//*****************************************************************************
// �����蔻��֘A��`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGame(int type);				// �Q�[�������̏���������
void UninitGame(void);					// �Q�[�������̏I������
void UpdateGame(void);					// �Q�[�������̍X�V����
void DrawGame(void);					// �Q�[�������̕`�揈��

// �Q�[���f�[�^���擾
GAMEDATA *GetGameData(void);

// �x�N�g���̒�����ݒ�
D3DXVECTOR3 *SetVecLength(D3DXVECTOR3 *vec, float length);

// �ړ�����
void Move(D3DXVECTOR3 *pos, D3DXVECTOR3 *speed, D3DXVECTOR3 *accel, float friction);

// �ǂ���x�N�g������
D3DXVECTOR3 *WallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 normal);

// ���˃x�N�g������
D3DXVECTOR3 *ReflectVector(D3DXVECTOR3 *out, D3DXVECTOR3 front, D3DXVECTOR3 normal);

// �_���[�W����
int Damage(int *hp, int damage);

// �񕜏���
int Recovery(int hpMax, int *hp, int recovery);

#endif
