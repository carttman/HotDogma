// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_CompanionManager.h"
#include "HD_CompanionCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "HotDogma/Ksw/CompanionComponents/HD_CompanionStateComponent.h"
#include "../CompanionComponents/HD_SorcererStateComponent.h"
#include "../CompanionComponents/HD_WarriorStateComponent.h"

// Sets default values
AHD_CompanionManager::AHD_CompanionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHD_CompanionManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어 위치를 가져온다.
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	// 플레이어 주변에 동료 캐릭터를 생성한다.

	// 동료 캐릭터를 생성할 위치를 설정한다.
	// 내비게이션 메쉬 위치에 생성

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr != NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSystem is nullptr"));
	}

	// 적이 생성된 기본 위치를 가져옴
	// 생성할 위치를 설정
	{
		Formations.Add(FVector(0, 300, 0));
		auto* Sorcerer = GetWorld()->SpawnActor<AHD_CompanionCharacter>(SorcererCompanionFactory, PlayerPawn->GetActorLocation() + FVector(0, 100, 0), FRotator::ZeroRotator);
		if (Sorcerer)
		{
			Companions.Add(Sorcerer->SetupCompanionStateComp(false));
			Sorcerer->CompanionId = 0;
		}
	}
	
	{
		// 생성할 위치를 설정
		Formations.Add(FVector(300, -100, 0));
		auto* Warrior1 = GetWorld()->SpawnActor<AHD_CompanionCharacter>(WarriorCompanionFactory, PlayerPawn->GetActorLocation() + FVector(100, -100, 0), FRotator::ZeroRotator);
		if (Warrior1)
		{
			Companions.Add(Warrior1->SetupCompanionStateComp(true));
			Warrior1->CompanionId = 1;
		}
	}

	{
		// 생성할 위치를 설정
		Formations.Add(FVector(500, 0, 0));
		auto* Warrior2 = GetWorld()->SpawnActor<AHD_CompanionCharacter>(WarriorCompanion2Factory, PlayerPawn->GetActorLocation() + FVector(200, 0, 0), FRotator::ZeroRotator);
		if (Warrior2)
		{
			Companions.Add(Warrior2->SetupCompanionStateComp(true));
			Warrior2->CompanionId = 2;
		}
	}

	//for (int i = 0; i < 10; i++)
	//{
	//	for (int j = 0; j < 10; j++)
	//	{
	//		Formations.Add(FVector(200 + 150 * i, 200 + 150 * j, 0));
	//		// Formations을 플레이어 방향으로 회전한다.
	//		FVector FormationByPlayer = Formations[i * 10 + j];
	//		FormationByPlayer = FormationByPlayer.RotateAngleAxis(PlayerPawn->GetController()->GetControlRotation().Yaw, FVector(0, 0, 1));

	//		auto* Warrior = GetWorld()->SpawnActor<AHD_CompanionCharacter>(WarriorCompanionFactory, PlayerPawn->GetActorLocation() + FormationByPlayer, FRotator::ZeroRotator);
	//		if (nullptr != Warrior)
	//			Companions.Add(Warrior->GetCompanionStateComp());
	//	}
	//}

	SetCommand(ECompanionCommand::Command_Following);
}

// Called every frame
void AHD_CompanionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < Companions.Num(); i++)
	{
		if (nullptr != Companions[i])
		{
			//Companions[i]->Flocking(Companions, PlayerPawn->GetActorLocation(), SeparationValue, AlignmentValue, CohesionValue);
		}
	}

	// state 처리
	switch (CurrentCommand)
	{
		case ECompanionCommand::Command_Following:
			// 동료 캐릭터가 플레이어를 따라다닌다.
			TickFollow(DeltaTime);
			break;
		case ECompanionCommand::Command_Go:
			// 동료 캐릭터가 특정 위치로 이동한다.
			TickGo(DeltaTime);
			break;
		case ECompanionCommand::Command_Wait:
			// 동료 캐릭터가 특정 위치에서 대기한다.
			TickWait(DeltaTime);
			break;
		case ECompanionCommand::Command_Help:
			// 동료 캐릭터가 플레이어를 도와준다.
			TickHelp(DeltaTime);
			break;
	}

	ChangeCommand = false;
	//FString myState = UEnum::GetValueOrBitfieldAsString(CurrentCommand);
	//DrawDebugString(GetWorld(), PlayerPawn->GetActorLocation(), myState, 0, FColor::Yellow, 0);
}

void AHD_CompanionManager::TickFollow(float DeltaTime)
{
	// 플레이어 위치를 가져온다.
	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	for (int i = 0; i < Companions.Num(); i++)
	{
		if (nullptr != Companions[i])
		{
			FVector FormationByPlayer = Formations[i];
			// 캐릭터 모델의 앞으로 이동
			FormationByPlayer = FormationByPlayer.RotateAngleAxis(PlayerPawn->GetActorRotation().Yaw, FVector(0, 0, 1));
			
			MoveBoid(Companions[i], PlayerLocation + FormationByPlayer);
		}
	}
}

void AHD_CompanionManager::TickGo(float DeltaTime)
{
	// 플레이어 위치를 가져온다.
	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	for (int i = 0; i < Companions.Num(); i++)
	{
		if (nullptr != Companions[i])
		{
			FVector FormationByPlayer = Formations[i] + FVector(1500, 0, 0);
			// 캐릭터 모델의 앞으로 이동
			FormationByPlayer = FormationByPlayer.RotateAngleAxis(PlayerPawn->GetActorRotation().Yaw, FVector(0, 0, 1));
			MoveBoid(Companions[i], PlayerLocation + FormationByPlayer);
		}
	}
}

void AHD_CompanionManager::TickWait(float DeltaTime)
{
	for (int i = 0; i < Companions.Num(); i++)
	{
		if (nullptr != Companions[i])
		{
			Companions[i]->StopMove();
		}
	}
}

void AHD_CompanionManager::TickHelp(float DeltaTime)
{
	for (int i = 0; i < Companions.Num(); i++)
	{
		if (nullptr != Companions[i])
		{
			Companions[i]->DoHelp();
		}
	}
}

void AHD_CompanionManager::MoveBoid(UHD_CompanionStateComponent* Companion, FVector Pos)
{
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	if (!ChangeCommand && Companion->CurrentState == ECompanionState::State_Wait)
	{
		FVector GapPos = PlayerLocation - Companion->CharacterPoint;
		if (GapPos.Length() < 500)
		{
			return;
		}
	}

	Companion->SetMovePoint(PlayerLocation, Pos);
}

bool CompareDistance(const TPair<FVector, float>& A, const TPair<FVector, float>& B)
{
	return A.Value < B.Value;
}

FVector AHD_CompanionManager::StrafingLocation(APawn* AiPlayer, APawn* Target, int LocCount, float Range)
{
	TArray<FVector> Directions;
	// Target을 기준으로 Range 범위에 있는 16개 위치들을 구한다.
	for (int32 i = 0; i < LocCount; i++)
	{
		float AngleDegrees = i * 360.0f/ LocCount;
		float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

		FVector Direction = FVector(FMath::Cos(AngleRadians), FMath::Sin(AngleRadians), 0.0f);
		FVector Position = Target->GetActorLocation() + Direction * Range;

		Directions.Add(Position);
	}

	// Direction 위치에 Debug Draw Circle
	/*for (int32 i = 0; i < Directions.Num(); i++)
	{
		DrawDebugSphere(GetWorld(), Directions[i], 50.0f, 16, FColor::Green, false, 0.0f);
	}*/

	// AiPlayer 위치에서 가장 가깝고 다른 플레이어와 상대적으로 먼 위치를 구한다.
	int32 AIPlayerIdx = 0;
	TArray<FVector> CompanionsPos;
	for (int32 i = 0; i < Companions.Num(); i++)
	{
		if (Companions[i]->Me != AiPlayer)
		{
			CompanionsPos.Add(Companions[i]->Me->GetActorLocation());
		}
	}

	// 3개정도 BestLocation을 구한다.
	TArray<FVector> BestLocations;
	TArray<TPair<FVector, float>> ValidLocations;

	for (int32 i = 0; i < Directions.Num(); i++)
	{
		bool bIgnor = false;
		for (int32 j = 0; j < CompanionsPos.Num(); j++)
		{
			float Distance = FVector::Dist(Directions[i], CompanionsPos[j]);
			if (Distance < (Range / 2))
			{
				bIgnor = true;
				break;
			}
		}

		if (!bIgnor)
		{
			float Distance = FVector::Dist(Directions[i], AiPlayer->GetActorLocation());
			ValidLocations.Add(TPair<FVector, float>(Directions[i], Distance));
		}
	}

	ValidLocations.Sort(CompareDistance);
	for (int32 i = 0; i < FMath::Min(4, ValidLocations.Num()); ++i)
	{
		BestLocations.Add(ValidLocations[i].Key);
	}

	if (BestLocations.Num() == 0)
	{
		return AiPlayer->GetActorLocation();
	}

	if (BestLocations.Num() == 1)
	{
		return BestLocations[0];
	}

	int32 RandomIndex = FMath::RandRange(1, BestLocations.Num() - 1);
	return BestLocations[RandomIndex];
}

void AHD_CompanionManager::SetCommand(ECompanionCommand Command)
{
	CurrentCommand = Command;
	ChangeCommand = true;

	for (int i = 0; i < Companions.Num(); i++)
	{
		if (nullptr != Companions[i])
		{
			Companions[i]->SetCommand(CurrentCommand);
		}
	}
}

