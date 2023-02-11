// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_Escape.h"
#include "Door_Escape_Side.h"
#include "Player_Jill.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ADoor_Escape::InteractWithMe()
{
	if (!bIsOn)
	{
		if (player)
		{
			//발전기가 다 돌아간 상태라면
			if (player->powerboxCount == 4)
			{
				sidedoor = Cast<ADoor_Escape_Side>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoor_Escape_Side::StaticClass()));

				if(sidedoor != nullptr)
				{
					sidedoor->timeline.Play();
				}
				
				
				
				player->escapeDoorUI->RemoveFromParent();
				timeline.Play();
			}
			else
			{
				player->escapeDoorUI->RemoveFromParent();
				player->escapeDoorUI->AddToViewport();
			}
		}
	}
}
