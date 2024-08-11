// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HD_WarriorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_WarriorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 총쏘기 애니메이션을 재생하고 싶다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* DieMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* ChargedSlashMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* HeavenwardSunderMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* IndomitableLashMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* HighfiveMontage;

	void PlayAttackMontage(int32 combo);

	void PlayChargedSlashMontage(int32 step);

	void PlayHeavenwardSunderMontage(int32 step);

	void PlayIndomitableLashMontage(int32 step);
	void PlayHighfiveMontage(int32 step);

	void ToggleHandIK(bool enable, FVector Target);

	UFUNCTION()
	void AnimNotify_Damage_On();
	UFUNCTION()
	void AnimNotify_Damage_Off();
	UFUNCTION()
	void AnimNotify_Highfive();

	UPROPERTY()
	class AHD_CompanionCharacter* Companion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HandTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandIKAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandIKTargetAlpha;
};
