// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_WarriorStateComponent.h"

#include "HotDogma/Ksw/Companions/HD_CompanionAIController.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHD_WarriorStateComponent::StartBattle()
{
	// 전투 상태로 전환
	CurrentBattleState = EWarriorBattleState::State_CombatCheck;
}

void UHD_WarriorStateComponent::AttackTick(float DeltaTime)
{
	Super::AttackTick(DeltaTime);
	
	switch (CurrentBattleState)
	{
		case EWarriorBattleState::State_CombatCheck:
			CombatCheck();
			break;
		case EWarriorBattleState::State_FindAttackPoint:
			FindAttackPoint();
			break;
		case EWarriorBattleState::State_MightySweep:
			break;
		case EWarriorBattleState::State_ChargedSlash:
			break;
		case EWarriorBattleState::State_HeavenwardSunder:
			break;
		case EWarriorBattleState::State_IndomitableLash:
			break;
	}
}

void UHD_WarriorStateComponent::CombatCheck()
{
	// 전투 상태에서는 드래곤을 공격한다.
	if (AIController)
	{
		if (TargetPawn != nullptr)
		{
			// 드래곤과의 거리
			float Distance = FVector::Dist(Me->GetActorLocation(), TargetPawn->GetActorLocation());
			FindAttackPoint();
			if (Distance > 500)
			{
				// 드래곤을 향해 이동한다.
				AIController->MoveToActor(TargetPawn, 200.0f);
			}
			else
			{
				// 드래곤을 공격한다.
				FindAttackPoint();
			}
		}
	}
}

bool UHD_WarriorStateComponent::FindAttackPoint()
{
	bool IsAttackPoint = false;
	// 360도 방향으로 레이를 쏜다.
	for (int i = 0; i < 36; i++)
	{
		FVector Start = Me->GetActorLocation();

		FVector End = UKismetMathLibrary::GetForwardVector(Me->GetActorRotation()) * 1000.0f;
		End = UKismetMathLibrary::RotateAngleAxis(End, 10.0f * i, FVector(0, 0, 1));
		End += Start;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		
		CollisionParams.AddIgnoredActor(Me);

		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel5, CollisionParams);

		if (HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, 0.1f, 0, 1.0f);
			if (IsAttackPoint)
			{

				int dist = FVector::Dist(Me->GetActorLocation(), AttackPoint);
				int dist2 = FVector::Dist(Me->GetActorLocation(), HitResult.ImpactPoint);
				if (dist > dist2)
				{
					AttackPoint = HitResult.ImpactPoint;
				}
			}
			else
			{
				AttackPoint = HitResult.ImpactPoint;
			}
			
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, 0, 1.0f);
		}
	}

	return IsAttackPoint;
}

void UHD_WarriorStateComponent::MightySweep()
{
	// 전투 상태에서는 드래곤을 공격한다.
	if (CurrentCombo == 0)
	{
		if (AIController)
		{
			if (TargetPawn != nullptr)
			{
				// 드래곤과의 거리
				float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
				FindAttackPoint();
				if (Distance > MightySweepRange)
				{
					// 드래곤을 향해 이동한다.
					AIController->MoveToLocation(AttackPoint, 200.0f);
				}
				else
				{
					// 드래곤을 공격한다.
					// 몽타주 실행
				}
			}
		}
	}
	else
	{
		// 몽타주가 끝났다면.
		if (FindAttackPoint())
		{
			float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
			if (Distance < MightySweepRange)
			{
				// 다음 공격
			}
		}
	}
}

void UHD_WarriorStateComponent::ChargedSlash()
{

}

void UHD_WarriorStateComponent::HeavenwardSunder()
{

}

void UHD_WarriorStateComponent::IndomitableLash()
{

}
