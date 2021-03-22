//**************************************************************
//UI表示のヘッダー
//Author：佐藤秀亮
//**************************************************************

#ifndef _UISET_H_
#define _UISET_H_

#include "main.h"
#include "UI.h"
#include "input.h"

//#define MAX_UISET (64)//UIの最大数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量

	int ntype;//種類
	bool bUse;//使用してるか

	float fWidth;
	float fHight;

	float colorA;
} UISet;

typedef struct
{
	int Serect;
} UISetSerect;

void InitUISet(void);
void UninitUISet(void);
void UpdateUISet(void);
void DrawUISet(void);

void SetUISet(D3DXVECTOR3 pos, float fWidth, float fHeight, int ntype);

UISet * GetUISet(void);

UISetSerect * GetUISetSerect(void);

#endif
