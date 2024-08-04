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
	// �ѽ�� �ִϸ��̼��� ����ϰ� �ʹ�.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	UAnimMontage* DieMontage;

	void PlayAttackMontage(int combo);

	void PlayLevitate();
	void EndLevitate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLevitate = false;
};
