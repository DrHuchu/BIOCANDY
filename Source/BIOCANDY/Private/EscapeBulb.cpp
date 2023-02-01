// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeBulb.h"

#include "Player_Jill.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEscapeBulb::AEscapeBulb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
	lightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("lightComp"));
	lightComp->SetupAttachment(meshComp);

}

// Called when the game starts or when spawned
void AEscapeBulb::BeginPlay()
{
	Super::BeginPlay();

	lightComp->SetIntensity(0);
}

// Called every frame
void AEscapeBulb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (player != nullptr)
	{
		if (player->powerboxCount == lightCount)
		{
			lightComp->SetIntensity(300);
		}
	}
}

void AEscapeBulb::LightOn()
{
	player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (player != nullptr)
	{
		if (player->powerboxCount == lightCount)
		{
			lightComp->SetIntensity(300);
		}
	}
}

