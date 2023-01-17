#include "Player_Jill.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/SkeletalMeshComponent.h"
#include "Bullet.h"

APlayer_Jill::APlayer_Jill()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	//���̷�Ż�޽� �����͸� �ҷ����� �ʹ�.
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.Skeleton'/Game/Characters/Mannequins/Meshes/SK_Mannequin.SK_Mannequin'"));
	//if (tempMesh.Succeeded())
	
		//Mesh ������Ʈ�� ��ġ�� ȸ������ �����ϰ� �ʹ�.
		//GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//Mesh ������Ʈ�� ��ġ�� �����ϰ� �ʹ�.
		//GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	
	//��������, ī�޶� ������Ʈ�� �����ϰ� �ʹ�.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	// ���������� ��Ʈ�� ���̰�
	springArmComp->SetupAttachment(RootComponent);
	// ī�޶�� �������Ͽ� ���̰�ʹ�.
	cameraComp->SetupAttachment(springArmComp);

	//�������� ������ ���� ���ڸ� ����
	springArmComp->SetRelativeLocation(FVector(0, 50, 100));
	//�������� ������ ���� ���ڸ� ����
	springArmComp->TargetArmLength = 250.0f;

	//�Է°��� ȸ���� �ݿ��ϰ�ʹ�.
	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;

	//UCharacterMovementComponent
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//�޸��� 2���
	SprintSpeedMultiplier = 1.5f; 

	//�� ���̷�Ż�޽� ������Ʈ ���
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	//�θ� ������Ʈ�� Mesh ������Ʈ�� ����
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("SK_Pistol"));
	//4-2 ���̷�Ż�޽� ������ �ε�
	//ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
	//4-3 ������ �ε尡 �����ߴٸ�
	//if (TempGunMesh.Succeeded())
	//{
		//4-4 ���̷�Ż�޽� ������ �Ҵ�
	//	gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		//4-5 ��ġ �����ϱ�
	//	gunMeshComp->SetRelativeLocation(FVector(-14,52,120));
	//}

	//5. �������۰� ������Ʈ ���
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	//5-1 �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	sniperGunComp->SetupAttachment(GetMesh());
	//5-2 ����ƽ�޽� ������ �ε� 
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	//5-3 �����ͷε尡 �����ߴٸ�
	if (TempSniperMesh.Succeeded())
	{
		//5.4 ����ƽ�޽� ������ �Ҵ�
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		//5.5 ��ġ �����ϱ�
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		//5.6 ũ�� �����ϱ�
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
}


void APlayer_Jill::BeginPlay()
{
	Super::BeginPlay();
	// �⺻���� �������� ����ϵ��� ����
	ChangeToSK_Pistol();
}


void APlayer_Jill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// direction �������� �̵��ϰ� �ʹ�.

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

	//�Ѿ� �߻� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayer_Jill::InputFire);

	PlayerInputComponent->BindAction(TEXT("SK_Pistol"), IE_Pressed, this, &APlayer_Jill::ChangeToSK_Pistol);
	//�� ��ü �̺�Ʈ ó�� �Լ� ���ε�
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
	// Pitch ȸ�� 
	AddControllerPitchInput(value);
}

void APlayer_Jill::OnAxisTurnRight(float value)
{
	// Yaw ȸ��
	AddControllerYawInput(value);
}

void APlayer_Jill::OnActionJump()
{
	Jump();
}

//�޸���
void APlayer_Jill::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}
//�޸��� ����
void APlayer_Jill::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

void APlayer_Jill::InputFire()
{
	//�Ѿ� ó�� �߻�
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}

//������ ����
void APlayer_Jill::ChangeToSK_Pistol()
{
	// ���� ��������� üũ
	bUsingSK_Pistol = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}
//�������۰����� ����
void APlayer_Jill::ChangeToSniperGun()
{
	bUsingSK_Pistol = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

