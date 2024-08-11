// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HD_CompanionCharacter.generated.h"

UCLASS()
class HOTDOGMA_API AHD_CompanionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHD_CompanionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void ToggleHandIK(bool enable);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Companion")
	class UHD_CompanionStateComponent* CompanionStateComp;

	class UHD_CompanionStateComponent* SetupCompanionStateComp(bool isWarrior);
	
	UPROPERTY(EditAnywhere, Category = "Companion")
	class UArrowComponent* ArrowComp;

	UPROPERTY(EditAnywhere, Category = "Companion")
	class UPlayerStatusComponent* PlayerStatusComp;

	UPROPERTY(EditAnywhere, Category = "Companion")
	class UWidgetComponent* HPComp;
	
	UPROPERTY()
	class UHD_CompanionWidget* Widget;

	// 문자열
	UPROPERTY(EditAnywhere, Category = "Companion")
	FString CompanionName;

	// 이미지
	UPROPERTY(EditAnywhere, Category = "Companion")
	class UTexture2D* CompanionImage;

	UPROPERTY(EditAnywhere, Category = "Companion")
	TSubclassOf<class AHD_CompanionWeapon> CompanionWeaponFactory;

	UPROPERTY()
	class AHD_CompanionWeapon* CompanionWeapon;

	UPROPERTY(EditAnywhere, Category = "Companion")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Companion")
	class UBoxComponent* BoxComp;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsDeath;

	// 사운드
	UPROPERTY(EditAnywhere, Category = "Companion")
	class USoundBase* HighfiveSound;
};
