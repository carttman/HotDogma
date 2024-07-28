// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionManager.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "EngineUtils.h"

void ACHJ_GameMode::BeginPlay()
{
	if (bCompanionsSpawn)
	{
		CompanionManager = GetWorld()->SpawnActor<AHD_CompanionManager>(CompanionManagerFactory);
	}

	// 드래곤을 찾아둔다.
	for (TActorIterator<AHD_Dragon> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Dragons.Add(*ActorItr);
	}
}

void ACHJ_GameMode::CommandCompanion(int num)
{
	UE_LOG(LogTemp, Warning, TEXT("CommandCompanion"));

	// 동료 캐릭터에게 명령을 내린다.
	if (CompanionManager) CompanionManager->SetCommand(ECompanionCommand(num));
}

APawn* ACHJ_GameMode::GetEnemy(FVector Pos)
{
		// 가장 가까운 드래곤을 찾는다.
	APawn* ClosestDragon = nullptr;
	float MinDistance = 9999999999.0f;

	for (auto Dragon : Dragons)
	{
		float Distance = FVector::Dist(Dragon->GetActorLocation(), Pos);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestDragon = Dragon;
		}
	}

	return ClosestDragon;
}
