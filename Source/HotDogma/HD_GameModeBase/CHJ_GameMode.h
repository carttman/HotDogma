// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CHJ_GameMode.generated.h"

USTRUCT()
struct FNarraionData
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "MyStruct")
    USoundBase* Sound;

    UPROPERTY(EditAnywhere, Category = "MyStruct")
    FString Name;

    UPROPERTY(EditAnywhere, Category = "MyStruct")
    FString Description;

    UPROPERTY(EditAnywhere, Category = "MyStruct")
    UTexture2D* Icon;
};
/**
 * 
 */
UCLASS()
class HOTDOGMA_API ACHJ_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void CommandCompanion(int num);
	// Pos���� ���� ����� �巡���� ã�´�.
	APawn* GetEnemy(FVector Pos);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompanionManager")
	bool bCompanionsSpawn = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompanionManager")
	TSubclassOf<class AHD_CompanionManager> CompanionManagerFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompanionManager")
	class AHD_CompanionManager* CompanionManager;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dragon")
	TSubclassOf<class AHD_Dragon> DragonFactory;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dragon")
	TArray<APawn*> Dragons;

	class AHD_CharacterPlayer* Player;

	void CreatePlayerWidget();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UHD_GamePlayWidget* GamePlayWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UHD_GamePlayWidget> GamePlayWidgetFactory;

	void CreateGamePlayWidget();
	
	void SetHPUI(float Curr, float Max);
	void SetDragonHPUI(float Curr, float Max, int RemainLineCnt);

	// 나레이션
	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> ArrNarration;
	UPROPERTY(EditAnywhere)
	TArray<FNarraionData> NarrationDatas;

	UFUNCTION()
	void PlaySoundAtIndex(int32 idx);
	
	UPROPERTY(EditAnywhere)
	class UHD_PlayerWidget* PlayerWidget;
};
