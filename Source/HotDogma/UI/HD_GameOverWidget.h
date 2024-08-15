// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_GameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Restart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_BackTitle;

	class AHD_CharacterPlayer* Player;
public:
	UFUNCTION()
	void OnMyReStart();
	UFUNCTION()
	void OnMyBackTitle();

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Fade_Out;
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Fade_In;
	
	FTimerHandle PlayTimerHandle;
	void OpenLevelTimer();
	void OpenTitle();
public:
	UFUNCTION(BlueprintCallable)
	void GameOverPlayFade();
	void GameOverReverseFade();
};
