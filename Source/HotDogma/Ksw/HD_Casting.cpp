// Fill out your copyright notice in the Description page of Project Settings.


#include "HotDogma/Ksw/HD_Casting.h"

// Sets default values
AHD_Casting::AHD_Casting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHD_Casting::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHD_Casting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHD_Casting::SetCastingColor(FLinearColor color)
{
	CastingColor = color;
}
