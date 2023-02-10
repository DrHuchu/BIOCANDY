// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class BIOCANDY_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

		virtual void NativeBeginPlay() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState state;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttackPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnemyDieEnd;

	UFUNCTION(BlueprintCallable)
	void OnHitEvent();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnHit();

	UPROPERTY()
	class AEnemy* me;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttackSide;
};
