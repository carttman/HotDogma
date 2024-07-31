// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HD_CompanionStateComponent.h"
#include "HD_WarriorStateComponent.generated.h"

// Enum���� ���� ����
UENUM(BlueprintType)
enum class EWarriorBattleState : uint8
{
	State_CombatCheck UMETA(DisplayName = "CombatCheck"),
	State_MightySweep UMETA(DisplayName = "�⺻����"),
	State_ChargedSlash UMETA(DisplayName = "���� ����"),
	State_HeavenwardSunder UMETA(DisplayName = "�÷� ġ��"),
	State_IndomitableLash UMETA(DisplayName = "���"),
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
