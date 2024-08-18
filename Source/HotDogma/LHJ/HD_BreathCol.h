// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HD_BreathCol.generated.h"

UCLASS()
class HOTDOGMA_API AHD_BreathCol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHD_BreathCol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;

	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 4900;

	void SetTarget(FTransform target);
	
	UPROPERTY()
	class AHD_Dragon* Dragon;
	UPROPERTY()
	class UHD_DragonAnim* Anim;
};
