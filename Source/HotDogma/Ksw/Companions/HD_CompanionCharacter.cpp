// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_CompanionCharacter.h"
#include "HotDogma/Ksw/CompanionComponents/HD_CompanionStateComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AHD_CompanionCharacter::AHD_CompanionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CompanionStateComp = CreateDefaultSubobject<UHD_CompanionStateComponent>(TEXT("CompanionStateComp"));
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHD_CompanionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHD_CompanionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHD_CompanionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
