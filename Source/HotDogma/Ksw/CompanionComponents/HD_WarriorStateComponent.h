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
	State_FindAttackPoint UMETA(DisplayName = "공격 지점"),
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void StartBattle();
	virtual void AttackTick(float DeltaTime);

	void CombatCheck();
	bool FindAttackPoint();
	void MightySweep();
	void ChargedSlash();
	void HeavenwardSunder();
	void IndomitableLash();
	void SetBattleState(EWarriorBattleState State);

	void PatternRotting();
	EWarriorBattleState NextPattern();
	EWarriorBattleState CurrentBattleState;

	UPROPERTY()
	TArray<EWarriorBattleState> PatternList;

	int32 PatternIndex = 0;
	FVector AttackPoint;
	
	float CurrentAttackTime = 0.0f;
	
	float CombatTime = 1.0f;
	
	// MightySweep
	int CurrentCombo = 0;
	float MinComboTime = 1.0f;
	float MaxComboTime = 2.0f;
	float MightySweepRange = 100.0f;
	float MightySweepDamage = 10.0f;

	// ChargedSlash
	float ChargedSlashRange = 300.0f;
	float ChargedSlashDamage = 20.0f;

	// HeavenwardSunder
	float HeavenwardSunderRange = 400.0f;
	float HeavenwardSunderDamage = 30.0f;

	// IndomitableLash
	float IndomitableLashRange = 500.0f;
	float IndomitableLashDamage = 40.0f;


	UPROPERTY()
	class UHD_WarriorAnimInstance* WarriorAnimInstance;

};
