// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_BossHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_BossHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Dragon_HealthBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Dragon_BackBar;
	
	UPROPERTY(meta = (BindWidget))
	class UHD_BossHPGemWidget* WBP_BossHPGem6;
	UPROPERTY(meta = (BindWidget))
	class UHD_BossHPGemWidget* WBP_BossHPGem5;
	UPROPERTY(meta = (BindWidget))
	class UHD_BossHPGemWidget* WBP_BossHPGem4;
	UPROPERTY(meta = (BindWidget))
	class UHD_BossHPGemWidget* WBP_BossHPGem3;
	UPROPERTY(meta = (BindWidget))
	class UHD_BossHPGemWidget* WBP_BossHPGem2;
	UPROPERTY(meta = (BindWidget))
	class UHD_BossHPGemWidget* WBP_BossHPGem1;
	UPROPERTY(meta = (BindWidget))
	class UHD_BossHPGemWidget* WBP_BossHPGem0;
	
	TArray<class UHD_BossHPGemWidget*> ArrEmptyGauge;

	void Set_DragonHP(float currHP, float maxHP, int RemainLineCnt);

	float CurrentPercent = 1.0f;
	float TargetPercent = 1.0f;
	float DecreaseRate = 0.2f;

	int RemainLine;
};
