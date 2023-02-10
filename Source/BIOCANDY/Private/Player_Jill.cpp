#include "Player_Jill.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/SkeletalMeshComponent.h"
#include "Bullet.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include "InteractionInterface.h"
#include "Components/BoxComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Blueprint/UserWidget.h"
#include "../BIOCANDY.h"
#include "PlayerJillAnim.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"

APlayer_Jill::APlayer_Jill()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	//스켈레탈메시 데이터를 불러오고 싶다.
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.Skeleton'/Game/Characters/Mannequins/Meshes/SK_Mannequin.SK_Mannequin'"));
	//if (tempMesh.Succeeded())
	
		//Mesh 컴포넌트의 위치와 회전값을 설정하고 싶다.
		//GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//Mesh 컴포넌트의 위치를 설정하고 싶다.
		//GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	
	//스프링암, 카메라 컴포넌트를 생성하고 싶다.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	// 스프링암을 루트에 붙이고
	springArmComp->SetupAttachment(RootComponent);
	// 카메라는 스프링암에 붙이고싶다.
	cameraComp->SetupAttachment(springArmComp);

	//디테일의 스프링 암의 숫자를 변경
	springArmComp->SetRelativeLocation(FVector(0, 50, 60));
	//디테일의 스프링 암의 숫자를 변경
	springArmComp->TargetArmLength = 250.0f;

	//입력값을 회전에 반영하고싶다.
	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;

	//UCharacterMovementComponent
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//달리기 2배속
	SprintSpeedMultiplier = 1.5f; 

	//총 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh());
	
	//인터랙션을 위한 박스콜리전 생성
	interactionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	interactionBox->SetupAttachment(RootComponent);

	//게임오버 때 주변 좀비 상태 전이를 위한 스피어 콜리전 생성
	gameOverSphere = CreateDefaultSubobject<USphereComponent>(TEXT("gameOverSphere"));
	gameOverSphere->SetupAttachment(RootComponent);
}

void APlayer_Jill::BeginPlay()
{
	Super::BeginPlay();
	// 기본으로 권총으로 사용하도록 설정

	// 스나이퍼 UI 위젯 인스턴스를 생성하고싶다.
	pistolCrossHairUI = CreateWidget(GetWorld(), pistolCrossHairUIFactory);

	//키 경고 UI 생성
	keyWarningUI = CreateWidget(GetWorld(), keyWarningUIFactory);

	//파워박스 완료 UI 생성
	powerboxDoneUI = CreateWidget(GetWorld(), powerboxUIFactory);

	//파워박스 전부 완료 UI 생성
	powerboxAllDoneUI = CreateWidget(GetWorld(), powerboxAllDoneUIFactory);

	//탈출문 전력부족 UI 생성
	escapeDoorUI = CreateWidget(GetWorld(), escapeDoorUIFactory);

	// 잔탄 UI 생성
	magUI = CreateWidget(GetWorld(), magUIFactory);

	//피격 효과 UI 생성
	damagedUI = CreateWidget(GetWorld(), damagedUIFactory);

	//게임 오버 UI 생성
	gameOverUI = CreateWidget(GetWorld(), gameOverUIFactory);

	// hp
	hp = maxHP;

	cameraComp->SetFieldOfView(60);

	// 줌을 위한 curveFloat 존재 시 진행
	if(curveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &APlayer_Jill::APlayer_Jill::Zoom);
		timeline.AddInterpFloat(curveFloat, TimelineProgress);
	}
}

// hp 체력 히트 이벤트
void APlayer_Jill::OnHitEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged"));
	hp--;
	if (hp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
	}
}
//재장전
void APlayer_Jill::OnActionReload_Implementation()
{
		bIsReloading = true;
		FTimerHandle reloadTimer;
		GetWorldTimerManager().SetTimer(reloadTimer, this, &APlayer_Jill::Reload, 2.2f, false);
}

void APlayer_Jill::Reload()
{
		//탄창에 남아있는 총알의 숫자가 탄탕의 최대치보다 작을때
		if (pistolCountMag < maxPistolCountMag)
		{
			// 재장전한다.
			if (pistolCountBag != 0)
			{
				if (pistolCountBag >= (maxPistolCountMag - pistolCountMag))
				{
					pistolCountBag = pistolCountBag - (maxPistolCountMag - pistolCountMag);
					pistolCountMag = pistolCountMag + (maxPistolCountMag - pistolCountMag);
					bIsReloading = false;
				}
				else
				{
					pistolCountMag = pistolCountMag + pistolCountBag;
					pistolCountBag = 0;
					bIsReloading = false;
				}
			}
		}
}

void APlayer_Jill::Zoom(float value)
{
	float zoomRate = 60 - 20 * value;
	cameraComp->SetFieldOfView(zoomRate);
}


void APlayer_Jill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// direction 방향으로 이동하고 싶다.
	timeline.TickTimeline(DeltaTime);

	FTransform trans(GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);

	resultDirection.Z = 0;
	resultDirection.Normalize();

	AddMovementInput(resultDirection.GetSafeNormal());

	direction = FVector::ZeroVector;


	//상호작용 오브젝트 여러개 겹칠 때 하나만 나오게 하는 방법
	//overlappingActors라는 배열을 생성한다.
	TArray<AActor*> overlappingActors;

	//overlappingActors 배열에 인터랙션 박스와 겹치는 액터들을 담는다.+
	interactionBox->GetOverlappingActors(overlappingActors);


	//아무것도 오버랩되지 않았을때는 = overlappingActors 배열의 수가 0일때는 아무것도 안 일어나게 함
	if(overlappingActors.Num() == 0)
	{
		//인터페이스가 널포인터가 아니라면(방어) 위젯이 안보이게 처리함
		if(interface)
		{
			interface->HideInteractionWidget();
			interface = nullptr;
		}
		return;
	}

	//첫 번째로 담기는 액터를 가리키는 포인터를 closestActor라고 정의한다.
	AActor* closestActor = overlappingActors[0];

	
	for(auto currentActor:overlappingActors)
	{
		//현재 체크한 배열의 액터와의 거리가 첫번째 담긴 액터보다 가까우면 현재 액터를 가장 가까운 액터로 초기화한다.
		if(GetDistanceTo(currentActor) < GetDistanceTo(closestActor))
		{
			closestActor = currentActor;
		}
	}

	//일반적인 경우 -> 오버랩된 액터가 그냥 액터일 경우에는
	if(interface)
	{
		//인터페이스를 숨긴다.
		interface->HideInteractionWidget();
	}


	//오버랩된 액터가 가장 가까운 액터라면
	interface=Cast<IInteractionInterface>(closestActor);

	if(interface)
	{
		//인터페이스를 표시한다.
		interface->ShowInteractionWidget();
	}

}


void APlayer_Jill::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayer_Jill::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayer_Jill::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Look Up"), this, &APlayer_Jill::OnAxisLookup);
	PlayerInputComponent->BindAxis(TEXT("Turn Right"), this, &APlayer_Jill::OnAxisTurnRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayer_Jill::OnActionJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayer_Jill::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayer_Jill::StopSprinting);

	//총알 발사 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayer_Jill::InputFire);

	//인터랙션 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &APlayer_Jill::OnInteract);
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Released, this, &APlayer_Jill::OffInteract);

	//스나이퍼 조준 모드 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this,&APlayer_Jill::PistolAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this,&APlayer_Jill::PistolAim);

	//재장전
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this,&APlayer_Jill::OnActionReload);
}

void APlayer_Jill::OnMyGameOver_Implementation()
{
	TArray<AActor*> overlappingActors;
	gameOverSphere->GetOverlappingActors(overlappingActors);

	for(auto currentActor : overlappingActors)
	{
		enemy = Cast<AEnemy>(currentActor);
		if(enemy)
		{
			if(enemy->enemyFsm->mState != EEnemyState::Die)
			{
				enemy->enemyFsm->SetState(EEnemyState::Eating);
				enemy->OnMyEating();
			}
		}
	}
	SetActorEnableCollision(false);
}

void APlayer_Jill::OnMyHit_Implementation(int AttackDamage)
{
	hp -= AttackDamage;

	if(bIsOver == false)
	{
		if(hp <= 0)
		{
			bIsOver = true;
			OnMyGameOver();
		}
		else
		{
			if(damagedUI->IsInViewport())
			{
				damagedUI->RemoveFromParent();
			}
			damagedUI->AddToViewport();
		}
	}
	
}

void APlayer_Jill::PistolAim()
{
	// Pressed 입력 처리
	if (bUsingSK_Pistol == false)
	{
			// 스나이퍼 조준 모드 활성화
			bUsingSK_Pistol = true;
			// 권총 크로스헤어 UI 화면에서 제거
		if(pistolCrossHairUI->IsInViewport())
		{
			pistolCrossHairUI->RemoveFromParent();
		}
			// 카메라 시야각 원래대로 복원
			//cameraComp->SetFieldOfView(60.0f);
			timeline.Reverse();
			//권총 발사 준비 X
			IsShootReady = false;
	}
	// Released 입력 처리
	else
	{
			//잔탄 수 UI 제거 및 재표시
		if(magUI->IsInViewport())
		{
			magUI->RemoveFromParent();
		}
			magUI->AddToViewport();
			// 스나이퍼 조준 모드 비활성화
			bUsingSK_Pistol = false;
			// 권총 크로스헤어 UI 화면에 추가
		if(pistolCrossHairUI->IsInViewport())
		{
			pistolCrossHairUI->RemoveFromParent();
		}
			pistolCrossHairUI->AddToViewport();
			// 카메라의 시야각 Field Of View 설정
			//cameraComp->SetFieldOfView(40.0f);
			timeline.Play();
			//권총 발사 준비 O
			IsShootReady = true;
		
	}
}


void APlayer_Jill::OnAxisHorizontal(float value)
{
	direction.Y = value;
}

void APlayer_Jill::OnAxisVertical(float value)
{
	direction.X = value;
}

void APlayer_Jill::OnAxisLookup(float value)
{
	// Pitch 회전 
	AddControllerPitchInput(value);
}

void APlayer_Jill::OnAxisTurnRight(float value)
{
	// Yaw 회전
	AddControllerYawInput(value);
}

void APlayer_Jill::OnActionJump()
{
	Jump();
}

void APlayer_Jill::OnInteract()
{
	if(interface != nullptr)
	{
		interface->InteractWithMe();
	}
}

void APlayer_Jill::OffInteract()
{
	if(interface != nullptr)
	{
		interface->OffInteractWithMe();
	}
}

//달리기
void APlayer_Jill::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}
//달리기 멈춤
void APlayer_Jill::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

void APlayer_Jill::InputFire()
{
}

