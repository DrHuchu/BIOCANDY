// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"
#include "Player_Jill.h"
#include "EnemyAnim.h"

#include "Components/SphereComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Radius for Recognition"));
	//sphereComp->SetupAttachment(RootComponent);
	//sphereComp->SetSphereRadius(800.0f);

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrint/ABP_Enemy.ABP_Enemy_C'"));

	if(tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	enemyFsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	pawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("pawnSensor"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//플레이어 인식용 sphere
	//sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::Moving);
	//sphereComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::RecognitionOff);

	enemyAnim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	////플레이어를 인식할 경우 = isRecognized가 true일 경우
	//if (isRecognized)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("recognized"));
	//	//direction을 플레이어 방향으로 설정
	//	direction = player->GetActorLocation() - GetActorLocation();
	//	//direction을 노말라이즈
	//	direction.Normalize();
	//	//direction 방향으로 moveSpeed의 속도로 이동
	//	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
	//	SetActorRotation(direction.Rotation());
	//}
	//else
	//{
	//	return;
	//}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Moving(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<APlayer_Jill>(OtherActor);
	//오버랩된게 플레이어라면
	if (player != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlap"));
		//recogDelay만큼 시간이 흐른 뒤에 is Recognized를 true로 변경
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

