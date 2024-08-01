// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "HD_PlayerAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOTDOGMA_API UHD_PlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHD_PlayerAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);
	void EnhancedSkill(const FInputActionValue& InputActionValue);
public:
	void PlayerAttack();
	void UpdatePlayerAttack(float DeltaTime);
public:
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_HD_Attack;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_HD_Skill;
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_BaseAttack;

public:
	UPROPERTY()
	class AHD_CharacterBase* Player;
	UPROPERTY()
	class UHD_PlayerAnimInstance* PlayerAnim;
	UPROPERTY()
	class AHD_Dragon* Dragon;

	int32 ComboCount = 0;
	float CurrComboTime = 0;
	float MinComboTime = 0.5f;
	float MaxComboTime = 1.4f;

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_Splitter;

	UFUNCTION()
	void PlayMontageNotifyBegin_Splitter(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void Skill_Splitter();
	void Update_Skill_Splitter();

	bool IsSplitting = false;
	float TargetFOV = 90;
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_Cutting;

	UFUNCTION()
	void PlayMontageNotifyBegin_Cutting(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void Skill_Cutting();
	void Update_Skill_Cuttring();
	void Cutting_GetTarget();
	bool CuttingHit;
	FRotator Cutting_Target_Rot;
	float Cutting_Attack_Range = 600.f;
};
