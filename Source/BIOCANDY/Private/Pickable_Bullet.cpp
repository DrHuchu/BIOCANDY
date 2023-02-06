// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Bullet.h"

#include "Player_Jill.h"

void APickable_Bullet::InteractWithMe()
{
	if (!bIsOn)
	{
		player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if(player)
		{
			//플레이어에 있는 탄약수 15 증가
			player->pistolCountBag =  player->pistolCountBag + 15;
		}

		Destroy();
		bIsOn = true;
	}
}
