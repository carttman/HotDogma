// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HD_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float dirH;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float dirV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float jumpVelocity;

	UPROPERTY(BlueprintReadOnly)
	float pitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDeath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAttacking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHitting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AHD_CharacterPlayer* Player;

	class AHD_PlayerWeaponBase* Left_Weapon;
	class AHD_PlayerWeaponBase* Right_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsClimbing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Climb_LR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Climb_UD;
	
public:
	UFUNCTION()
	void AnimNotify_Damage_On();
	UFUNCTION()
	void AnimNotify_Damage_Off();

	UFUNCTION()
	void AnimNotify_Highfive();

	void ToggleHandIK(bool enable, ACharacter* Target);
	void PlayHighfive();

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_HighfiveMontage;
	UPROPERTY()
	class ACharacter* TargetCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HandTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandIKAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandIKTargetAlpha;
	//UFUNCTION()
	//void AnimNotify_Start_Hit();
	//UFUNCTION()
	//void AnimNotify_End_Hit();
};
