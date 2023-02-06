// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "EnemyAnim.h"
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

	//���忡�� APlayer_Jill Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayer_Jill::StaticClass());

	//APlayer_Jill Ÿ������ ĳ����
	target = Cast<APlayer_Jill>(actor);

	//���� ��ü ��������
	me = Cast<AEnemy>(GetOwner());

	mState = EEnemyState::Idle;

	me->hp = me->maxHP;

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
	//1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;

	//2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)

		//3. �̵����·� ��ȯ�ϰ� �ʹ�.
	{
		SetState(EEnemyState::Move);
		//4. ��� �ð��� �ʱ�ȭ�Ѵ�.
		currentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	//1. Ÿ�� �������� �ʿ��ϴ�.
	FVector destination = target->GetActorLocation();

	//2. ������ �ʿ��ϴ�.
	FVector dir = destination - me->GetActorLocation();

	//3. �������� �̵��ϰ� �ʹ�.
	me->AddMovementInput(dir.GetSafeNormal());


	//Ÿ��� ��������� ���ݻ��·� ��ȯ�ϰ� �ʹ�.
	//1. ���� �Ÿ��� ���� ���� �ȿ� ������
	if (dir.Size() < attackRange)

		//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
	{
		SetState(EEnemyState::Attack);
	}
}

void UEnemyFSM::AttackState()
{
	//���� �ð��� �� ���� �����ϰ� �ʹ�.
	//1. �ð��� �귯�� �Ѵ�.
	currentTime += GetWorld()->DeltaTimeSeconds;

	//2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)

		//3. �����ϰ� �ʹ�.
	{
		//Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	//1. Ÿ����� �Ÿ��� �ʿ��ϴ�.
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

		//2. Ÿ����� �Ÿ��� ���� ������ ������ϱ�
		if (distance > attackRange)
		{
			//3. ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
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
	return;
}

void UEnemyFSM::BurntState()
{
	//���Ͱ� �������� �ִϸ��̼��� ����ϰ�
	me->OnMyBurn();
	//���Ϳ� ��Ÿ�� ����Ʈ�� ������Ų��.
	UGameplayStatics::SpawnEmitterAttached(fireFactory, me->GetMesh(), TEXT("Spine2Socket"));
	bDieEnd = true;
	me->hp = 0;
	SetState(EEnemyState::Die);
}

void UEnemyFSM::ShockedState()
{
	//���Ϳ� ������� ����Ʈ�� ������Ų��.
	UGameplayStatics::SpawnEmitterAttached(shockFactory, me->GetMesh(), TEXT("Spine2Socket"));
	
	//1. ���� �ð��� �ð��� ������Ų��.
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	//2. ���� �ð��� ��ũŸ�Ӻ��� Ŀ����
	if (currentTime > shockTime)

		//3. FSM�� ���¸� ���̵�� �����Ѵ�.
	{
		me->StopAnimMontage();
		SetState(EEnemyState::Idle);
	}
}

void UEnemyFSM::OnDamageProcess(int damageValue)
{
	//ü���� �Ҹ��ϰ�
	me->hp -= damageValue;
	//ü���� 0�� �Ǹ�
	if(me->hp <= 0)
	{
		if(bDieEnd == false)
		{
			//Die �Ѵ�.
			SetState(EEnemyState::Die);
			me->OnMyDamage(TEXT("Die"));
			bDieEnd = true;
		}
		else
		{
			return;
		}
	}

	//ü���� 0���� ũ��
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
				//Damage �ϰ� �ʹ�.
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
	me->enemyAnim->bAttackPlay = false;
	bAttackPlay = true;
	//3. ������ �ϰ� (������ ���ݰŸ� �ȿ� �ִ°�?)
	float dist = target->GetDistanceTo(me);
	if (dist <= attackRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is Attack"));
	}
}

