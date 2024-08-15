// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/TimelineComponent.h"
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
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHD_BossHPWidget* WBP_BossHPWidget;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* NarrationOverlay;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHD_NarrationWidget> NarrationWidgetFactory;
	
	void AddNarrationWidget(UHD_NarrationWidget* widget);

	UPROPERTY()
	FTimeline WidgetMoveTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCurveFloat* MoveCurve;

	UFUNCTION()
	void HandleProgress(float Value);

	UFUNCTION()
	void WidgetMoveEnd();

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// class UHD_GameOverWidget* WBP_GameOver;
public:
	void SetHP(float currHP, float maxHP);
	void Set_DragonHP(float currHP, float maxHP, int RemainLineCnt);
	
	UPROPERTY()
	TArray<FTimerHandle> DialogHnds;
	UPROPERTY(EditAnywhere, Category = "MyStruct")
    USoundBase* Sound;

	UFUNCTION()
	void ShowDialogForDuration(UTexture2D* Icon, FString Name, FString Description);

	UFUNCTION()
	void EnVisibleTextBlock(UHD_NarrationWidget* widget);
};
