// Fill out your copyright notice in the Description page of Project Settings.


#include "Explodable.h"

#include "Enemy.h"
#include "EnemyFSM.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplodable::AExplodable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("radius"));
	sphereComp->SetupAttachment(RootComponent);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AExplodable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplodable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplodable::Explosion()
{
	//폭발 카운트가 0이 아니라면
	if (explodeCount > 0)
	{
		//폭발 카운트를 1 깎는다.
		explodeCount -= 1;
	}
	//폭발 카운트가 0이라면
	else
	{
		//폭발 이펙트를 스폰시키고
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFactory, GetActorLocation());

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
		
		//폭발 범위 내의 적들을 모두 화형 상태로 만든다.
		//1. overlappingActors라는 배열을 만든다.
		TArray<AActor*> overlappingActors;
		//2. sphereComp에 오버랩 되는 액터들을 overlappingActors에 담는다.
		sphereComp->GetOverlappingActors(overlappingActors);
		//3. overlappingActors에 담긴 액터가 에너미라면 화형 상태로 전이시킨다.
		for (auto currentActor : overlappingActors)
		{
			enemy = Cast<AEnemy>(currentActor);

			if(enemy)
			{
				enemy->enemyFsm->SetState(EEnemyState::Burnt);
			}
		}
		//스스로를 제거한다.
		Destroy();
	}
}

void AExplodable::ElectricShock()
{
		//폭발 카운트가 0이 아니라면
		if (shockCount == 30 || shockCount == 25 || shockCount == 15 || shockCount == 0)
		{
			//전기충격 이펙트를 스폰시키고
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFactory, GetActorLocation(), FRotator::ZeroRotator, FVector(1), true);
			
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), shockSound, GetActorLocation());
			//범위 내의 적들을 모두 쇼크 상태로 만든다.
			//1. overlappingActors 라는 배열을 만든다.	
			TArray<AActor*> overlappingActors;

			//2. sphereComp에 닿는 액터들을 overlappingActors에 담는다.
			sphereComp->GetOverlappingActors(overlappingActors);

			//3. overlappingActors 안에 담긴 액터들의 상태를 모두 Shocked로 바꾼다.
			for (auto currentActor : overlappingActors)
			{
				//currentActor가 enemy로 캐스팅 됐을 경우
				enemy = Cast<AEnemy>(currentActor);
				if (enemy)
				{
					//에너미의 상태가 die가 아니라면
					if (enemy->enemyFsm->mState != EEnemyState::Die)
					{
						//enemy 안에 있는 mstate를 shocked로 전이한다.
						enemy->enemyFsm->SetState(EEnemyState::Shocked);
						enemy->OnMyShock();
					}
				}
			}

			//전기 파티클을 해제한다.
			//----------------------------------------------------------------------------------------------------

			//쇼크 카운트를 1 깎는다.
			shockCount -= 1;
			overlappingActors.Empty();

		}
		else
		{
			//쇼크 카운트를 1 깎는다.
			shockCount -= 1;
		}
}

