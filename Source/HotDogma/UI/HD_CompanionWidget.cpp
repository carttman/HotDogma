// Fill out your copyright notice in the Description page of Project Settings.


#include "HotDogma/UI/HD_CompanionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UHD_CompanionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHD_CompanionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (CurrentPercent > TargetPercent)
	{
		CurrentPercent -= DecreaseRate * InDeltaTime;
		if (CurrentPercent < TargetPercent)
		{
			CurrentPercent = TargetPercent;
		}

		if (HPProgressBarBack != nullptr)
		{
			HPProgressBarBack->SetPercent(CurrentPercent);
		}
	}
}

void UHD_CompanionWidget::SetHPBar(float HP, float MaxHP)
{
	TargetPercent = HP / MaxHP;
	if (HPProgressBar != nullptr)
	{
		HPProgressBar->SetPercent(TargetPercent);
	}
}

void UHD_CompanionWidget::SetImage(UTexture2D* Texture)
{
	if (CompanionImage != nullptr)
	{
		CompanionImage->SetBrushFromTexture(Texture);
	}
}

void UHD_CompanionWidget::SetText(FString Name)
{
	if (CompanionNameText != nullptr)
	{
		CompanionNameText->SetText(FText::FromString(Name));
	}
}
