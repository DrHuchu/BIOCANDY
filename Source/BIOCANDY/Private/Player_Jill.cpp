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

	//5. �������۰� ������Ʈ ���
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	//5-1 �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("SK_Pistol"));
	//5-2 ����ƽ�޽� ������ �ε� 
	// �������۰��� ������ �о ������Ʈ�� �ְ�ʹ�.
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	//5-3 �����ͷε尡 �����ߴٸ�
	if (TempSniperMesh.Succeeded())
	{
		//5.4 ����ƽ�޽� ������ �Ҵ�
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		//5.5 ��ġ �����ϱ�
		sniperGunComp->SetRelativeLocationAndRotation(FVector(-1.65f, 27.06f, 50.34f), FRotator(0, 0, 0));
		//5.6 ũ�� �����ϱ�
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
	

	//���ͷ����� ���� �ڽ��ݸ��� ����
	interactionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	interactionBox->SetupAttachment(RootComponent);
}

void APlayer_Jill::BeginPlay()
{
	Super::BeginPlay();
	// �⺻���� �������� ����ϵ��� ����
	ChangeToSK_Pistol();

	// �������� UI ���� �ν��Ͻ��� �����ϰ�ʹ�.
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	//�Ϲ� ���� UI ũ�ν���� �ν��Ͻ� ����
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	//�Ϲ� ���� UI ���
	//_crosshairUI->AddToViewport();

	//Ű ��� UI ����
	keyWarningUI = CreateWidget(GetWorld(), keyWarningUIFactory);

	// hp
	hp = initialHp;
}

// hp ü�� ��Ʈ �̺�Ʈ
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
	// direction �������� �̵��ϰ� �ʹ�.

	FTransform trans(GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);

	resultDirection.Z = 0;
	resultDirection.Normalize();

	AddMovementInput(resultDirection);

	direction = FVector::ZeroVector;


	//��ȣ�ۿ� ������Ʈ ������ ��ĥ �� �ϳ��� ������ �ϴ� ���
	//overlappingActors��� �迭�� �����Ѵ�.
	TArray<AActor*> overlappingActors;

	//overlappingActors �迭�� ���ͷ��� �ڽ��� ��ġ�� ���͵��� ��´�.
	interactionBox->GetOverlappingActors(overlappingActors);


	//�ƹ��͵� ���������� �ʾ������� = overlappingActors �迭�� ���� 0�϶��� �ƹ��͵� �� �Ͼ�� ��
	if(overlappingActors.Num() == 0)
	{
		//�������̽��� �������Ͱ� �ƴ϶��(���) ������ �Ⱥ��̰� ó����
		if(interface)
		{
			interface->HideInteractionWidget();
			interface = nullptr;
		}
		return;
	}

	//ù ��°�� ���� ���͸� ����Ű�� �����͸� closestActor��� �����Ѵ�.
	AActor* closestActor = overlappingActors[0];

	
	for(auto currentActor:overlappingActors)
	{
		//���� üũ�� �迭�� ���Ϳ��� �Ÿ��� ù��° ��� ���ͺ��� ������ ���� ���͸� ���� ����� ���ͷ� �ʱ�ȭ�Ѵ�.
		if(GetDistanceTo(currentActor) < GetDistanceTo(closestActor))
		{
			closestActor = currentActor;
		}
	}

	//�Ϲ����� ��� -> �������� ���Ͱ� �׳� ������ ��쿡��
	if(interface)
	{
		//�������̽��� �����.
		interface->HideInteractionWidget();
	}


	//�������� ���Ͱ� ���� ����� ���Ͷ��
	interface=Cast<IInteractionInterface>(closestActor);

	if(interface)
	{
		//�������̽��� ǥ���Ѵ�.
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

	//�Ѿ� �߻� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayer_Jill::InputFire);

	PlayerInputComponent->BindAction(TEXT("SK_Pistol"), IE_Pressed, this, &APlayer_Jill::ChangeToSK_Pistol);
	//�� ��ü �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("sniperGun"), IE_Pressed, this, &APlayer_Jill::ChangeToSniperGun);

	//���ͷ��� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &APlayer_Jill::OnInteract);

	//�������� ���� ��� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this,&APlayer_Jill::PistolAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this,&APlayer_Jill::PistolAim);
}
void APlayer_Jill::PistolAim()
{
	//�������۰� ��尡 �ƴϸ� ó������ �ʴ´�.
	if (bSniperAim)
	{
		return;
	}
	// Pressed �Է� ó��
	if (bUsingSK_Pistol == false)
	{
		// �������� ���� ��� Ȱ��ȭ
		bUsingSK_Pistol = true;
		// �������� ���� UI ȭ�鿡�� ����
		_sniperUI->RemoveFromParent();
		// ī�޶� �þ߰� ���ʴ�� ����
		cameraComp->SetFieldOfView(90.0f);
		// �Ϲ� ���� UI ���
		//_crosshairUI->AddToViewport();
		IsShootReady = false;
	}
	// Released �Է� ó��
	else
	{
		// �������� ���� ��� ��Ȱ��ȭ
		bUsingSK_Pistol = false;
		// �������� ���� UI ���
		_sniperUI->AddToViewport();
		// ī�޶��� �þ߰� Field Of View ����
		cameraComp->SetFieldOfView(45.0f);
		// �Ϲ� ���� UI ����
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

void APlayer_Jill::OnInteract()
{
	if(interface != nullptr)
	{
		interface->InteractWithMe();
	}
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

	//���� ����
	if (bUsingSK_Pistol)
	{
		// LineTrace ������ġ
		FVector startPos = cameraComp->GetComponentLocation();
		// LineTrace ������ġ
		FVector endPos = cameraComp->GetComponentLocation() + cameraComp->GetForwardVector() * 5000;
		// LineTrace�� �浹 ������ ���� ����
		//FHitResult hitInfo;
		//�浹 �ɼ� ���� ����
		FCollisionQueryParams params;
		//�ڱ� �ڽ�(�÷��̾�)�� �浹���� ����
		params.AddIgnoredActor(this);
		//Channel ���͸� �̿��� LineTrace �浹���� (�浹 ����, ���� ��ġ, ���� ��ġ, ���� ä��,�浹 �ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	}

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

