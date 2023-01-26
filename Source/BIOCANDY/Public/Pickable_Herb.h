// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickable.h"
#include "Pickable_Herb.generated.h"

/**
 * 
 */
UCLASS()
class BIOCANDY_API APickable_Herb : public APickable
{
	GENERATED_BODY()

public:
	virtual void InteractWithMe() override;
};
