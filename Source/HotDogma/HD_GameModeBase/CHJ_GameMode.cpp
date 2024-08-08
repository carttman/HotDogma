// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionManager.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "EngineUtils.h"
#include "HotDogma/HD_Character/HD_PlayerComponent/HD_PlayerCamera.h"
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

	CreatePlayerWidget();
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

void ACHJ_GameMode::CreatePlayerWidget()
{
	// 위젯 클래스 담고 생성
	if(PlayerWidgetFactory)
	{
		PlayerWidget = Cast<UHD_PlayerWidget>(CreateWidget(GetWorld(), PlayerWidgetFactory));
		PlayerWidget->AddToViewport();	
	}
	
}

void ACHJ_GameMode::SetHPUI(float Curr, float Max)
{
	PlayerWidget->SetHP(Curr, Max);
}

void ACHJ_GameMode::SetDragonHPUI(float Curr, float Max)
{
	PlayerWidget->Set_DragonHP(Curr, Max);
}
