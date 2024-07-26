// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HD_PlayerClimbComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOTDOGMA_API UHD_PlayerClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHD_PlayerClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Player_Climb;

	class AHD_CharacterBase* Player;
public:
	void Climb();
	void TraceMovement();
	UFUNCTION(BlueprintCallable)
	FVector GetMovementDirection();

	FHitResult Hit_F1;
	FHitResult Hit_F2;
};
