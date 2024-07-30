// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonAnim.h"
#include "HD_Dragon.h"

void UHD_DragonAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		fsm = OwnerActor->FindComponentByClass<UHD_DragonFSM>();
		//middleBoss = OwnerActor->FindComponentByClass<AHJ_MiddleBoss>();
		Dragon = Cast<AHD_Dragon>(OwnerActor);

		if (Dragon)
		{
			FVector velo = Dragon->GetVelocity();
			FVector forwardVec = Dragon->GetActorForwardVector();
			Speed = FVector::DotProduct(forwardVec, velo);
			Direction = FVector::DotProduct(Dragon->GetActorRightVector(), velo);
		}
	}
}

void UHD_DragonAnim::PlayShoutAnim()
{
	bPlayShoutAnim = true;
}

void UHD_DragonAnim::ChangeState(DragonState ChangeState)
{
	AnimState = ChangeState;
	if (fsm)
		fsm->State = ChangeState;
}

void UHD_DragonAnim::ChangeNormalAttack(AttackState ChangeState)
{
	AnimNormalAttackState = ChangeState;
	if (fsm)
		fsm->normalAttackState = ChangeState;
}

void UHD_DragonAnim::StartFlyUpFunction()
{
	if (fsm)
		fsm->bStartFlyPress = true;
}

void UHD_DragonAnim::AnimNotify_SleepEnd()
{
	bSleepEnd=false;
}

void UHD_DragonAnim::AnimNotify_StartFight()
{
	bPlayShoutAnim=true;
}

void UHD_DragonAnim::AnimNotify_endShout()
{
	bPlayShoutAnim=false;
	bEndStartAnim=true;
}
