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
		float Duration = Montage_Play(AttackMontage, 1.f);
		//UE_LOG(LogTemp, Warning, TEXT("Montage_Play : %s"), *MontageSection.ToString());
		if (Duration > 0.0f)
		{
			// Start at a given Section.
			Montage_JumpToSection(MontageSection, AttackMontage);
			//UE_LOG(LogTemp, Warning, TEXT("Montage_JumpToSection : %s"), *MontageSection.ToString());
		}
	}
}