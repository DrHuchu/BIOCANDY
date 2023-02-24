// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"

#include "Player_Jill.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEndPoint::AEndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEndPoint::OnEndPointOverlap);

	gameClearUI = CreateWidget(GetWorld(), gameClearUIFactory);
}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AEndPoint::OnEndPointOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	player = Cast<APlayer_Jill>(OtherActor);
	if(player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));

		player->bIsCleared = true;

		UGameplayStatics::SetGamePaused(this, true);

		float fireCount = (float) player->fireCount;
		float hitCount = (float) player->hitCount;
		float headCount = (float) player->headShotCount;

		player->hitRate = fireCount / hitCount;
		player->headRate = headCount / hitCount;

		gameClearUI->AddToViewport();

		//클리어 타임에 따른 점수 분배
		if(player->clearMinute <= 1)
		{
			player->finalScore += 500;
		}
		else if( player->clearMinute == 2)
		{
			if(player->clearSecond <= 30)
			{
				player->finalScore  += 500;
			}
			else if(player->clearSecond <= 45)
			{
				player->finalScore += 200;
			}
			else
			{
				player->finalScore += 100;
			}
		}
		else
		{
			player->finalScore += 100;
		}

		UE_LOG(LogTemp, Warning, TEXT("%d"), player->finalScore);
		
		//명중률에 따른 점수 분배
		if(player->hitRate >= 0.85f)
		{
			player->finalScore +=500;
		}
		else if(player->hitRate >=0.8f)
		{
			player->finalScore += 200;
		}
		else
		{
			player->finalScore += 100;
		}

		UE_LOG(LogTemp, Warning, TEXT("hit rate : %f"), player->hitRate);
		UE_LOG(LogTemp, Warning, TEXT("%d"), player->finalScore);
		
		//급소 명중률에 따른 점수 분배
		if(player->headRate >= 0.95f)
		{
			player->finalScore += 500;
		}
		else if(player->headRate >= 0.9f)
		{
			player->finalScore += 200;
		}
		else
		{
			player->finalScore += 100;
		}

		UE_LOG(LogTemp, Warning, TEXT("head rate : %f"), player->headRate);
		UE_LOG(LogTemp, Warning, TEXT("%d"), player->finalScore);
		
		
		//피격 횟수에 따른 점수 분배
		if(player->damageCount <= 5)
		{
			player->finalScore += 500;
		}
		else if(player->damageCount <= 7)
		{
			player->finalScore += 200;
		}
		else
		{
			player->finalScore += 100;
		}

		UE_LOG(LogTemp, Warning, TEXT("%d"), player->finalScore);
		
		//아이템 사용 횟수에 따른 점수 분배
		if(player->herbUseCount <= 1)
		{
			player->finalScore += 500;
		}
		else if(player->herbUseCount == 2)
		{
			player->finalScore += 200;
		}
		else
		{
			player->finalScore += 100;
		}	

		player->clearRankUI->AddToViewport();
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		UE_LOG(LogTemp, Warning, TEXT("%d"), player->finalScore);
	}
}
