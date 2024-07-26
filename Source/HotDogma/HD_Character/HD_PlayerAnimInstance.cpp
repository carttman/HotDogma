// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_PlayerAnimInstance.h"

#include "HD_CharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HD_PlayerItem/HD_PlayerWeaponBase.h"
#include "Kismet/KismetMathLibrary.h"

void UHD_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Player = Cast<AHD_CharacterPlayer>(TryGetPawnOwner());
	PlayerWeapon = Cast<AHD_PlayerWeaponBase>(GetOwningActor());
}

void UHD_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		Velocity = Player->GetCharacterMovement()->Velocity;
		Speed = UKismetMathLibrary::VSizeXY(Velocity);
		
		if(Player->GetCharacterMovement()->GetCurrentAcceleration() != FVector(0) && Speed != 3) ShouldMove = true;
		else ShouldMove = false;

		isFalling = Player->GetCharacterMovement()->IsFalling();
	}
}

void UHD_PlayerAnimInstance::AnimNotify_Damage_On()
{
	//캡슐 콜리전 on
	UE_LOG(LogTemp, Warning, TEXT("On"));
	PlayerWeapon->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UHD_PlayerAnimInstance::AnimNotify_Damage_Off()
{
	//캡슐 콜리전 off
	UE_LOG(LogTemp, Warning, TEXT("Off"));
	PlayerWeapon->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
