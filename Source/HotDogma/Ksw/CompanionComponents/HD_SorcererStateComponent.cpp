// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_SorcererStateComponent.h"
#include "HotDogma/Ksw/Companions/HD_CompanionAIController.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "HotDogma/Ksw/Companions/HD_SorcererAnimInstance.h"

void UHD_SorcererStateComponent::BeginPlay()
{
	Super::BeginPlay();

	SorcererAnimInstance = Cast<UHD_SorcererAnimInstance>(Me->GetMesh()->GetAnimInstance());
}

void UHD_SorcererStateComponent::StartBattle()
{
}

void UHD_SorcererStateComponent::AttackTick(float DeltaTime)
{
	Super::AttackTick(DeltaTime);

	// 전투 상태에서는 드래곤을 공격한다.
	if (AIController)
	{
		if (TargetPawn != nullptr)
		{
			// 드래곤과의 거리
			float Distance = FVector::Dist(Me->GetActorLocation(), TargetPawn->GetActorLocation());
			if (Distance > 200)
			{
				// 드래곤을 향해 이동한다.
				AIController->MoveToActor(TargetPawn, 100.0f);
			}
			else
			{
				// 드래곤을 공격한다.
				// UE_LOG(LogTemp, Warning, TEXT("Attack"));
			}
		}
	}
}
