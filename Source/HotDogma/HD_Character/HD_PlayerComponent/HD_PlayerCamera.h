// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "HD_PlayerCamera.generated.h"

/**
 * 
 */
UCLASS()
class HOTDOGMA_API AHD_PlayerCamera : public ACameraActor
{
	GENERATED_BODY()
	
	AHD_PlayerCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class AHD_CharacterPlayer* Player;

	// Function to auto activate the camera for a player
	void AutoActivateForPlayer(APlayerController* PlayerController);

	float CameraSpeed = 300;
};
