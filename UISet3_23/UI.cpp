//**************************************************************
//UIの処理(UI.cpp)
//**************************************************************

#include "UI.h"

LPDIRECT3DTEXTURE9 g_pTextureGameUI[62] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;//頂点バッファへのポインタ

UI g_aUI[MAX_UI];
UIserect g_UIserect;

int g_MaxUITexture;
int g_UITexture;

//初期化
void InitUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ
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

	if (pFile != NULL)//一度ファイルを読み込んで必要な情報を抜く
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "NUM_TEXTURE") == 0)	//NUM_TEXTUREの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_MaxUITexture);
			}
			if (strcmp(&aFile[0], "TEXTURE_FILENAME") == 0) //TEXTURE_FILENAMEの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXCreateTextureFromFile(pDevice, &aFile[0], &g_pTextureGameUI[g_UITexture]);
				g_UITexture++;
				g_UIserect.TotalNumber++;
			}
			
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}
		}
	}
	fclose(pFile);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGameUI, NULL);
	//それぞれの初期化
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
	VERTEX_2D*pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		for (int nVtxUI = 0; nVtxUI < 4; nVtxUI++)
		{
			//頂点の座標
			pVtx[nVtxUI].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (0.0f / 2), g_aUI[nCntUI].pos.y - (0.0f / 2), 0.0f);

			//rhwの設定
			pVtx[nVtxUI].rhw = 1.0f;

			//頂点の色
			pVtx[nVtxUI].col = g_aUI[nCntUI].color;

			//テクスチャのUV座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffGameUI->Unlock();
	//pFile = fopen(FILENAMETEXT, "r");//二度目の読み込みでセット

	//	if (pFile != NULL)//一度ファイルを読み込んで必要な情報を抜く
	//	{
	//		while (true)
	//		{
	//			fscanf(pFile, "%s", &aFile[0]);
	//			if (bSetGameUI == true)
	//			{
	//				if (bSet == true)
	//				{
	//					if (strcmp(&aFile[0], "TYPE") == 0)	//TYPEの文字列を見つけたら
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%d", &Textype);
	//					}
	//					if (strcmp(&aFile[0], "POS") == 0)	//POSの文字列を見つけたら
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%f%f%f", &UIpos.x, &UIpos.y, &UIpos.z);
	//					}
	//					if (strcmp(&aFile[0], "WIDTH") == 0)	//WIDTHの文字列を見つけたら
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%f", &Width);
	//					}
	//					if (strcmp(&aFile[0], "HEIGTH") == 0)	//HEIGTHの文字列を見つけたら
	//					{
	//						fscanf(pFile, "%s", &aFile[0]);
	//						fscanf(pFile, "%f", &Heigth);
	//					}
	//				}

	//				if (strcmp(&aFile[0], "UISET") == 0)	//UISETの文字列を見つけたら
	//				{
	//					bSet = true;
	//				}
	//				if (strcmp(&aFile[0], "END_UISET") == 0)	//END_UISETの文字列を見つけたら
	//				{
	//					bSet = false;
	//					/*SetUI(UIpos, Width, Heigth, Textype);*/
	//				}
	//			}
	//			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
	//			{
	//				break;
	//			}

	//		}
	//	}
	//	fclose(pFile);

	}



//終了処理
void UninitUI(void)
{
	int nCntUI;
	for (nCntUI = 0; nCntUI < g_MaxUITexture; nCntUI++)
	{
		//テクスチャの破棄
		if (g_pTextureGameUI[nCntUI] != NULL)
		{
			g_pTextureGameUI[nCntUI]->Release();
			g_pTextureGameUI[nCntUI] = NULL;
		}

		//頂点バッファも破棄
		if (g_pVtxBuffGameUI != NULL)
		{
			g_pVtxBuffGameUI->Release();
			g_pVtxBuffGameUI = NULL;
		}
	}
}

//更新処理
void UpdateUI(void)
{

}

//描画処理
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	pDevice = GetDevice();//デバイスを取得する

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureGameUI[g_aUI[nCntUI].ntype]);//テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);//ポリゴンの描画
		}
	}

}

//UIの設定処理
void SetUI(D3DXVECTOR3 pos, float fWidth, float fHeight, int ntype)
{
	VERTEX_2D*pVtx;//頂点情報へのポインタ

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
	{
		if (g_aUI[nCntUI].bUse == false)
		{
			g_aUI[nCntUI].pos = pos;//座標入れる
			g_aUI[nCntUI].ntype = ntype;//タイプを入れる
			g_aUI[nCntUI].fWidth = fWidth;
			g_aUI[nCntUI].fHeight = fHeight;

			//頂点バッファをロック
			g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
			for (int nVtxUI = 0; nVtxUI <= 4; nVtxUI++)
			{
				pVtx += (nCntUI * 4);
				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (fWidth / 2), g_aUI[nCntUI].pos.y - (fHeight / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (fWidth / 2), g_aUI[nCntUI].pos.y - (fHeight / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (fWidth / 2), g_aUI[nCntUI].pos.y + (fHeight / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (fWidth / 2), g_aUI[nCntUI].pos.y + (fHeight / 2), 0.0f);

				//rhwの設定
				pVtx[nVtxUI].rhw = 1.0f;

				//頂点の色
				pVtx[nVtxUI].col = g_aUI[nCntUI].color;

				//テクスチャのUV座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			
			//アンロック
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
		fprintf(pFile, "#セットしたUIをこのテキストファイルに書き出します\n");
		fprintf(pFile, "#読み込むテキストファイルにコピペするなりして下さい\n");
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