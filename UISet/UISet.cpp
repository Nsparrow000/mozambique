//**************************************************************
//UIの処理(UI.cpp)
//**************************************************************

#include "UISet.h"

#define FILENAMETEXT "data/UINameRead.txt"	//読み込むスクリプトファイル名
#define TIME (20)
#define PRESSTIME (20)
#define PRSTIME (3)

LPDIRECT3DTEXTURE9 g_pTextureUISet[32] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUISet = {};//頂点バッファへのポインタ

UISet g_aUISet[MAX_UI];
UISetSerect g_UISetserect;

int g_MaxUISetTexture;
int g_UISetTexture;
int g_nCntTime = TIME;

float g_Color;

bool g_bTimeX;
bool g_bTimeY;
bool g_bPressTimeX;
bool g_bPressTimeY;

int g_nPressTimeY;
int g_nPressTimeX;
int g_nPrsTime;

//初期化
void InitUISet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ
	int nCntUISet;

	g_MaxUISetTexture = 0;
	g_UISetTexture = 0;

	g_bPressTimeX = false;
	g_bPressTimeY = false;

	g_bTimeY = true;
	g_bTimeX = true;

	int Textype = 0;
	D3DXVECTOR3 UIpos = D3DXVECTOR3(0.0, 0.0, 0.0);
	float Width = 0;
	float Heigth = 0;

	g_nPressTimeY = PRESSTIME;
	g_nPressTimeX = PRESSTIME;
	g_nPrsTime = PRSTIME;

	char aFile[256];
	g_Color = 255;
	FILE *pFile = fopen(FILENAMETEXT, "r");

	if (pFile != NULL)//一度ファイルを読み込んで必要な情報を抜く
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "NUM_TEXTURE") == 0)	//NUM_TEXTUREの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_MaxUISetTexture);
			}
			if (strcmp(&aFile[0], "TEXTURE_FILENAME") == 0) //TEXTURE_FILENAMEの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXCreateTextureFromFile(pDevice, &aFile[0], &g_pTextureUISet[g_UISetTexture]);
				g_UISetTexture++;
			}

			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}
		}
	}
	fclose(pFile);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffUISet, NULL);

	//それぞれの初期化
	for (nCntUISet = 0; nCntUISet < MAX_UI; nCntUISet++)
	{
		g_aUISet[nCntUISet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUISet[nCntUISet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUISet[nCntUISet].ntype;
		g_aUISet[nCntUISet].bUse = false;
		g_aUISet[nCntUISet].fHight = 0;
		g_aUISet[nCntUISet].fWidth = 0;
		g_aUISet[nCntUISet].colorA = 255;
	}

	g_UISetserect.Serect = 0;

	VERTEX_2D*pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffUISet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUISet = 0; nCntUISet < MAX_UI; nCntUISet++)
	{
		for (int nVtxUI = 0; nVtxUI < 4; nVtxUI++)
		{
			//頂点の座標
			pVtx[nVtxUI].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x - (0.0f / 2), g_aUISet[nCntUISet].pos.y - (0.0f / 2), 0.0f);

			//rhwの設定
			pVtx[nVtxUI].rhw = 1.0f;

			//頂点の色
			pVtx[nVtxUI].col = D3DXCOLOR(255, 255, 255, 255);

			//テクスチャのUV座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffUISet->Unlock();
	
}

//終了処理
void UninitUISet(void)
{
	int nCntUISet;
	for (nCntUISet = 0; nCntUISet > g_MaxUISetTexture; nCntUISet++)
	{
		//テクスチャの破棄
		if (g_pTextureUISet[nCntUISet] != NULL)
		{
			g_pTextureUISet[nCntUISet]->Release();
			g_pTextureUISet[nCntUISet] = NULL;
		}
		//頂点バッファも破棄
		if (g_pVtxBuffUISet != NULL)
		{
			g_pVtxBuffUISet->Release();
			g_pVtxBuffUISet = NULL;
		}
	}
}


//更新処理
void UpdateUISet(void)
{
	VERTEX_2D*pVtx;//頂点情報へのポインタ

	UIserect * pUIserect = GetUIserect();
	UI * pUI = GetUI();

	for (int nCntUISet = 0; nCntUISet < MAX_UI; nCntUISet++)
	{
		if (g_aUISet[nCntUISet].bUse == true)
		{
			//pos操作
			if (GetKeyboardPress(DIK_S) == true)
			{
				if (g_bPressTimeY == false)
				{
					g_aUISet[nCntUISet].pos.y += 10;
				}
				g_bPressTimeY = true;
				g_nPressTimeY--;
				if (g_bPressTimeY == true && g_nPressTimeY < 0)
				{
					if (g_bTimeY == true)
					{
						g_aUISet[nCntUISet].pos.y += 10;
						g_bTimeY = false;
					}
					else if (g_bTimeY == false)
					{
						g_nPrsTime--;
						if (g_nPrsTime < 0)
						{
							g_nPrsTime = PRSTIME;
							g_bTimeY = true;
						}
					}
				}
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{
				if (g_bPressTimeY == false)
				{
					g_aUISet[nCntUISet].pos.y -= 10;
				}
				g_bPressTimeY = true;
				g_nPressTimeY--;
				if (g_bPressTimeY == true && g_nPressTimeY < 0)
				{
					if (g_bTimeY == true)
					{
						g_aUISet[nCntUISet].pos.y -= 10;
						g_bTimeY = false;
					}
					else if (g_bTimeY == false)
					{
						g_nPrsTime--;
						if (g_nPrsTime < 0)
						{
							g_nPrsTime = PRSTIME;
							g_bTimeY = true;
						}
					}
				}

			}
			if (GetKeyboardPress(DIK_A) == true)
			{
				if (g_bPressTimeX == false)
				{
					g_aUISet[nCntUISet].pos.x -= 10;
				}
				g_bPressTimeX = true;
				g_nPressTimeX--;
				if (g_bPressTimeX == true && g_nPressTimeX < 0)
				{
					if (g_bTimeX == true)
					{
						g_aUISet[nCntUISet].pos.x -= 10;
						g_bTimeX = false;
					}
					else if (g_bTimeX == false)
					{
						g_nPrsTime--;
						if (g_nPrsTime < 0)
						{
							g_nPrsTime = PRSTIME;
							g_bTimeX = true;
						}
					}
				}

			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				if (g_bPressTimeX == false)
				{
					g_aUISet[nCntUISet].pos.x += 10;
				}
				g_bPressTimeX = true;
				g_nPressTimeX--;
				if (g_bPressTimeX == true && g_nPressTimeX < 0)
				{
					if (g_bTimeX == true)
					{
						g_aUISet[nCntUISet].pos.x += 10;
						g_bTimeX = false;
					}
					else if (g_bTimeX == false)
					{
						g_nPrsTime--;
						if (g_nPrsTime < 0)
						{
							g_nPrsTime = PRSTIME;
							g_bTimeX = true;
						}
					}
				}

			}

			///リリース時に初期化と同じ設定に
			if (GetKeyboardRelese(DIK_S) == true ||
				GetKeyboardRelese(DIK_W) == true)
			{
				g_bPressTimeY = false;
				g_nPressTimeY = PRESSTIME;
				g_bTimeY = false;
			}

			if (GetKeyboardRelese(DIK_A) == true ||
				GetKeyboardRelese(DIK_D) == true)
			{
				g_bTimeX = false;
				g_nPressTimeX = PRESSTIME;
				g_bPressTimeX = false;
			}


			//拡大縮小
			if (GetKeyboardTrigger(DIK_E) == true)
			{
				g_aUISet[nCntUISet].fWidth -= 10;
			}
			else if (GetKeyboardTrigger(DIK_Q) == true)
			{
				g_aUISet[nCntUISet].fWidth += 10;
			}
			if (GetKeyboardTrigger(DIK_C) == true)
			{
				g_aUISet[nCntUISet].fHight -= 10;
			}
			else if (GetKeyboardTrigger(DIK_Z) == true)
			{
				g_aUISet[nCntUISet].fHight += 10;
			}

			//セットできるUIの変更
			if (GetKeyboardTrigger(DIK_UP) == true)
			{
				g_aUISet[nCntUISet].ntype += 1;
				if (g_aUISet[nCntUISet].ntype >= pUIserect->TotalNumber)
				{
					g_aUISet[nCntUISet].ntype = 0;
				}
			}
			else if (GetKeyboardTrigger(DIK_DOWN) == true)
			{
				g_aUISet[nCntUISet].ntype -= 1;
				if (g_aUISet[nCntUISet].ntype < 0)
				{
					g_aUISet[nCntUISet].ntype = pUIserect->TotalNumber-1;
				}

			}

			//全消去
			if (GetKeyboardTrigger(DIK_K) == true)
			{
				for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
				{
					if (pUI[nCntUI].bUse == true)
					{
						pUI[nCntUI].bUse = false;
						pUIserect->nSetNumber = 0;
					}
				}
			}

			//セーブ
			if (GetKeyboardTrigger(DIK_F9) == true)
			{
				SaveUI();
			}

			//セット
			if (GetKeyboardTrigger(DIK_RETURN) == true )
			{
				SetUI(g_aUISet[nCntUISet].pos, g_aUISet[nCntUISet].fWidth, g_aUISet[nCntUISet].fHight, g_aUISet[nCntUISet].ntype);
			}
			g_nCntTime--;
			//g_aUISet[nCntUISet].colorA--;
			//if(g_Color == 255 && g_nCntTime < 0)
			//{
			//	g_Color = 1;
			//	g_nCntTime = TIME;
			//}
			//else if (g_Color == 1 && g_nCntTime < 0)
			//{
			//	g_Color = 255;
			//	g_nCntTime = TIME;
			//}
			

			//頂点バッファをロック
			g_pVtxBuffUISet->Lock(0, 0, (void**)&pVtx, 0);

			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x - (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y - (g_aUISet[nCntUISet].fHight / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x + (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y - (g_aUISet[nCntUISet].fHight / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x - (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y + (g_aUISet[nCntUISet].fHight / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x + (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y + (g_aUISet[nCntUISet].fHight / 2), 0.0f);

			//頂点の色
			pVtx[0].col = D3DXCOLOR(255,255,255, g_aUISet[nCntUISet].colorA);
			pVtx[1].col = D3DXCOLOR(255,255,255, g_aUISet[nCntUISet].colorA);
			pVtx[2].col = D3DXCOLOR(255,255,255, g_aUISet[nCntUISet].colorA);
			pVtx[3].col = D3DXCOLOR(255,255,255, g_aUISet[nCntUISet].colorA);

			//テクスチャのUV座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//アンロック
			g_pVtxBuffUISet->Unlock();

		}

	}

}

//描画処理
void DrawUISet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	pDevice = GetDevice();//デバイスを取得する

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUISet, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	for (int nCntUISet = 0; nCntUISet < MAX_UI; nCntUISet++)
	{
		if (g_aUISet[nCntUISet].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureUISet[g_aUISet[nCntUISet].ntype]);//テクスチャの設定

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUISet * 4, 2);//ポリゴンの描画
		}
	}
}

//UIの設定処理
void SetUISet(D3DXVECTOR3 pos, float fWidth, float fHeight, int ntype)
{
	VERTEX_2D*pVtx;//頂点情報へのポインタ

	for (int nCntUISet = 0; nCntUISet < MAX_UI; nCntUISet++)
	{
		if (g_aUISet[nCntUISet].bUse == false)
		{
			g_aUISet[nCntUISet].pos = pos;//座標入れる
			g_aUISet[nCntUISet].ntype = ntype;//タイプを入れる
			g_aUISet[nCntUISet].fHight = fHeight;
			g_aUISet[nCntUISet].fWidth = fWidth;


			//頂点バッファをロック
			g_pVtxBuffUISet->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (nCntUISet * 4);
				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x - (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y - (g_aUISet[nCntUISet].fHight / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x + (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y - (g_aUISet[nCntUISet].fHight / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x - (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y + (g_aUISet[nCntUISet].fHight / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aUISet[nCntUISet].pos.x + (g_aUISet[nCntUISet].fWidth / 2), g_aUISet[nCntUISet].pos.y + (g_aUISet[nCntUISet].fHight / 2), 0.0f);

				//rhwの設定
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;

				//頂点の色
				pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
				pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
				pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
				pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);

				//テクスチャのUV座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			
			//アンロック
			g_pVtxBuffUISet->Unlock();

			g_aUISet[nCntUISet].bUse = true;

			break;
		}

	}
}

UISet *GetUISet(void)
{
	return &g_aUISet[0];
}

UISetSerect *GetUISetSerect(void)
{
	return &g_UISetserect;
}