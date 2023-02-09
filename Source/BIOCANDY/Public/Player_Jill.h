// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSM.h"
#include "Components/TimelineComponent.h"
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
	UFUNCTION(BlueprintNativeEvent)
	void OnMyHit(int AttackDamage);

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
	
	//권총 사용 중인지 여부
	bool bUsingSK_Pistol = true;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* interactionBox;

	class IInteractionInterface* interface;

	//  권총 조준 함수 선언
	void PistolAim();

	// 스나이퍼 UI 위젯 공장
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUserWidget> pistolCrossHairUIFactory;
	// 스나이퍼 UI 위젯 인스턴스
	class UUserWidget*pistolCrossHairUI;

	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	int32 hp;
	// 초기 hp값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	int32 maxHP = 10;
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

	// 장탄수 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		class UUserWidget* magUI;
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UUserWidget> magUIFactory;

	//탄창메거진, 재장전

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnActionReload();

	UFUNCTION()
	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* reloadingSound;

	//총탄창, 탄창 MAX, 탄창 가방
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int pistolCountMag;
	int maxPistolCountMag = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int pistolCountBag = 30;
	UPROPERTY(EditAnywhere)
	bool canShoot = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsReloading = false;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	class UUserWidget* damagedUI;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> damagedUIFactory;
	
	FTimeline timeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* curveFloat;

	UFUNCTION()
	void Zoom(float value);


private:
	float walkSpeed = 600;
};
