//=============================================================================
//
// ���͏��� [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// �}�N���E�񋓌^��`
//*****************************************************************************
// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD					// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE						// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD							// �錾����ƃp�b�h�ő���\�ɂȂ�

/* game pad��� */
#define BUTTON_UP		0x00000001l		// �����L�[��(.IY<0)
#define BUTTON_DOWN		0x00000002l		// �����L�[��(.IY>0)
#define BUTTON_LEFT		0x00000004l		// �����L�[��(.IX<0)
#define BUTTON_RIGHT	0x00000008l		// �����L�[�E(.IX>0)

#define BUTTON_A		0x00000010l	
#define BUTTON_B		0x00000020l	
#define BUTTON_X		0x00000080l	
#define BUTTON_Y		0x00000100l		
#define BUTTON_RT		0x00002000l	
#define BUTTON_LT		0x00000200l		
#define BUTTON_L		0x00000400l		
#define BUTTON_R		0x00000800l		
#define BUTTON_SR		0x00000040l	
#define BUTTON_START	0x00001000l	
#define BUTTON_M		0x00004000l
#define GAMEPADMAX		(4)				// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g
#define BUTTON_MAX		(14)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// JOYSTICK�\����
typedef struct{
	BOOL	use;		// �g�p����
	long	axisX;		// X���W
	long	axisY;		// Y���W
	float	rotAngle;	// ��]�p�x
}JOYSTICK;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);


//---------------------------- keyboard
bool GetKeyboardPress(int nKey);		// �L�[�v���X
bool GetKeyboardTrigger(int nKey);		// �L�[�g���K�[
bool GetKeyboardRepeat(int nKey);		// �L�[���s�[�g
bool GetKeyboardRelease(int nKey);		// �L�[�����[�X
bool GetKeyState(void);					//�L�[���̗͂L���𔻒肵TRUE��FALSE��Ԃ�


//---------------------------- mouse
BOOL IsMouseLeftPressed(void);			// ���N���b�N�������
BOOL IsMouseLeftTriggered(void);		// ���N���b�N�����u��
BOOL IsMouseRightPressed(void);			// �E�N���b�N�������
BOOL IsMouseRightTriggered(void);		// �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);		// ���N���b�N�������
BOOL IsMouseCenterTriggered(void);		// ���N���b�N�����u��
long GetMouseX(void);					// �}�E�X��X�����ɓ��������Βl
long GetMouseY(void);					// �}�E�X��Y�����ɓ��������Βl
long GetMouseZ(void);					// �}�E�X�z�C�[�������������Βl


//---------------------------- game pad
BOOL IsButtonPressed(int padNo,DWORD button);	// �{�^���v���X
BOOL IsButtonTriggered(int padNo,DWORD button);	// �{�^���g���K�[
BOOL IsButtonReleased(int padNo, DWORD button);	// �{�^�������[�X
BOOL IsButtonRepeat(int padNo, DWORD button);	// �{�^�����s�[�g
DWORD GetPadState(int no);						// �p�b�h�̏�Ԃ𒲂ׂ�
JOYSTICK *GetStickL(int padNo);					// ���X�e�B�b�N���擾
JOYSTICK *GetStickR(int padNo);					// �E�X�e�B�b�N���擾
int GetPadCount(void);							// �Q�[���p�b�h�̐ڑ��䐔���擾


#endif
