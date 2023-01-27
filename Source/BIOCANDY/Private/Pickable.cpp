// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable.h"

#include "Player_Jill.h"
#include "Components/WidgetComponent.h"

// Sets default values
APickable::APickable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	interactionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	interactionWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickable::BeginPlay()
{
	Super::BeginPlay();
	interactionWidget->SetVisibility(false);
}

// Called every frame
void APickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APickable::InteractWithMe()
{
	if (!bIsOn)
	{
		//Destroy();
		bIsOn = true;
	}
	else
	{
		bIsOn = false;
	}
}

void APickable::ShowInteractionWidget()
{
	interactionWidget->SetVisibility(true);
}

void APickable::HideInteractionWidget()
{
	interactionWidget->SetVisibility(false);
}

