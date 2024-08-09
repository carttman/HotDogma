// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_GamePlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_GamePlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// class UHD_GameOverWidget* WBP_GameOver;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	void ChangeSwitcher(int32 index);

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHD_PlayerWidget* WBP_PlayerWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHD_GameOverWidget* WBP_GameOver;
};
