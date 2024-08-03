// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_WarriorAnimInstance.h"

void UHD_WarriorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}


void UHD_WarriorAnimInstance::PlayAttackMontage(int combo)
{
	if (AttackMontage)
	{
		FName MontageSection = FName(*FString::Printf(TEXT("Attack_%d"), combo));
		Montage_Play(AttackMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
	}
}