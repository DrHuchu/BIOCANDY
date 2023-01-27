// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Key.h"
#include "Player_Jill.h"

void APickable_Key::InteractWithMe()
{
	if (!bIsOn)
	{
		player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (player)
		{
			player->bIsKeyOn = true;
		}
		Destroy();
		bIsOn = true;
	}
}
