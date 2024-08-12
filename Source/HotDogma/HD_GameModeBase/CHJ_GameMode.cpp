// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionManager.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "EngineUtils.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerComponent/HD_PlayerCamera.h"
#include "HotDogma/UI/HD_GameOverWidget.h"
#include "HotDogma/UI/HD_GamePlayWidget.h"
#include "HotDogma/UI/HD_PlayerWidget.h"
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

	//CreatePlayerWidget();
	Player  = Cast<AHD_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CreateGamePlayWidget();
}

void ACHJ_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
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


void ACHJ_GameMode::CreateGamePlayWidget()
{
	if(GamePlayWidgetFactory)
	{
		GamePlayWidget = Cast<UHD_GamePlayWidget>(CreateWidget(GetWorld(), GamePlayWidgetFactory));
		GamePlayWidget->AddToViewport();
		GamePlayWidget->PlayBGM_Crow();
	}
}

void ACHJ_GameMode::SetHPUI(float Curr, float Max)
{
	GamePlayWidget->WBP_PlayerWidget->SetHP(Curr, Max);
}

void ACHJ_GameMode::SetDragonHPUI(float Curr, float Max, int RemainLineCnt)
{
	GamePlayWidget->WBP_PlayerWidget->Set_DragonHP(Curr, Max, RemainLineCnt);
}
