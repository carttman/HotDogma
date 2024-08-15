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
#include "Curves/CurveFloat.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>

UHD_SorcererStateComponent::UHD_SorcererStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AHD_Projectile> MagickBolt(TEXT("/Script/Engine.Blueprint'/Game/Ksw/Magics/BP_MagickBolt.BP_MagickBolt_C'"));
	if (MagickBolt.Succeeded())
	{
		MagickBoltFactory = MagickBolt.Class;
	}

	ConstructorHelpers::FClassFinder<AActor> HighHagol(TEXT("/Script/Engine.Blueprint'/Game/Ksw/Magics/BP_HighHagol.BP_HighHagol_C'"));
	if (HighHagol.Succeeded())
	{
		HighHagolFactory = HighHagol.Class;
	}

	ConstructorHelpers::FClassFinder<AActor> HighLevin(TEXT("/Script/Engine.Blueprint'/Game/Ksw/Magics/BP_HighLevin.BP_HighLevin_C'"));
	if (HighLevin.Succeeded())
	{
		HighLevinFactory = HighLevin.Class;
	}

	ConstructorHelpers::FClassFinder<AActor> ArgentSuccor(TEXT("/Script/Engine.Blueprint'/Game/Ksw/Magics/BP_ArgentSuccor.BP_ArgentSuccor_C'"));
	if (ArgentSuccor.Succeeded())
	{
		ArgentSuccorFactory = ArgentSuccor.Class;
	}

	//메직 볼트 사운드
static ConstructorHelpers::FObjectFinder<USoundBase> MagickBoltSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Assets/SFX/Magic/IMPACT_Generic_15_mono.IMPACT_Generic_15_mono'"));
	if (MagickBoltSoundAsset.Succeeded())
	{
		MagickBoltSound = MagickBoltSoundAsset.Object;
	}
}

void UHD_SorcererStateComponent::BeginPlay()
{
	Super::BeginPlay();

	SorcererAnimInstance = Cast<UHD_SorcererAnimInstance>(Me->GetMesh()->GetAnimInstance());
	
	// 전투 패턴을 세팅한다.
	PatternList.Add(ESorcererBattleState::State_MagickBolt);
	PatternList.Add(ESorcererBattleState::State_HighHagol);
	PatternList.Add(ESorcererBattleState::State_HighLevin);
	PatternList.Add(ESorcererBattleState::State_Levitate);
	PatternList.Add(ESorcererBattleState::State_ArgentSuccor);
	//PatternList.Add(ESorcererBattleState::State_Galvanize);
	bMaxLevitate = false;
	PatternRotting();
}

void UHD_SorcererStateComponent::StartBattle()
{
	// 전투 상태로 전환
	SetBattleState(ESorcererBattleState::State_CombatCheck);
	// 멈춘다.
	AIController->StopMovement();
}

void UHD_SorcererStateComponent::EndBattle()
{
	// 레비테이션 종료
	GetWorld()->GetTimerManager().ClearTimer(LevitateTimerHandle);
	EndLevitate();
}

void UHD_SorcererStateComponent::AttackTick(float DeltaTime)
{
	Super::AttackTick(DeltaTime);
	CurrentAttackTime += DeltaTime;

	Me->GetCharacterMovement()->MaxWalkSpeed = 150;
	Me->GetCharacterMovement()->bOrientRotationToMovement = false;
	
	switch (CurrentBattleState)
	{
		case ESorcererBattleState::State_CombatCheck:
			CombatCheck();
			break;
		case ESorcererBattleState::State_MagickBolt:
			MagickBolt();
			break;
		case ESorcererBattleState::State_HighHagol:
			HighHagol();
			break;
		case ESorcererBattleState::State_HighLevin:
			HighLevin();
			break;
		case ESorcererBattleState::State_Levitate:
			Levitate();
			break;
		case ESorcererBattleState::State_ArgentSuccor:
			ArgentSuccor();
			break;
		case ESorcererBattleState::State_Galvanize:
			break;
	}

	if (SorcererAnimInstance->bLevitate)
	{
		LevitateTick(DeltaTime);
	}

	RotateToTarget(DeltaTime, TargetPawn->GetActorLocation());

	//FString myState = UEnum::GetValueOrBitfieldAsString(CurrentBattleState);
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() + FVector(0, 0, 100), myState, 0, FColor::Yellow, 0);
}

void UHD_SorcererStateComponent::SetBattleState(ESorcererBattleState state)
{
	CurrentBattleState = state;
	CurrentAttackTime = 0.0f;
	if (state == ESorcererBattleState::State_HighLevin)
	{
		auto* gm = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			gm->PlaySoundAtIndex(14);
		}
	}
}

void UHD_SorcererStateComponent::CombatCheck()
{
	// 전투 상태에서는 드래곤을 공격한다.
	if (AIController)
	{
		if (TargetPawn != nullptr)
		{
			FindAttackPoint();
			float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
			if (Distance > 1500)
			{
				// 타겟 대상으로 좌우로 조금씩 이동한다.
				if (SorcererAnimInstance->bLevitate && !bMaxLevitate)
				{
					AttackPoint.Z = 300;
				}
				AIController->MoveToLocation(AttackPoint, 1300.0f);
			}

			{
				if (!bStrafing)
				{
					bStrafing = true;
					ACHJ_GameMode* gameMode = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
					if (gameMode)
					{
						FVector Loc = gameMode->CompanionManager->StrafingLocation(Me, TargetPawn, 16, 1300);
						if (SorcererAnimInstance->bLevitate && !bMaxLevitate)
						{
							Loc.Z = 300;
						}

						AIController->MoveToLocation(Loc, 100.0f);
					}
				}

				if (CombatTime < CurrentAttackTime)
				{
					// 드래곤을 공격한다.
					SetBattleState(NextPattern());
					bStrafing = false;
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
			// 타겟을 찾는다.
			if (FindAttackPoint())
			{
				// 볼트를 스폰한다.
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
				AIController->SetControlRotation(LookAt);

				// 사운드
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), MagickBoltSound, Me->GetActorLocation());

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
	// 시작
	if (!bCastingHighHagol)
	{
		// 캐스팅 애니메이션을 실행한다.
		SorcererAnimInstance->PlayHighHagolMontage(0);
		bCastingHighHagol = true;
	}

	if (HighHagolCastTime < CurrentAttackTime)
	{
		SorcererAnimInstance->PlayHighHagolMontage(1);
		// 타겟을 찾는다.
		if (FindAttackPoint())
		{
			// 볼트를 스폰한다.
			AttackPoint.Z = 0;
			GetWorld()->SpawnActor<AActor>(HighHagolFactory, AttackPoint, FRotator::ZeroRotator);
		}

		CurrentAttackTime = 0.0f;
		bCastingHighHagol = false;
		SetBattleState(ESorcererBattleState::State_CombatCheck);
	}
}

void UHD_SorcererStateComponent::HighLevin()
{
	// 시작
	if (!bCastingHighLevin)
	{
		// 캐스팅 애니메이션을 실행한다.
		SorcererAnimInstance->PlayHighLevinMontage(0);
		bCastingHighLevin = true;
		return;
	}

	float AttackTime = HighLevinTime;
	if (HighLevinCount == 0)
	{
		AttackTime += HighLevinCastTime;
	}
	
	if (AttackTime < CurrentAttackTime)
	{
		if (HighLevinCount < MaxHighLevinCount)
		{
			SorcererAnimInstance->PlayHighLevinMontage(1);
			// 타겟을 찾는다.
			if (FindAttackPoint())
			{
				// 레빈을 스폰한다.
				AttackPoint.Z = 0;
				GetWorld()->SpawnActor<AActor>(HighLevinFactory, AttackPoint, FRotator::ZeroRotator);
			}
			
			HighLevinCount++;
			CurrentAttackTime = 0.0f;
		}
		else
		{
			// 캐스팅을 종료한다.
			bCastingHighLevin = false;
			CurrentAttackTime = 0.0f;
			HighLevinCount = 0;
			SetBattleState(ESorcererBattleState::State_CombatCheck);
		}
	}
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
		if (Me->GetActorLocation().Z > 0)
		{
			SetBattleState(ESorcererBattleState::State_CombatCheck);
		}
		
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Levitate"));

	// 부유 애니메이션을 실행한다.
	SorcererAnimInstance->PlayLevitate();
	bMaxLevitate = false;
	// 중력을 제거하고 캐릭터를 올린다.
	// 무브먼트 모드 변경
	Me->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	// 천천히 올린다.
	Me->GetCharacterMovement()->MaxFlySpeed = 150;

	auto* dust = GetWorld()->SpawnActor<AActor>(DustFactory, Me->GetActorTransform());
	dust->AttachToComponent(Me->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//dust 위치를 조금 올린다.
	dust->SetActorRelativeLocation(FVector(0, 0, 70));

	// 5초 후에 중력을 다시 적용한다.
	GetWorld()->GetTimerManager().SetTimer(LevitateTimerHandle, this, &UHD_SorcererStateComponent::EndLevitate, LevitateTime, false);
}

void UHD_SorcererStateComponent::EndLevitate()
{
	// 중력을 다시 적용한다.
	// Me->GetCharacterMovement()->bOrientRotationToMovement = true;
	Me->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SorcererAnimInstance->EndLevitate();
	bMaxLevitate = false;
}

void UHD_SorcererStateComponent::ArgentSuccor()
{
	if (!bCastingArgentSuccor)
	{
		// 캐스팅 애니메이션을 실행한다.
		SorcererAnimInstance->PlayArgentSuccorMontage(0);
		bCastingArgentSuccor = true;
		return;
	}

	if (ArgentSuccorCastTime < CurrentAttackTime)
	{
		SorcererAnimInstance->PlayArgentSuccorMontage(1);
		// 타겟을 찾는다.
		auto* Player = Cast<AHD_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player)
		{
			AActor* actor = GetWorld()->SpawnActor<AActor>(ArgentSuccorFactory, Player->GetActorLocation(), FRotator::ZeroRotator);
			actor->AttachToActor(Player, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			UGameplayStatics::ApplyDamage(Player, -1000.0f, Me->GetController(), Me, UDamageType::StaticClass());
		}

		CurrentAttackTime = 0.0f;
		bCastingArgentSuccor = false;
		SetBattleState(ESorcererBattleState::State_CombatCheck);
	}
}

void UHD_SorcererStateComponent::Galvanize()
{
}

void UHD_SorcererStateComponent::LevitateTick(float DeltaTime)
{
	if (!bMaxLevitate)
	{
		//Me->GetCharacterMovement()->Velocity = FVector(0, 0, 1) * 200;
		Me->AddMovementInput(FVector(0, 0, 1), 1);
		if (Me->GetActorLocation().Z > 300)
		{
			bMaxLevitate = true;
		}
	}
	else
	{
		static float Time = 0.0f;
		Time += DeltaTime;

		if (Time > LevitateAnimTime)
		{
			Time = 0.0f;
			LevitateUp *= -1;
		}
		
		Me->GetCharacterMovement()->Velocity = FVector(0, 0, LevitateUp) * 20;
	}
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
