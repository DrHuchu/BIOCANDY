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
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UCameraComponent* cameraComp;
	//달리기
	UPROPERTY(EditAnywhere)
	float SprintSpeedMultiplier;

	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookup(float value);
	void OnAxisTurnRight(float value);
	void OnActionJump();

	//달리기 
	void Sprint();
	void StopSprinting();

	FVector direction;

	//총 스켈레탈메시
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	// 총알 공장
	UPROPERTY(EditDefaultsOnly, Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	// 총알 발사 처리 함수
	void InputFire();
	//스나이퍼건 스태틱메시 추가
	UPROPERTY(EditAnywhere, Category=GunMesh)
	class UStaticMeshComponent* sniperGunComp;
	//권총 사용 중인지 여부
	bool bUsingSK_Pistol = true;
	// 권총으로 변경
	void ChangeToSK_Pistol();
	// 스나이퍼건으로 변경
	void ChangeToSniperGun();

	//zoom을 구현하고싶다. in/out
	void OnActionZoomIn(); //확대 FOV 30
	void OnActionZoomOut(); //확대 FOV 90

	// 총알 파편 효과 공장
	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;
private:
	float walkSpeed = 600;
};
