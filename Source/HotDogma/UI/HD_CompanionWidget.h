// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_CompanionWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_CompanionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetHPBar(float HP, float MaxHP);
	void SetImage(UTexture2D* Texture);
	void SetText(FString Name);

	float CurrentPercent = 1.0f;
	float TargetPercent = 1.0f;
	float DecreaseRate = 0.1f;

	// 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* CompanionImage;

	// 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CompanionNameText;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HPProgressBarBack;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HPProgressBar;
};
