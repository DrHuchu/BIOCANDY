#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class BIOCANDY_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABullet();

protected:
	
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

public:
	//발사체의 이동을 담당할 컴포넌트
	UPROPERTY(VisibleAnywhere, Category=Movement)
	class UProjectileMovementComponent*movementComp;
	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category=Collision)
	class USphereComponent*collisionComp;
	//외관 컴포넌트
	UPROPERTY(VisibleAnywhere, Category=BodyMesh)
	class UStaticMeshComponent*bodyMeshComp;
	//총알 제거 함수 
	void Die();

	
};
