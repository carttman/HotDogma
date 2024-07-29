// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_CompanionCharacter.h"
#include "DrawDebugHelpers.h"
#include "HD_CompanionAIController.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"

// Sets default values
AHD_CompanionCharacter::AHD_CompanionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AHD_CompanionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// AI ��Ʈ�ѷ��� �����ϰ� �����Ѵ�.
	// ���� ���� ����

	AIController = Cast<AAIController>(GetController());
	Acc = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));
	Acc.Normalize();
}

// Called every frame
void AHD_CompanionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState == ECompanionState::State_Run)
	{
		float Distance = FVector::Dist(GetActorLocation(), MovePoint);
		if (Distance < 100)
		{
			SetState(ECompanionState::State_Wait);
		}

		ACHJ_GameMode* gameMode = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode != nullptr)
		{
			TargetPawn = gameMode->GetEnemy(GetActorLocation());
			if (TargetPawn != nullptr)
			{
				// �巡����� �Ÿ��� ����Ѵ�.
				float EnemyDistance = FVector::Dist(GetActorLocation(), TargetPawn->GetActorLocation());
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
				float Distance = FVector::Dist(GetActorLocation(), TargetPawn->GetActorLocation());
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
	DrawDebugString(GetWorld(), GetActorLocation(), myState, 0, FColor::Yellow, 0);
}

// Called to bind functionality to input
void AHD_CompanionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AHD_CompanionCharacter::Separation(const TArray<AHD_CompanionCharacter*>& Boids)
{
	FVector Steer = FVector::ZeroVector;
	int Count = 0;

	for (AHD_CompanionCharacter* Other : Boids)
	{
		if (Other == nullptr)
		{
			continue;
		}
		float Distance = FVector::Dist(GetActorLocation(), Other->GetActorLocation());
		if (0 < Distance && Distance < 1000)
		{
			FVector Diff = GetActorLocation() - Other->GetActorLocation();
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

FVector AHD_CompanionCharacter::Alignment(const TArray<AHD_CompanionCharacter*>& Boids)
{
	FVector Steer = FVector::ZeroVector;
	int Count = 0;

	for (AHD_CompanionCharacter* Other : Boids)
	{
		if (Other == nullptr)
		{
			continue;
		}
		float Distance = FVector::Dist(GetActorLocation(), Other->GetActorLocation());
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

FVector AHD_CompanionCharacter::Cohesion(const TArray<AHD_CompanionCharacter*>& Boids)
{
	FVector Steer = FVector::ZeroVector;
	int Count = 0;

	for (AHD_CompanionCharacter* Other : Boids)
	{
		if (Other == nullptr)
		{
			continue;
		}
		float Distance = FVector::Dist(GetActorLocation(), Other->GetActorLocation());
		if (Distance > 0 && Distance < 100)
		{
			Steer += Other->GetActorLocation();
			Count++;
		}
	}

	if (Count > 0)
	{
		Steer /= Count;
		// steer�� �̵��ϴ� ������ ���Ѵ�.
		Steer -= GetActorLocation();
	}

	return Steer;
}

FVector AHD_CompanionCharacter::FollowPlayer(const FVector& PlayerLocation)
{
	FVector Steer = PlayerLocation - GetActorLocation();

	return Steer;
}

void AHD_CompanionCharacter::Flocking(const TArray<AHD_CompanionCharacter*>& Boids, const FVector& PlayerLocation
	, float SeparationValue, float AlignmentValue, float CohesionValue)
{
	FVector Sep = Separation(Boids) * SeparationValue;
	FVector Ali = Alignment(Boids) * AlignmentValue;
	FVector Coh = Cohesion(Boids) * CohesionValue;
	FVector Foll = FollowPlayer(PlayerLocation) * 0.2f;

	Acc = Sep + Ali + Coh + Foll;
	//Acc = Acc.GetClampedToMaxSize(1.0f);
	//Acc.Normalize();
}

/** 
 * @brief ���� ĳ���Ͱ� Ư�� ��ġ�� �̵��Ѵ�.
 * @param chacterPos ĳ���� ��ġ
 * @param Point �̵��� ��ġ
*/
void AHD_CompanionCharacter::SetMovePoint(const FVector& chacterPos, const FVector& Point)
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

void AHD_CompanionCharacter::StopMove()
{
	if (CurrentState == ECompanionState::State_Run)
	{
		if (AIController)
			AIController->StopMovement();
	}

	SetState(ECompanionState::State_Wait);
}

void AHD_CompanionCharacter::DoHelp()
{
	// ���� ��ų�� �ִ��� Ȯ���ϰ� ����Ѵ�.
	// SetState(ECompanionState::State_Help);
}

void AHD_CompanionCharacter::SetState(ECompanionState State)
{
	CurrentState = State;
}
