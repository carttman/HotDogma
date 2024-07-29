// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_CompanionStateComponent.h"
#include "DrawDebugHelpers.h"
#include "HotDogma/Ksw/Companions/HD_CompanionAIController.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionCharacter.h"

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
	APawn* pawn = Cast<APawn>(GetOwner());
	if (pawn != nullptr)
	{
		AIController = Cast<AAIController>(pawn->GetController());
	}

	Acc = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));
	Acc.Normalize();
}


// Called every frame
void UHD_CompanionStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState == ECompanionState::State_Run)
	{
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
				if (EnemyDistance < 2000)
				{
					SetState(ECompanionState::State_Battle);
				}
			}
		}
	}
	else if (CurrentState == ECompanionState::State_Battle)
	{
		// ���� ���¿����� �巡���� �����Ѵ�.
		if (AIController)
		{
			if (TargetPawn != nullptr)
			{
				// �巡����� �Ÿ�
				float Distance = FVector::Dist(GetOwner()->GetActorLocation(), TargetPawn->GetActorLocation());
				if (Distance > 200)
				{
					// �巡���� ���� �̵��Ѵ�.
					AIController->MoveToActor(TargetPawn, 100.0f);
				}
				else
				{
					// �巡���� �����Ѵ�.
					UE_LOG(LogTemp, Warning, TEXT("Attack"));
				}
			}
		}
	}

	FString myState = UEnum::GetValueOrBitfieldAsString(CurrentState);
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), myState, 0, FColor::Yellow, 0);
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
			FVector Diff = GetOwner()->GetActorLocation() - Other->GetOwner()->GetActorLocation();
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
	if (CurrentState == ECompanionState::State_Battle)
	{
		return;
	}

	SetState(ECompanionState::State_Run);
	MovePoint = Point;
	CharcterPoint = chacterPos;
	if (AIController != nullptr)
	{
		AIController->MoveToLocation(MovePoint, -1.0f, true, true, true);
	}
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
	// SetState(ECompanionState::State_Help);
}

void UHD_CompanionStateComponent::SetState(ECompanionState State)
{
	CurrentState = State;
}

