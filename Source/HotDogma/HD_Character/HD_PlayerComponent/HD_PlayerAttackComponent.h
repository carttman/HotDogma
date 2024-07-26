// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HD_PlayerAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOTDOGMA_API UHD_PlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHD_PlayerAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);

public:
	void PlayerAttack();
	void UpdatePlayerAttack(float DeltaTime);
public:
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_HD_Attack;
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_BaseAttack;

public:
	UPROPERTY()
	class AHD_CharacterBase* Player;

	int32 ComboCount = 0;

	float CurrComboTime = 0;
	float MinComboTime = 0.5f;
	float MaxComboTime = 1.4f;
};
