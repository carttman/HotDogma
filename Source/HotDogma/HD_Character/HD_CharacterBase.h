// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "HD_CharacterBase.generated.h"

UCLASS()
class HOTDOGMA_API AHD_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHD_CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArm;
	
	//Input for Gameplay
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_HDMapping;
	
	//player input
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DH_Move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DH_Look;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DH_Jump;
	// UPROPERTY(EditAnywhere)
	// class UInputAction* IA_HD_Skill;
	
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DH_Order;
	
	UPROPERTY(EditAnywhere)
	class UHD_PlayerClimbComponent* PlayerClimbComponent;
	UPROPERTY(EditAnywhere)
	class UPlayerStatusComponent* PlayerStatusComponent;
	UPROPERTY(EditAnywhere)
	class UMotionWarpingComponent* MotionWarpingComponent;
public:
	FVector2D MovementVector;
	float Climb_UpDown;
	float Climb_LeftRight;
public:
	void EnhancedMove(const FInputActionValue& InputActionValue);
	void EnhancedJump(const FInputActionValue& InputActionValue);
	void EnhancedLook(const FInputActionValue& InputActionValue);
	void EnhancedOrder(const FInputActionValue& InputActionValue);
public:
	UPROPERTY(EditAnywhere)
	class UCableComponent* CableCompoent;

	UPROPERTY()
	APawn* Dragon;

	UPROPERTY()
	class ACHJ_GameMode* PlayerGameMode;
};
