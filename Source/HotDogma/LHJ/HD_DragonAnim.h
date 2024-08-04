// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <vector>

#include "CoreMinimal.h"
#include "HD_DragonFSM.h"
#include "Animation/AnimInstance.h"

#include "HD_DragonAnim.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API UHD_DragonAnim : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	DragonState AnimState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	AttackState AnimNormalAttackState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	class UHD_DragonFSM* fsm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	class AHD_Dragon* Dragon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bSleepEnd = false; // 플레이어 인지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bPlayShoutAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bFlyPress = false; // 공중 찍기 시전

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bEndFlyUp = false; // 날아 오르기 완료

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float InnerAngle = 0.f; // 캐릭터와 내적 값

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool chkAngle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool isFly = false; // 공중이동 전용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool isGroggy = false; // 그로기 전용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true))
	float Direction;

	bool chkUsingSkillCnt = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true))
	float Speed;

	UFUNCTION(BlueprintCallable)
	void ChangeState(DragonState ChangeState);

	UFUNCTION(BlueprintCallable)
	void ChangeAttackState(AttackState ChangeState);

	UFUNCTION(BlueprintCallable)
	void StartFlyUpFunction();

	//=======================전투시작
	UFUNCTION()
	void AnimNotify_SleepEnd(); // 주변 캐릭터 인지

	UFUNCTION()
	void AnimNotify_StartFight(); // 전투 시작(포효 시작)

	UFUNCTION()
	void AnimNotify_endShout(); // 전투 시작 포효 종료

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bEndStartAnim = false;
	//=========================도약찍기
	UFUNCTION()
	void AnimNotify_bPress();

	UFUNCTION()
	void AnimNotify_StartFlyPress();

	UFUNCTION()
	void AnimNotify_PressEnd();

	UFUNCTION()
	void AnimNotify_AttackJumpPress();

	UFUNCTION()
	void AnimNotify_AttackHandPress();

	bool GetAttackPress(const float &AttackDistance);

	FRotator JumpPress_Target_Rot;

	UPROPERTY()
	TSet<AActor*> DamageActorSet;
	//=========================꼬리치기, 손바닥 내려치기 변수 초기화
	UFUNCTION()
	void AnimNotify_StartTailSlap();

	UFUNCTION()
	void AnimNotify_EndTailSlap();

	UFUNCTION()
	void AnimNotify_StartScratch();

	UFUNCTION()
	void AnimNotify_EndScratch();

	UFUNCTION()
	void AnimNotify_ClearSet();
	//=========================공격 시작, 종료
	UFUNCTION()
	void AnimNotify_StartAttack();

	UFUNCTION()
	void AnimNotify_EndAttack();
	//=========================
	UFUNCTION()
	void AnimNotify_RotateFire();

	UFUNCTION()
	void AnimNotify_StartFlyUp();

	UFUNCTION()
	void AnimNotify_EndFlyUp();

	UFUNCTION()
	void AnimNotify_StartFlyDown();

	UFUNCTION()
	void AnimNotify_EndFlyDown();

	UFUNCTION()
	void AnimNotify_StartFlyAttack();

	UFUNCTION()
	void AnimNotify_EndFlyAttack();

	UFUNCTION()
	void AnimNotify_StartBreath();

	UFUNCTION()
	void AnimNotify_EndBreath();
	//=================================데미지 값
	UPROPERTY(EditAnywhere)
	float Damage_JumpPress = 10.f; // 점프찍기

	UPROPERTY(EditAnywhere)
	float Damage_HandPress = 10.f; // 짓누르기

	UPROPERTY(EditAnywhere)
	float Damage_TailSlap = 10.f; // 꼬리치기

	UPROPERTY(EditAnywhere)
	float Damage_Scratch = 10.f; // 할퀴기

	UPROPERTY(EditAnywhere)
	float Damage_Methor = 10.f; // 메테오

	UPROPERTY(EditAnywhere)
	float Damage_Thunder = 10.f; // 번개
};
