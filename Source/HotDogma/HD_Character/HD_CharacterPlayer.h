// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HD_CharacterBase.h"
#include "Components/TimelineComponent.h"
#include "HD_CharacterPlayer.generated.h"

UCLASS()
class HOTDOGMA_API AHD_CharacterPlayer : public AHD_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHD_CharacterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	class AHD_PlayerController* PlayerContoller;

	UPROPERTY(EditAnywhere)
	class USceneComponent* Left_WeaponScene;
	UPROPERTY(EditAnywhere)
	class USceneComponent* Right_WeaponScene;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_PlayerWeaponBase> PlayerWeaponFactory;

	UPROPERTY()
	class AHD_PlayerWeaponBase* Right_Weapon;
	UPROPERTY()
	class AHD_PlayerWeaponBase* Left_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHD_PlayerAttackComponent* PlayerAttackComponent;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_Hit_Montage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_KnockDown_Montage;
	
public:
	void AttachWeapon();
public:
	void DeathProcess();
	FTimerHandle DeathTimerHandle;
	void DeathTimer();
public:
	UFUNCTION()
	void PlayMontageNotifyBegin_KnockDown(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	void PlayMontageNotifyBegin_Hit(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
public:
	void SlowDownTime_Hit(float SlowDownFactor, float Duration);
	// 타임 딜레이션을 복원하는 함수
	void RestoreTimeDilation_Hit();
	// 원래 타임 딜레이션
	float OriginalTimeDilation;
	// 타임 딜레이션 타이머 핸들
	FTimerHandle TimeDilationTimerHandle;

	void GetPlayerCameraShake();
public:
	UPROPERTY()
	class APostProcessVolume* PostProcessVolume; // 하울링 맞았을때 포스트 프로세스 볼륨
	void OnPostProcess();
	void OffPostProcess();
	
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* MI_Chromatic;
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* MI_Radial_Zoom;
	float ScalarValue = 0;

	UMaterialInstanceDynamic* ChromaticMaterialInstance;
	
	// 타임라인 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UTimelineComponent* PostProcessTimeline;

	// 타임라인 곡선
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;
	
	UMaterialInstanceDynamic* RadialMaterialInstance;

	// 타임라인의 업데이트 함수
	UFUNCTION()
	void TimelineFloatReturn(float Value);

private:
	FOnTimelineFloat TimelineInterpFunction;
};
