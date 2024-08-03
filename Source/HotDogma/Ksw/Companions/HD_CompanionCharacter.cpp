// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_CompanionCharacter.h"
#include "HotDogma/Ksw/CompanionComponents/HD_CompanionStateComponent.h"
#include "Components/ArrowComponent.h"
#include "../CompanionComponents/HD_WarriorStateComponent.h"
#include "../CompanionComponents/HD_SorcererStateComponent.h"
#include "HotDogma/HD_Character/HD_PlayerComponent/PlayerStatusComponent.h"

// Sets default values
AHD_CompanionCharacter::AHD_CompanionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerStatusComp = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatusComp"));

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

float AHD_CompanionCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("Companion Take Damage : %f"), Damage);
	
	return Damage;
}

class UHD_CompanionStateComponent* AHD_CompanionCharacter::SetupCompanionStateComp(bool isWarrior)
{
	if (isWarrior)
	{
		CompanionStateComp = NewObject<UHD_WarriorStateComponent>(this, UHD_WarriorStateComponent::StaticClass(), TEXT("CompanionStateComp"));
		CompanionStateComp->RegisterComponent();
		// CompanionStateComp->AttachTo(RootComponent);
	}
	else
	{
		CompanionStateComp = NewObject<UHD_SorcererStateComponent>(this, UHD_SorcererStateComponent::StaticClass(), TEXT("CompanionStateComp"));
		CompanionStateComp->RegisterComponent();
		//CompanionStateComp->AttachTo(RootComponent);
	}

	return CompanionStateComp;
}
