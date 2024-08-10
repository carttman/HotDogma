// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HotDogma/Ksw/Companions/HD_CompanionManager.h"
#include "HD_CompanionStateComponent.generated.h"

// Enum���� ���� ����
UENUM(BlueprintType)
enum class ECompanionState : uint8
{
	State_Wait UMETA(DisplayName = "Wait"),
	State_Run UMETA(DisplayName = "Run"),
	State_Battle UMETA(DisplayName = "Battle"),
	State_Help UMETA(DisplayName = "Help"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOTDOGMA_API UHD_CompanionStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHD_CompanionStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void WaitTick(float DeltaTime);
	void RunTick(float DeltaTime);
	void BattleTick(float DeltaTime);
	void HelpTick(float DeltaTime);
	void SetCommand(ECompanionCommand Command){ CurrentCommand = Command; }

	virtual void AttackTick(float DeltaTime);
	virtual void StartBattle();
	virtual void EndBattle();

	// boids
	// separation : �ٸ� ��ü����� �Ÿ��� ����
	FVector Separation(const TArray<UHD_CompanionStateComponent*>& Boids);
	// alignment : �ٸ� ��ü����� ������ ��ġ
	FVector Alignment(const TArray<UHD_CompanionStateComponent*>& Boids);
	// cohesion : �ٸ� ��ü����� �Ÿ��� �����ϸ鼭 �׷��� �߽��� ����
	FVector Cohesion(const TArray<UHD_CompanionStateComponent*>& Boids);
	// follow player : �÷��̾ ����
	FVector FollowPlayer(const FVector& PlayerLocation);

	void Flocking(const TArray<UHD_CompanionStateComponent*>& Boids, const FVector& PlayerLocation, float SeparationValue, float AlignmentValue, float CohesionValue);

	void SetMovePoint(const FVector& chacterPos, const FVector& Point);
	void StopMove();
	void DoHelp();

	void RotateToTarget(float DeltaTime, FVector Target);
	void SetState(ECompanionState State);

	FVector MovePoint;
	FVector CharcterPoint;
	ECompanionState CurrentState;
	ECompanionCommand CurrentCommand;
	
	UPROPERTY()
	class AHD_CompanionManager* CompanionManager;

	UPROPERTY()
	class AAIController* AIController;
	
	UPROPERTY()
	class AHD_CompanionCharacter* Me;

	UPROPERTY()
	class APawn* TargetPawn;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	FVector Acc;

	bool bCasting = false;
};
