// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

UCLASS()
class BIOCANDY_API ADoor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InteractWithMe() override;
	virtual void ShowInteractionWidget() override;
	virtual void HideInteractionWidget() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* rootComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* interactionWidget;

	UPROPERTY(EditAnywhere)
	bool bIsOn = false;


	FTimeline timeline;
	UPROPERTY(EditAnywhere)
		UCurveFloat* curveFloat;

	UPROPERTY(EditAnywhere)
		float doorRotateAngle = 90.0f;

	UFUNCTION()
		void OpenDoor(float value);

	class APlayer_Jill* player;
};
