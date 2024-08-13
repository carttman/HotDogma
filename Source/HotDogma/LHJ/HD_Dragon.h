// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HD_Dragon.generated.h"

UCLASS()
class HOTDOGMA_API AHD_Dragon : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHD_Dragon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin_Scratch(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapBegin_TailSlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UPROPERTY()
	class ACHJ_GameMode* gm;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* HandCollision_R;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* HandCollision_L;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* TailCollision;

	// 공격 추적용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint4;

	// 더미 콜리전
	// NoCollision 처리할건데 충돌처리 문제나면 변경하기
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	// 스켈레탈 매쉬
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SkeletalComp;

	UPROPERTY(EditAnywhere)
	class UHD_DragonFSM* fsm;

	UPROPERTY()
	TArray<class ACharacter*> CharacterArr; // 공격 대상 인지

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 3500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrHP;

	UPROPERTY(EditAnywhere)
	class USceneComponent* FireScene;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* FireCollision;

	UPROPERTY(EditAnywhere)
	float MaxFireCollisionSize = 10;

	UPROPERTY()
	float CurrFireCollisionSize = 0;

	UPROPERTY()
	TSet<AActor*> DamageActorSet;

	// ThunderMagic Point
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint8;

	UFUNCTION()
	void CreateThunderPoint();

	UPROPERTY()
	FString strDamageAttackType;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ThunderVFX1;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ThunderVFX2;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* MeteorVFX;

	// 클라이밍용
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbHand_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm1_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm2_L;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbHand_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm1_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm2_R;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbFoot_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg1_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg2_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg3_L;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbFoot_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg1_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg2_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg3_R;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail1;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail2;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail3;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail4;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail5;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbSpine1;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbSpine2;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck1;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck2;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck3;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck4;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck5;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbHead;

	UPROPERTY(EditAnywhere)
	class USceneComponent* MeteorPoint1;
	UPROPERTY(EditAnywhere)
	class USceneComponent* MeteorPoint2;
	UPROPERTY(EditAnywhere)
	class USceneComponent* MeteorPoint3;
	UPROPERTY(EditAnywhere)
	class USceneComponent* MeteorPoint4;

	UFUNCTION()
	void CreateClimbCollision();

	//UI
	int LineCnt = 7; // 피통 줄 수
	float LineHpValue; // 한줄 당 피 값

	UPROPERTY()
	class AHD_CharacterPlayer* Player;

	FTimerHandle DeathTimerHandle;

	void CallCredit();

	bool narTirgger = true;
};
