// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HD_BossHPGemWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_BossHPGemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void AnimHideWidget();
};
