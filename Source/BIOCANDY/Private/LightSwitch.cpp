// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSwitch.h"
#include "Components/PointLightComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ALightSwitch::ALightSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	lightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Button"));
	lightComp->SetupAttachment(RootComponent);

	interactionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	interactionWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();

	lightComp->SetIntensity(0);
	interactionWidget->SetVisibility(false);
}

// Called every frame
void ALightSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightSwitch::InteractWithMe()
{
	if(bIsOn)
	{
		lightComp->SetIntensity(0);
		bIsOn = false;
	}
	else
	{
		lightComp->SetIntensity(10000);
		bIsOn = true;
	}
}

void ALightSwitch::ShowInteractionWidget()
{
	interactionWidget->SetVisibility(true);
}

void ALightSwitch::HideInteractionWidget()
{
	interactionWidget->SetVisibility(false);
}

