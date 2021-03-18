//**************************************************************
//UI�\���̃w�b�_�[
//Author�F�����G��
//**************************************************************

#ifndef _UISET_H_
#define _UISET_H_

#include "main.h"
#include "UI.h"
#include "input.h"

//#define MAX_UISET (64)//UI�̍ő吔

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���

	int ntype;//���
	bool bUse;//�g�p���Ă邩

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
