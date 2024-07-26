// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerItem/HD_PlayerWeaponBase.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AHD_PlayerWeaponBase::AHD_PlayerWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCollisionProfileName(TEXT("WeaponCapsuleColl"));
	
	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComp"));
	WeaponMeshComp->SetupAttachment(CapsuleComp);
	WeaponMeshComp->SetCollisionProfileName(TEXT("WeaponMeshColl"));

	WeaponMeshComp->SetRelativeLocation(FVector(0, 0, -30));
	WeaponMeshComp->SetRelativeScale3D(FVector(2, 2, 2));
}

// Called when the game starts or when spawned
void AHD_PlayerWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHD_PlayerWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

