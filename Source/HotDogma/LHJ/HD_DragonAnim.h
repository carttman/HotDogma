// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	NormalAttackState AnimNormalAttackState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	FlyAttackState AnimFlyAttackStateState;

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
	float InnerAngle = 0.f;	// 캐릭터와 내적 값

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool chkAngle=false;

	UFUNCTION(BlueprintCallable)
	void PlayShoutAnim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true))
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true))
	float Speed;

	UFUNCTION(BlueprintCallable)
	void ChangeState(DragonState ChangeState);

	UFUNCTION(BlueprintCallable)
	void ChangeNormalAttack(NormalAttackState ChangeState);

	UFUNCTION(BlueprintCallable)
	void ChangeFlyAttack(FlyAttackState ChangeState);

	UFUNCTION(BlueprintCallable)
	void StartFlyUpFunction();
};
