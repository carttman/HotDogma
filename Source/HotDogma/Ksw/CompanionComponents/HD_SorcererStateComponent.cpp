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

	//���� ��Ʈ ����
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
	
	// ���� ������ �����Ѵ�.
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
	// ���� ���·� ��ȯ
	SetBattleState(ESorcererBattleState::State_CombatCheck);
	// �����.
	AIController->StopMovement();
}

void UHD_SorcererStateComponent::EndBattle()
{
	// �������̼� ����
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
	// ���� ���¿����� �巡���� �����Ѵ�.
	if (AIController)
	{
		if (TargetPawn != nullptr)
		{
			FindAttackPoint();
			float Distance = FVector::Dist(Me->GetActorLocation(), AttackPoint);
			if (Distance > 1500)
			{
				// Ÿ�� ������� �¿�� ���ݾ� �̵��Ѵ�.
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
					// �巡���� �����Ѵ�.
					SetBattleState(NextPattern());
					bStrafing = false;
				}
			}
		}
	}
}

void UHD_SorcererStateComponent::MagickBolt()
{
	// ����
	if (! bCastingMagickBolt)
	{
		// ĳ���� �ִϸ��̼��� �����Ѵ�.
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
			// Ÿ���� ã�´�.
			if (FindAttackPoint())
			{
				// ��Ʈ�� �����Ѵ�.
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), AttackPoint);
				AIController->SetControlRotation(LookAt);

				// ����
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
			// ĳ������ �����Ѵ�.
			bCastingMagickBolt = false;
			MagickBoltCount = 0;
			SetBattleState(ESorcererBattleState::State_CombatCheck);
		}
	}
}

void UHD_SorcererStateComponent::HighHagol()
{
	// ����
	if (!bCastingHighHagol)
	{
		// ĳ���� �ִϸ��̼��� �����Ѵ�.
		SorcererAnimInstance->PlayHighHagolMontage(0);
		bCastingHighHagol = true;
	}

	if (HighHagolCastTime < CurrentAttackTime)
	{
		SorcererAnimInstance->PlayHighHagolMontage(1);
		// Ÿ���� ã�´�.
		if (FindAttackPoint())
		{
			// ��Ʈ�� �����Ѵ�.
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
	// ����
	if (!bCastingHighLevin)
	{
		// ĳ���� �ִϸ��̼��� �����Ѵ�.
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
			// Ÿ���� ã�´�.
			if (FindAttackPoint())
			{
				// ������ �����Ѵ�.
				AttackPoint.Z = 0;
				GetWorld()->SpawnActor<AActor>(HighLevinFactory, AttackPoint, FRotator::ZeroRotator);
			}
			
			HighLevinCount++;
			CurrentAttackTime = 0.0f;
		}
		else
		{
			// ĳ������ �����Ѵ�.
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
		// ���� ����
		if (Me->GetActorLocation().Z > 0)
		{
			SetBattleState(ESorcererBattleState::State_CombatCheck);
		}
		
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Levitate"));

	// ���� �ִϸ��̼��� �����Ѵ�.
	SorcererAnimInstance->PlayLevitate();
	bMaxLevitate = false;
	// �߷��� �����ϰ� ĳ���͸� �ø���.
	// �����Ʈ ��� ����
	Me->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	// õõ�� �ø���.
	Me->GetCharacterMovement()->MaxFlySpeed = 150;

	auto* dust = GetWorld()->SpawnActor<AActor>(DustFactory, Me->GetActorTransform());
	dust->AttachToComponent(Me->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//dust ��ġ�� ���� �ø���.
	dust->SetActorRelativeLocation(FVector(0, 0, 70));

	// 5�� �Ŀ� �߷��� �ٽ� �����Ѵ�.
	GetWorld()->GetTimerManager().SetTimer(LevitateTimerHandle, this, &UHD_SorcererStateComponent::EndLevitate, LevitateTime, false);
}

void UHD_SorcererStateComponent::EndLevitate()
{
	// �߷��� �ٽ� �����Ѵ�.
	// Me->GetCharacterMovement()->bOrientRotationToMovement = true;
	Me->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SorcererAnimInstance->EndLevitate();
	bMaxLevitate = false;
}

void UHD_SorcererStateComponent::ArgentSuccor()
{
	if (!bCastingArgentSuccor)
	{
		// ĳ���� �ִϸ��̼��� �����Ѵ�.
		SorcererAnimInstance->PlayArgentSuccorMontage(0);
		bCastingArgentSuccor = true;
		return;
	}

	if (ArgentSuccorCastTime < CurrentAttackTime)
	{
		SorcererAnimInstance->PlayArgentSuccorMontage(1);
		// Ÿ���� ã�´�.
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
