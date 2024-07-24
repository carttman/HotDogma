// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonFSM.h"

#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHD_DragonFSM::UHD_DragonFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHD_DragonFSM::BeginPlay()
{
	Super::BeginPlay();

	NearTarget = UGameplayStatics::GetActorOfClass(GetWorld(), AHD_CharacterPlayer::StaticClass());
	if(NearTarget==nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NearTarget Is NullPtr"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NearTarget Is Not NullPtr"));
}


// Called every frame
void UHD_DragonFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

