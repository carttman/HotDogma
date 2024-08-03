// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionManager.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "EngineUtils.h"
#include "HotDogma/HD_Character/HD_PlayerComponent/HD_PlayerCamera.h"
#include "Kismet/GameplayStatics.h"

void ACHJ_GameMode::BeginPlay()
{
	if (bCompanionsSpawn)
	{
		CompanionManager = GetWorld()->SpawnActor<AHD_CompanionManager>(CompanionManagerFactory);
	}

	// �巡���� ã�Ƶд�.
	for (TActorIterator<AHD_Dragon> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Dragons.Add(*ActorItr);
	}

	// // 플레이어 컨트롤러 가져오기(0번)
	// APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// if (PlayerController)
	// {
	// 	// 카메라 액터 찾기
	// 	TArray<AActor*> FoundActors;
	// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHD_PlayerCamera::StaticClass(), FoundActors);
	//
	// 	if (FoundActors.Num() > 0)
	// 	{
	// 		AHD_PlayerCamera* PlayerCamera = Cast<AHD_PlayerCamera>(FoundActors[0]);
	// 		if (PlayerCamera)
	// 		{
	// 			// 컨트롤러에 카메라 자동 활성화
	// 			PlayerCamera->AutoActivateForPlayer(PlayerController);
	// 		}
	// 	}
	// }
}

void ACHJ_GameMode::CommandCompanion(int num)
{
	UE_LOG(LogTemp, Warning, TEXT("CommandCompanion"));

	// ���� ĳ���Ϳ��� ����� ������.
	if (CompanionManager) CompanionManager->SetCommand(ECompanionCommand(num));
}

APawn* ACHJ_GameMode::GetEnemy(FVector Pos)
{
		// ���� ����� �巡���� ã�´�.
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
