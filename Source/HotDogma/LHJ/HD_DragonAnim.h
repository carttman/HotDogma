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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	class UHD_DragonFSM* fsm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	class AHD_Dragon* Dragon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bSleepEnd = false; // 플레이어 인지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	bool bPlayShoutAnim = false;

	UFUNCTION(BlueprintCallable)
	void PlayShoutAnim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true))
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess=true))
	float Speed;	
};
