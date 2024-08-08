// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/HD_PlayerWidget.h"

#include "Components/ProgressBar.h"

void UHD_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar->SetPercent(1);
	Dragon_HealthBar->SetPercent(1);
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

void UHD_PlayerWidget::Set_DragonHP(float currHP, float maxHP)
{
	float percent = currHP / maxHP;
	Dragon_HealthBar->SetPercent(percent);
}
