// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "Player_Jill.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//월드에서 APlayer_Jill 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayer_Jill::StaticClass());

	//APlayer_Jill 타입으로 캐스팅
	target = Cast<APlayer_Jill>(actor);

	//소유 객체 가져오기
	me = Cast<AEnemy>(GetOwner());
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;

	case EEnemyState::Move:
		MoveState();
		break;

	case EEnemyState::Attack:
		AttackState();
		break;

	case EEnemyState::Damage:
		DamageState();
		break;

	case EEnemyState::Die:
		DieState();
		break;

	case EEnemyState::Burnt:
		BurntState();
		break;

	case EEnemyState::Shocked:
		ShockedState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	//1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;

	//2. 만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > idleDelayTime)

		//3. 이동상태로 전환하고 싶다.
	{
		mState = EEnemyState::Move;
		//4. 경과 시간을 초기화한다.
		currentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	//1. 타깃 목적지가 필요하다.
	FVector destination = target->GetActorLocation();

	//2. 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();

	//3. 방향으로 이동하고 싶다.
	me->AddMovementInput(dir.GetSafeNormal());


	//타깃과 가까워지면 공격상태로 전환하고 싶다.
	//1. 만약 거리가 공격 범위 안에 들어오면
	if(dir.Size() < attackRange)

	//2. 공격 상태로 전환하고 싶다.
	{
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	//일정 시간에 한 번씩 공격하고 싶다.
	//1. 시간이 흘러야 한다.
	currentTime += GetWorld()->DeltaTimeSeconds;

	//2. 공격 시간이 됐으니까
	if (currentTime > attackDelayTime)

		//3. 공격하고 싶다.
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		//4. 경과 시간 초기화
		currentTime = 0;
	}

	//타깃이 공격 범위를 벗어나면 상태를 이동으로 전환하고 싶다.
	//1. 타깃과의 거리가 필요하다.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	//2. 타깃과의 거리가 공격 범위를 벗어났으니까
	if(distance > attackRange)

	//3. 상태를 이동으로 전환하고 싶다.
	{
		mState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState()
{

}

void UEnemyFSM::DieState()
{

}

void UEnemyFSM::BurntState()
{
	//액터가 쓰러지는 애니메이션을 재생하고

	//액터에 불타는 이펙트를 스폰시킨다.
	UGameplayStatics::SpawnEmitterAttached(fireFactory, me->GetMesh(), TEXT("Spine2Socket"));

	mState = EEnemyState::Die;
}

void UEnemyFSM::ShockedState()
{
	//쇼크 애니메이션을 재생하고

	//15초 후에 아이들 상태로 전환한다.

	//액터에 전기충격 이펙트를 스폰시킨다.
	UGameplayStatics::SpawnEmitterAttached(shockFactory, me->GetMesh(), TEXT("Spine2Socket"));

	//1. 현재 시간에 시간을 누적시킨다.
	currentTime += GetWorld()->DeltaTimeSeconds;



	//2. 누적 시간이 쇼크타임보다 커지면
	if(currentTime > shockTime)

	//3. FSM의 상태를 아이들로 전이한다.
	{
		mState = EEnemyState::Idle;
	}
}

