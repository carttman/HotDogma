// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "HD_DragonFSM.generated.h"

UENUM()
enum class DragonState:uint8
{
	Sleep, // 초기 상태
	Idle, // 대기
	Shout, // 포효
	Move, // 걷기
	Attack, //공격
	FlyUp, // 이륙
	FlyDown, // 착륙
	Groggy // 그로기
};

UENUM()
enum class NormalAttackState:uint8
{
	Breath, // 브레스
	Shout, // 포효
	HandPress, // 손바닥 내려치기
	Scratch, // 할퀴기	
	TailSlap, // 꼬리치기
	ThunderMagic, // 전기마법공격
	Meteor, // 메테오
};

UENUM()
enum class AirAttackState:uint8
{
	FlyPress, // 날아 올랐다가 찍기
	FlyBreath, // 공중 브레스
	ThunderMagic, // 전기마법공격
	Meteor, // 메테오
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTDOGMA_API UHD_DragonFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHD_DragonFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma region State Function
	UFUNCTION()
	void SleepState();

	UFUNCTION()
	void IdleState(float DeltaTime);

	UFUNCTION()
	void MoveState(float DeltaTime);
#pragma endregion

	UPROPERTY(EditAnywhere)
	DragonState State = DragonState::Sleep; //Default State를 Idle로 설정

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	class UHD_DragonAnim* Anim;

	// 가까운 플레이어
	UPROPERTY(EditAnywhere)
	class AActor* NearTargetActor;

	// 내 위치
	UPROPERTY(EditAnywhere)
	class AActor* DragonActor;

	UPROPERTY(EditAnywhere)
	class AHD_Dragon* Dragon;

	// 타겟과 거리
	FVector dir;

	// 타겟과 거리 내적값 추출
	UFUNCTION()
	double GetRadianFromCharacter();

	UFUNCTION()
	bool ChkCharacterIntoRadian();

#pragma region Idle Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ThresholdRadian = 2500;

	UPROPERTY()
	float ShoutAnimCurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayShoutAnimTime = 5.f;
#pragma endregion

#pragma region Move Function
	EPathFollowingRequestResult::Type MoveToLocation(FVector targetLoc);

	UPROPERTY()
	bool bFly = false;
#pragma endregion

#pragma region Attack
	UPROPERTY()
	int32 PatternPageNum = 1;

	TArray<NormalAttackState> NormalAttackPattern1Page = {
		NormalAttackState::Breath, NormalAttackState::ThunderMagic, NormalAttackState::HandPress
	};

	TArray<AirAttackState> AirAttackPattern;
#pragma endregion

	// UFUNCTION()
	// void changeState(DragonState NextState);
};
