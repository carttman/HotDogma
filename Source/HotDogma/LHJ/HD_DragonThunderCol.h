// Fill out your copyright notice in the Description page of Project Settings.

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

};
