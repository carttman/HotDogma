// Fill out your copyright notice in the Description page of Project Settings.


#include "HotDogma/UI/HD_BossHPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "HD_BossHPGemWidget.h"

void UHD_BossHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Dragon_HealthBar->SetPercent(1);
	Dragon_BackBar->SetPercent(1);

	if (WBP_BossHPGem6)
		ArrEmptyGauge.Add(WBP_BossHPGem6);
	if (WBP_BossHPGem5)
		ArrEmptyGauge.Add(WBP_BossHPGem5);
	if (WBP_BossHPGem4)
		ArrEmptyGauge.Add(WBP_BossHPGem4);
	if (WBP_BossHPGem3)
		ArrEmptyGauge.Add(WBP_BossHPGem3);
	if (WBP_BossHPGem2)
		ArrEmptyGauge.Add(WBP_BossHPGem2);
	if (WBP_BossHPGem1)
		ArrEmptyGauge.Add(WBP_BossHPGem1);
	if (WBP_BossHPGem0)
		ArrEmptyGauge.Add(WBP_BossHPGem0);

	RemainLine = ArrEmptyGauge.Num();
	//WBP_GameOver->SetVisibility(ESlateVisibility::Hidden);
}

void UHD_BossHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (CurrentPercent > TargetPercent)
	{
		CurrentPercent -= DecreaseRate * InDeltaTime;
		if (CurrentPercent < TargetPercent)
		{
			CurrentPercent = TargetPercent;
		}

		if (Dragon_BackBar != nullptr)
		{
			Dragon_BackBar->SetPercent(CurrentPercent);
		}
	}
}

void UHD_BossHPWidget::Set_DragonHP(float currHP, float maxHP, int RemainLineCnt)
{
	float percent = currHP / maxHP;
	if (TargetPercent < percent)
	{
		CurrentPercent = percent;
		Dragon_BackBar->SetPercent(percent);
	}

	TargetPercent = percent;
	Dragon_HealthBar->SetPercent(TargetPercent);

	for (int i = RemainLine - 1; i >= RemainLineCnt; i--)
	{
		ArrEmptyGauge[i]->AnimHideWidget();
	}

	RemainLine = RemainLineCnt;
}
