// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HD_CompanionCharacter.generated.h"

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


	UPROPERTY(EditAnywhere, Category = "Companion")
	class UHD_CompanionStateComponent* CompanionStateComp;

	class UHD_CompanionStateComponent* GetCompanionStateComp() { return CompanionStateComp; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
	class UArrowComponent* ArrowComp;
};
