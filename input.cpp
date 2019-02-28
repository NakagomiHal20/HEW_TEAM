//=============================================================================
//
// ���͏��� [input.cpp]
// Author : GP11B341 24 �����a�P
//
//=============================================================================


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)			// �L�[�ő吔

// ���s�[�g�p
#define FIRST_REPEAT_COUNT	(25)			// �J�E���^1
#define SECOND_REPEAT_COUNT	(7)				// �J�E���^2

// game pad�p�ݒ�l
#define DEADZONE			(2500)			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX			(1000)			// �L���͈͂̍ő�l
#define RANGE_MIN			(-1000)			// �L���͈͂̍ŏ��l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);			// �L�[�{�[�h�̏�����
void UninitKeyboard(void);									// �L�[�{�[�h�I������
HRESULT UpdateKeyboard(void);								// �L�[�{�[�h�̍X�V����

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow);		// �}�E�X�̏�����
void UninitMouse();											// �}�E�X�̏I������
HRESULT UpdateMouse();										// �}�E�X�̍X�V����

HRESULT InitializePad(void);								// �p�b�h������
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);// �p�b�h�����R�[���o�b�N
void UninitPad(void);										// �p�b�h�̏I������
void UpdatePad(void);										// �p�b�h�̍X�V����


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//------------------------------- keyboard
LPDIRECTINPUT8			DInput = NULL;						// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	DIDevKeyboard = NULL;				// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					keyState[NUM_KEY_MAX];				// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						keyStateRepeatCnt[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g�J�E���^

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 mouse = NULL;					// mouse
static DIMOUSESTATE2	mouseState;							// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2	mouseTrigger;						// �����ꂽ�u�Ԃ���ON

//--------------------------------- game pad
static LPDIRECTINPUTDEVICE8	gamePad[GAMEPADMAX] = {NULL,NULL,NULL,NULL};// �p�b�h�f�o�C�X
static DWORD			padState[GAMEPADMAX];				// �p�b�h���i�����Ή��j
DWORD					lastPadState[GAMEPADMAX];			// ���O�̃p�b�h���i�����Ή��j
static DWORD			padTrigger[GAMEPADMAX];				// �g���K�[���
static DWORD			padRepeat[GAMEPADMAX];				// ���s�[�g���
static int				padCount = 0;						// ���o�����p�b�h�̐�

int						padRepeatCnt1[GAMEPADMAX][BUTTON_MAX];// ���s�[�g�J�E���^1
int						padRepeatCnt2[GAMEPADMAX];			// ���s�[�g�J�E���^2

JOYSTICK				stickL[GAMEPADMAX];					// ���X�e�B�b�N�\����
JOYSTICK				stickR[GAMEPADMAX];					// �E�X�e�B�b�N�\����


//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!DInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&DInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

 	// �}�E�X�̏�����
	//InitializeMouse(hInst, hWnd);
	
	// �p�b�h�̏�����
	InitializePad();

	return S_OK;
}


//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	//UninitMouse();

	// �p�b�h�̏I������
	UninitPad();

	if(DInput)
	{
		DInput->Release();
		DInput = NULL;
	}
}


//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	//UpdateMouse();
	
	// �p�b�h�̍X�V
	UpdatePad();
}


//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = DInput->CreateDevice(GUID_SysKeyboard, &DIDevKeyboard, NULL);
	if(FAILED(hr) || DIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = DIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	DIDevKeyboard->Acquire();

	return S_OK;
}


//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if(DIDevKeyboard)
	{
		DIDevKeyboard->Release();
		DIDevKeyboard = NULL;
	}
}


//=============================================================================
// �L�[�{�[�h�̍X�V
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = DIDevKeyboard->GetDeviceState(sizeof(keyState), keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			keyStateTrigger[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & keyState[cnt];
			keyStateRelease[cnt] = (keyStateOld[cnt] ^ keyState[cnt]) & ~keyState[cnt];
			keyStateRepeat[cnt] = keyStateTrigger[cnt];

			if(keyState[cnt])
			{
				keyStateRepeatCnt[cnt]++;
				if(keyStateRepeatCnt[cnt] >= FIRST_REPEAT_COUNT)
				{
					/* �����ŒǋL : ��x��~������̃��s�[�g�̑��x��x�����鏈��*/
					padRepeatCnt2[0]++;
					if (padRepeatCnt2[0] >= SECOND_REPEAT_COUNT )
					{
						padRepeatCnt2[0] = 0;
						keyStateRepeat[cnt] = keyState[cnt];
					}
				}
			}
			else
			{
				keyStateRepeatCnt[cnt] = 0;
				keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		DIDevKeyboard->Acquire();
	}

	return S_OK;
}


//----------------------------------------------- ����

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (keyState[key] & 0x80) ? true : false;
}


//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (keyStateTrigger[key] & 0x80) ? true : false;
}


//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (keyStateRepeat[key] & 0x80) ? true : false;
}


//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// �L�[���͔���֐�
//=============================================================================
bool GetKeyState(void)
{
	bool flag = FALSE;
	for (int i = 0; i < NUM_KEY_MAX; i++)
	{
		if (keyState[i] != NULL)
		{
			flag = TRUE;
		}
	}
	return flag;
}


//////////////////////////////////// Mouse ////////////////////////////////////

//=============================================================================
// �}�E�X�̏�����
//=============================================================================
HRESULT InitializeMouse(HINSTANCE hInst,HWND hWindow)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = DInput->CreateDevice(GUID_SysMouse,&mouse,NULL);
	if(FAILED(result) || mouse==NULL)
	{
		MessageBox(hWindow,"No mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = mouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Can't setup mouse","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = mouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse mode error","Warning",MB_OK | MB_ICONWARNING);
		return result;
	}
	
	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;
	
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = mouse->SetProperty(DIPROP_AXISMODE,&prop.diph);
	if(FAILED(result))
	{
		MessageBox(hWindow,"Mouse property error","Warning",MB_OK | MB_ICONWARNING);
		return result;	
	}
	
	// �A�N�Z�X���𓾂�
	mouse->Acquire();
	return result;
}


//=============================================================================
// �}�E�X�̏I������
//=============================================================================
void UninitMouse()
{
	if(mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = NULL;
	}

}


//=============================================================================
// �}�E�X�̍X�V����
//=============================================================================
HRESULT UpdateMouse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = mouseState;
	// �f�[�^�擾
	result = mouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// �}�E�X�̃{�^�����
		for(int i=0;i<8;i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = mouse->Acquire();
	}
	return result;
	
}


//----------------------------------------------- ����

//=============================================================================
// ���v���X
//=============================================================================
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
}


//=============================================================================
// ���N���b�N
//=============================================================================
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}


//=============================================================================
// �E�v���X
//=============================================================================
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}


//=============================================================================
// �E�N���b�N
//=============================================================================
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}


//=============================================================================
// �z�C�[���v���X
//=============================================================================
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}


//=============================================================================
// �z�C�[���N���b�N
//=============================================================================
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}


//=============================================================================
// �}�E�X���W�擾
//=============================================================================
// X���W
long GetMouseX(void)
{
	return mouseState.lX;
}

// Y���W
long GetMouseY(void)
{
	return mouseState.lY;
}

// Z���W
long GetMouseZ(void)
{
	return mouseState.lZ;
}


//////////////////////////////////Game Pad/////////////////////////////////////

//=============================================================================
// �R�[���o�b�N�֐�
//=============================================================================

BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID )
{
	HRESULT result;

	result = DInput->CreateDevice(lpddi->guidInstance, &gamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}


//=============================================================================
// �Q�[���p�b�h�̏�����
//=============================================================================
HRESULT InitializePad(void)	
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// �W���C�p�b�h��T��
	DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for ( i = 0 ; i < padCount; i++ ) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = gamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(result) )
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		result = gamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_X; 
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj		= DIJOFS_RZ;
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Y;
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj		= DIJOFS_Z;
		gamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		// X���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_X;
		gamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		// Y���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Y;
		gamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		// �W���C�X�e�B�b�N���͐���J�n
		gamePad[i]->Acquire();

		// �X�e�B�b�N�g�p�t���O��������
		stickL[i].use = FALSE;
		stickR[i].use = FALSE;
	}

	return true;
}


//=============================================================================
// �Q�[���p�b�h�̏I��
//=============================================================================
void UninitPad(void)
{
	for (int i = 0 ; i < GAMEPADMAX ; i++) {
		if ( gamePad[i] )
		{
			gamePad[i]->Unacquire();
			gamePad[i]->Release();
		}
	}

}


//=============================================================================
// �Q�[���p�b�h�̍X�V
//=============================================================================
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs[GAMEPADMAX];
	int				i;

	for (i = 0; i < padCount; i++ ) 
	{
		lastPadState[i] = padState[i];
		padState[i] = 0x00000000l;	// ������

		result = gamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if ( FAILED(result) ) {
			result = gamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = gamePad[i]->Acquire();
		}

		result = gamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs[i]);	// �f�o�C�X��Ԃ�ǂݎ��
		if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED ) {
			result = gamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = gamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if ( dijs[i].lY < 0 )					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if ( dijs[i].lY > 0 )					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if ( dijs[i].lX < 0 )					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if ( dijs[i].lX > 0 )					padState[i] |= BUTTON_RIGHT;
		//* 1 - X�{�^��
		if ( dijs[i].rgbButtons[0] & 0x80 )		padState[i] |= BUTTON_X;
		//* 2 - Y�{�^��
		if ( dijs[i].rgbButtons[1] & 0x80 )		padState[i] |= BUTTON_Y;
		//* 3 - A�{�^��
		if ( dijs[i].rgbButtons[2] & 0x80 )		padState[i] |= BUTTON_A;
		//* 4 - B�{�^��
		if ( dijs[i].rgbButtons[3] & 0x80 )		padState[i] |= BUTTON_B;
		//* 5 - L�{�^��
		if (dijs[i].rgbButtons[4] & 0x80)		padState[i] |= BUTTON_L;
		//* 6 - R�{�^��
		if ( dijs[i].rgbButtons[5] & 0x80)		padState[i] |= BUTTON_R;
		//* 7 - LT�{�^��
		if (dijs[i].rgbButtons[6] & 0x80)		padState[i] |= BUTTON_LT;
		//* 8 - RT�{�^��
		if (dijs[i].rgbButtons[7] & 0x80)		padState[i] |= BUTTON_RT;
		//* 10 - R�X�e�B�b�N�{�^��
		if ( dijs[i].rgbButtons[9] & 0x80 )		padState[i] |= BUTTON_SR;
		//* 12 - START�{�^��
		if ( dijs[i].rgbButtons[11] & 0x80)		padState[i] |= BUTTON_START;
		//* 13 - Guide�{�^��
		if ( dijs[i].rgbButtons[12] & 0x80)		padState[i] |= BUTTON_M;


		// *Trigger�ݒ�
		padTrigger[i] = ((lastPadState[i] ^ padState[i])	// �O��ƈ���Ă���
						& padState[i]);						// ��������ON�̂��

		// *Repeat�ݒ�
		padRepeat[i] = padTrigger[i];
		// ������Ă���{�^���̃��s�[�g�J�E���^���񂷁@������Ă��Ȃ��{�^����0�ɖ߂�
		padState[i] & BUTTON_UP		?	padRepeatCnt1[i][0]++ : padRepeatCnt1[i][0] = 0;
		padState[i] & BUTTON_DOWN	?	padRepeatCnt1[i][1]++ : padRepeatCnt1[i][1] = 0;
		padState[i] & BUTTON_LEFT	?	padRepeatCnt1[i][2]++ : padRepeatCnt1[i][2] = 0;
		padState[i] & BUTTON_RIGHT	?	padRepeatCnt1[i][3]++ : padRepeatCnt1[i][3] = 0;
		for (int j = 0; j < BUTTON_MAX; j++)
		{
			if (padRepeatCnt1[i][j] >= FIRST_REPEAT_COUNT)
			{
				padRepeatCnt2[i]++;
				/* ��x��~������̃��s�[�g�̑��x��x�����鏈�� */
				if (padRepeatCnt2[i] >= SECOND_REPEAT_COUNT)
				{
					padRepeatCnt2[i] = 0;
					padRepeat[i] = padState[i];
				}
			}
		}

		// ���e�B�b�N
		stickL[i].axisX = dijs[i].lX;
		stickL[i].axisY = dijs[i].lY;
		if (dijs[i].lX == 0 && dijs[i].lY == 0)
		{
			stickL[i].use = FALSE;
		}
		else
		{
			// �X�e�B�b�N���͎��ɍ��W�E��]�p�x���L�^
			stickL[i].use = TRUE;
			stickL[i].rotAngle = (float)atan2(-dijs[i].lY, dijs[i].lX);
		}

		// �E�X�e�B�b�N
		stickR[i].axisX = dijs[i].lZ;
		stickR[i].axisY = dijs[i].lRz;
		if (dijs[i].lZ == 0 && dijs[i].lRz == 0)
		{
			stickR[i].use = FALSE;
		}
		else
		{
			// �X�e�B�b�N���͎��ɉ�]�p�x���L�^
			stickR[i].use = TRUE;
			stickR[i].rotAngle = (float)atan2(-dijs[i].lRz, dijs[i].lZ);
		}
	}
}


//----------------------------------------------- ����

//=============================================================================
// �{�^���v���X
//=============================================================================
BOOL IsButtonPressed(int padNo,DWORD button)
{
	return (button & padState[padNo]);
}


//=============================================================================
// �{�^���g���K�[
//=============================================================================
BOOL IsButtonTriggered(int padNo,DWORD button)
{
	return (button & padTrigger[padNo]);
}


//=============================================================================
// �{�^�������[�X
//=============================================================================
BOOL IsButtonReleased(int padNo, DWORD button)
{
	if (!(padState[padNo] & button)				// ���ׂ����{�^����������Ă��Ȃ�
		&& lastPadState[padNo] & button)		// �����O�͉�����Ă����ꍇ
	{
		return TRUE;
	}
	return FALSE;
}


//=============================================================================
// �{�^�����s�[�g
//=============================================================================
BOOL IsButtonRepeat(int padNo, DWORD button)
{	
	if (button & padRepeat[padNo])
	{
		padRepeat[padNo] = 0x00000000l;			// ������
		return TRUE;
	}
	return FALSE;
}


//=============================================================================
// �Q�[���p�b�h�̏�Ԏ擾
//=============================================================================
DWORD GetPadState(int no)
{
	return padState[no];
}


//=============================================================================
// ���X�e�B�b�N�擾�֐�
//=============================================================================
JOYSTICK *GetStickL(int padNo)
{
	return &stickL[padNo];
}


//=============================================================================
// �E�X�e�B�b�N�擾�֐�
//=============================================================================
JOYSTICK *GetStickR(int padNo)
{
	return &stickR[padNo];
}


//=============================================================================
// �Q�[���p�b�h�̐ڑ��䐔���擾
//=============================================================================
int GetPadCount(void)
{
	return padCount;
}