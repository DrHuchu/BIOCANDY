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
	//���� ī��Ʈ�� 0�� �ƴ϶��
	if (explodeCount > 0)
	{
		//���� ī��Ʈ�� 1 ��´�.
		explodeCount -= 1;
	}
	//���� ī��Ʈ�� 0�̶��
	else
	{
		//���� ����Ʈ�� ������Ű��
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFactory, GetActorLocation());

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
		
		//���� ���� ���� ������ ��� ȭ�� ���·� �����.
		//1. overlappingActors��� �迭�� �����.
		TArray<AActor*> overlappingActors;
		//2. sphereComp�� ������ �Ǵ� ���͵��� overlappingActors�� ��´�.
		sphereComp->GetOverlappingActors(overlappingActors);
		//3. overlappingActors�� ��� ���Ͱ� ���ʹ̶�� ȭ�� ���·� ���̽�Ų��.
		for (auto currentActor : overlappingActors)
		{
			enemy = Cast<AEnemy>(currentActor);

			if(enemy)
			{
				enemy->enemyFsm->SetState(EEnemyState::Burnt);
			}
		}
		//�����θ� �����Ѵ�.
		Destroy();
	}
}

void AExplodable::ElectricShock()
{
		//���� ī��Ʈ�� 0�� �ƴ϶��
		if (shockCount == 30 || shockCount == 25 || shockCount == 15 || shockCount == 0)
		{
			//������� ����Ʈ�� ������Ű��
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFactory, GetActorLocation(), FRotator::ZeroRotator, FVector(1), true);
			
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), shockSound, GetActorLocation());
			//���� ���� ������ ��� ��ũ ���·� �����.
			//1. overlappingActors ��� �迭�� �����.	
			TArray<AActor*> overlappingActors;

			//2. sphereComp�� ��� ���͵��� overlappingActors�� ��´�.
			sphereComp->GetOverlappingActors(overlappingActors);

			//3. overlappingActors �ȿ� ��� ���͵��� ���¸� ��� Shocked�� �ٲ۴�.
			for (auto currentActor : overlappingActors)
			{
				//currentActor�� enemy�� ĳ���� ���� ���
				enemy = Cast<AEnemy>(currentActor);
				if (enemy)
				{
					//���ʹ��� ���°� die�� �ƴ϶��
					if (enemy->enemyFsm->mState != EEnemyState::Die)
					{
						//enemy �ȿ� �ִ� mstate�� shocked�� �����Ѵ�.
						enemy->enemyFsm->SetState(EEnemyState::Shocked);
						enemy->OnMyShock();
					}
				}
			}

			//���� ��ƼŬ�� �����Ѵ�.
			//----------------------------------------------------------------------------------------------------

			//��ũ ī��Ʈ�� 1 ��´�.
			shockCount -= 1;
			overlappingActors.Empty();

		}
		else
		{
			//��ũ ī��Ʈ�� 1 ��´�.
			shockCount -= 1;
		}
}

