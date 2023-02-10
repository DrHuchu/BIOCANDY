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
	springArmComp->SetRelativeLocation(FVector(0, 50, 60));
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
	gunMeshComp->SetupAttachment(GetMesh());
	
	//���ͷ����� ���� �ڽ��ݸ��� ����
	interactionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	interactionBox->SetupAttachment(RootComponent);

	//���ӿ��� �� �ֺ� ���� ���� ���̸� ���� ���Ǿ� �ݸ��� ����
	gameOverSphere = CreateDefaultSubobject<USphereComponent>(TEXT("gameOverSphere"));
	gameOverSphere->SetupAttachment(RootComponent);
}

void APlayer_Jill::BeginPlay()
{
	Super::BeginPlay();
	// �⺻���� �������� ����ϵ��� ����

	// �������� UI ���� �ν��Ͻ��� �����ϰ�ʹ�.
	pistolCrossHairUI = CreateWidget(GetWorld(), pistolCrossHairUIFactory);

	//Ű ��� UI ����
	keyWarningUI = CreateWidget(GetWorld(), keyWarningUIFactory);

	//�Ŀ��ڽ� �Ϸ� UI ����
	powerboxDoneUI = CreateWidget(GetWorld(), powerboxUIFactory);

	//�Ŀ��ڽ� ���� �Ϸ� UI ����
	powerboxAllDoneUI = CreateWidget(GetWorld(), powerboxAllDoneUIFactory);

	//Ż�⹮ ���º��� UI ����
	escapeDoorUI = CreateWidget(GetWorld(), escapeDoorUIFactory);

	// ��ź UI ����
	magUI = CreateWidget(GetWorld(), magUIFactory);

	//�ǰ� ȿ�� UI ����
	damagedUI = CreateWidget(GetWorld(), damagedUIFactory);

	//���� ���� UI ����
	gameOverUI = CreateWidget(GetWorld(), gameOverUIFactory);

	// hp
	hp = maxHP;

	cameraComp->SetFieldOfView(60);

	// ���� ���� curveFloat ���� �� ����
	if(curveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &APlayer_Jill::APlayer_Jill::Zoom);
		timeline.AddInterpFloat(curveFloat, TimelineProgress);
	}
}

// hp ü�� ��Ʈ �̺�Ʈ
void APlayer_Jill::OnHitEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged"));
	hp--;
	if (hp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
	}
}
//������
void APlayer_Jill::OnActionReload_Implementation()
{
		bIsReloading = true;
		FTimerHandle reloadTimer;
		GetWorldTimerManager().SetTimer(reloadTimer, this, &APlayer_Jill::Reload, 2.2f, false);
}

void APlayer_Jill::Reload()
{
		//źâ�� �����ִ� �Ѿ��� ���ڰ� ź���� �ִ�ġ���� ������
		if (pistolCountMag < maxPistolCountMag)
		{
			// �������Ѵ�.
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
	// direction �������� �̵��ϰ� �ʹ�.
	timeline.TickTimeline(DeltaTime);

	FTransform trans(GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);

	resultDirection.Z = 0;
	resultDirection.Normalize();

	AddMovementInput(resultDirection.GetSafeNormal());

	direction = FVector::ZeroVector;


	//��ȣ�ۿ� ������Ʈ ������ ��ĥ �� �ϳ��� ������ �ϴ� ���
	//overlappingActors��� �迭�� �����Ѵ�.
	TArray<AActor*> overlappingActors;

	//overlappingActors �迭�� ���ͷ��� �ڽ��� ��ġ�� ���͵��� ��´�.+
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

	//���ͷ��� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &APlayer_Jill::OnInteract);
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Released, this, &APlayer_Jill::OffInteract);

	//�������� ���� ��� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this,&APlayer_Jill::PistolAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this,&APlayer_Jill::PistolAim);

	//������
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
	// Pressed �Է� ó��
	if (bUsingSK_Pistol == false)
	{
			// �������� ���� ��� Ȱ��ȭ
			bUsingSK_Pistol = true;
			// ���� ũ�ν���� UI ȭ�鿡�� ����
		if(pistolCrossHairUI->IsInViewport())
		{
			pistolCrossHairUI->RemoveFromParent();
		}
			// ī�޶� �þ߰� ������� ����
			//cameraComp->SetFieldOfView(60.0f);
			timeline.Reverse();
			//���� �߻� �غ� X
			IsShootReady = false;
	}
	// Released �Է� ó��
	else
	{
			//��ź �� UI ���� �� ��ǥ��
		if(magUI->IsInViewport())
		{
			magUI->RemoveFromParent();
		}
			magUI->AddToViewport();
			// �������� ���� ��� ��Ȱ��ȭ
			bUsingSK_Pistol = false;
			// ���� ũ�ν���� UI ȭ�鿡 �߰�
		if(pistolCrossHairUI->IsInViewport())
		{
			pistolCrossHairUI->RemoveFromParent();
		}
			pistolCrossHairUI->AddToViewport();
			// ī�޶��� �þ߰� Field Of View ����
			//cameraComp->SetFieldOfView(40.0f);
			timeline.Play();
			//���� �߻� �غ� O
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

void APlayer_Jill::OffInteract()
{
	if(interface != nullptr)
	{
		interface->OffInteractWithMe();
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
}

