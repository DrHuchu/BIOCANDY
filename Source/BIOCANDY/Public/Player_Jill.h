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
	//�������۰� ����ƽ�޽� �߰�
	UPROPERTY(EditAnywhere, Category=GunMesh)
	class UStaticMeshComponent* sniperGunComp;
	//���� ��� ������ ����
	bool bUsingSK_Pistol = true;
	// �������� ����
	void ChangeToSK_Pistol();
	// �������۰����� ����
	void ChangeToSniperGun();

	//zoom�� �����ϰ�ʹ�. in/out
	void OnActionZoomIn(); //Ȯ�� FOV 30
	void OnActionZoomOut(); //Ȯ�� FOV 90

	// �Ѿ� ���� ȿ�� ����
	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FHitResult hitInfo;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* interactionBox;

	class IInteractionInterface* interface;

	// �������� Ȯ�� ����
	void PistolAim();
	// �������� ���� ������ ����
	bool bSniperAim = false;
	// �������� UI ���� ����
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;
	// �������� UI ���� �ν��Ͻ�
	class UUserWidget*_sniperUI;
	// �Ϲ� ���� ũ�ν���� UI ����
	UPROPERTY(EditAnywhere, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// ũ�ν���� �ν��Ͻ�
	class UUserWidget* _crosshairUI;

	// ���� ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	int32 hp;
	// �ʱ� hp��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	int32 initialHp = 10;
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

private:
	float walkSpeed = 600;
};
