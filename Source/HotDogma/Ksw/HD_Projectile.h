// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HD_Projectile.generated.h"

UCLASS()
class HOTDOGMA_API AHD_Projectile : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AHD_Projectile();

public:
	//발사체 이동을 담당할 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* movementComp;
	//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
	//외관 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;

	//발사체 이펙트
	UPROPERTY(EditAnywhere, Category = Effect)
	class UParticleSystem* trailEffect;

	// 터질때 이펙트
	UPROPERTY(EditAnywhere, Category = Effect)
	class UParticleSystem* explosionEffect;

	void Die();

	void SetTarget(FVector target);

	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 5000;

	// Projectile 소유자
	enum class EOwnerType
	{
		Player,
		Enemy,
		Ally
	};

	// Projectile 소유자 타입
	EOwnerType OwnerType;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
