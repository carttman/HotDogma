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
	NormalAttack, //일반공격
	FlyAttack, //공중공격
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
	JumpPress // 공중찍기
};

UENUM()
enum class FlyAttackState:uint8
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

	UFUNCTION()
	void F_NormalAttackState(float DeltaTime);
#pragma endregion

#pragma region Normal Attack Function

#pragma region FlyPress
	UPROPERTY()
	bool bFly = false;
	
	UFUNCTION()
	void FlyPress(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlyPressCollTime = 10.f; // 공중찍기 스킬 쿨타임

	UPROPERTY()
	bool bUseFlyPress = true; // 공중찍기 스킬 사용 여부

	UPROPERTY()
	bool bStartFlyPress = false; // 공중찍기 시작

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlyPressHeight = 500; // 공중찍기 높이

	UPROPERTY()
	float x;
	UPROPERTY()
	float y;
	UPROPERTY()
	float z;
	UPROPERTY()
	float FallSpeed = 0.f; // 초기 낙하 속도
#pragma endregion

	UFUNCTION()
	void NormalBreath(float DeltaTime);

#pragma endregion

	UPROPERTY(EditAnywhere)
	DragonState State = DragonState::Sleep; //Default State를 Idle로 설정

	UPROPERTY(EditAnywhere)
	NormalAttackState normalAttackState;

	UPROPERTY(EditAnywhere)
	FlyAttackState flyAttackState;

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
	float GetRadianFromCharacter();

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

#pragma region Attack
	UPROPERTY()
	int32 PatternPageNum = 1;

	TArray<NormalAttackState> NormalAttackPattern1Page = {
		NormalAttackState::Breath, NormalAttackState::ThunderMagic, NormalAttackState::HandPress
	};

	TArray<FlyAttackState> AirAttackPattern;
#pragma endregion

	// UFUNCTION()
	// void changeState(DragonState NextState);
};
