// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HD_CompanionStateComponent.h"
#include "HD_WarriorStateComponent.generated.h"

// Enum으로 상태 관리
UENUM(BlueprintType)
enum class EWarriorBattleState : uint8
{
	State_CombatCheck UMETA(DisplayName = "CombatCheck"),
	State_MightySweep UMETA(DisplayName = "기본공격"),
	State_ChargedSlash UMETA(DisplayName = "충전 공격"),
	State_HeavenwardSunder UMETA(DisplayName = "올려 치기"),
	State_IndomitableLash UMETA(DisplayName = "찍기"),
};

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_WarriorStateComponent : public UHD_CompanionStateComponent
{
	GENERATED_BODY()
public:
	virtual void AttackTick(float DeltaTime);

};
