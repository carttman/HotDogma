// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/HD_PlayerWidget.h"

#include "HD_GameOverWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "HD_NarrationWidget.h"
#include "Components/Overlay.h"
#include "Components/TimelineComponent.h"
#include "HD_BossHPWidget.h"

void UHD_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar->SetPercent(1);

	if (MoveCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleProgress"));

		// 종료
		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("WidgetMoveEnd"));

		WidgetMoveTimeline.AddInterpFloat(MoveCurve, ProgressFunction);
		WidgetMoveTimeline.SetTimelineFinishedFunc(FinishedEvent);

		WidgetMoveTimeline.SetLooping(false);
	}
}

void UHD_PlayerWidget::NativeDestruct()
{
	Super::NativeDestruct();

	for (auto& DialogHnd : DialogHnds)
	{
		GetWorld()->GetTimerManager().ClearTimer(DialogHnd);
	}

	DialogHnds.Empty();
}

void UHD_PlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (WidgetMoveTimeline.IsPlaying())
	{
		WidgetMoveTimeline.TickTimeline(InDeltaTime);
	}
}

void UHD_PlayerWidget::SetHP(float currHP, float maxHP)
{
	float percent = currHP / maxHP;
	HealthBar->SetPercent(percent);
}

void UHD_PlayerWidget::Set_DragonHP(float currHP, float maxHP, int RemainLineCnt)
{
	WBP_BossHPWidget->Set_DragonHP(currHP, maxHP, RemainLineCnt);
}

void UHD_PlayerWidget::ShowDialogForDuration(UTexture2D* Icon, FString Name, FString Description)
{
	auto* WidgetItem = Cast<UHD_NarrationWidget>(CreateWidget(GetWorld(), NarrationWidgetFactory));
	
	WidgetItem->MessageStr = Description;
	WidgetItem->NarrationImage->SetBrushFromTexture(Icon);
	WidgetItem->NarrationText->SetText(FText::FromString(Name));

	AddNarrationWidget(WidgetItem);
}

void UHD_PlayerWidget::EnVisibleTextBlock(UHD_NarrationWidget* widget, FTimerHandle DialogHnd)
{
	// 제거한다.
	GetWorld()->GetTimerManager().SetTimer(DialogHnd, [this, DialogHnd, widget]() {
		NarrationOverlay->RemoveChild(widget);
		DialogHnds.Remove(DialogHnd);
		}, 0.4f, false);
}

void UHD_PlayerWidget::AddNarrationWidget(UHD_NarrationWidget* widget)
{
	// Overlay에 자식이 있다면 마지막 자식을 제거한다.
	if (NarrationOverlay->GetChildrenCount() > 0)
	{
		auto* LastWidget = Cast<UHD_NarrationWidget>(NarrationOverlay->GetChildAt(NarrationOverlay->GetChildrenCount() - 1));
		if (LastWidget)
		{
			LastWidget->AnimNextWidget();
		}
	}

	NarrationOverlay->AddChild(widget);
	FTimerHandle DialogHnd;
	DialogHnds.Add(DialogHnd);

	WidgetMoveTimeline.PlayFromStart();
	widget->AnimStartWidget();

	GetWorld()->GetTimerManager().SetTimer(DialogHnd, [this, DialogHnd, widget]() {
		EnVisibleTextBlock(widget, DialogHnd);
		widget->AnimHideWidget();
		}, 10.0f, false);
}

void UHD_PlayerWidget::HandleProgress(float Value)
{
	for (auto* Widget : NarrationOverlay->GetAllChildren())
	{
		// 마지막으로 들어온 애는 제외
		if (Widget == NarrationOverlay->GetChildAt(NarrationOverlay->GetChildrenCount() - 1))
		{
			continue;
		}
		auto* NarrationWidget = Cast<UHD_NarrationWidget>(Widget);
		if (NarrationWidget)
		{
			// 기존 위치에서 value만큼 이동시킨다.
			NarrationWidget->SetRenderTranslation(FVector2D(0, NarrationWidget->yPos + Value));
		}
	}
}

void UHD_PlayerWidget::WidgetMoveEnd()
{
	for (auto* Widget : NarrationOverlay->GetAllChildren())
	{
		// 마지막으로 들어온 애는 제외
		if (Widget == NarrationOverlay->GetChildAt(NarrationOverlay->GetChildrenCount() - 1))
		{
			continue;
		}
		auto* NarrationWidget = Cast<UHD_NarrationWidget>(Widget);
		if (NarrationWidget)
		{
			// 기존 위치에서 value만큼 이동시킨다.
			NarrationWidget->yPos += -100;
		}
	}
}
