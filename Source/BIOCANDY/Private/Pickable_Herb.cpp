// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Herb.h"

#include "Player_Jill.h"

void APickable_Herb::InteractWithMe()
{
	if (!bIsOn)
	{
		if(player)
		{
			// 플레이어에 있는 약초 수 1 증가
			player->herbCount ++;	
		}
		Destroy();
		bIsOn = true;
	}
}
