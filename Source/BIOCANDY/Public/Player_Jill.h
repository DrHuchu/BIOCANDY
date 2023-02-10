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

	//�÷��̾� �ǰ� �Լ�
	UFUNCTION(BlueprintNativeEvent)
	void OnMyHit(int AttackDamage);
	//�÷��̾� ���� �Լ�
	UFUNCTION(BlueprintNativeEvent)
	void OnMyGameOver();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOver = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* gameOverSphere;
	
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UCameraComponent* cameraComp;
	//�޸���
	UPROPERTY(EditAnywhere)
	float SprintSpeedMultiplier;


	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookup(float value);
	void OnAxisTurnRight(float value);
	void OnActionJump();
	void OnInteract();
	void OffInteract();

	//�޸��� 
	void Sprint();
	void StopSprinting();

	FVector direction;

	//�� ���̷�Ż�޽�
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	// �Ѿ� ����
	UPROPERTY(EditDefaultsOnly, Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	// �Ѿ� �߻� ó�� �Լ�
	void InputFire();
	
	//���� ��� ������ ����
	bool bUsingSK_Pistol = true;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* interactionBox;
	class IInteractionInterface* interface;

	//  ���� ���� �Լ� ����
	void PistolAim();

	// �������� UI ���� ����
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUserWidget> pistolCrossHairUIFactory;
	// �������� UI ���� �ν��Ͻ�
	UPROPERTY()
	class UUserWidget*pistolCrossHairUI;

	// ���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	int32 hp;
	// �ʱ� hp��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	int32 maxHP = 10;
	// �ǰ� ������ �� ó��
	UFUNCTION(BlueprintCallable, Category = Health)
	void OnHitEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsShootReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsKeyOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 powerboxCount = 0;

	//UI ����

	//Ű ���� ���
	UPROPERTY(EditAnywhere)
	class UUserWidget* keyWarningUI;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> keyWarningUIFactory;

	//�̹� ���ư� ������ ���
	UPROPERTY(EditAnywhere)
	class UUserWidget* powerboxDoneUI;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> powerboxUIFactory;

	//������ ��� ���ư� Ż���϶� ���
	UPROPERTY(EditAnywhere)
		class UUserWidget* powerboxAllDoneUI;
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UUserWidget> powerboxAllDoneUIFactory;

	//Ż�⹮ ���� ���� ���
	UPROPERTY(EditAnywhere)
		class UUserWidget* escapeDoorUI;
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UUserWidget> escapeDoorUIFactory;

	// ��ź�� UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		class UUserWidget* magUI;
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UUserWidget> magUIFactory;

	//źâ�ް���, ������

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnActionReload();
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInventoryOn;

	UFUNCTION()
	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* reloadingSound;

	//��źâ, źâ MAX, źâ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int pistolCountMag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	UPROPERTY()
	FTimeline timeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* curveFloat;

	UFUNCTION()
	void Zoom(float value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	class UUserWidget* gameOverUI;
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> gameOverUIFactory;

	UPROPERTY()
	class AEnemy* enemy;

private:
	float walkSpeed = 600;
};
