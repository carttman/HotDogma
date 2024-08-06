// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HD_SorcererAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_SorcererAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	virtual void NativeInitializeAnimation() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* DieMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* HighHagolMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* HighLevinMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* ArgentSuccorMontage;

	void PlayAttackMontage(int32 combo);
	void PlayHighHagolMontage(int32 cast);
	void PlayHighLevinMontage(int32 cast);
	void PlayArgentSuccorMontage(int32 cast);

	void PlayLevitate();
	void EndLevitate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLevitate = false;
};
