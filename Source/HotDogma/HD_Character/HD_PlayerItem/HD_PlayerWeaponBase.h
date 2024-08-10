// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HD_PlayerWeaponBase.generated.h"

UCLASS()
class HOTDOGMA_API AHD_PlayerWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHD_PlayerWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* WeaponMeshComp;

	bool IsWeaponHit = false;
public:
	UPROPERTY()
	class AHD_CharacterPlayer* Player;
	UPROPERTY()
	class UHD_PlayerAnimInstance* PlayerAnim;

public:
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* BloodVFX;
	UPROPERTY(EditAnywhere)
	class USoundBase* Hit_SFX;
};
