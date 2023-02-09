// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "AIController.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "NavigationSystem.h"
#include "Player_Jill.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

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

	//mState = EEnemyState::Idle;

	me->hp = me->maxHP;

	// ai 컨트롤러 캐스팅하기
	ai = Cast<AAIController>(me->GetController());

	//최초로 아이들 대기 시간 한 번 설정
	idleDelayTime = FMath::RandRange(0.0f, 8.0f);

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

	case EEnemyState::Roam:
		RoamState();
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

	case EEnemyState::Prone:
		ProneState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	//1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;

	//2. 만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > idleDelayTime)
		//3. 배회 상태로 전환하고 싶다.
	{
		//4. 경과 시간을 초기화한다.
		currentTime = 0;
		// 아이들 대기 시간을 랜덤으로 재설정한다.
		idleDelayTime = FMath::RandRange(0, 5);
		SetState(EEnemyState::Roam);
	}
}

void UEnemyFSM::RoamState()
{
	if(me->bRoamEnd == false)
	{
		me->bRoamEnd = true;
		me->RandomRoam();
	}
}

void UEnemyFSM::MoveState()
{
	//1. 타깃 목적지가 필요하다.
	FVector destination = target->GetActorLocation();
	
	//2. 방향이 필요하다.
	FVector dir = destination - me->GetActorLocation();

	// A. 내가 갈 수 있는 길 위에 타겟이 있는가?
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest request;
	request.SetAcceptanceRadius(5);
	request.SetGoalLocation(target->GetActorLocation());
	ai->BuildPathfindingQuery(request, query);
	
	auto result = ns->FindPathSync(query);

	// B. 갈 수 있다면 타겟 쪽으로 이동
	if(result.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(target->GetActorLocation());
	}
	
	//타깃과 가까워지면 공격상태로 전환하고 싶다.
	//1. 만약 거리가 공격 범위 안에 들어오면
	if (dir.Size() < attackRange)
		//2. 공격 상태로 전환하고 싶다.
	{
		SetState(EEnemyState::Attack);
	}
	//if(dir.Size() < chaseRange)
	//{
	//	SetState(EEnemyState::Idle);
	//}
}

void UEnemyFSM::AttackState()
{
	ai->StopMovement();
	//일정 시간에 한 번씩 공격하고 싶다.
	//1. 시간이 흘러야 한다.
	currentTime += GetWorld()->DeltaTimeSeconds;

	//2. 공격 시간이 됐으니까
	if (currentTime > attackDelayTime)

		//3. 공격하고 싶다.
	{
		randIntAttackSide = FMath::RandRange(0,99);
		if(randIntAttackSide<=49)
		{
			me->enemyAnim->bAttackSide = true;
		}
		else
		{
			me->enemyAnim->bAttackSide = false;
		}
		//타깃이 공격 범위를 벗어나면 상태를 이동으로 전환하고 싶다.
	//1. 타깃과의 거리가 필요하다.
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

		//2. 타깃과의 거리가 공격 범위를 벗어났으니까
		if (distance > attackRange)
		{
			//3. 상태를 이동으로 전환하고 싶다.
			SetState(EEnemyState::Move);
		}
		else
		{
			me->enemyAnim->bAttackPlay = true;
			currentTime = 0;
			bAttackPlay = false;
		}
	}

	
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > damageDelayTime)
	{
		SetState(EEnemyState::Move);
		currentTime = 0;
	}
}

void UEnemyFSM::DieState()
{
	ai->StopMovement();
	me->pawnSensor->bSeePawns = false;
	me->pawnSensor->bHearNoises = false;
	return;
}

void UEnemyFSM::BurntState()
{
	//액터가 쓰러지는 애니메이션을 재생하고
	me->OnMyBurn();
	//액터에 불타는 이펙트를 스폰시킨다.
	UGameplayStatics::SpawnEmitterAttached(fireFactory, me->GetMesh(), TEXT("Spine2Socket"));
	bDieEnd = true;
	me->hp = 0;
	SetState(EEnemyState::Die);
}

void UEnemyFSM::ShockedState()
{
	me->pawnSensor->bSeePawns = false;
	me->pawnSensor->bHearNoises = false;
	ai->StopMovement();	
	//액터에 전기충격 이펙트를 스폰시킨다.
	UGameplayStatics::SpawnEmitterAttached(shockFactory, me->GetMesh(), TEXT("Spine2Socket"));
	
	//1. 현재 시간에 시간을 누적시킨다.
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	//2. 누적 시간이 쇼크타임보다 커지면
	if (currentTime > shockTime)

		//3. FSM의 상태를 아이들로 전이한다.
	{
		currentTime = 0;
		me->StopAnimMontage();
		me->pawnSensor->bSeePawns = true;
		me->pawnSensor->bHearNoises = true;
		SetState(EEnemyState::Roam);
	}
}

void UEnemyFSM::ProneState()
{
	ai->StopMovement();
}

void UEnemyFSM::OnDamageProcess(int damageValue)
{
	
	//체력을 소모하고
	me->hp -= damageValue;
	//체력이 0이 되면
	if(me->hp <= 0)
	{
		if(bDieEnd == false)
		{
			//Die 한다.
			SetState(EEnemyState::Die);
			me->OnMyDamage(TEXT("Die"));
			bDieEnd = true;
		}
		else
		{
			return;
		}
	}

	//체력이 0보다 크면
	else
	{
		if(damageValue >= 10)
		{
			if(mState == EEnemyState::Shocked)
			{
				return;
			}
			else
			{
				if(ai)
				{
					ai->StopMovement();
				}
				//Damage 하고 싶다.
				SetState(EEnemyState::Damage);
				me->OnMyDamage(TEXT("Damage0"));
			}
		}
		else
		{
			//SetState(EEnemyState::Damage);
		}
		
		
	}
}

void UEnemyFSM::SetState(EEnemyState next)
{
	mState = next;
	me->enemyAnim->state = next;
}

void UEnemyFSM::OnHitEvent()
{
	if(me)
	{
		me->enemyAnim->bAttackPlay = false;
	}
	bAttackPlay = true;
	//3. 공격을 하고 (조건은 공격거리 안에 있는가?)
	float dist = target->GetDistanceTo(me);
	if (dist <= attackRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is Attack"));
	}
}

void UEnemyFSM::OnZombieRun()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 400;
}

void UEnemyFSM::OffZombieRun()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 20;
}

