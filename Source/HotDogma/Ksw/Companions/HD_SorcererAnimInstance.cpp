// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_SorcererAnimInstance.h"

void UHD_SorcererAnimInstance::NativeInitializeAnimation()
{
}

void UHD_SorcererAnimInstance::PlayAttackMontage(int combo)
{
	if (AttackMontage)
	{
		FName MontageSection = FName(*FString::Printf(TEXT("Cast_%d"), combo));
		float Duration = Montage_Play(AttackMontage, 1.f);
		if (Duration > 0.0f)
		{
			Montage_JumpToSection(MontageSection, AttackMontage);
		}
	}
}

void UHD_SorcererAnimInstance::PlayLevitate()
{
	bLevitate = true;
}

void UHD_SorcererAnimInstance::EndLevitate()
{
	bLevitate = false;
}
