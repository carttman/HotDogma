// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HD_CompanionManager.generated.h"

// Enum으로 상태 관리
UENUM(BlueprintType)
enum class ECompanionCommand : uint8
{
	Command_Following UMETA(DisplayName = "Following"),
	Command_Go UMETA(DisplayName = "Go"),
	Command_Wait UMETA(DisplayName = "Wait"),
	Command_Help UMETA(DisplayName = "Help"),
};

UCLASS()
class HOTDOGMA_API AHD_CompanionManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHD_CompanionManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TickFollow(float DeltaTime);
	void TickGo(float DeltaTime);
	void TickWait(float DeltaTime);
	void TickHelp(float DeltaTime);

	void MoveBoid(class AHD_CompanionCharacter* Companion, FVector Pos);

	// 블루프린트 호출가능
	UFUNCTION(BlueprintCallable)
	void SetCommand(ECompanionCommand Command);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_CompanionCharacter> SorcererCompanionFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_CompanionCharacter> WarriorCompanionFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_CompanionCharacter> WarriorCompanion2Factory;

	UPROPERTY(EditAnywhere)
	TArray<class AHD_CompanionCharacter*> Companions;

	TArray<FVector> Formations;

	UPROPERTY(EditAnywhere)
	ECompanionCommand CurrentCommand;

	UPROPERTY(EditAnywhere)
	class APawn* PlayerPawn;

	UPROPERTY(EditAnywhere)
	float SeparationValue;

	UPROPERTY(EditAnywhere)
	float AlignmentValue;

	UPROPERTY(EditAnywhere)
	float CohesionValue;

	bool ChangeCommand;
};
