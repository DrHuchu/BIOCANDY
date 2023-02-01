// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerbox.h"
#include "Player_Jill.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "EscapeBulb.h"
#include "Kismet/GameplayStatics.h"

void APowerbox::InteractWithMe()
{
	player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (player != nullptr)
	{
		//�÷��̾ �������� ���������� ��ȣ�ۿ� ����
		if (!bIsOn)
		{
			if (player->GetVelocity() == FVector(0, 0, 0))
			{//��ȣ�ۿ� Ű�� ������ currentTime�� ������Ų��.
				powerOn = true;
			}
		}
		else
		{
			//�����Ⱑ ��� ���ư� ���¶��
			if(player->powerboxCount == 4)
			{
				//�����Ⱑ ��� �����ִٴ� UI ǥ��
				player->powerboxAllDoneUI->RemoveFromParent();
				player->powerboxAllDoneUI->AddToViewport();
			}
			//�����Ⱑ ��� ���ư��� ���� ���¶��
			else
			{
				player->powerboxDoneUI->RemoveFromParent();
				//�̹� ���� �������� UI ǥ��
				player->powerboxDoneUI->AddToViewport();
			}
		}
	}
	
}

void APowerbox::OffInteractWithMe()
{
	//��ȣ�ۿ� Ű�� ���� (Released)
	//�ð� ������ ���߰�
	powerOn = false;
	//currentTime�� 0���� �ʱ�ȭ��Ų��.
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

	//ĳ���Ͱ� �����̸� ���� �ð��� �ʱ�ȭ��Ų��.
	if(player->GetVelocity() != FVector::ZeroVector)
	{
		powerOn = false;
		currentTime = 0;
	}

	//currentTime�� powerTime���� Ŀ����
	if (currentTime > powerTime)
	{
		//�÷��̾��� powerboxCount �� 1 �ø���.
		player->powerboxCount += 1;

		//Ż�⹮ ������ LightOn �Լ��� ȣ���Ѵ�.
		bulb = Cast<AEscapeBulb>(UGameplayStatics::GetActorOfClass(GetWorld(), AEscapeBulb::StaticClass()));
		if(bulb != nullptr)
		{
			bulb->LightOn();
		}

		//�ð��� �� �������� �ʰ� �ϰ� �����ð��� �ʱ�ȭ��Ų��. 
		powerOn = false;
		currentTime = powerTime;

		//��ȣ�ۿ����� ���ϵ��� �Ѵ�.
		bIsOn = true;

		//�׸��� ���� ������ �� UI�� ����.

	}
	
	
		
	//UE_LOG(LogTemp, Warning, TEXT("%f"), currentTime);
}
