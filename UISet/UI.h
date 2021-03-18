//**************************************************************
//UI表示のヘッダー
//Author：佐藤秀亮
//**************************************************************

#ifndef _UI_H_
#define _UI_H_

#include"main.h"

#define MAX_UI (64)//UIの最大数

#define FILENAMETEXT "data/UINameRead.txt"	//読み込むスクリプトファイル名
#define SAVEFILETEXT "data/UISave.txt"

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR color;//色
	int ntype;//種類
	bool bUse;//使用してるか

	float fWidth;
	float fHeight;
} UI;

typedef struct
{
	int nSetNumber;
	int Serect;
	int TotalNumber;
} UIserect;

void InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

void SetUI(D3DXVECTOR3 pos,float fWidth,float fHeight,int ntype);

void SaveUI(void);

UI * GetUI(void);
UIserect * GetUIserect(void);

#endif


