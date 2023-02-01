// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeBulb.generated.h"

UCLASS()
class BIOCANDY_API AEscapeBulb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEscapeBulb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPointLightComponent* lightComp;

	class APlayer_Jill* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 lightCount;

	UFUNCTION()
	void LightOn();
};
