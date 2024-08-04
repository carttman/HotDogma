// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonAnim.h"
#include "HD_Dragon.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

void UHD_DragonAnim::AnimNotify_AttackJumpPress()
{
	bool bRtn = GetAttackPress(fsm->JumpPressAttackDist);
	if (bRtn)
	{
		for (auto OtherActor : DamageActorSet)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage_JumpPress, Dragon->GetController(), Dragon,
			                              UDamageType::StaticClass());
		}

		DamageActorSet.Empty();
	}
}

void UHD_DragonAnim::AnimNotify_AttackHandPress()
{
	bool bRtn = GetAttackPress(fsm->HandPressAttackDist);
	if (bRtn)
	{
		for (auto OtherActor : DamageActorSet)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage_HandPress, Dragon->GetController(), Dragon,
			                              UDamageType::StaticClass());
		}

		DamageActorSet.Empty();
	}
}


bool UHD_DragonAnim::GetAttackPress(const float& AttackDistance)
{
	bool bRtn = false;
	FVector Start = Dragon->GetActorLocation();
	FVector End = Dragon->GetActorLocation();
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel4;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Dragon);
	bRtn = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, AttackDistance,
	                                              UEngineTypes::ConvertToTraceType(CollisionChannel), false,
	                                              ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits,
	                                              true);

	for (auto& Hit : OutHits)
	{
		if (IsValid(Hit.GetActor()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s TargetLoc : %.f, %.f, %.f"),
			       *Hit.GetActor()->GetName(), Hit.GetActor()->GetActorLocation().X,
			       Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z);
			FVector JumpPressTarget = Hit.GetActor()->GetActorLocation(); //- Player->GetActorLocation();
			//UE_LOG(LogTemp, Warning, TEXT("Cutting_TargetLoc : %.f, %.f, %.f"), Cutting_TargetLoc.X, Cutting_TargetLoc.Y, Cutting_TargetLoc.Z);
			//UE_LOG(LogTemp, Warning, TEXT("Player->GetActorLocation() : %.f, %.f, %.f"), Player->GetActorLocation().X, Player->GetActorLocation().Y, Player->GetActorLocation().Z);
			FVector newLoc = JumpPressTarget - Dragon->GetActorLocation();
			newLoc.Normalize();

			JumpPress_Target_Rot = UKismetMathLibrary::MakeRotFromX(newLoc);

			if (Hit.GetActor()->Tags.Num() > 0 && (Hit.GetActor()->Tags[0].ToString().Equals("HD_Player") || Hit.
				GetActor()->Tags[0].ToString().Equals("CompanionCharacter")))
			{
				if (!DamageActorSet.Contains(Hit.GetActor()))
				{
					DamageActorSet.Add(Hit.GetActor());
				}
			}
		}
	}

	if (DamageActorSet.Num() > 0)
		bRtn = true;

	return bRtn;
}
#pragma endregion

#pragma region 꼬리치기, 손바닥 내려치기 Notify
void UHD_DragonAnim::AnimNotify_StartTailSlap()
{
	Dragon->TailCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UHD_DragonAnim::AnimNotify_EndTailSlap()
{
	Dragon->TailCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	chkAngle = false;
	Dragon->DamageActorSet.Empty();
}

void UHD_DragonAnim::AnimNotify_StartScratch()
{
	Dragon->HandCollision_L->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Dragon->HandCollision_R->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UHD_DragonAnim::AnimNotify_EndScratch()
{
	Dragon->HandCollision_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dragon->HandCollision_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	chkAngle = false;
	Dragon->DamageActorSet.Empty();
}

void UHD_DragonAnim::AnimNotify_ClearSet()
{
	Dragon->DamageActorSet.Empty();
}
#pragma endregion

void UHD_DragonAnim::AnimNotify_StartAttack()
{
	fsm->isAttack = true;
	fsm->CurrUsedSkillCnt++;
}

void UHD_DragonAnim::AnimNotify_EndAttack()
{
	if (!fsm->chkOnceFly)
	{
		// 최초로 75%보다 낮아지면 하늘로 날아오른다.
		if (Dragon->MaxHP * 0.75 >= Dragon->CurrHP)
		{
			ChangeAttackState(AttackState::None);
			ChangeState(DragonState::Fly);
			fsm->chkOnceFly = true;
		}
		else
		{
			ChangeState(DragonState::Idle);
		}
	}
	else
	{
		ChangeState(DragonState::Idle);
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

void UHD_DragonAnim::AnimNotify_StartBreath()
{
	//Dragon->FireCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UHD_DragonAnim::AnimNotify_EndBreath()
{
	//Dragon->FireCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
