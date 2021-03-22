//=========================================================================================================================
// DirectX_Sample [main.cpp]
// Author:satou syusuke
//=========================================================================================================================
#include "main.h"
#include "input.h"
#include <Windows.h>

//=========================================================================================================================
// �}�N����`
//=========================================================================================================================
#define CLASS_NAME "UISET"	// �E�B���h�E�N���X�̖��O
#define WINDOW_NAME "UISET"		// �E�B���h�E�̖��O
#define WINDOW_COLOR_R	(0)		// �E�B���h�E�̐F(��)
#define WINDOW_COLOR_G	(0)			// �E�B���h�E�̐F(��)
#define WINDOW_COLOR_B	(0)			// �E�B���h�E�̐F(��)
#define WINDOW_COLOR_A	(0)			// �E�B���h�E�̐F(�����x)

//=========================================================================================================================
// �v���g�^�C�v�錾
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							// ����������
void Uninit(void);																	// �I������
void Update(void);																	// �X�V����
void Draw(void);																	// �`�揈��
void DrawFPS(void);																	// 
void DrawTxT(void);

//=========================================================================================================================
// �O���[�o���ϐ�
//=========================================================================================================================
LPDIRECT3D9 g_pD3D = NULL;													// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;										// Direct3D�f�o�C�X�ւ̃|�C���^(�`�揈���ɕK�v)
LPD3DXFONT g_pFont = NULL;													// �t�H���g�ւ̃|�C���^
int g_nCountFPS;															// FPS�J�E���^

//MODE g_mode;

//ModeStyle g_Style;
//=========================================================================================================================
// ���C���֐�
//=========================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	HWND hWnd;		// �E�B���h�E�n���h��(���ʎq)
	MSG  msg;		// ���b�Z�[�W���i�[����
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGTH };


	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);
	
	
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	

	// �E�B���h�E�̐���
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right,
		rect.bottom,
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DWORD dwExecLastTime;	// �������s�ŏI����
	DWORD dwFPSLastTime;	// FPS�v���ŏI����
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwFrameCount;		// �t���[���J�E���g

	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();
	dwCurrentTime = timeGetTime();
	dwFrameCount = 0;

	//���b�Z�[�W���[�v
	while (1)
	{	// Windows�̏���
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// ���b�Z�[�W���擾���Ȃ������ꍇ "0" ��Ԃ�
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}

	// �I������
	Uninit();

	// �E�B���h�E�N���X�̓o�^����
	UnregisterClass(
		CLASS_NAME,
		wcex.hInstance);

	return (int)msg.wParam;
	return 0;
}

//=========================================================================================================================
// �E�B���h�E�v���V�[�W��
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGTH };

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);	// WM_QUIT���b�Z�[�W��Ԃ�
		break;
	case WM_CLOSE:
		
			DestroyWindow(hWnd);	// WM_DESTROY���b�Z�[�W��Ԃ�l
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			
			DestroyWindow(hWnd);	// WM_DESTROY���b�Z�[�W��Ԃ�

			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=========================================================================================================================
// ����������
//=========================================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// DIrect3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// �f�B�X�v���C���[�h�̎擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// �X�N���[���̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGTH;		// �X�N���[���̍���
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h(���l�̍���)�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);			// �\�[�X(�`�挳)�̍������@�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V����(�`���)�̍������@�̐ݒ�

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �k���� ���
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �g�厞 ���
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�l �J��Ԃ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�l �J��Ԃ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// ���l�̍���
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// ���l�̍���
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ���l�̍���

	//�e�L�X�g����
	D3DXCreateFont(g_pD3DDevice, 20, 0, 0, 0,//�傫��
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "Terminal", &g_pFont);


	// ���͂̏�����
	InitKeyboard(hInstance, hWnd);

	InitUI();
	InitUISet();

	SetUISet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGTH / 2, 0.0), 100, 50, 0);

	return S_OK; 

}

//=========================================================================================================================
// �I������
//=========================================================================================================================
void Uninit(void)
{

	UninitKeyboard();
	UninitUI();
	UninitUISet();

	// Direct3D�f�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

}

//=========================================================================================================================
// �X�V����
//=========================================================================================================================
void Update(void)
{
	UpdateKeyboard();
	UpdateUISet();
	UpdateUI();
}

//=========================================================================================================================
// �`�揈��
//=========================================================================================================================
void Draw(void)
{

	// �o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A(��ʃN���A)
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(WINDOW_COLOR_R, WINDOW_COLOR_G, WINDOW_COLOR_B, WINDOW_COLOR_A),
		1.0f,
		0);

	// �`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		DrawUISet();
		DrawUI();
		

		DrawTxT();
		DrawFPS();

		// �`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	
}

//=========================================================================================================================
// �f�o�C�X�̎擾
//=========================================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=========================================================================================================================
// FPS�̕`��
//=========================================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGTH };
	char aStr[256];

	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// �e�L�X�g�̕`��
	g_pFont->DrawText(
		NULL,
		&aStr[0],
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}

void DrawTxT(void)
{
	UISet * pUISet = GetUISet();
	UIserect * pUISerect = GetUIserect();

	RECT rect = { 0, 25, SCREEN_WIDTH, SCREEN_HEIGTH };
	char aStr[400];

	int nNum = sprintf(&aStr[0], "�Z�b�g������W:(%.1f : %.1f)\n", pUISet->pos.x, pUISet->pos.y);
	nNum += sprintf(&aStr[nNum], "�傫��[���F����]:(%.1f : %.1f)\n", pUISet->fWidth, pUISet->fHight);
	nNum += sprintf(&aStr[nNum], "�Z�b�g�ł���UI�̑���:(%d)\n", pUISerect->TotalNumber);
	nNum += sprintf(&aStr[nNum], "�Z�b�g����UI�ԍ�:(%d)\n", pUISerect->TotalNumber);
	nNum += sprintf(&aStr[nNum], "UI���ݒu��:(%d)\n\n", pUISerect->nSetNumber);
	nNum += sprintf(&aStr[nNum], "[WASD]:�\��ꏊ�̈ړ�\n");
	nNum += sprintf(&aStr[nNum], "[Q E]:���̕ύX\n");
	nNum += sprintf(&aStr[nNum], "[Z C]:�����̕ύX\n");
	nNum += sprintf(&aStr[nNum], "[�� ��]:��ނ̕ύX\n");
	nNum += sprintf(&aStr[nNum], "[Enter]:�Z�b�g\n\n");
	nNum += sprintf(&aStr[nNum], "[K]:�ݒu����UI�̍폜\n");
	nNum += sprintf(&aStr[nNum], "[F9]:�Z�b�g����UI�̏����o��\n");
	nNum += sprintf(&aStr[nNum], "�����o�����t�@�C���̃p�X[%s]\n",SAVEFILETEXT);



	// �e�L�X�g�̕`��
g_pFont->DrawText(
		NULL,
		&aStr[0],
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}

//void SetMode(MODE mode)
//{
//
//	g_mode = mode;
//}
//
//MODE GetMode(void)
//{
//	return g_mode;
//}

