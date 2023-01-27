// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Herb.h"

void APickable_Herb::InteractWithMe()
{
	if (!bIsOn)
	{
		//게임모드에 있는 약초 수 1 증가

		Destroy();
		bIsOn = true;
	}
}
