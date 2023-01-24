// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "LightSwitch.generated.h"

UCLASS()
class BIOCANDY_API ALightSwitch : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightSwitch();

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
	class UPointLightComponent* lightComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* interactionWidget;

	bool bIsOn = false;

	
};
