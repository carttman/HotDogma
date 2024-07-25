// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_PlayerAnimInstance.h"

#include "HD_CharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHD_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Player = Cast<AHD_CharacterPlayer>(TryGetPawnOwner());
}

void UHD_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		// 앞 뒤 (dirV)
		//dirV = FVector::DotProduct(Player->GetActorForwardVector(), Player->GetVelocity());

		// 좌 우 (dirH)
		//dirH = FVector::DotProduct(Player->GetActorRightVector(), Player->GetVelocity());

		//0 ~ 360 --> -180 ~ 180
		//pitchAngle = -Player->GetBaseAimRotation().Pitch;
		
		//pitchAngle = -Player->GetBaseAimRotation().GetNormalized().Pitch;

		// GetVelocity의 크기가 0보다 크면 무브 true 로
		//if (Player->GetVelocity().Length() > 0)
		//{
		//	ShouldMove = true;
		//}
		//else
		//{
		//	ShouldMove = false;
		//}

		//jumpVelocity = Player->GetVelocity().Z;

		// 높이가 0보다 크면 falling
		//if (jumpVelocity > 0)
		//{
		//	isFalling = true;
		//}
		//else if(jumpVelocity == 0)
		//{
		//	isFalling = false;
		//}
		Velocity = Player->GetCharacterMovement()->Velocity;
		Speed = UKismetMathLibrary::VSizeXY(Velocity);
		
		if(Player->GetCharacterMovement()->GetCurrentAcceleration() != FVector(0) && Speed != 3) ShouldMove = true;
		else ShouldMove = false;

		isFalling = Player->GetCharacterMovement()->IsFalling();
		
	}
}
