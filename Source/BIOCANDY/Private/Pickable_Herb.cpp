// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Herb.h"

#include "Player_Jill.h"

void APickable_Herb::InteractWithMe()
{
	if (!bIsOn)
	{
		if(player)
		{
			// �÷��̾ �ִ� ���� �� 1 ����
			player->herbCount ++;	
		}
		Destroy();
		bIsOn = true;
	}
}
