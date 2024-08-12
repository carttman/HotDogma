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

	if(Narration1)
		Narration1->SetVisibility(ESlateVisibility::Hidden);
	if(Narration2)
		Narration2->SetVisibility(ESlateVisibility::Hidden);
	if(Narration3)
		Narration3->SetVisibility(ESlateVisibility::Hidden);
	if(Narration4)
		Narration4->SetVisibility(ESlateVisibility::Hidden);
	if(Narration5)
		Narration5->SetVisibility(ESlateVisibility::Hidden);
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

void UHD_PlayerWidget::ShowDialogForDuration(int32 idx, float duration)
{
	// TextBlock을 선언한다.
	UTextBlock* DialogTextBlock = nullptr;

	switch (idx)
	{
	case 0:
		DialogTextBlock = Narration1;
		break;
	case 1:
		DialogTextBlock = Narration2;
		break;
	case 2:
		DialogTextBlock = Narration3;
		break;
	case 3:
		DialogTextBlock = Narration4;
		break;
	case 4:
		DialogTextBlock = Narration5;
		break;
	}

	if (DialogTextBlock)
	{
		// 선택한 TextBlock Visible을 True 변경
		DialogTextBlock->SetVisibility(ESlateVisibility::Visible);

		// GetWorld()->GetTimerManager().SetTimer(DialogHnd, [this, idx]()
		// {
		// 	EnVisibleTextBlock(idx);
		// }, duration, false);
	}
}

void UHD_PlayerWidget::EnVisibleTextBlock(int32 idx)
{
	// TextBlock을 선언한다.
	UTextBlock* DialogTextBlock = nullptr;
	switch (idx)
	{
	case 0:
		DialogTextBlock = Narration1;
		break;
	case 1:
		DialogTextBlock = Narration2;
		break;
	case 2:
		DialogTextBlock = Narration3;
		break;
	case 3:
		DialogTextBlock = Narration4;
		break;
	case 4:
		DialogTextBlock = Narration5;
		break;
	}

	if (DialogTextBlock)
		DialogTextBlock->SetVisibility(ESlateVisibility::Hidden);
}
