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

	// ���� ���¿����� �巡���� �����Ѵ�.
	if (AIController)
	{
		if (TargetPawn != nullptr)
		{
			// �巡����� �Ÿ�
			float Distance = FVector::Dist(Me->GetActorLocation(), TargetPawn->GetActorLocation());
			if (Distance > 200)
			{
				// �巡���� ���� �̵��Ѵ�.
				AIController->MoveToActor(TargetPawn, 100.0f);
			}
			else
			{
				// �巡���� �����Ѵ�.
				// UE_LOG(LogTemp, Warning, TEXT("Attack"));
			}
		}
	}
}
