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

	class AHD_PlayerWeaponBase* PlayerWeapon;
public:
	UFUNCTION()
	void AnimNotify_Damage_On();
	UFUNCTION()
	void AnimNotify_Damage_Off();

	//UFUNCTION()
	//void AnimNotify_Start_Hit();
	//UFUNCTION()
	//void AnimNotify_End_Hit();
};
