// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "Door_Escape.generated.h"

/**
 * 
 */
UCLASS()
class BIOCANDY_API ADoor_Escape : public ADoor
{
	GENERATED_BODY()
	virtual void InteractWithMe() override;
public:

	UPROPERTY()
	class ADoor_Escape_Side* sidedoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* openSound;
};
