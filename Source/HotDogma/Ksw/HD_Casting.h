// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HD_Casting.generated.h"

UCLASS()
class HOTDOGMA_API AHD_Casting : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHD_Casting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// CastingColor 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting")
	FLinearColor CastingColor = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);

	void SetCastingColor(FLinearColor color);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCastingStart(float CastingTime);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCastingCancel();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCastingStop();
};
