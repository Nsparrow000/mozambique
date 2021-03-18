//**************************************************************
//UI�\���̃w�b�_�[
//Author�F�����G��
//**************************************************************

#ifndef _UI_H_
#define _UI_H_

#include"main.h"

#define MAX_UI (64)//UI�̍ő吔

#define FILENAMETEXT "data/UINameRead.txt"	//�ǂݍ��ރX�N���v�g�t�@�C����
#define SAVEFILETEXT "data/UISave.txt"

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXCOLOR color;//�F
	int ntype;//���
	bool bUse;//�g�p���Ă邩

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


