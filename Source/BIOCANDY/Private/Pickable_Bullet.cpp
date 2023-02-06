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
			//�÷��̾ �ִ� ź��� 15 ����
			player->pistolCountBag =  player->pistolCountBag + 15;
		}

		Destroy();
		bIsOn = true;
	}
}
