// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <random>
#include <vector>
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
	bool RotateToTarget(const float& DeltaTime);
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
	float x = 0.f;
	UPROPERTY()
	float y = 0.f;
	UPROPERTY()
	float z = 0.f;
	UPROPERTY()
	float FallSpeed = 0.f; // 초기 낙하 속도
#pragma endregion

	UFUNCTION()
	void NormalBreath(const float& DeltaTime);

#pragma endregion

#pragma region Attack Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDist = 2000.f; // 공격범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandPressAttackDist = 2000.f; // 앞발찍기 공격범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HandPressCameraDist = 3000.f; // 앞발찍기 카메라쉐이크

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpPressAttackDist = 2000.f; // 점프찍기 공격범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpPressCameraDist = 3000.f; // 점프찍기 카메라쉐이크
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

	float GetDegreeFromCharacter();

#pragma region Idle Property
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ThresholdRadian = 6000;

	UPROPERTY()
	float ShoutAnimCurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayShoutAnimTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DuringIdleTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrIdleTime;

	UFUNCTION()
	void F_NormalIdle(const float& DeltaTime);
#pragma endregion

#pragma region Attack
	UPROPERTY()
	int32 PatternPageNum = 1;

	std::vector<AttackState> OrgAttackPattern = {
		AttackState::Breath,
		AttackState::Shout,
		AttackState::HandPress,
		AttackState::Scratch,
		AttackState::TailSlap,
		AttackState::JumpPress,
		AttackState::ThunderMagic,
		AttackState::Meteor,
	};

	std::vector<AttackState> RndAttackPattern;

	UFUNCTION()
	void ShuffleAttackPattern();

	std::vector<AttackState> OrgFlyAttackPattern = {
		AttackState::Breath,
		AttackState::ThunderMagic,
		AttackState::Meteor
	};

	std::vector<AttackState> RndFlyAttackPattern;

	UFUNCTION()
	void ShuffleFlyAttackPattern();

	UPROPERTY()
	bool isAttack = false;

	int int_rand = 0;

	void ChooseAttackState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NowUsedSkillCnt = 0; // 지상에서 사용한 스킬 개수(올라갈때 초기화)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredSkillCnt = 4; // 다음 공중 올라갈 때까지 필요한 스킬 사용 개수

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrUsedSkillCnt; // 현재 사용한 스킬 개수

	FRotator NowRotator;
#pragma endregion

#pragma region	FlyProperty
	bool chkOnceFly = false; // 한번이라도 날았는지 확인	

	int ApplySkillAsFly = 0; // 공중에서 사용할 스킬 개수
#pragma endregion

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Breath")
	class UCurveFloat* BreathCurve;

	UPROPERTY()
	FTimeline BreathTimeline;

	UFUNCTION()
	void BreathRStart(const float& Alpha);

	UFUNCTION()
	void BreathREnd();

	bool bBreathAttack = false;

	UFUNCTION()
	void ProjectileBreathCollision(const float& DeltaTime);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_BreathCol> Breath_Projectile;

	//=================================데미지 값
#pragma region 데미지 값
	UPROPERTY(EditAnywhere)
	float Damage_JumpPress = 10.f; // 점프찍기

	UPROPERTY(EditAnywhere)
	float Damage_HandPress = 10.f; // 짓누르기

	UPROPERTY(EditAnywhere)
	float Damage_TailSlap = 10.f; // 꼬리치기

	UPROPERTY(EditAnywhere)
	float Damage_Scratch = 10.f; // 할퀴기

	UPROPERTY(EditAnywhere)
	float Damage_Meteor = 10.f; // 메테오

	UPROPERTY(EditAnywhere)
	float Damage_Thunder = 10.f; // 번개

	UPROPERTY(EditAnywhere)
	float Damage_Shout = 0.1f; // 포효

	UPROPERTY(EditAnywhere)
	float Damage_Breath = 10.f; // 브레스
#pragma endregion

#pragma region 번개공격
	UFUNCTION()
	void F_ThunderMagic(const float& DeltaTime);

	std::vector<FVector> ThunderPatern;
	std::vector<FVector> CastingAttack_CharacterLoc;
	void F_GetCharacterLoc_Casting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 iCastingCnt = 0;

	bool bStartThunder = false;

	int int_rand_Thunder = 0;

	UPROPERTY(EditAnywhere)
	float MakeThunderTime = 1.f;

	UPROPERTY()
	float CurrThunderTime = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_DragonThunderCol> ThunderCol;

#pragma endregion

#pragma region 메테오
	bool bStartMeteor = false;

	UPROPERTY(EditAnywhere)
	float MakeMeteorTime = 1.f;

	UPROPERTY()
	float CurrMeteorTime = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_Meteor> Meteor_Projectile;

	UFUNCTION()
	void F_MeteorMagic(const float& DeltaTime);

	FVector F_GetSpawnMeteorLoc();
#pragma endregion

	UPROPERTY(EditAnywhere)
	class ADirectionalLight* DirectionalLight; // DirectionalLight

	FLinearColor OldColor;
	FLinearColor BreathColor = {1.f, 0.208637f, 0.198069f, 1.f};

	void SetFirelLight();
	void SetNormalLight();

	bool bReturnLightColor = false;

	float LightColorAlpha=0.f;
};
