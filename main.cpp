//=============================================================================
// ���C������ [main.cpp]
//
// Author : GP11B341 24 �����a�P
// �쐬�� : 2019/2/25
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include <stdio.h>
//#include "title.h"
#include "font.h"
#include "debugproc.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "bg.h"
#include "road.h"
#include "timer.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"Team Adult Game"	// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"Team Adult Game"	// �E�C���h�E�̃L���v�V������


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			Direct3D = NULL;		// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)
STAGE				stage;					// ���݂̃X�e�[�W
DWORD				currentTime;			// ���݂̃V�X�e������
#ifdef _DEBUG
int					cntFPS;					// FPS�J�E���^
#endif


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv��
	DWORD execLastTime;
	DWORD fpsLastTime;
	DWORD frameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(NULL)),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	execLastTime = 
	fpsLastTime = timeGetTime();
	currentTime = 0;
	frameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}
        }
		else
		{
			currentTime = timeGetTime();
			if((currentTime - fpsLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				cntFPS =frameCount * 1000 / (currentTime - fpsLastTime);
#endif
				fpsLastTime = currentTime;
				frameCount = 0;
			}

			if((currentTime - execLastTime) >= (1000 / FPS))
			{
				execLastTime = currentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				frameCount++;
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}


//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(Direct3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(Direct3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(Direct3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if(FAILED(Direct3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// �Q�[���V�X�e���̏�����
	InitInput(hInstance, hWnd);	// ���͏����̏�����
	InitSound(hWnd);			// �T�E���h�̏�����
	InitFont();					// �t�H���g�̏�����
	InitPlayer(0);				// �v���C���[�̏�����
	InitBullet(0);				// �o���b�g�̏�����
	InitEnemy(0);				// ENEMY�̏�����
	InitBg(0);					// BG�̏�����
	InitRoad(0);				// ���̏�����
	InitTimer(0);				// �^�C�}�[�̏�����


#ifdef _DEBUG
	InitDebugProc();			// �f�o�b�O�\���̏�����
#endif

	// ���ʒ���
	//GetSound(BGM_BATTLE_1)->SetVolume(-200);

	// �ŏ��̃X�e�[�W��ݒ�
	//stage = TITLE;
	stage = GAME;

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	UninitDebugProc();			// �f�o�b�O�\���̏I��
#endif

	//UninitTitle();			// �^�C�g���̏I������
	UninitInput();				// ���͏����̏I������
	UninitSound();				// �T�E���h�̏I������
	UninitFont();				// �t�H���g�̏I������
	UninitPlayer();				// �v���C���[�̏I������
	UninitBullet();				// �o���b�g�̏I������
	UninitEnemy();				// ENEMY�̏I������
	UninitBg();					// BG�̏I������
	UninitRoad();				// ���̏I������
	UninitTimer();				// �^�C�}�[�̏I������

	// �f�o�C�X�̊J��
	SAFE_RELEASE(pD3DDevice);
	// Direct3D�I�u�W�F�N�g�̊J��
	SAFE_RELEASE(Direct3D);
	//if(pD3DDevice != NULL)
	//{
	//	pD3DDevice->Release();
	//	pD3DDevice = NULL;
	//}
	//
	//if(Direct3D != NULL)
	//{
	//	Direct3D->Release();
	//	Direct3D = NULL;
	//}
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	UpdateInput();					// ���͍X�V		
	switch (stage)
	{
	case SPLASH:
		break;

	case TITLE:
		break;

	case TUTORIAL:
		break;

	case GAME:
		UpdatePlayer();				// �v���C���[�̍X�V
		UpdateBullet();				// �o���b�g�̍X�V
		UpdateEnemy();				// ENEMY�̍X�V
		UpdateBg();					// BG�̍X�V
		UpdateRoad();				// ���̍X�V
		UpdateTimer();				// �^�C�}�[�̍X�V
		break;

	case PAUSE:
		break;

	case RESULT:
		break;

	case EXIT:
		PostQuitMessage(0);
		break;
	}

#ifdef _DEBUG
	UpdateDebugProc();			// �f�o�b�O�\���̍X�V
#endif
}


//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�̃N���A
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(pD3DDevice->BeginScene()))
	{
		switch (stage)
		{
		case SPLASH:
			break;

		case TITLE:
			break;

		case TUTORIAL:
			break;

		case GAME:
			DrawBg();					// BG�̕`��
			DrawRoad();					// ���̕`��
			DrawPlayer();				// �v���C���[�̕`��
			DrawBullet();				// �o���b�g�̕`��
			DrawEnemy();				// ENEMY�̕`��
			
			// UI
			DrawTimer();				// �^�C�}�[�̕`��
			break;

		case PAUSE:
			break;

		case RESULT:
			break;

		case EXIT:
			break;
		}

#ifdef _DEBUG
		DrawDebugProc();		// �f�o�b�O�\���̕`��
#endif

		// Direct3D�ɂ��`��̏I��
		pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//=============================================================================
// �f�o�C�X�擾�֐�
//-----------------------------------------------------------------------------
// �߂�l�FLPDIRECT3DDEVICE9 �f�o�C�X�ւ̃|�C���^
// ����  �Fvoid
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(pD3DDevice);
}


//=============================================================================
// �X�e�[�W��ݒ�
//-----------------------------------------------------------------------------
// �߂�l�Fvoid
// ����  �FSTAGE�@�ݒ肷��X�e�[�W
//=============================================================================
void SetStage(STAGE set)
{
	stage = set;
}


//=============================================================================
// ���ݎ����擾�擾
//-----------------------------------------------------------------------------
// �߂�l�FDWORD�@���݂̃V�X�e������
// ����  �Fvoid
//=============================================================================
DWORD GetTime(void)
{
	return currentTime;
}


#ifdef _DEBUG
//=============================================================================
// FPS�擾
//-----------------------------------------------------------------------------
// �߂�l�Fint�@���݂̃t���[�����[�g
// ����  �Fvoid
//=============================================================================
int GetFPS(void)
{
	return cntFPS;
}
#endif
