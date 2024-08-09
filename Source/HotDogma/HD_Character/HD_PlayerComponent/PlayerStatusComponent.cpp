// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/PlayerStatusComponent.h"

#include "HotDogma/HD_Character/HD_CharacterBase.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/UI/HD_PlayerWidget.h"

// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	CurrHP = MaxHP;
}


// Called every frame
void UPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}
