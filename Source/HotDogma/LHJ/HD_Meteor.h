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

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
	
	UPROPERTY()
	class AHD_Dragon* Dragon;

	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 4500;

	float AttackDist = 300;
	float CameraShakeDist = 1000;
	UPROPERTY()
	TSet<AActor*> DamageActorSet;
	
	void SetTarget(FVector target);	

	bool GetAttackMeteor(const float& AttackDistance);

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* MeteorNia;
	UPROPERTY( EditAnywhere )
	class USoundBase* MeteorSound;
};
