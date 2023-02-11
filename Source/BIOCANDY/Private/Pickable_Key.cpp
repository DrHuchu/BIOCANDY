// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Key.h"
#include "Player_Jill.h"

void APickable_Key::InteractWithMe()
{
	if (!bIsOn)
	{

		if (player)
		{
			player->bIsKeyOn = true;
		}
		Destroy();
		bIsOn = true;
	}
}
