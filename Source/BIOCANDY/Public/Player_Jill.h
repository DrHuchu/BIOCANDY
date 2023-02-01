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
	void OnInteract();
	void OffInteract();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FHitResult hitInfo;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* interactionBox;

	class IInteractionInterface* interface;

	// 스나이퍼 확대 조준
	void PistolAim();
	// 스나이퍼 조준 중인지 여부
	bool bSniperAim = false;
	// 스나이퍼 UI 위젯 공장
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;
	// 스나이퍼 UI 위젯 인스턴스
	class UUserWidget*_sniperUI;
	// 일반 조준 크로스헤어 UI 위젯
	UPROPERTY(EditAnywhere, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// 크로스헤어 인스턴스
	class UUserWidget* _crosshairUI;

	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	int32 hp;
	// 초기 hp값
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	int32 initialHp = 10;
	// 피격 당했을 때 처리
	UFUNCTION(BlueprintCallable, Category = Health)
	void OnHitEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsShootReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsKeyOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 powerboxCount = 0;

	//UI 관련

	//키 없음 경고
	UPROPERTY(EditAnywhere)
	class UUserWidget* keyWarningUI;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> keyWarningUIFactory;

	//이미 돌아간 발전기 경고
	UPROPERTY(EditAnywhere)
	class UUserWidget* powerboxDoneUI;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> powerboxUIFactory;

	//발전기 모두 돌아감 탈출하라 경고
	UPROPERTY(EditAnywhere)
		class UUserWidget* powerboxAllDoneUI;
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UUserWidget> powerboxAllDoneUIFactory;

	//탈출문 전력 부족 경고
	UPROPERTY(EditAnywhere)
		class UUserWidget* escapeDoorUI;
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UUserWidget> escapeDoorUIFactory;

private:
	float walkSpeed = 600;
};
