#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HD_DragonThunderCol.generated.h"

UCLASS()
class HOTDOGMA_API AHD_DragonThunderCol : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHD_DragonThunderCol();

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

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY()
	class AHD_Dragon* Dragon;

	UPROPERTY(EditAnywhere)
	float CollisionOnTime = 0.25f;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 0.1f;
	float CurrTime = 0.f;
	
	float CameraShakeDist = 1000;

	bool GetCameraShackThunder(const float& AttackDistance);

	UPROPERTY()
	TSet<AActor*> DamageActorSet;
	
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* ThunderSmog;
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* StonVfX;
	UPROPERTY( EditAnywhere )
	class USoundBase* ThunderSound;
};
