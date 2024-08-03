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

	// �ѽ�� �ִϸ��̼��� ����ϰ� �ʹ�.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	UAnimMontage* DieMontage;

	void PlayAttackMontage(int combo);
};
