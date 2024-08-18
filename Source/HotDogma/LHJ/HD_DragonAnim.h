#pragma once

#include "CoreMinimal.h"
#include "HD_DragonFSM.h"
#include "Animation/AnimInstance.h"
#include "HD_DragonAnim.generated.h"

UCLASS()
class HOTDOGMA_API UHD_DragonAnim : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY()
	class AHD_Dragon* Dragon;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	DragonState AnimState;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	AttackState AnimNormalAttackState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool isFly = false; // 공중이동 전용

	UPROPERTY(BlueprintReadWrite, Category = "Component")
	class UHD_DragonFSM* fsm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sleep")
	bool bSleepEnd = false; // 플레이어 인지

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sleep")
	bool bPlayShoutAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sleep")
	bool bEndStartAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpPress")
	bool bFlyPress = false; // 공중 찍기 시전

	UPROPERTY()
	bool bEndFlyUp = false; // JumpPress

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing")
	float InnerAngle = 0.f; // 캐릭터와 내적 값

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing")
	bool chkAngle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate")
	bool isRotate = false; // 타겟으로 회전

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true), Category = "Move")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true), Category = "Move")
	float Speed;

	UPROPERTY()
	TSet<AActor*> DamageActorSet;	// 중복 데미지 처리 방지용
	
#pragma region Change State
	UFUNCTION(BlueprintCallable)
	void ChangeState(DragonState ChangeState);
	UFUNCTION(BlueprintCallable)
	void ChangeAttackState(AttackState ChangeState);
#pragma endregion
	
#pragma region Sleep
	UFUNCTION()
	void AnimNotify_SleepEnd(); // 주변 캐릭터 인지
	UFUNCTION()
	void AnimNotify_StartFight(); // 전투 시작(포효 시작)
	UFUNCTION()
	void AnimNotify_endShout(); // 전투 시작 포효 종료
#pragma endregion
	
#pragma region Jump Press
	UFUNCTION()
	void AnimNotify_bPress();
	// UFUNCTION()
	// void AnimNotify_StartFlyPress();
	UFUNCTION()
	void AnimNotify_AttackJumpPress();
	UFUNCTION()
	void AnimNotify_PressEnd();
#pragma endregion

#pragma region HandPress
	UFUNCTION()
	void AnimNotify_AttackHandPress();
#pragma endregion

#pragma region Attack Common
	UFUNCTION()
	void AnimNotify_StartAttack();
	UFUNCTION()
	void AnimNotify_EndAttack();
	UFUNCTION()
	void AnimNotify_StartFlyAttack();
	UFUNCTION()
	void AnimNotify_EndFlyAttack();
	UFUNCTION()
	bool GetAttackPress(const float& AttackDistance);
	UFUNCTION()
	void ApplyDamageToTarget(AActor* Target, float Damage, FString DamageType);
	void EndAttack();
#pragma endregion

#pragma region Tail & Scratch
	UFUNCTION()
	void AnimNotify_MyStartTailSlap();
	UFUNCTION()
	void AnimNotify_EndTailSlap();
	UFUNCTION()
	void AnimNotify_StartScratch();
	UFUNCTION()
	void AnimNotify_EndScratch();
	UFUNCTION()
	void AnimNotify_ClearSet();
#pragma endregion

#pragma region FlyUp & FlyDown
	UFUNCTION()
	void AnimNotify_StartFlyUp();

	UFUNCTION()
	void AnimNotify_EndFlyUp();

	UFUNCTION()
	void AnimNotify_StartFlyDown();

	UFUNCTION()
	void AnimNotify_EndFlyDown();
#pragma endregion

#pragma region Breath
	UFUNCTION()
	void AnimNotify_StartBreath();

	UFUNCTION()
	void AnimNotify_EndBreath();
#pragma endregion

#pragma region Shout
	UFUNCTION()
	void AnimNotify_AttackShout();
#pragma endregion

#pragma region Magic Attack
	UFUNCTION()
	void AnimNotify_StartThunderAttack();
	UFUNCTION()
	void AnimNotify_StartMeteorAttack();
#pragma endregion

#pragma region Change Material
	UFUNCTION()
	void AnimNotify_ChangeMat();
	UFUNCTION()
	void AnimNotify_ReturnMat();
#pragma endregion

	UFUNCTION()
	void AnimNotify_RotateFire();
};