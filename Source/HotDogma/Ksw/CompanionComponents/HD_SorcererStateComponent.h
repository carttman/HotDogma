// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HD_CompanionStateComponent.h"
#include "Components/TimelineComponent.h"
#include "HD_SorcererStateComponent.generated.h"

UENUM(BlueprintType)
enum class ESorcererBattleState : uint8
{
	State_CombatCheck UMETA(DisplayName = "CombatCheck"),
	State_MagickBolt UMETA(DisplayName = "기본공격"),
	State_HighHagol UMETA(DisplayName = "풍속성 공격"),
	State_HighLevin UMETA(DisplayName = "전기 찌짐이"),
	State_Levitate UMETA(DisplayName = "공중부양"),
	State_ArgentSuccor UMETA(DisplayName = "힐"),
	State_Galvanize UMETA(DisplayName = "스테미나 채우기"),
};
/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_SorcererStateComponent : public UHD_CompanionStateComponent
{
	GENERATED_BODY()

public:
	UHD_SorcererStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void StartBattle();
	virtual void EndBattle();
	virtual void AttackTick(float DeltaTime);

	void SetBattleState(ESorcererBattleState state);

	void CombatCheck();

	void MagickBolt();
	void HighHagol();
	void HighLevin();
	void Levitate();
	void ArgentSuccor();
	void Galvanize();

	void LevitateTick(float DeltaTime);
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> HighHagolFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> HighLevinFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> ArgentSuccorFactory;

	UPROPERTY()
	class UHD_SorcererAnimInstance* SorcererAnimInstance;

	float CurrentAttackTime = 0.0f;

	bool bStrafing = false;
	float CombatTime = 2.0f;

	// MagickBolt
	bool bCastingMagickBolt = false;
	float MagickBoltCastTime = 0.25f;
	float MagickBoltRange = 20000.0f;
	float MagickBoltTime = 0.3f;
	int32 MagickBoltCount = 0;
	int32 MaxMagickBoltCount = 12;

	// HighHagol
	bool bCastingHighHagol = false;
	float HighHagolCastTime = 2.0f;
	float HighHagolRange = 30000.0f;
	float HighHagolTime = 1.0f;

	// HighLevin
	bool bCastingHighLevin = false;
	float HighLevinCastTime = 3.0f;
	float HighLevinRange = 25000.0f;
	float HighLevinTime = 0.3f;
	int32 HighLevinCount = 0;
	int32 MaxHighLevinCount = 12;

	// Levitate
	bool bMaxLevitate = false;
	float LevitateTime = 10.0f;
	int32 LevitateUp = 1;
	float LevitateAnimTime = 1.0f;

	FTimerHandle LevitateTimerHandle;

	// ArgentSuccor
	bool bCastingArgentSuccor = false;
	float ArgentSuccorCastTime = 3.0f;
	float ArgentSuccorRange = 1000.0f;
	float ArgentSuccorTime = 1.0f;
	// Galvanize

	float GalvanizeRange = 1000.0f;
	float GalvanizeTime = 1.0f;

};
