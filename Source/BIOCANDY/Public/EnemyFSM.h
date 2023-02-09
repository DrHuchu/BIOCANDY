// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"


//사용할 상태 정의
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
	
	
	//상태 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;

	//대기상태
	void IdleState();

	//배회상태
	void RoamState();
	
	//이동상태
	void MoveState();

	//공격상태
	void AttackState();

	//피격상태
	void DamageState();

	//죽음상태
	void DieState();

	//화형상태
	void BurntState();

	//쇼크상태
	void ShockedState();

	//죽은 척 상태
	void ProneState();

	//대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime;

	//경과 시간
	float currentTime = 0;

	//쇼크 시간
	float shockTime = 15.0f;

	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class APlayer_Jill* target;

	//소유 액터
	UPROPERTY()
		class AEnemy* me;

	//공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	//공격 좌우 랜덤
	int randIntAttackSide;

	//공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	//추격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float chaseRange = 1500.0f;
	
	bool bAttackPlay;

	//히트리커버리 타임
	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 1.5;

	//불붙는 이펙트 팩토리
	UPROPERTY(EditAnywhere)
		class UParticleSystem* fireFactory;

	//전기 충격 이펙트 팩토리
	UPROPERTY(EditAnywhere)
		class UParticleSystem* shockFactory;
	
	//피격 함수
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess(int damageValue);
	

	//상태 전이 함수
	void SetState(EEnemyState next);

	UFUNCTION(BlueprintCallable)
	void OnHitEvent();

	//죽음 완료 bool 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDieEnd = false;

	// 에너미의 컨트롤러
	class AAIController* ai;

	UFUNCTION(BlueprintCallable)
	void OnZombieRun();

	UFUNCTION(BlueprintCallable)
	void OffZombieRun();
	
};
