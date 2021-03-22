//**************************************************************
//UI�̏���(UI.cpp)
//**************************************************************

#include "UI.h"

LPDIRECT3DTEXTURE9 g_pTextureGameUI[62] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;//���_�o�b�t�@�ւ̃|�C���^

UI g_aUI[MAX_UI];
UIserect g_UIserect;

int g_MaxUITexture;
int g_UITexture;

//������
void InitUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^
	int nCntUI;
	g_UIserect.TotalNumber = 0;
	g_MaxUITexture = 0;
	g_UITexture = 0;

	int Textype = 0;
	D3DXVECTOR3 UIpos = D3DXVECTOR3(0.0, 0.0, 0.0);
	float Width = 0;
	float Heigth = 0;

	char aFile[256];
	bool bSet = false;
	bool bSetGameUI = false;

	FILE *pFile = fopen(FILENAMETEXT, "r");

	if (pFile != NULL)//��x�t�@�C����ǂݍ���ŕK�v�ȏ��𔲂�
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "NUM_TEXTURE") == 0)	//NUM_TEXTURE�̕��������������
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_MaxUITexture);
			}
			if (strcmp(&aFile[0], "TEXTURE_FILENAME") == 0) //TEXTURE_FILENAME�̕��������������
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXCreateTextureFromFile(pDevice, &aFile[0], &g_pTextureGameUI[g_UITexture]);
				g_UITexture++;
				g_UIserect.TotalNumber++;
			}
			
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPT�̕��������������
			{
				break;
			}
		}
	}
	fclose(pFile);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGameUI, NULL);
	//���ꂼ��̏�����
	for (nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		g_aUI[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUI[nCntUI].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUI[nCntUI].color = D3DXCOLOR(255, 255, 255, 255);
		g_aUI[nCntUI].ntype;
		g_aUI[nCntUI].bUse = false;

	}
	g_UIserect.Serect = 0;
	g_UIserect.nSetNumber = 0;
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		for (int nVtxUI = 0; nVtxUI < 4; nVtxUI++)
		{
			//���_�̍��W
			pVtx[nVtxUI].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (0.0f / 2), g_aUI[nCntUI].pos.y - (0.0f / 2), 0.0f);

			//rhw�̐ݒ�
			pVtx[nVtxUI].rhw = 1.0f;

			//���_�̐F
			pVtx[nVtxUI].col = g_aUI[nCntUI].color;

			//�e�N�X�`����UV���W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameUI->Unlock();
	//pFile = fopen(FILENAMETEXT, "r");//��x�ڂ̓ǂݍ��݂ŃZ�b�g

	//	if (pFile != NULL)//��x�t�@�C����ǂݍ���ŕK�v�ȏ��𔲂�
	//	{
	//		while (true)
	//		{
	//			fscanf(pFile, "%s", &aFile[0]);
	//			if (bSetGameUI == true)
	//			{
	//				if (bSet == true)
	//				{
	//					if (strcmp(&aFile[0], "TYPE") == 0)	//TYPE�̕��������������
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%d", &Textype);
	//					}
	//					if (strcmp(&aFile[0], "POS") == 0)	//POS�̕��������������
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%f%f%f", &UIpos.x, &UIpos.y, &UIpos.z);
	//					}
	//					if (strcmp(&aFile[0], "WIDTH") == 0)	//WIDTH�̕��������������
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%f", &Width);
	//					}
	//					if (strcmp(&aFile[0], "HEIGTH") == 0)	//HEIGTH�̕��������������
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%f", &Heigth);
	//					}
	//				}

	//				if (strcmp(&aFile[0], "UISET") == 0)	//UISET�̕��������������
	//				{
	//					bSet = true;
	//				}
	//				if (strcmp(&aFile[0], "END_UISET") == 0)	//END_UISET�̕��������������
	//				{
	//					bSet = false;
	//					/*SetUI(UIpos, Width, Heigth, Textype);*/
	//				}
	//			}
	//			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPT�̕��������������
	//			{
	//				break;
	//			}

	//		}
	//	}
	//	fclose(pFile);

	}



//�I������
void UninitUI(void)
{
	int nCntUI;
	for (nCntUI = 0; nCntUI < g_MaxUITexture; nCntUI++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureGameUI[nCntUI] != NULL)
		{
			g_pTextureGameUI[nCntUI]->Release();
			g_pTextureGameUI[nCntUI] = NULL;
		}

		//���_�o�b�t�@���j��
		if (g_pVtxBuffGameUI != NULL)
		{
			g_pVtxBuffGameUI->Release();
			g_pVtxBuffGameUI = NULL;
		}
	}
}

//�X�V����
void UpdateUI(void)
{

}

//�`�揈��
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();//�f�o�C�X���擾����

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureGameUI[g_aUI[nCntUI].ntype]);//�e�N�X�`���̐ݒ�
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);//�|���S���̕`��
		}
	}

}

//UI�̐ݒ菈��
void SetUI(D3DXVECTOR3 pos, float fWidth, float fHeight, int ntype)
{
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].bUse == false)
		{
			g_aUI[nCntUI].pos = pos;//���W�����
			g_aUI[nCntUI].ntype = ntype;//�^�C�v������
			g_aUI[nCntUI].fWidth = fWidth;
			g_aUI[nCntUI].fHeight = fHeight;

			//���_�o�b�t�@�����b�N
			g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
			for (int nVtxUI = 0; nVtxUI <= 4; nVtxUI++)
			{
				pVtx += (nCntUI * 4);
				//���_�̍��W
				pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (fWidth / 2), g_aUI[nCntUI].pos.y - (fHeight / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (fWidth / 2), g_aUI[nCntUI].pos.y - (fHeight / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (fWidth / 2), g_aUI[nCntUI].pos.y + (fHeight / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (fWidth / 2), g_aUI[nCntUI].pos.y + (fHeight / 2), 0.0f);

				//rhw�̐ݒ�
				pVtx[nVtxUI].rhw = 1.0f;

				//���_�̐F
				pVtx[nVtxUI].col = g_aUI[nCntUI].color;

				//�e�N�X�`����UV���W
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			
			//�A�����b�N
			g_pVtxBuffGameUI->Unlock();

			g_aUI[nCntUI].bUse = true;

			g_UIserect.nSetNumber++;

			break;
		}

	}
}

void SaveUI(void)
{

	FILE *pFile = fopen(SAVEFILETEXT, "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "#========================================================================\n");
		fprintf(pFile, "#�Z�b�g����UI�����̃e�L�X�g�t�@�C���ɏ����o���܂�\n");
		fprintf(pFile, "#�ǂݍ��ރe�L�X�g�t�@�C���ɃR�s�y����Ȃ肵�ĉ�����\n");
		fprintf(pFile, "#========================================================================\n\n\n");


			for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
			{
				if (g_aUI[nCntUI].bUse == true)
				{
					fprintf(pFile, "UISET\n");
					fprintf(pFile, "	TYPE = %d\n", g_aUI[nCntUI].ntype);
					fprintf(pFile, "	POS = %.1f %.1f 0.0\n", g_aUI[nCntUI].pos.x, g_aUI[nCntUI].pos.y);
					fprintf(pFile, "	WIDTH = %.1f\n", g_aUI[nCntUI].fWidth);
					fprintf(pFile, "	HEIGTH = %.1f\n", g_aUI[nCntUI].fHeight);
					fprintf(pFile, "END_UISET\n\n");

				}
			}
		
		fclose(pFile);
	}
}

UI *GetUI(void)
{
	return &g_aUI[0];
}

UIserect *GetUIserect(void)
{
	return &g_UIserect;
}