// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
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
	void PlayerBaseAttackPlay(int32 ComboCnt, FName SectionName);
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
	class AHD_CharacterPlayer* CharacterPlayer;
	UPROPERTY()
	class UHD_PlayerAnimInstance* PlayerAnim;
	UPROPERTY()
	class AHD_PlayerWeaponBase* Left_Weapon;
	UPROPERTY()
	class AHD_PlayerWeaponBase* Right_Weapon;

	UPROPERTY()
	class AHD_Dragon* Dragon;

	int32 ComboCount = 0;
	float CurrComboTime = 0;
	float MinComboTime = 0.3f;
	float MaxComboTime = 1.0f;

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_Splitter;

	UFUNCTION()
	void PlayMontageNotifyBegin_Splitter(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void Skill_Splitter();
	void Update_Skill_Splitter();

	bool IsSplitting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	float Cutting_Attack_Range = 200.f;

	bool IsCutting = false;

	void RotatingCamera();
	FTimerHandle CutterTimerHandle;

	// Timeline Component
	UPROPERTY()
	class UTimelineComponent* YawRotationTimeline;
	// Float Curve for Timeline
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* YawCurve;
	// Timeline Function
	UFUNCTION()
	void HandleYawRotation(float Value);
	// Input function to trigger timeline
	void RotateCamera();

protected:
	// Timeline Progress Function
	FOnTimelineFloat YawRotationProgress;
	
public:
	void SlowDownTime(float SlowDownFactor, float Duration);
	// 타임 딜레이션을 복원하는 함수
	void RestoreTimeDilation();
	// 원래 타임 딜레이션
	float OriginalTimeDilation;
	// 타임 딜레이션 타이머 핸들
	FTimerHandle TimeDilationTimerHandle;
};
