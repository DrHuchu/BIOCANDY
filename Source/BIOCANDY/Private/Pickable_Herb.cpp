// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Herb.h"

void APickable_Herb::InteractWithMe()
{
	if (!bIsOn)
	{
		//���Ӹ�忡 �ִ� ���� �� 1 ����

		Destroy();
		bIsOn = true;
	}
}
