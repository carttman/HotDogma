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

void UHD_SorcererStateComponent::BeginPlay()
{
	Super::BeginPlay();

	SorcererAnimInstance = Cast<UHD_SorcererAnimInstance>(Me->GetMesh()->GetAnimInstance());
	
	// ���� ������ �����Ѵ�.
	PatternList.Add(ESorcererBattleState::State_MagickBolt);
	PatternList.Add(ESorcererBattleState::State_HighHagol);
	PatternList.Add(ESorcererBattleState::State_HighLevin);
	PatternList.Add(ESorcererBattleState::State_Levitate);
	PatternList.Add(ESorcererBattleState::State_ArgentSuccor);
	PatternList.Add(ESorcererBattleState::State_Galvanize);
}

void UHD_SorcererStateComponent::StartBattle()
{
	// ���� ���·� ��ȯ
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
{// ���� ���¿����� �巡���� �����Ѵ�.
	if (AIController)
	{
		if (TargetPawn != nullptr)
		{
			// �巡����� �Ÿ�
			float Distance = FVector::Dist(Me->GetActorLocation(), TargetPawn->GetActorLocation());
			// FindAttackPoint();
			if (Distance > 1200)
			{
				// Ÿ�� ������� �¿�� ���ݾ� �̵��Ѵ�.
				AIController->MoveToActor(TargetPawn, 1000.0f);
			}
			else
			{
				if (CombatTime < CurrentAttackTime)
				{
					// �巡���� �����Ѵ�.
					SetBattleState(ESorcererBattleState::State_MagickBolt);
				}
				else
				{
					// Ÿ���� �������� �������� �̵���Ų��.
				}
			}
		}
	}
}

void UHD_SorcererStateComponent::MagickBolt()
{
	//if (nullptr == SorcererAnimInstance)
	//{
	//	return;
	//}

	//// ���� �ִϸ��̼��� �����Ѵ�.
	//SorcererAnimInstance->PlayMagickBolt();

	//// ���� ��ġ�� ã�´�.
	//if (FindAttackPoint())
	//{
	//	// ���� ��ġ�� �̵��Ѵ�.
	//	AIController->MoveToLocation(AttackPoint, 100.0f);
	//}
}

void UHD_SorcererStateComponent::HighHagol()
{
}

void UHD_SorcererStateComponent::HighLevin()
{
}

void UHD_SorcererStateComponent::Levitate()
{
}

void UHD_SorcererStateComponent::ArgentSuccor()
{
}

void UHD_SorcererStateComponent::Galvanize()
{
}

void UHD_SorcererStateComponent::PatternRotting()
{
	// ���� ���� ������ 3���� �������� �ʴ´�.
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

	// �巡������ ��ȯ�Ѵ�.
	AHD_Dragon* Dragon = Cast<AHD_Dragon>(TargetPawn);
	if (nullptr == Dragon)
	{
		return false;
	}

	// ���� ����� Ÿ�� ��ġ�� �����Ѵ�.
	FVector TargetPoint1 = Dragon->TargetPoint1->GetComponentLocation();
	FVector TargetPoint2 = Dragon->TargetPoint2->GetComponentLocation();
	FVector TargetPoint3 = Dragon->TargetPoint3->GetComponentLocation();
	FVector TargetPoint4 = Dragon->TargetPoint4->GetComponentLocation();

	// ���� ����� Ÿ�� ��ġ�� �����Ѵ�.
	float Distance1 = FVector::Distance(Me->GetActorLocation(), TargetPoint1);
	float Distance2 = FVector::Distance(Me->GetActorLocation(), TargetPoint2);
	float Distance3 = FVector::Distance(Me->GetActorLocation(), TargetPoint3);
	float Distance4 = FVector::Distance(Me->GetActorLocation(), TargetPoint4);

	// ���� ����� Ÿ�� ��ġ�� �����Ѵ�.
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
