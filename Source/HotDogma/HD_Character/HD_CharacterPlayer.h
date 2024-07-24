// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HD_CharacterBase.h"
#include "HD_CharacterPlayer.generated.h"

UCLASS()
class HOTDOGMA_API AHD_CharacterPlayer : public AHD_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHD_CharacterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:
	//camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArm;
};
