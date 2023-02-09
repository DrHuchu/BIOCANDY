// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"


//����� ���� ����
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Roam,
	Move,
	Attack,
	Damage,
	Die,
	Burnt,
	Shocked,
	Prone,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOCANDY_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;

	//������
	void IdleState();

	//��ȸ����
	void RoamState();
	
	//�̵�����
	void MoveState();

	//���ݻ���
	void AttackState();

	//�ǰݻ���
	void DamageState();

	//��������
	void DieState();

	//ȭ������
	void BurntState();

	//��ũ����
	void ShockedState();

	//���� ô ����
	void ProneState();

	//��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime;

	//��� �ð�
	float currentTime = 0;

	//��ũ �ð�
	float shockTime = 15.0f;

	//Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class APlayer_Jill* target;

	//���� ����
	UPROPERTY()
		class AEnemy* me;

	//���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	//���� �¿� ����
	int randIntAttackSide;

	//���� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	//�߰� ����
	UPROPERTY(EditAnywhere, Category = FSM)
	float chaseRange = 1500.0f;
	
	bool bAttackPlay;

	//��Ʈ��Ŀ���� Ÿ��
	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 1.5;

	//�Һٴ� ����Ʈ ���丮
	UPROPERTY(EditAnywhere)
		class UParticleSystem* fireFactory;

	//���� ��� ����Ʈ ���丮
	UPROPERTY(EditAnywhere)
		class UParticleSystem* shockFactory;
	
	//�ǰ� �Լ�
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess(int damageValue);
	

	//���� ���� �Լ�
	void SetState(EEnemyState next);

	UFUNCTION(BlueprintCallable)
	void OnHitEvent();

	//���� �Ϸ� bool ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDieEnd = false;

	// ���ʹ��� ��Ʈ�ѷ�
	class AAIController* ai;

	UFUNCTION(BlueprintCallable)
	void OnZombieRun();

	UFUNCTION(BlueprintCallable)
	void OffZombieRun();
	
};
