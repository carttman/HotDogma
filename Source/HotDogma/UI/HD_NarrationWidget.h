// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_NarrationWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_NarrationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 블루프린트에서 구현
	UFUNCTION(BlueprintImplementableEvent)
	void AnimHideWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void AnimNextWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void AnimStartWidget();
	
	UPROPERTY(meta = (BindWidget))
	class UImage* NarrationImage;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NarrationText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class URichTextBlock* MessageText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString MessageStr;

	float yPos = 0.0f;
};
