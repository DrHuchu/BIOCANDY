// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player_Jill.generated.h"

UCLASS()
class BIOCANDY_API APlayer_Jill : public ACharacter
{
	GENERATED_BODY()

public:
	APlayer_Jill();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;

	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookup(float value);
	void OnAxisTurnRight(float value);
	void OnActionJump();

	FVector direction;
private:
	float walkSpeed = 600;
};
