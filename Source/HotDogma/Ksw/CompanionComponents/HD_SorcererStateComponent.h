// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HD_CompanionStateComponent.h"
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void StartBattle();
	virtual void AttackTick(float DeltaTime);

	UPROPERTY()
	class UHD_SorcererAnimInstance* SorcererAnimInstance;
};
