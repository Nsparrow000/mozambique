//ーーーーーーーーーーーーーーーーーーーーーーーーー
//エフェクトのヘッダー
//Author：satou syusuke
//ーーーーーーーーーーーーーーーーーーーーーーーーー

#ifndef _EFFECT_H_
#define _EFFECT_H_

#define MAX_EFFECT (10000)//大量に必要
#define CIRCLE ((rand() % 314) / 100.0f - (rand() % 314) / 100.0f) //円のマクロ
#define POSRAND (rand() % 10 + 10)

typedef enum
{
	EFFECTTYPE_CIRCLE,//円形に出現
	EFFECTTYPE_STRIGHIT,//まっすぐに飛ぶ
	EFFECTTYPE_FOLL,//上がった後落ちる

	EFFECTTYPE_MAX,
} EFFECTTYPE;

typedef struct
{
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxView;

	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;

	D3DXVECTOR3 move;

	int colorR;
	int colorG;
	int colorB;
	int colorA;

	int ntype;
	int nlife;

	float fHight;
	float fWhidth;

	bool bUse;

} Effect;

typedef struct
{
	int colorR;
	int colorG;
	int colorB;
	int colorA;

	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;

	float fHight;
	float fWhidth;

	int SerectType;
} Effectstate;

//プレイヤー用の宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXVECTOR3 rot, float fWhidth, float fHight, int ntype, int nLife);//出現位置　色・発射角度　幅　高さ　タイプ　寿命

Effect *GetEffect(void);
Effectstate * GetEffectstate(void);
#endif

