// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/Ksw/Companions/HD_CompanionManager.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "EngineUtils.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/UI/HD_GamePlayWidget.h"
#include "HotDogma/UI/HD_PlayerWidget.h"
#include "Kismet/GameplayStatics.h"

ACHJ_GameMode::ACHJ_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ACHJ_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 나레이션 로드
	ArrNarration = {
		LoadObject<USoundBase>(nullptr,TEXT("/Script/Engine.SoundWave'/Game/LHJ/Sounds/Narration/N1.N1'")),
		LoadObject<USoundBase>(nullptr,TEXT("/Script/Engine.SoundWave'/Game/LHJ/Sounds/Narration/N2.N2'")),
		LoadObject<USoundBase>(nullptr,TEXT("/Script/Engine.SoundWave'/Game/LHJ/Sounds/Narration/N3.N3'")),
		LoadObject<USoundBase>(nullptr,TEXT("/Script/Engine.SoundWave'/Game/LHJ/Sounds/Narration/N4.N4'")),
		LoadObject<USoundBase>(nullptr,TEXT("/Script/Engine.SoundWave'/Game/LHJ/Sounds/Narration/N5.N5'"))
	};
}

void ACHJ_GameMode::BeginPlay()
{
	Super::BeginPlay();

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
	Player = Cast<AHD_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CreateGamePlayWidget();

	PlaySoundAtIndex(0);
}

void ACHJ_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CurrentNarrationTime += DeltaSeconds;
	if (NarrationQueue.IsEmpty() == false && NarrationDuration < CurrentNarrationTime)
	{
		int32 idx;
		NarrationQueue.Dequeue(idx);

		auto& DialogSB = NarrationDatas[idx];
		if (GamePlayWidget)
		{
			// 나레이션 음성 파일을 재생한다.
			UGameplayStatics::PlaySound2D(GetWorld(), DialogSB.Sound);
			NarrationDuration = DialogSB.Sound->GetDuration();
			CurrentNarrationTime = 0.0f;
			GamePlayWidget->WBP_PlayerWidget->ShowDialogForDuration(DialogSB.Icon, DialogSB.Name, DialogSB.Description);
		}
	}
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
	if (GamePlayWidgetFactory)
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

void ACHJ_GameMode::PlaySoundAtIndex(int32 idx)
{
	if (NarrationDatas.IsValidIndex(idx))
	{
		NarrationQueue.Enqueue(idx);
	}
}
