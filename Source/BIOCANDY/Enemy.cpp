// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/SphereComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Radius for Recognition"));
	sphereComp->SetupAttachment(RootComponent);
	sphereComp->SetSphereRadius(recognitionRadius);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::Moving);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::RecognitionOff);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//�÷��̾ �ν��� ��� = isRecognized�� true�� ���
	if (isRecognized)
	{
		//direction�� �÷��̾� �������� ����
		//direction = player->GetActorLocation() - GetActorLocation();
		//direction�� �븻������
		//direction.Normalize();
		//direction �������� moveSpeed�� �ӵ��� �̵�
		SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
	}
	else
	{
		return;
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Moving(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�������Ȱ� �÷��̾���
	//if (player != nullptr)
	//{
		//recogDelay��ŭ �ð��� �帥 �ڿ� is Recognized�� true�� ����
		FTimerHandle recogTimer;
		GetWorld()->GetTimerManager().SetTimer(recogTimer, this, &AEnemy::Recognition, recogDelay, false);
	//}
}

void AEnemy::Recognition()
{
	isRecognized = true;
}

void AEnemy::RecognitionOff(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isRecognized = false;
}

