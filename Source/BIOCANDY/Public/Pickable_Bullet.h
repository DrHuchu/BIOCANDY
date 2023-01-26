// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickable.h"
#include "Pickable_Bullet.generated.h"

/**
 * 
 */
UCLASS()
class BIOCANDY_API APickable_Bullet : public APickable
{
	GENERATED_BODY()

	virtual void InteractWithMe() override;
};
