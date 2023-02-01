// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerbox.h"
#include "Player_Jill.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

void APowerbox::InteractWithMe()
{
	player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//플레이어가 움직이지 않을때에만 상호작용 가능
	if (!bIsOn && player != nullptr && player->GetVelocity() == FVector(0, 0, 0))
	{
		//상호작용 키를 누르면 currentTime을 누적시킨다.
		powerOn = true;


	}
	else
	{
		player->powerboxDoneUI->RemoveFromParent();
		//이미 켜진 발전기라는 UI 표시
		player->powerboxDoneUI->AddToViewport();
	}
	
}

void APowerbox::OffInteractWithMe()
{
	//상호작용 키를 떼면 (Released)
	//시간 누적을 멈추고
	powerOn = false;
	//currentTime을 0으로 초기화시킨다.
	currentTime = 0;
}

void APowerbox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (powerOn)
	{
		currentTime += GetWorld()->GetDeltaSeconds();
	}

	player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//캐릭터가 움직이면 누적 시간을 초기화시킨다.
	if(player->GetVelocity() != FVector::ZeroVector)
	{
		powerOn = false;
		currentTime = 0;
	}

	//currentTime이 powerTime보다 커지면
	if (currentTime > powerTime)
	{
		//플레이어의 powerboxCount 를 1 올린다.
		player->powerboxCount += 1;

		//시간이 더 누적되지 않게 하고 누적시간을 초기화시킨다. 
		powerOn = false;
		currentTime = 0;

		//상호작용하지 못하도록 한다.
		bIsOn = true;


		//그리고 남은 발전기 수 UI를 띄운다.

	}
	
	
		
	UE_LOG(LogTemp, Warning, TEXT("%f"), currentTime);
}
