// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickable.h"
#include "Powerbox.generated.h"

/**
 * 
 */
UCLASS()
class BIOCANDY_API APowerbox : public APickable
{
	GENERATED_BODY()

	virtual void InteractWithMe() override;
	virtual void OffInteractWithMe() override;

public:
	UPROPERTY(EditAnywhere)
	float currentTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float powerTime = 8;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	bool powerOn = false;


};
