// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_SorcererStateComponent.h"
#include "HotDogma/Ksw/Companions/HD_CompanionAIController.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "HotDogma/Ksw/Companions/HD_SorcererAnimInstance.h"
#include <HotDogma/LHJ/HD_Dragon.h>
#include "Components/CapsuleComponent.h"
#include <HotDogma/Ksw/HD_Projectile.h>
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

void UHD_SorcererStateComponent::BeginPlay()
{
	Super::BeginPlay();

	SorcererAnimInstance = Cast<UHD_SorcererAnimInstance>(Me->GetMesh()->GetAnimInstance());
	
	// 전투 패턴을 세팅한다.
	PatternList.Add(ESorcererBattleState::State_MagickBolt);
	//PatternList.Add(ESorcererBattleState::State_HighHagol);
	//PatternList.Add(ESorcererBattleState::State_HighLevin);
	PatternList.Add(ESorcererBattleState::State_Levitate);
	//PatternList.Add(ESorcererBattleState::State_ArgentSuccor);
	//PatternList.Add(ESorcererBattleState::State_Galvanize);
}

void UHD_SorcererStateComponent::StartBattle()
{
	// 전투 상태로 전환
	SetBattleState(ESorcererBattleState::State_CombatCheck);
}

void UHD_SorcererStateComponent::AttackTick(float DeltaTime)
{
	Super::AttackTick(DeltaTime);
	CurrentAttackTime += DeltaTime;
	switch (CurrentBattleState)
	{
		case ESorcererBattleState::State_CombatCheck:
			CombatCheck();
			break;
		case ESorcererBattleState::State_MagickBolt:
			MagickBolt();
			break;
		case ESorcererBattleState::State_HighHagol:
			break;
		case ESorcererBattleState::State_HighLevin:
			break;
		case ESorcererBattleState::State_Levitate:
			Levitate();
			break;
		case ESorcererBattleState::State_ArgentSuccor:
			break;
		case ESorcererBattleState::State_Galvanize:
			break;
	}

	FString myState = UEnum::GetValueOrBitfieldAsString(CurrentBattleState);
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() + FVector(0, 0, 100), myState, 0, FColor::Yellow, 0);
}

void UHD_SorcererStateComponent::SetBattleState(ESorcererBattleState state)
{
	CurrentBattleState = state;
	CurrentAttackTime = 0.0f;
	
}

void UHD_SorcererStateComponent::CombatCheck()
{// 전투 상태에서는 드래곤을 공격한다.
	if (AIController)
	{
		if (TargetPawn != nullptr)
		{
			// 드래곤과의 거리
			float Distance = FVector::Dist(Me->GetActorLocation(), TargetPawn->GetActorLocation());
			// FindAttackPoint();
			if (Distance > 1200)
			{
				// 타겟 대상으로 좌우로 조금씩 이동한다.
				AIController->MoveToActor(TargetPawn, 1000.0f);
			}
			else
			{
				if (CombatTime < CurrentAttackTime)
				{
					// 드래곤을 공격한다.
					SetBattleState(ESorcererBattleState::State_Levitate);
				}
				else
				{
					// 타겟을 원점으로 원형으로 이동시킨다.
				}
			}
		}
	}
}

void UHD_SorcererStateComponent::MagickBolt()
{
	// 시작
	if (! bCastingMagickBolt)
	{
		// 캐스팅 애니메이션을 실행한다.
		SorcererAnimInstance->PlayAttackMontage(0);
		bCastingMagickBolt = true;

		return;
	}

	float AttackTime = MagickBoltTime;
	if (MagickBoltCount == 0)
	{
		AttackTime += MagickBoltCastTime;
	}

	if (AttackTime < CurrentAttackTime)
	{
		if (MagickBoltCount < MaxMagickBoltCount)
		{
			SorcererAnimInstance->PlayAttackMontage(1);
			// 볼트를 발사한다.
			// 타겟을 찾는다.
			if (FindAttackPoint())
			{
				// 볼트를 스폰하고 타겟을 향해 발사한다.
				FVector StartLocation = Me->GetActorLocation();
				FVector EndLocation = AttackPoint;
				// 볼트를 스폰한다.
				AHD_Projectile* MagickBolt = GetWorld()->SpawnActor<AHD_Projectile>(MagickBoltFactory, Me->ArrowComp->GetComponentTransform());
				MagickBolt->SetTarget(AttackPoint);
			}

			MagickBoltCount++;
			CurrentAttackTime = 0.0f;
		}
		else
		{
			SorcererAnimInstance->PlayAttackMontage(2);
			// 캐스팅을 종료한다.
			bCastingMagickBolt = false;
			MagickBoltCount = 0;
			SetBattleState(ESorcererBattleState::State_CombatCheck);
		}
	}
}

void UHD_SorcererStateComponent::HighHagol()
{
}

void UHD_SorcererStateComponent::HighLevin()
{
}

void UHD_SorcererStateComponent::Levitate()
{
	if (nullptr == SorcererAnimInstance)
	{
		return;
	}
	if (SorcererAnimInstance->bLevitate)
	{
		// 높이 제한
		if (Me->GetActorLocation().Z < 300)
		{
			Me->AddMovementInput(FVector(0, 0, 0.5), 1);
		}
		else
		{
			SetBattleState(ESorcererBattleState::State_MagickBolt);
		}
		
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Levitate"));

	// 부유 애니메이션을 실행한다.
	SorcererAnimInstance->PlayLevitate();

	// 중력을 제거하고 캐릭터를 올린다.
	// 무브먼트 모드 변경
	Me->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	// 천천히 올린다.
	Me->AddMovementInput(FVector(0, 0, 0.5), 1);

	// 5초 후에 중력을 다시 적용한다.
	GetWorld()->GetTimerManager().SetTimer(LevitateTimerHandle, this, &UHD_SorcererStateComponent::EndLevitate, LevitateTime, false);
}

void UHD_SorcererStateComponent::EndLevitate()
{
	// 중력을 다시 적용한다.
	Me->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SorcererAnimInstance->EndLevitate();

}

void UHD_SorcererStateComponent::ArgentSuccor()
{
}

void UHD_SorcererStateComponent::Galvanize()
{
}

void UHD_SorcererStateComponent::PatternRotting()
{
	// 랜덤 셔플 마지막 3번은 변경하지 않는다.
	for (int i = 0; i < PatternList.Num(); i++)
	{
		int32 RandomInt = FMath::RandRange(0, PatternList.Num() - 1);
		ESorcererBattleState Temp = PatternList[i];
		PatternList[i] = PatternList[RandomInt];
		PatternList[RandomInt] = Temp;
	}
}

ESorcererBattleState UHD_SorcererStateComponent::NextPattern()
{
	if (PatternIndex < PatternList.Num())
	{
		return PatternList[PatternIndex++];
	}
	else
	{
		PatternRotting();
		PatternIndex = 0;
		return PatternList[PatternIndex++];
	}
}

bool UHD_SorcererStateComponent::FindAttackPoint()
{
	if (nullptr == TargetPawn)
	{
		return false;
	}

	// 드래곤으로 변환한다.
	AHD_Dragon* Dragon = Cast<AHD_Dragon>(TargetPawn);
	if (nullptr == Dragon)
	{
		return false;
	}

	// 가장 가까운 타격 위치를 저장한다.
	FVector TargetPoint1 = Dragon->TargetPoint1->GetComponentLocation();
	FVector TargetPoint2 = Dragon->TargetPoint2->GetComponentLocation();
	FVector TargetPoint3 = Dragon->TargetPoint3->GetComponentLocation();
	FVector TargetPoint4 = Dragon->TargetPoint4->GetComponentLocation();

	// 가장 가까운 타격 위치를 저장한다.
	float Distance1 = FVector::Distance(Me->GetActorLocation(), TargetPoint1);
	float Distance2 = FVector::Distance(Me->GetActorLocation(), TargetPoint2);
	float Distance3 = FVector::Distance(Me->GetActorLocation(), TargetPoint3);
	float Distance4 = FVector::Distance(Me->GetActorLocation(), TargetPoint4);

	// 가장 가까운 타격 위치를 저장한다.
	AttackPoint = TargetPoint1;
	float MinDistance = Distance1;
	if (MinDistance > Distance2)
	{
		AttackPoint = TargetPoint2;
		MinDistance = Distance2;
	}

	if (MinDistance > Distance3)
	{
		AttackPoint = TargetPoint3;
		MinDistance = Distance3;
	}

	if (MinDistance > Distance4)
	{
		AttackPoint = TargetPoint4;
		MinDistance = Distance4;
	}

	return true;
}
