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
	//�޸���
	UPROPERTY(EditAnywhere)
	float SprintSpeedMultiplier;

	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookup(float value);
	void OnAxisTurnRight(float value);
	void OnActionJump();

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
private:
	float walkSpeed = 600;
};
