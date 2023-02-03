// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerJillAnim.h"
#include "Player_Jill.h"
#include "GameFramework/CharacterMovementComponent.h"
void UPlayerJillAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//본체(Owner)의 Velocity를 가져와서
	APlayer_Jill* owner = Cast<APlayer_Jill>(TryGetPawnOwner());
	if (nullptr != owner)
	{
		//RightVelocity, ForwardVelocity를 구하고싶다.
		//Vector의 내적을 이용해서 방향값을 구하고싶다.
		FVector velocity = owner->GetVelocity();
		RightVelocity = FVector::DotProduct(velocity, owner->GetActorRightVector());
		ForwardVelocity = FVector::DotProduct(velocity, owner->GetActorForwardVector());
	}
}
