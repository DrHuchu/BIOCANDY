// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class BIOCANDY_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
		class USphereComponent* sphereComp;

	FVector direction;
	FRotator targetDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
		float moveSpeed = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
		float recogDelay = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
		bool isRecognized = false;


	UFUNCTION()
		void Moving(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void Recognition();
	UFUNCTION()
		void RecognitionOff(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	


	class APlayer_Jill* player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//적 AI관리 컴포넌트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemyFSM* enemyFsm;

	UPROPERTY()
		class UEnemyAnim* enemyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPawnSensingComponent* pawnSensor;

	UFUNCTION(BlueprintImplementableEvent)
	void OnMyDamage(FName sectionName);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMyShock();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMyBurn();

	UFUNCTION(BlueprintImplementableEvent)
		void OnMyEating();
	
	//에너미 HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int hp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int maxHP = 30;

	//에너미의 배회 함수 선언
	UFUNCTION(BlueprintImplementableEvent)
	void RandomRoam();

	//배회 상태 완료 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRoamEnd = false;
};
