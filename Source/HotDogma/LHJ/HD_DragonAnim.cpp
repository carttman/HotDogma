#include "../LHJ/HD_DragonAnim.h"
#include "HD_Dragon.h"
#include "Components/CapsuleComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerComponent/PlayerStatusComponent.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UHD_DragonAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		fsm = OwnerActor->FindComponentByClass<UHD_DragonFSM>();
		Dragon = Cast<AHD_Dragon>(OwnerActor);
	}
}

#pragma region Change State
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
#pragma endregion

#pragma region Sleep
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
	if (fsm)
		fsm->bRotate = true;

	ChangeState(DragonState::Idle);
}
#pragma endregion

#pragma region FlyUp & FlyDown
void UHD_DragonAnim::AnimNotify_StartFlyUp()
{
	if (fsm)
	{
		fsm->isAttack = true;
		fsm->CurrUsedSkillCnt = 0;
	}
}

void UHD_DragonAnim::AnimNotify_EndFlyUp()
{
	ChangeState(DragonState::Idle);
	if (fsm)
	{
		fsm->isAttack = false;
		fsm->chkOnceFly = true;
	}
	isFly = true;

	if (Dragon && Dragon->gm)
		Dragon->gm->PlaySoundAtIndex(16);
}

void UHD_DragonAnim::AnimNotify_StartFlyDown()
{
	if (fsm)
	{
		fsm->isAttack = true;
		fsm->CurrUsedSkillCnt = 0;
		fsm->ApplySkillAsFly = 0;
	}
}

void UHD_DragonAnim::AnimNotify_EndFlyDown()
{
	if (fsm)
		fsm->isAttack = false;

	ChangeState(DragonState::Idle);
}
#pragma endregion

#pragma region Attack Common
void UHD_DragonAnim::AnimNotify_StartAttack()
{
	if (fsm)
	{
		fsm->isAttack = true;
		fsm->CurrUsedSkillCnt++;
		fsm->TotalUsingSkillCnt++;
	}
}

void UHD_DragonAnim::AnimNotify_EndAttack()
{
	EndAttack();
	ChangeState(DragonState::Idle);
	chkAngle = false;
}

void UHD_DragonAnim::AnimNotify_StartFlyAttack()
{
	if (fsm)
	{
		fsm->isAttack = true;
		fsm->CurrUsedSkillCnt++;
		fsm->TotalUsingSkillCnt++;
	}
}

void UHD_DragonAnim::AnimNotify_EndFlyAttack()
{
	EndAttack();
	
	if (fsm)
	{		
		// 날고있는 상태이고, 정해진 개수만큼 스킬을 사용했을 때
		if (fsm->CurrUsedSkillCnt >= fsm->ApplySkillAsFly)
		{
			ChangeState(DragonState::FlyDown);
		}
		else
		{
			ChangeState(DragonState::Idle);
		}		
	}
}

void UHD_DragonAnim::EndAttack()
{
	if (Dragon && Dragon->gm && fsm)
	{
		if (fsm->normalAttackState == AttackState::Meteor)
		{
			Dragon->gm->PlaySoundAtIndex(12);
		}
	}
	ChangeAttackState(AttackState::None);
	if(fsm)
	{		
		fsm->isAttack = false;
		fsm->iCastingCnt = 0;
		fsm->bStartMeteor = false;
		fsm->bStartThunder = false;
		fsm->bReturnLightColor = false;
	}
	if (Dragon)
		Dragon->strDamageAttackType = "";
}

void UHD_DragonAnim::ApplyDamageToTarget(AActor* Target, float Damage, FString DamageType)
{
	Dragon->strDamageAttackType = DamageType;
	UGameplayStatics::ApplyDamage(Target, Damage, Dragon->GetController(), Dragon,
										  UDamageType::StaticClass());
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
												  ActorsToIgnore, EDrawDebugTrace::None, OutHits,
												  true);

	for (auto& Hit : OutHits)
	{
		if (IsValid(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s TargetLoc : %.f, %.f, %.f"),
				   *Hit.GetActor()->GetName(), Hit.GetActor()->GetActorLocation().X,
				   Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z);

			if (Hit.GetActor()->Tags.Num() > 0 && (Hit.GetActor()->Tags[0].ToString().Equals("HD_Player")))
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

#pragma region Breath
void UHD_DragonAnim::AnimNotify_StartBreath()
{
	if (fsm)
	{
		fsm->bBreathAttack = true;
		if (fsm)
			fsm->SetDynamicMaterialInstanceValue(150.f);
	}
}

void UHD_DragonAnim::AnimNotify_EndBreath()
{
	if (fsm)
	{
		fsm->bBreathAttack = false;
		fsm->bReturnLightColor = true;
		fsm->LightColorAlpha = 0;
		if (fsm)
			fsm->SetDynamicMaterialInstanceValue(1.f);
	}
}
#pragma endregion

#pragma region Shout
void UHD_DragonAnim::AnimNotify_AttackShout()
{
	bool bRtn = GetAttackPress(fsm->HandPressAttackDist);
	if (bRtn)
	{
		for (auto OtherActor : DamageActorSet)
			ApplyDamageToTarget(OtherActor, fsm->Damage_Shout,"Shout");

		DamageActorSet.Empty();
	}
}
#pragma endregion

#pragma region HandPress
void UHD_DragonAnim::AnimNotify_AttackHandPress()
{
	bool bRtn = GetAttackPress(fsm->HandPressCameraDist);
	if (bRtn)
	{
		for (auto DamageOtherActor : DamageActorSet)
		{
			if (DamageOtherActor->GetName().Contains("Player"))
			{
				AHD_CharacterPlayer* player = Cast<AHD_CharacterPlayer>(DamageOtherActor);
				if (player)
				{
					if (player->PlayerStatusComponent->CurrHP > 0)
						player->GetPlayerCameraShake();
				}
			}
		}

		DamageActorSet.Empty();
	}

	bRtn = GetAttackPress(fsm->HandPressAttackDist);
	if (bRtn)
	{
		for (auto OtherActor : DamageActorSet)
			ApplyDamageToTarget(OtherActor, fsm->Damage_HandPress,"HandPress");

		DamageActorSet.Empty();
	}
}
#pragma endregion

#pragma region Jump Press
void UHD_DragonAnim::AnimNotify_bPress()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this; // or other relevant setup for LatentInfo
	UKismetSystemLibrary::Delay(this, 2.f, LatentInfo);
	bFlyPress = true;
}

void UHD_DragonAnim::AnimNotify_AttackJumpPress()
{
	bool bRtn = GetAttackPress(fsm->JumpPressCameraDist);
	if (bRtn)
	{
		for (auto DamageOtherActor : DamageActorSet)
		{
			if (DamageOtherActor->GetName().Contains("Player"))
			{
				AHD_CharacterPlayer* player = Cast<AHD_CharacterPlayer>(DamageOtherActor);
				if (player)
				{
					if (player->PlayerStatusComponent->CurrHP > 0)
						player->GetPlayerCameraShake();
				}
			}
		}

		DamageActorSet.Empty();
	}

	bRtn = GetAttackPress(fsm->JumpPressAttackDist);
	if (bRtn)
	{
		for (auto OtherActor : DamageActorSet)
			ApplyDamageToTarget(OtherActor, fsm->Damage_JumpPress,"JumpPress");

		DamageActorSet.Empty();
	}
}

void UHD_DragonAnim::AnimNotify_PressEnd()
{
	bFlyPress = false;
}

// void UHD_DragonAnim::AnimNotify_StartFlyPress()
// {
// 	if (fsm)
// 		fsm->bStartFlyPress = true;
// }
#pragma endregion

#pragma region Tail & Scratch
void UHD_DragonAnim::AnimNotify_MyStartTailSlap()
{
	Dragon->TailCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UHD_DragonAnim::AnimNotify_EndTailSlap()
{
	Dragon->TailCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	Dragon->DamageActorSet.Empty();
}

void UHD_DragonAnim::AnimNotify_ClearSet()
{
	Dragon->DamageActorSet.Empty();
}
#pragma endregion

#pragma region Magic Attack
void UHD_DragonAnim::AnimNotify_StartThunderAttack()
{
	if (fsm)
	{
		fsm->iCastingCnt = 0;
		fsm->bStartThunder = true;
	}
}

void UHD_DragonAnim::AnimNotify_StartMeteorAttack()
{
	if (fsm)
	{
		fsm->iCastingCnt = 0;
		fsm->bStartMeteor = true;
	}
}
#pragma endregion

#pragma region Change Material
void UHD_DragonAnim::AnimNotify_ChangeMat()
{
	if (fsm)
		fsm->SetDynamicMaterialInstanceValue(150.f);
}

void UHD_DragonAnim::AnimNotify_ReturnMat()
{
	if (fsm)
		fsm->SetDynamicMaterialInstanceValue(1.f);
}
#pragma endregion


void UHD_DragonAnim::AnimNotify_RotateFire()
{
	if (fsm)
	{
		fsm->NowRotator = Dragon->GetActorRotation();
		//fsm->BreathTimeline.PlayFromStart();
	}
}