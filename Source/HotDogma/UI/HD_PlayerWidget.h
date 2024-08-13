// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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
	
	UPROPERTY(meta = (BindWidget))
	class UImage* EmptyGauge0;
	UPROPERTY(meta = (BindWidget))
	class UImage* EmptyGauge1;
	UPROPERTY(meta = (BindWidget))
	class UImage* EmptyGauge2;
	UPROPERTY(meta = (BindWidget))
	class UImage* EmptyGauge3;
	UPROPERTY(meta = (BindWidget))
	class UImage* EmptyGauge4;
	UPROPERTY(meta = (BindWidget))
	class UImage* EmptyGauge5;
	UPROPERTY(meta = (BindWidget))
	class UImage* EmptyGauge6;
	
	TArray<class UImage*> ArrEmptyGauge;
	
	UPROPERTY(meta = (BindWidget))
	class UOverlay* NarrationOverlay;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHD_NarrationWidget> NarrationWidgetFactory;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	// class UHD_GameOverWidget* WBP_GameOver;
public:
	void SetHP(float currHP, float maxHP);
	void Set_DragonHP(float currHP, float maxHP, int RemainLineCnt);
	
	UPROPERTY()
	FTimerHandle DialogHnd;
	UPROPERTY(EditAnywhere, Category = "MyStruct")
    USoundBase* Sound;

	UFUNCTION()
	void ShowDialogForDuration(UTexture2D* Icon, FString Name, FString Description, float duration);

	UFUNCTION()
	void EnVisibleTextBlock(UHD_NarrationWidget* widget);
};
