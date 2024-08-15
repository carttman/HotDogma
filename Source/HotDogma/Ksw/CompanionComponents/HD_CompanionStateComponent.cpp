// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_CompanionStateComponent.h"
#include "DrawDebugHelpers.h"
#include "HotDogma/Ksw/Companions/HD_CompanionAIController.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include <HotDogma/LHJ/HD_Dragon.h>

// Sets default values for this component's properties
UHD_CompanionStateComponent::UHD_CompanionStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHD_CompanionStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// AI ��Ʈ�ѷ��� �����ϰ� �����Ѵ�.
	// ���� ���� ����
	Me = Cast<AHD_CompanionCharacter>(GetOwner());
	if (Me != nullptr)
	{
		AIController = Cast<AAIController>(Me->GetController());
	}

	Acc = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));
	Acc.Normalize();

	AnimInstance = Cast<UAnimInstance>(Me->GetMesh()->GetAnimInstance());
}

// Called every frame
void UHD_CompanionStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (CurrentState)
	{
		case ECompanionState::State_Wait:
			WaitTick(DeltaTime);
			break;
		case ECompanionState::State_Run:
			RunTick(DeltaTime);
			break;
		case ECompanionState::State_Battle:
			BattleTick(DeltaTime);
			break;
		case ECompanionState::State_Help:
			HelpTick(DeltaTime);
			break;
		case ECompanionState::State_BattleEnd:
			BattleEndTick(DeltaTime);
			break;
		case ECompanionState::State_Dead:
			DeadTick(DeltaTime);
			break;
		case ECompanionState::State_Stun:
			StunTick(DeltaTime);
			break;
	}

	if (TargetPawn && ! bHighfive)
	{
		AHD_Dragon* dragon = Cast<AHD_Dragon>(TargetPawn);
		if (dragon->CurrHP <= 0.0f)
		{
			SetState(ECompanionState::State_BattleEnd);
		}
	}
	else
	{
		SetState(ECompanionState::State_BattleEnd);
	}

	//FString myState = UEnum::GetValueOrBitfieldAsString(CurrentState);
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), myState, 0, FColor::Yellow, 0);
}

void UHD_CompanionStateComponent::WaitTick(float DeltaTime)
{
	if (CurrentCommand != ECompanionCommand::Command_Wait)
	{
		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), MovePoint);
		if (Distance > 100)
		{
			SetState(ECompanionState::State_Run);
		}

		ACHJ_GameMode* gameMode = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode != nullptr)
		{
			TargetPawn = gameMode->GetEnemy(GetOwner()->GetActorLocation());
			if (TargetPawn != nullptr)
			{
				// �巡����� �Ÿ��� ����Ѵ�.
				float EnemyDistance = FVector::Dist(GetOwner()->GetActorLocation(), TargetPawn->GetActorLocation());
				if (EnemyDistance < 1500)
				{
					SetState(ECompanionState::State_Battle);
				}
			}
		}
	}
	
}

void UHD_CompanionStateComponent::RunTick(float DeltaTime)
{
	if (AIController)
	{
		AIController->MoveToLocation(MovePoint, -1.0f, true, true, true);
	}

	float Distance = FVector::Dist(GetOwner()->GetActorLocation(), MovePoint);
	if (Distance < 100)
	{
		SetState(ECompanionState::State_Wait);
	}

	ACHJ_GameMode* gameMode = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
	{
		TargetPawn = gameMode->GetEnemy(GetOwner()->GetActorLocation());
		if (TargetPawn != nullptr)
		{
			// �巡����� �Ÿ��� ����Ѵ�.
			float EnemyDistance = FVector::Dist(GetOwner()->GetActorLocation(), TargetPawn->GetActorLocation());
			if (EnemyDistance < 1500)
			{
				SetState(ECompanionState::State_Battle);
				StartBattle();
			}
		}
	}
}

void UHD_CompanionStateComponent::BattleTick(float DeltaTime)
{
	if (CurrentCommand == ECompanionCommand::Command_Following)
	{
		// �÷��̾�� �Ÿ��� �־��� ���
		auto* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		float Dist = FVector::Dist(Player->GetActorLocation(), TargetPawn->GetActorLocation());
		if (Dist > 2000)
		{
			Me->GetCharacterMovement()->MaxWalkSpeed = 300;
			Me->GetCharacterMovement()->bOrientRotationToMovement = false;
			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), TargetPawn->GetActorLocation());
			Me->SetActorRotation(LookAt);

			// �巡���� ���Ѻ��� �÷��̾ ���󰣴�.
			if (AIController)
			{
				AIController->MoveToActor(Player, 100.0f);
			}

			// �巡��� ����� �־��� ��� ���� ���¸� Ǭ��.
			{
				float Distance = FVector::Dist(TargetPawn->GetActorLocation(), Me->GetActorLocation());
				if (Distance > 1500)
				{
					Me->GetCharacterMovement()->MaxWalkSpeed = 600;
					Me->GetCharacterMovement()->bOrientRotationToMovement = true;
					EndBattle();
					SetState(ECompanionState::State_Run);
				}
			}

			return;
		}
	}

	// �ڽ��� �����Ѵ�.
	AttackTick(DeltaTime);

}

void UHD_CompanionStateComponent::HelpTick(float DeltaTime)
{
}

void UHD_CompanionStateComponent::BattleEndTick(float DeltaTime)
{
	if (! bHighfive)
	{
		if (AIController)
			AIController->StopMovement();
		bHighfive = true;
		HighfiveReady();
	}
}

void UHD_CompanionStateComponent::DeadTick(float DeltaTime)
{
}

void UHD_CompanionStateComponent::StunTick(float DeltaTime)
{
	StunTime += DeltaTime;
	if (StunDuration < StunTime)
	{
		StunTime = 0.0f;
		SetState(ECompanionState::State_Wait);
	}
}

void UHD_CompanionStateComponent::AttackTick(float DeltaTime)
{

}

void UHD_CompanionStateComponent::StartBattle()
{
}

void UHD_CompanionStateComponent::EndBattle()
{
}

void UHD_CompanionStateComponent::HighfiveReady()
{
}

void UHD_CompanionStateComponent::Highfive()
{
}

FVector UHD_CompanionStateComponent::Separation(const TArray<UHD_CompanionStateComponent*>& Boids)
{
	FVector Steer = FVector::ZeroVector;
	int Count = 0;

	for (UHD_CompanionStateComponent* Other : Boids)
	{
		if (Other == nullptr)
		{
			continue;
		}
		
		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Other->GetOwner()->GetActorLocation());
		if (0 < Distance && Distance < 1000)
		{
			FVector Diff = Me->GetActorLocation() - Other->Me->GetActorLocation();
			Diff.Normalize();
			Diff /= Distance;
			Steer += Diff;
			Count++;
		}
	}

	if (Count > 0)
	{
		Steer /= Count;
	}

	return Steer;
}

FVector UHD_CompanionStateComponent::Alignment(const TArray<UHD_CompanionStateComponent*>& Boids)
{
	FVector Steer = FVector::ZeroVector;
	int Count = 0;

	for (UHD_CompanionStateComponent* Other : Boids)
	{
		if (Other == nullptr)
		{
			continue;
		}

		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Other->GetOwner()->GetActorLocation());
		if (0 < Distance && Distance < 100)
		{
			Steer += Other->Acc;
			Count++;
		}
	}

	if (Count > 0)
	{
		Steer /= Count;
	}

	return Steer;
}

FVector UHD_CompanionStateComponent::Cohesion(const TArray<UHD_CompanionStateComponent*>& Boids)
{
	FVector Steer = FVector::ZeroVector;
	int Count = 0;

	for (UHD_CompanionStateComponent* Other : Boids)
	{
		if (Other == nullptr)
		{
			continue;
		}
		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Other->GetOwner()->GetActorLocation());
		if (Distance > 0 && Distance < 100)
		{
			Steer += Other->GetOwner()->GetActorLocation();
			Count++;
		}
	}

	if (Count > 0)
	{
		Steer /= Count;
		// steer�� �̵��ϴ� ������ ���Ѵ�.
		Steer -= GetOwner()->GetActorLocation();
	}

	return Steer;
}

FVector UHD_CompanionStateComponent::FollowPlayer(const FVector& PlayerLocation)
{
	FVector Steer = PlayerLocation - GetOwner()->GetActorLocation();

	return Steer;
}

void UHD_CompanionStateComponent::Flocking(const TArray<UHD_CompanionStateComponent*>& Boids, const FVector& PlayerLocation, float SeparationValue, float AlignmentValue, float CohesionValue)
{
	FVector Sep = Separation(Boids) * SeparationValue;
	FVector Ali = Alignment(Boids) * AlignmentValue;
	FVector Coh = Cohesion(Boids) * CohesionValue;
	FVector Foll = FollowPlayer(PlayerLocation) * 0.2f;

	Acc = Sep + Ali + Coh + Foll;
	//Acc = Acc.GetClampedToMaxSize(1.0f);
	//Acc.Normalize();
}

void UHD_CompanionStateComponent::SetMovePoint(const FVector& chacterPos, const FVector& Point)
{
	MovePoint = Point;
	CharacterPoint = chacterPos;
}

void UHD_CompanionStateComponent::StopMove()
{
	if (CurrentState == ECompanionState::State_Run)
	{
		if (AIController)
			AIController->StopMovement();
	}

	SetState(ECompanionState::State_Wait);
}

void UHD_CompanionStateComponent::DoHelp()
{

	// ���� ��ų�� �ִ��� Ȯ���ϰ� ����Ѵ�.
	SetState(ECompanionState::State_BattleEnd);
}

void UHD_CompanionStateComponent::DoDead()
{
	if (AIController)
		AIController->StopMovement();
}

void UHD_CompanionStateComponent::DoStun(float Duration)
{
	StunDuration = Duration;
	// ������ ����
	if (AIController)
		AIController->StopMovement();
	SetState(ECompanionState::State_Stun);
}

void UHD_CompanionStateComponent::RotateToTarget(float DeltaTime, FVector Target)
{
	FVector Direction = (Target - Me->GetActorLocation()).GetSafeNormal2D(); // z�ุ ���
	FRotator TargetRotation = Direction.Rotation();

	// �ε巯�� ȸ���� ���� InterpTo �Լ� ���
	FRotator NewRotation = FMath::RInterpTo(Me->GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
	NewRotation.Pitch = Me->GetActorRotation().Pitch; // ��ġ �� ����
	NewRotation.Roll = Me->GetActorRotation().Roll; // �� �� ����

	// ĳ������ ȸ�� ���� ������Ʈ
	Me->SetActorRotation(NewRotation);
}

void UHD_CompanionStateComponent::SetState(ECompanionState State)
{
	CurrentState = State;
}

bool UHD_CompanionStateComponent::IsHighfive()
{
	return bHighfive;
}

