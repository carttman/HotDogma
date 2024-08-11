// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HD_Meteor.generated.h"

UCLASS()
class HOTDOGMA_API AHD_Meteor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHD_Meteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileComp;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;

	void SetTarget(FVector target);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	bool GetAttackMeteor(const float& AttackDistance);

	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 5000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	class AHD_Dragon* Dragon;

	float AttackDist = 300;

	float CameraShakeDist = 1000;

	UPROPERTY()
	TSet<AActor*> DamageActorSet;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* MeteorNia;

	UPROPERTY( EditAnywhere )
	class USoundBase* MeteorSound;
};
