// Fill out your copyright notice in the Description page of Project Settings.


#include "Explodable.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AExplodable::AExplodable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
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
	if (explodeCount != 0)
	{
		//���� ī��Ʈ�� 1 ��´�.
		explodeCount -= 1;
	}
	//���� ī��Ʈ�� 0�̶��
	else
	{
		//���� ����Ʈ�� ������Ű��

		//�����θ� �����Ѵ�.

	}
}

void AExplodable::ElectricShock()
{
	//���� ī��Ʈ�� 0�� �ƴ϶��
	if (explodeCount != 0)
	{
		//���� ī��Ʈ�� 1 ��´�.
		explodeCount -= 1;
	}
	//���� ī��Ʈ�� 0�̶��
	else
	{
		//���� ����Ʈ�� ������Ű��

		//�����θ� �����Ѵ�.

	}
}

