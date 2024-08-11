// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_GameClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_GameClearWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Credits;

	void PlayCreditAnim();
};
