// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "HD_DragonFSM.generated.h"

UENUM()
enum class DragonState:uint8
{
	Sleep, // 초기 상태
	Shout, // 포효
	Idle, // 대기
	Move, // 걷기
	Fly, // 이륙
	FlyDown, // 착륙
	Attack, //일반공격
	Groggy, // 그로기
	Death, // 사망
};

UENUM()
enum class AttackState:uint8
{
	Breath, // 브레스
	Shout, // 포효
	HandPress, // 손바닥 내려치기
	Scratch, // 할퀴기	
	TailSlap, // 꼬리치기
	ThunderMagic, // 전기마법공격
	Meteor, // 메테오
	JumpPress, // 공중찍기
	//FlyBreath, // 공중 브레스
	None, // 빈값
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

	UPROPERTY()
	class AAIController* ai;

#pragma region State Function
	UFUNCTION()
	void SleepState();

	UFUNCTION()
	void IdleState(const float& DeltaTime);

	UFUNCTION()
	void MoveState(const float& DeltaTime);

	UFUNCTION()
	void F_NormalAttackState(const float& DeltaTime);

	UPROPERTY()
	float al = 0;

	UPROPERTY()
	bool bRotate = false;

	UFUNCTION()
	void RotateToTarget(const float& DeltaTime);
#pragma endregion

#pragma region Attack Function

#pragma region FlyPress
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

#pragma region Attack Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDist = 2000.f; // 공격범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandPressAttackDist = 2000.f; // 앞발찍기 공격범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpPressAttackDist = 2000.f; // 점프찍기 공격범위
#pragma endregion

	UPROPERTY(EditAnywhere)
	DragonState State = DragonState::Sleep; //Default State를 Idle로 설정

	UPROPERTY(EditAnywhere)
	AttackState normalAttackState = AttackState::None;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	class UHD_DragonAnim* Anim;

	// 가까운 플레이어
	UPROPERTY(EditAnywhere)
	class ACharacter* NearTargetActor;

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

	bool chkCharacterUsingSleep = false;

	UFUNCTION()
	bool ChkCharacterIntoRadian();

#pragma region Idle Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ThresholdRadian = 6000;

	UPROPERTY()
	float ShoutAnimCurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayShoutAnimTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DuringIdleTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrIdleTime;
#pragma endregion

#pragma region Attack
	UPROPERTY()
	int32 PatternPageNum = 1;

	TArray<AttackState> AttackPattern1Page = {
		AttackState::Breath, AttackState::ThunderMagic, AttackState::HandPress
	};

	UPROPERTY()
	bool isAttack = false;

	int int_rand = 0;

	void ChooseAttackState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NowUsedSkillCnt = 0; // 지상에서 사용한 스킬 개수(올라갈때 초기화)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredSkillCnt = 4; // 다음 공중 올라갈 때까지 필요한 스킬 사용 개수

	int CurrUsedSkillCnt;	// 현재 사용한 스킬 개수

	FRotator NowRotator;
#pragma endregion

#pragma region	Fly Property
	bool chkOnceFly = false; // 한번이라도 날았는지 확인	
	
	int ApplySkillAsFly; // 공중에서 사용할 스킬 개수
#pragma endregion

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Breath")
	class UCurveFloat* BreathCurve;

	UPROPERTY()
	FTimeline BreathTimeline;

	UFUNCTION()
	void BreathRStart(float Alpha);

	UFUNCTION()
	void BreathREnd();
};
