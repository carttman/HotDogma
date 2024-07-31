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
	TArray<class ACharacter*> CharacterArr;	// 공격 대상 인지

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrHP = MaxHP;
	
};
