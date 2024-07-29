// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HD_CompanionCharacter.generated.h"

// Enum���� ���� ����
UENUM(BlueprintType)
enum class ECompanionState : uint8
{
	State_Wait UMETA(DisplayName = "Wait"),
	State_Run UMETA(DisplayName = "Run"),
	State_Battle UMETA(DisplayName = "Battle"),
	State_Help UMETA(DisplayName = "Help"),
};

UCLASS()
class HOTDOGMA_API AHD_CompanionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHD_CompanionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// boids
	// separation : �ٸ� ��ü����� �Ÿ��� ����
	FVector Separation(const TArray<AHD_CompanionCharacter*>& Boids);
	// alignment : �ٸ� ��ü����� ������ ��ġ
	FVector Alignment(const TArray<AHD_CompanionCharacter*>& Boids);
	// cohesion : �ٸ� ��ü����� �Ÿ��� �����ϸ鼭 �׷��� �߽��� ����
	FVector Cohesion(const TArray<AHD_CompanionCharacter*>& Boids);
	// follow player : �÷��̾ ����
	FVector FollowPlayer(const FVector& PlayerLocation);

	void Flocking(const TArray<AHD_CompanionCharacter*>& Boids, const FVector& PlayerLocation, float SeparationValue, float AlignmentValue, float CohesionValue);
	
	void SetMovePoint(const FVector& chacterPos, const FVector& Point);
	void StopMove();
	void DoHelp();

	void SetState(ECompanionState State);

	FVector Acc;

	FVector MovePoint;
	FVector CharcterPoint;
	ECompanionState CurrentState;
	
	UPROPERTY()
	class AAIController* AIController;

	UPROPERTY()
	class APawn* TargetPawn;
};
