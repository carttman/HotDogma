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

	void SetBattleState(ESorcererBattleState state);

	void CombatCheck();

	void MagickBolt();
	void HighHagol();
	void HighLevin();
	void Levitate();
	void ArgentSuccor();
	void Galvanize();

	UFUNCTION()
	void EndLevitate();

	void PatternRotting();
	ESorcererBattleState NextPattern();
	ESorcererBattleState CurrentBattleState;

	bool FindAttackPoint();
	FVector AttackPoint;

	UPROPERTY()
	TArray<ESorcererBattleState> PatternList;

	int32 PatternIndex = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_Projectile> MagickBoltFactory;

	UPROPERTY()
	class UHD_SorcererAnimInstance* SorcererAnimInstance;

	float CurrentAttackTime = 0.0f;

	float CombatTime = 1.0f;

	// MagickBolt
	bool bCastingMagickBolt = false;
	float MagickBoltCastTime = 0.5f;
	float MagickBoltRange = 1000.0f;
	float MagickBoltTime = 0.3f;
	int32 MagickBoltCount = 0;
	int32 MaxMagickBoltCount = 12;

	// HighHagol
	float HighHagolRange = 1000.0f;
	float HighHagolTime = 1.0f;

	// HighLevin
	float HighLevinRange = 1000.0f;
	float HighLevinTime = 1.0f;

	// Levitate
	float LevitateTime = 10.0f;
	FTimerHandle LevitateTimerHandle;

	// ArgentSuccor
	float ArgentSuccorRange = 1000.0f;
	float ArgentSuccorTime = 1.0f;
	// Galvanize

	float GalvanizeRange = 1000.0f;
	float GalvanizeTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Companion")
	class UArrowComponent* ArrowComp;
};
