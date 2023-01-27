// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Player_Jill.h"
#include "Components/WidgetComponent.h"


// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	interactionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	interactionWidget->SetupAttachment(meshComp);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	interactionWidget->SetVisibility(false);

	if(curveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor::OpenDoor);
		timeline.AddInterpFloat(curveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timeline.TickTimeline(DeltaTime);
}

void ADoor::InteractWithMe()
{
	if(!bIsOn)
	{
		timeline.Play();
		bIsOn = true;
	}
	else
	{
		timeline.Reverse();
		bIsOn = false;
	}
}

void ADoor::ShowInteractionWidget()
{
	interactionWidget->SetVisibility(true);
}

void ADoor::HideInteractionWidget()
{
	interactionWidget->SetVisibility(false);
}

void ADoor::OpenDoor(float value)
{
	FRotator rot = FRotator(0, doorRotateAngle * value, 0);

	meshComp->SetRelativeRotation(rot);
}

