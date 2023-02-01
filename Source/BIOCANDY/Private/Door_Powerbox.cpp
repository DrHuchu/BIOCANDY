// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_Powerbox.h"

void ADoor_Powerbox::InteractWithMe()
{
	if(!bIsOn)
	{
		timeline.Play();
		HideInteractionWidget();
		bIsOn = true;
	}
}
