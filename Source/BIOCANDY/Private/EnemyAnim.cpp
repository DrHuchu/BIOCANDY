// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "Enemy.h"

void UEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	me = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnim::OnHitEvent()
{
	if(me)
	{
		me->enemyFsm->OnHitEvent();
	}
}

void UEnemyAnim::AnimNotify_OnHit()
{
	if (this == nullptr)
		return;

	if (me)
		//enemyFSM의 OnHitEvent 함수를 호출한다.
	{
		me->enemyFsm->OnHitEvent();
	}
}
