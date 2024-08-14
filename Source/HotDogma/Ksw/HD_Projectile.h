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
	//�߻�ü �̵��� ����� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* movementComp;
	//�浹ü ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
	//�ܰ� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;

	//�߻�ü ����Ʈ
	UPROPERTY(EditAnywhere, Category = Effect)
	class UParticleSystem* trailEffect;

	// ������ ����Ʈ
	UPROPERTY(EditAnywhere, Category = Effect)
	class UParticleSystem* explosionEffect;

	void Die();

	void SetTarget(FVector target);

	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 5000;

	// Projectile ������
	enum class EOwnerType
	{
		Player,
		Enemy,
		Ally
	};

	// Projectile ������ Ÿ��
	EOwnerType OwnerType;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
