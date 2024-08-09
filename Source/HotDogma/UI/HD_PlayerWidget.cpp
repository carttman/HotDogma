// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/HD_PlayerWidget.h"

#include "HD_GameOverWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UHD_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar->SetPercent(1);
	Dragon_HealthBar->SetPercent(1);

	if (EmptyGauge5)
		ArrEmptyGauge.Add(EmptyGauge5);
	if (EmptyGauge4)
		ArrEmptyGauge.Add(EmptyGauge4);
	if (EmptyGauge3)
		ArrEmptyGauge.Add(EmptyGauge3);
	if (EmptyGauge2)
		ArrEmptyGauge.Add(EmptyGauge2);
	if (EmptyGauge1)
		ArrEmptyGauge.Add(EmptyGauge1);
	if (EmptyGauge0)
		ArrEmptyGauge.Add(EmptyGauge0);
	//WBP_GameOver->SetVisibility(ESlateVisibility::Hidden);
}

void UHD_PlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHD_PlayerWidget::SetHP(float currHP, float maxHP)
{
	float percent = currHP / maxHP;
	HealthBar->SetPercent(percent);
}

void UHD_PlayerWidget::Set_DragonHP(float currHP, float maxHP, int RemainLineCnt)
{
	float percent = currHP / maxHP;
	Dragon_HealthBar->SetPercent(percent);

	for (int i = 0; i < ArrEmptyGauge.Num(); i++)
	{
		if (i < RemainLineCnt)
		{
			ArrEmptyGauge[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ArrEmptyGauge[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
