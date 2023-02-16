// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "EndPoint.generated.h"

UCLASS()
class BIOCANDY_API AEndPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndPoint();

protected:
	UFUNCTION()
	void OnEndPointOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	class APlayer_Jill* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	class UUserWidget* gameClearUI;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> gameClearUIFactory;

	
};