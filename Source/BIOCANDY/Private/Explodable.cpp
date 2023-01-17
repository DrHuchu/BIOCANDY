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
	//폭발 카운트가 0이 아니라면
	if (explodeCount != 0)
	{
		//폭발 카운트를 1 깎는다.
		explodeCount -= 1;
	}
	//폭발 카운트가 0이라면
	else
	{
		//폭발 이펙트를 스폰시키고

		//스스로를 제거한다.

	}
}

void AExplodable::ElectricShock()
{
	//폭발 카운트가 0이 아니라면
	if (explodeCount != 0)
	{
		//폭발 카운트를 1 깎는다.
		explodeCount -= 1;
	}
	//폭발 카운트가 0이라면
	else
	{
		//폭발 이펙트를 스폰시키고

		//스스로를 제거한다.

	}
}

