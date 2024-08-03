// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HD_CompanionStateComponent.h"
#include "HD_SorcererStateComponent.generated.h"

UENUM(BlueprintType)
enum class ESorcererBattleState : uint8
{
	State_CombatCheck UMETA(DisplayName = "CombatCheck"),
	State_MagickBolt UMETA(DisplayName = "�⺻����"),
	State_HighHagol UMETA(DisplayName = "ǳ�Ӽ� ����"),
	State_HighLevin UMETA(DisplayName = "���� ������"),
	State_Levitate UMETA(DisplayName = "���ߺξ�"),
	State_ArgentSuccor UMETA(DisplayName = "��"),
	State_Galvanize UMETA(DisplayName = "���׹̳� ä���"),
};
/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_SorcererStateComponent : public UHD_CompanionStateComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void StartBattle();
	virtual void AttackTick(float DeltaTime);

	UPROPERTY()
	class UHD_SorcererAnimInstance* SorcererAnimInstance;
};
