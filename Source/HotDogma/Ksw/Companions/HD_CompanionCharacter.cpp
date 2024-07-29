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
	
	// AI 컨트롤러를 생성하고 설정한다.
	// 랜덤 백터 생성

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
				// 드래곤과의 거리를 계산한다.
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
		// 전투 상태에서는 드래곤을 공격한다.
		if (AIController)
		{
			if (TargetPawn != nullptr)
			{
				// 드래곤과의 거리
				float Distance = FVector::Dist(GetActorLocation(), TargetPawn->GetActorLocation());
				if (Distance > 200)
				{
					// 드래곤을 향해 이동한다.
					AIController->MoveToActor(TargetPawn, 100.0f);
				}
				else
				{
					// 드래곤을 공격한다.
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
		// steer로 이동하는 방향을 구한다.
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
 * @brief 동료 캐릭터가 특정 위치로 이동한다.
 * @param chacterPos 캐릭터 위치
 * @param Point 이동할 위치
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
	// 지원 스킬이 있는지 확인하고 사용한다.
	// SetState(ECompanionState::State_Help);
}

void AHD_CompanionCharacter::SetState(ECompanionState State)
{
	CurrentState = State;
}
