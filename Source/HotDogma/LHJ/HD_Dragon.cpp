// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_Dragon.h"

#include "HD_DragonFSM.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AHD_Dragon::AHD_Dragon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));
	SkeletalComp->SetupAttachment(CapsuleComp);
	SkeletalComp->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeleton(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/LHJ/QuadrapedCreatures/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
	if (tempSkeleton.Succeeded())
	{
		SkeletalComp->SetSkeletalMesh(tempSkeleton.Object);
	}

	fsm = CreateDefaultSubobject<UHD_DragonFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AHD_Dragon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHD_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
