// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Bullet.h"

void APickable_Bullet::InteractWithMe()
{
	if (!bIsOn)
	{
		//게임모드에 있는 탄약수 15 증가

		Destroy();
		bIsOn = true;
	}
}