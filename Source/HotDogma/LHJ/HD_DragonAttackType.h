// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "HD_DragonAttackType.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_DragonAttackType : public UDamageType
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString strAttackType; 
};
