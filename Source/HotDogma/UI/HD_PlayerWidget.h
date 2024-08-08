// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Dragon_HealthBar;
public:
	void SetHP(float currHP, float maxHP);
	void Set_DragonHP(float currHP, float maxHP);
};
