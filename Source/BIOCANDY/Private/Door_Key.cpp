// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_Key.h"

#include "Player_Jill.h"
#include "Components/WidgetComponent.h"


// Sets default values
ADoor_Key::ADoor_Key()
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
void ADoor_Key::BeginPlay()
{
	Super::BeginPlay();

	interactionWidget->SetVisibility(false);

	if (curveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor_Key::OpenDoor);
		timeline.AddInterpFloat(curveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor_Key::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timeline.TickTimeline(DeltaTime);
}

void ADoor_Key::InteractWithMe()
{
	if (!bIsOn)
	{
		player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (player)
		{
			if (player->bIsKeyOn)
			{
				timeline.Play();
				bIsOn = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Key is needed"));
			}
		}
	}
	else
	{
		player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (player)
		{
			if (player->bIsKeyOn)
			{
				timeline.Reverse();
				bIsOn = false;
			}
		}
	}
}

void ADoor_Key::ShowInteractionWidget()
{
	interactionWidget->SetVisibility(true);
}

void ADoor_Key::HideInteractionWidget()
{
	interactionWidget->SetVisibility(false);
}

void ADoor_Key::OpenDoor(float value)
{
	FRotator rot = FRotator(0, doorRotateAngle * value, 0);

	meshComp->SetRelativeRotation(rot);
}

