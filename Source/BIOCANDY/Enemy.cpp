// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Player_Jill.h"
#include "Math/Rotator.h"

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
		//UE_LOG(LogTemp, Warning, TEXT("recognized"));
		//direction�� �÷��̾� �������� ����
		direction = player->GetActorLocation() - GetActorLocation();
		//direction�� �븻������
		direction.Normalize();
		//direction �������� moveSpeed�� �ӵ��� �̵�
		SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
		SetActorRotation(direction.Rotation());
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
	player = Cast<APlayer_Jill>(OtherActor);
	//�������Ȱ� �÷��̾���
	if (player != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlap"));
		//recogDelay��ŭ �ð��� �帥 �ڿ� is Recognized�� true�� ����
		FTimerHandle recogTimer;
		GetWorld()->GetTimerManager().SetTimer(recogTimer, this, &AEnemy::Recognition, recogDelay, false);
	}
}

void AEnemy::Recognition()
{
	isRecognized = true;
}

void AEnemy::RecognitionOff(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Ovelap End"));
	isRecognized = false;
}

