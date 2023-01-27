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
	springArmComp->SetRelativeLocation(FVector(0, 50, 100));
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
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("SK_Pistol"));

	//5. 스나이퍼건 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	//5-1 부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("SK_Pistol"));
	//5-2 스태틱메시 데이터 로드 
	// 스나이퍼건의 에셋을 읽어서 컴포넌트에 넣고싶다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	//5-3 데이터로드가 성공했다면
	if (TempSniperMesh.Succeeded())
	{
		//5.4 스태틱메시 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		//5.5 위치 조정하기
		sniperGunComp->SetRelativeLocationAndRotation(FVector(-1.65f, 27.06f, 50.34f), FRotator(0, 0, 0));
		//5.6 크기 조정하기
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
	

	//인터랙션을 위한 박스콜리전 생성
	interactionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	interactionBox->SetupAttachment(RootComponent);
}

void APlayer_Jill::BeginPlay()
{
	Super::BeginPlay();
	// 기본으로 권총으로 사용하도록 설정
	ChangeToSK_Pistol();

	// 스나이퍼 UI 위젯 인스턴스를 생성하고싶다.
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	//일반 조준 UI 크로스헤어 인스턴스 생성
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	//일반 조준 UI 등록
	//_crosshairUI->AddToViewport();

	//키 경고 UI 생성
	keyWarningUI = CreateWidget(GetWorld(), keyWarningUIFactory);

	// hp
	hp = initialHp;
}

// hp 체력 히트 이벤트
void APlayer_Jill::OnHitEvent()
{
	PRINT_LOG(TEXT("Damaged !!!!!")); 
	hp--;
	if (hp <= 0)
	{
		PRINT_LOG(TEXT("Player is dead!"));
	}
}

void APlayer_Jill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// direction 방향으로 이동하고 싶다.

	FTransform trans(GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);

	resultDirection.Z = 0;
	resultDirection.Normalize();

	AddMovementInput(resultDirection);

	direction = FVector::ZeroVector;


	//상호작용 오브젝트 여러개 겹칠 때 하나만 나오게 하는 방법
	//overlappingActors라는 배열을 생성한다.
	TArray<AActor*> overlappingActors;

	//overlappingActors 배열에 인터랙션 박스와 겹치는 액터들을 담는다.
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

	PlayerInputComponent->BindAction(TEXT("SK_Pistol"), IE_Pressed, this, &APlayer_Jill::ChangeToSK_Pistol);
	//총 교체 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("sniperGun"), IE_Pressed, this, &APlayer_Jill::ChangeToSniperGun);

	//인터랙션 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &APlayer_Jill::OnInteract);

	//스나이퍼 조준 모드 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this,&APlayer_Jill::PistolAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this,&APlayer_Jill::PistolAim);
}
void APlayer_Jill::PistolAim()
{
	//스나이퍼건 모드가 아니면 처리하지 않는다.
	if (bSniperAim)
	{
		return;
	}
	// Pressed 입력 처리
	if (bUsingSK_Pistol == false)
	{
		// 스나이퍼 조준 모드 활성화
		bUsingSK_Pistol = true;
		// 스나이퍼 조준 UI 화면에서 제거
		_sniperUI->RemoveFromParent();
		// 카메라 시야각 원례대로 복원
		cameraComp->SetFieldOfView(90.0f);
		// 일반 조준 UI 등록
		//_crosshairUI->AddToViewport();
		IsShootReady = false;
	}
	// Released 입력 처리
	else
	{
		// 스나이퍼 조준 모드 비활성화
		bUsingSK_Pistol = false;
		// 스나이퍼 조준 UI 등록
		_sniperUI->AddToViewport();
		// 카메라의 시야각 Field Of View 설정
		cameraComp->SetFieldOfView(45.0f);
		// 일반 조준 UI 제거
		//_crosshairUI->RemoveFromParent();
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

	//권총 사용시
	if (bUsingSK_Pistol)
	{
		// LineTrace 시작위치
		FVector startPos = cameraComp->GetComponentLocation();
		// LineTrace 종료위치
		FVector endPos = cameraComp->GetComponentLocation() + cameraComp->GetForwardVector() * 5000;
		// LineTrace의 충돌 정보를 담을 변수
		//FHitResult hitInfo;
		//충돌 옵션 설정 변수
		FCollisionQueryParams params;
		//자기 자신(플레이어)는 충돌에서 제외
		params.AddIgnoredActor(this);
		//Channel 필터를 이용한 LineTrace 충돌검출 (충돌 정보, 시작 위치, 종료 위치, 검출 채널,충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	}

}

//권총을 변경
void APlayer_Jill::ChangeToSK_Pistol()
{
	// 권총 사용중으로 체크
	bUsingSK_Pistol = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}
//스나이퍼건으로 변경
void APlayer_Jill::ChangeToSniperGun()
{
	bUsingSK_Pistol = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

