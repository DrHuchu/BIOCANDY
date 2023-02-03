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
		//enemyFSM�� OnHitEvent �Լ��� ȣ���Ѵ�.
	{
		me->enemyFsm->OnHitEvent();
	}
}
