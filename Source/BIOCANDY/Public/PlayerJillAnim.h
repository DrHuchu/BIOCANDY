// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerJillAnim.generated.h"

/**
 * 
 */
UCLASS()
class BIOCANDY_API UPlayerJillAnim : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	// forwardVelocity, rightVelocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardVelocity; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RightVelocity;

	void OnFire();
};
