// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_WarriorAnimInstance.h"
#include "HD_CompanionCharacter.h"
#include "HotDogma/Ksw/HD_CompanionWeapon.h"

void UHD_WarriorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Companion = Cast<AHD_CompanionCharacter>(TryGetPawnOwner());
}

void UHD_WarriorAnimInstance::PlayAttackMontage(int32 combo)
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

void UHD_WarriorAnimInstance::PlayChargedSlashMontage(int32 step)
{
	if (ChargedSlashMontage)
	{
		FName MontageSection = FName(*FString::Printf(TEXT("ChargedSlash_%d"), step));
		float Duration = Montage_Play(ChargedSlashMontage, 1.f);
		if (Duration > 0.0f)
		{
			Montage_JumpToSection(MontageSection, ChargedSlashMontage);
		}
	}
}

void UHD_WarriorAnimInstance::PlayHeavenwardSunderMontage(int32 step)
{
	if (HeavenwardSunderMontage)
	{
		FName MontageSection = FName(*FString::Printf(TEXT("HeavenwardSunder_%d"), step));
		float Duration = Montage_Play(HeavenwardSunderMontage, 1.f);
		if (Duration > 0.0f)
		{
			Montage_JumpToSection(MontageSection, HeavenwardSunderMontage);
		}
	}
}

void UHD_WarriorAnimInstance::PlayIndomitableLashMontage(int32 step)
{
	if (IndomitableLashMontage)
	{
		FName MontageSection = FName(*FString::Printf(TEXT("IndomitableLash_%d"), step));
		float Duration = Montage_Play(IndomitableLashMontage, 1.f);
		if (Duration > 0.0f)
		{
			Montage_JumpToSection(MontageSection, IndomitableLashMontage);
		}
	}
}

void UHD_WarriorAnimInstance::AnimNotify_Damage_On()
{
	Companion->CompanionWeapon->OnCollision(true);
}

void UHD_WarriorAnimInstance::AnimNotify_Damage_Off()
{
	Companion->CompanionWeapon->OnCollision(false);
}
