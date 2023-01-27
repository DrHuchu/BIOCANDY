// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_KeyNeeded.h"
#include "Player_Jill.h"
#include "Blueprint/UserWidget.h"

void ADoor_KeyNeeded::InteractWithMe()
{
	if (!bIsOn)
	{
		player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (player)
		{
			if (player->bIsKeyOn)
			{
				player->keyWarningUI->RemoveFromParent();
				timeline.Play();
				bIsOn = true;
			}
			else
			{
				player->keyWarningUI->RemoveFromParent();
				player->keyWarningUI->AddToViewport();

				/*FTimerHandle uiTimer;
				GetWorld()->GetTimerManager().SetTimer(uiTimer, FTimerDelegate::CreateLambda([&]()
				{
					player->keyWarningUI->RemoveFromParent();
				}), 5, false);*/

			}
		}
	}
	else
	{
		player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (player)
		{
			if (player->bIsKeyOn)
			{
				timeline.Reverse();
				bIsOn = false;
			}
		}
	}
}
