// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explodable.generated.h"

UCLASS()
class BIOCANDY_API AExplodable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplodable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Explodable Settings")
		class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "Explodable Settings")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "Explodable Settings")
		float explodeCount;

	UFUNCTION()
		void Explosion();

	UFUNCTION()
		void ElectricShock();
};
