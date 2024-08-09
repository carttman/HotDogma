// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/HD_GamePlayWidget.h"

#include "Components/WidgetSwitcher.h"

void UHD_GamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHD_GamePlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHD_GamePlayWidget::ChangeSwitcher(int32 index)
{
	WidgetSwitcher->SetActiveWidgetIndex(index);
}
