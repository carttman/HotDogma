// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonAnim.h"
#include "HD_Dragon.h"
#include "Kismet/KismetSystemLibrary.h"

void UHD_DragonAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		fsm = OwnerActor->FindComponentByClass<UHD_DragonFSM>();
		//middleBoss = OwnerActor->FindComponentByClass<AHJ_MiddleBoss>();
		Dragon = Cast<AHD_Dragon>(OwnerActor);

		// if (Dragon)
		// {
		// 	FVector velo = Dragon->GetVelocity();
		// 	FVector forwardVec = Dragon->GetActorForwardVector();
		// 	Speed = FVector::DotProduct(forwardVec, velo);
		// 	Direction = FVector::DotProduct(Dragon->GetActorRightVector(), velo);
		// }
	}
}

void UHD_DragonAnim::ChangeState(DragonState ChangeState)
{
	AnimState = ChangeState;
	if (fsm)
		fsm->State = ChangeState;
}

void UHD_DragonAnim::ChangeAttackState(AttackState ChangeState)
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

#pragma region 전투시작 전 Notify
void UHD_DragonAnim::AnimNotify_SleepEnd()
{
	bSleepEnd = false;
}

void UHD_DragonAnim::AnimNotify_StartFight()
{
	bPlayShoutAnim = true;
}

void UHD_DragonAnim::AnimNotify_endShout()
{
	bPlayShoutAnim = false;
	bEndStartAnim = true;

	fsm->bRotate = true;

	ChangeState(DragonState::Idle);
}
#pragma endregion

#pragma region 도약찍기 Notify
void UHD_DragonAnim::AnimNotify_bPress()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this; // or other relevant setup for LatentInfo
	UKismetSystemLibrary::Delay(this, 2.f, LatentInfo);
	bFlyPress = true;
}

void UHD_DragonAnim::AnimNotify_StartFlyPress()
{
	StartFlyUpFunction();
}

void UHD_DragonAnim::AnimNotify_PressEnd()
{
	bFlyPress = false;
}
#pragma endregion

#pragma region 꼬리치기, 손바닥 내려치기 Notify
void UHD_DragonAnim::AnimNotify_EndTailSlap()
{
	chkAngle = false;
}

void UHD_DragonAnim::AnimNotify_EndScratch()
{
	chkAngle = false;
}
#pragma endregion

void UHD_DragonAnim::AnimNotify_StartAttack()
{
	fsm->isAttack = true;
	fsm->CurrUsedSkillCnt++;
}

void UHD_DragonAnim::AnimNotify_EndAttack()
{
	ChangeState(DragonState::Idle);
	if (!fsm->chkOnceFly)
	{
		// 최초로 75%보다 낮아지면 하늘로 날아오른다.
		if (Dragon->MaxHP * 0.75 >= Dragon->CurrHP)
		{
			ChangeAttackState(AttackState::None);
			ChangeState(DragonState::Fly);
			fsm->chkOnceFly = true;
		}
	}

	fsm->isAttack = false;
}

void UHD_DragonAnim::AnimNotify_RotateFire()
{
	fsm->NowRotator = Dragon->GetActorRotation();
	fsm->BreathTimeline.PlayFromStart();
}

void UHD_DragonAnim::AnimNotify_StartFlyUp()
{
	fsm->isAttack = true;
}

void UHD_DragonAnim::AnimNotify_EndFlyUp()
{
	ChangeState(DragonState::Idle);
	fsm->CurrUsedSkillCnt = 0;
	chkUsingSkillCnt = true;
}

void UHD_DragonAnim::AnimNotify_StartFlyDown()
{
	fsm->isAttack = true;
}

void UHD_DragonAnim::AnimNotify_EndFlyDown()
{
	fsm->isAttack = false;
	fsm->CurrUsedSkillCnt = 0;
	chkUsingSkillCnt = false;
}

void UHD_DragonAnim::AnimNotify_StartFlyAttack()
{
	fsm->isAttack = true;
	fsm->CurrUsedSkillCnt++;
}

void UHD_DragonAnim::AnimNotify_EndFlyAttack()
{
	ChangeState(DragonState::Idle);
	// 날고있는 상태이고, 정해진 개수만큼 스킬을 사용했을 때
	if (fsm->CurrUsedSkillCnt == fsm->ApplySkillAsFly)
	{
		ChangeState(DragonState::FlyDown);
	}

	fsm->isAttack = false;
}
