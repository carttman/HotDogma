// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_WarriorStateComponent.h"

#include "HotDogma/Ksw/Companions/HD_CompanionAIController.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "HotDogma/Ksw/Companions/HD_WarriorAnimInstance.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

void UHD_WarriorStateComponent::BeginPlay()
{
	Super::BeginPlay();
	WarriorAnimInstance = Cast<UHD_WarriorAnimInstance>(AnimInstance);

	// 전투 패턴을 세팅한다.
	PatternList.Add(EWarriorBattleState::State_MightySweep);
	PatternList.Add(EWarriorBattleState::State_ChargedSlash);
	PatternList.Add(EWarriorBattleState::State_HeavenwardSunder);
	PatternList.Add(EWarriorBattleState::State_IndomitableLash);

	PatternRotting();
}

void UHD_WarriorStateComponent::StartBattle()
{
	// 전투 상태로 전환
	SetBattleState(EWarriorBattleState::State_CombatCheck);
}

void UHD_WarriorStateComponent::AttackTick(float DeltaTime)
{
	Super::AttackTick(DeltaTime);
	CurrentAttackTime += DeltaTime;
	switch (CurrentBattleState)
	{
		case EWarriorBattleState::State_CombatCheck:
			CombatCheck();
			break;
		case EWarriorBattleState::State_MightySweep:
			MightySweep();
			break;
		case EWarriorBattleState::State_ChargedSlash:
			ChargedSlash();
			break;
		case EWarriorBattleState::State_HeavenwardSunder:
			HeavenwardSunder();
			break;
		case EWarriorBattleState::State_IndomitableLash:
			IndomitableLash();
			break;
	}

	FString myState = UEnum::GetValueOrBitfieldAsString(CurrentBattleState);
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() + FVector(0, 0, 100), myState, 0, FColor::Yellow, 0);
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
			// FindAttackPoint();
			if (Distance > 700)
			{
				// 타겟 대상으로 좌우로 조금씩 이동한다.
				AIController->MoveToActor(TargetPawn, 300.0f);
			}
			else
			{
				if (CombatTime < CurrentAttackTime)
				{
					// 드래곤을 공격한다.
					SetBattleState(NextPattern());
					//SetBattleState(EWarriorBattleState::State_MightySweep);
				}
				else
				{
					// 타겟을 원점으로 원형으로 이동시킨다.
				}
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
		
		// 플레이어, 동료를 무시한다.
		// HD_CompanionCharacter 타입을 무시한다.
		for (TActorIterator<AHD_CompanionCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			CollisionParams.AddIgnoredActor(*ActorItr);
		}

		// 플레이어를 가져온다.
		AHD_CompanionCharacter* Player = Cast<AHD_CompanionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		CollisionParams.AddIgnoredActor(Player);

		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel5, CollisionParams);

		if (HitResult.bBlockingHit)
		{
			//DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, 0.1f, 0, 1.0f);
			// 히트된게 캡슐이면 무시한다.
			if (HitResult.Component->IsA<UCapsuleComponent>())
			{
				continue;
			}

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

			IsAttackPoint = true;
		}
		else
		{
			// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, 0, 1.0f);
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
				if (FindAttackPoint())
				{
					float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
					if (Distance > MightySweepRange)
					{
						// 드래곤을 향해 이동한다.
						AIController->MoveToLocation(AttackPoint, MightySweepRange - 20.0f, false);
						// 대상으로 회전한다.
						FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
						AIController->SetControlRotation(LookAt);
						//UE_LOG(LogTemp, Warning, TEXT("MightySweep First Move"));
					}
					else
					{
						// 드래곤을 공격한다.
						// 몽타주 실행
						//UE_LOG(LogTemp, Warning, TEXT("MightySweep First Attack"));
						WarriorAnimInstance->PlayAttackMontage(CurrentCombo);
						CurrentCombo++;
						CurrentAttackTime = 0.0f;

					}
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("MightySweep First Not Find"));
					// 다음 공격
					SetBattleState(EWarriorBattleState::State_CombatCheck);
				}
			}
		}
	}
	// 다음 콤보를 넣는다.
	else if (CurrentCombo < 4)
	{
		bool ComboAttackFailed = false;
		if (MinComboTime < CurrentAttackTime && CurrentAttackTime < MaxComboTime)
		{
			if (FindAttackPoint())
			{
				float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
				if (MightySweepRange < Distance && Distance < MightySweepRange + 200.0f)
				{
					//UE_LOG(LogTemp, Warning, TEXT("MightySweep Move"));
					// 드래곤을 향해 이동한다.
					AIController->MoveToLocation(AttackPoint, MightySweepRange - 20.0f, false);
					// 대상으로 회전한다.
					FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
					AIController->SetControlRotation(LookAt);
				}
				else if (Distance < MightySweepRange)
				{
					// 대상으로 회전한다.
					FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
					AIController->SetControlRotation(LookAt);

					// 다음 공격
					//UE_LOG(LogTemp, Warning, TEXT("MightySweep %d"), CurrentCombo);
					Cast<UHD_WarriorAnimInstance>(AnimInstance)->PlayAttackMontage(CurrentCombo);
					CurrentAttackTime = 0.0f;
					CurrentCombo++;
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("MightySweep Range Failed %f"), Distance);
					SetBattleState(EWarriorBattleState::State_CombatCheck);
				}
			}
			else
			{
				SetBattleState(EWarriorBattleState::State_CombatCheck);
				//UE_LOG(LogTemp, Warning, TEXT("MightySweep Find AttackPoint Failed"));

			}
		}
		else if (MaxComboTime <= CurrentAttackTime)
		{
			// 다음 공격
			//UE_LOG(LogTemp, Warning, TEXT("MightySweep CurrentAttackTime Failed %f"), CurrentAttackTime);
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}
	else
	{
		// 모든 콤보가 종료됨
		if (MinComboTime < CurrentAttackTime)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MightySweep End Combo"), CurrentAttackTime);
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}
}

void UHD_WarriorStateComponent::ChargedSlash()
{
	MaxChargeTime = 4.0f;
	if (CurrentCombo == 0)
	{
		// 드래곤과의 거리
		if (FindAttackPoint())
		{
			float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
			if (Distance > ChargedSlashRange)
			{
				// 드래곤을 향해 이동한다.
				AIController->MoveToLocation(AttackPoint, ChargedSlashRange - 20.0f, false);
				// 대상으로 회전한다.
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
				AIController->SetControlRotation(LookAt);
			}
			else
			{
				// 몽타주 실행
				WarriorAnimInstance->PlayChargedSlashMontage(CurrentCombo);
				CurrentCombo++;
				CurrentAttackTime = 0.0f;
			}
		}
		else
		{
			// 다음 공격
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}
	else if (CurrentCombo == 1)
	{
		if (MaxChargeTime < CurrentAttackTime)
		{
			// 몽타주 실행
			WarriorAnimInstance->PlayChargedSlashMontage(CurrentCombo);
			CurrentCombo++;
			CurrentAttackTime = 0.0f;
		}
	}
	else
	{
		// 모든 콤보가 종료됨
		if (PostDelayTime < CurrentAttackTime)
		{
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}
}

void UHD_WarriorStateComponent::HeavenwardSunder()
{
	MaxChargeTime = 3.0f;
	if (CurrentCombo == 0)
	{
		// 드래곤과의 거리
		if (FindAttackPoint())
		{
			float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
			if (Distance > HeavenwardSunderRange)
			{
				// 드래곤을 향해 이동한다.
				AIController->MoveToLocation(AttackPoint, HeavenwardSunderRange - 20.0f, false);
				// 대상으로 회전한다.
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
				AIController->SetControlRotation(LookAt);
			}
			else
			{
				// 몽타주 실행
				WarriorAnimInstance->PlayHeavenwardSunderMontage(CurrentCombo);
				CurrentCombo++;
				CurrentAttackTime = 0.0f;
			}
		}
		else
		{
			// 다음 공격
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}
	else if (CurrentCombo == 1)
	{
		if (MaxChargeTime < CurrentAttackTime)
		{
			// 몽타주 실행
			// 캐릭터를 점프시킨다.
			// 충돌을 무시한다.
			
			Me->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			Me->LaunchCharacter(FVector(0, 0, 1000), false, false);
			
			WarriorAnimInstance->PlayHeavenwardSunderMontage(CurrentCombo);
			CurrentCombo++;
			CurrentAttackTime = 0.0f;
		}
	}
	else
	{
		// 모든 콤보가 종료됨
		if (PostDelayTime < CurrentAttackTime)
		{
			//충돌체
			Me->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
			
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}
}

void UHD_WarriorStateComponent::IndomitableLash()
{
	MaxChargeTime = 5.0f;
	if (CurrentCombo == 0)
	{
		// 드래곤과의 거리
		if (FindAttackPoint())
		{
			float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
			if (Distance > IndomitableLashRange)
			{
				// 드래곤을 향해 이동한다.
				AIController->MoveToLocation(AttackPoint, IndomitableLashRange - 20.0f, false);
				// 대상으로 회전한다.
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
				AIController->SetControlRotation(LookAt);
			}
			else
			{
				// 몽타주 실행
				WarriorAnimInstance->PlayIndomitableLashMontage(CurrentCombo);
				CurrentCombo++;
				CurrentAttackTime = 0.0f;
			}
		}
		else
		{
			// 다음 공격
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}
	else if (CurrentCombo == 1)
	{
		if (MaxChargeTime < CurrentAttackTime)
		{
			// 몽타주 실행
			WarriorAnimInstance->PlayIndomitableLashMontage(CurrentCombo);
			CurrentCombo++;
			CurrentAttackTime = 0.0f;
		}
	}
	else
	{
		// 모든 콤보가 종료됨
		if (PostDelayTime < CurrentAttackTime)
		{
			SetBattleState(EWarriorBattleState::State_CombatCheck);
		}
	}

}

void UHD_WarriorStateComponent::SetBattleState(EWarriorBattleState State)
{
	CurrentBattleState = State;
	CurrentAttackTime = 0.0f;
	CurrentCombo = 0;
}

void UHD_WarriorStateComponent::PatternRotting()
{
	// 랜덤 셔플 마지막 3번은 변경하지 않는다.
	for (int i = 0; i < PatternList.Num(); i++)
	{
		int32 RandomInt = FMath::RandRange(0, PatternList.Num() - 1);
		EWarriorBattleState Temp = PatternList[i];
		PatternList[i] = PatternList[RandomInt];
		PatternList[RandomInt] = Temp;
	}
}

EWarriorBattleState UHD_WarriorStateComponent::NextPattern()
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
