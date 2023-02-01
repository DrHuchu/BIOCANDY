// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerbox.h"
#include "Player_Jill.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

void APowerbox::InteractWithMe()
{
	player = Cast<APlayer_Jill>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//�÷��̾ �������� ���������� ��ȣ�ۿ� ����
	if (!bIsOn && player != nullptr && player->GetVelocity() == FVector(0, 0, 0))
	{
		//��ȣ�ۿ� Ű�� ������ currentTime�� ������Ų��.
		powerOn = true;


	}
	else
	{
		player->powerboxDoneUI->RemoveFromParent();
		//�̹� ���� �������� UI ǥ��
		player->powerboxDoneUI->AddToViewport();
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

		//�ð��� �� �������� �ʰ� �ϰ� �����ð��� �ʱ�ȭ��Ų��. 
		powerOn = false;
		currentTime = 0;

		//��ȣ�ۿ����� ���ϵ��� �Ѵ�.
		bIsOn = true;


		//�׸��� ���� ������ �� UI�� ����.

	}
	
	
		
	UE_LOG(LogTemp, Warning, TEXT("%f"), currentTime);
}
