// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Bullet.h"

void APickable_Bullet::InteractWithMe()
{
	if (!bIsOn)
	{
		//���Ӹ�忡 �ִ� ź��� 15 ����

		Destroy();
		bIsOn = true;
	}
}