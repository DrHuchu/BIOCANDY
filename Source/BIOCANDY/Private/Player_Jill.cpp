#include "Player_Jill.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/SkeletalMeshComponent.h"
#include "Bullet.h"

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
	//4-2 스켈레탈메시 데이터 로드
	//ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
	//4-3 데이터 로드가 성공했다면
	//if (TempGunMesh.Succeeded())
	//{
		//4-4 스켈레탈메시 데이터 할당
	//	gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		//4-5 위치 조정하기
	//	gunMeshComp->SetRelativeLocation(FVector(-14,52,120));
	//}

	//5. 스나이퍼건 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	//5-1 부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperGunComp->SetupAttachment(GetMesh());
	//5-2 스태틱메시 데이터 로드 
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	//5-3 데이터로드가 성공했다면
	if (TempSniperMesh.Succeeded())
	{
		//5.4 스태틱메시 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		//5.5 위치 조정하기
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		//5.6 크기 조정하기
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
}


void APlayer_Jill::BeginPlay()
{
	Super::BeginPlay();
	// 기본으로 권총으로 사용하도록 설정
	ChangeToSK_Pistol();
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
	//총알 처리 발사
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
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

