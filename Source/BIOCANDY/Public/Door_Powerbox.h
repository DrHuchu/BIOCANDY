// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "Door_Powerbox.generated.h"

/**
 * 
 */
UCLASS()
class BIOCANDY_API ADoor_Powerbox : public ADoor
{
	GENERATED_BODY()

	virtual void InteractWithMe() override;
};
