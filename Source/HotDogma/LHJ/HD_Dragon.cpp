// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_Dragon.h"

#include "EngineUtils.h"
#include "HD_DragonFSM.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AHD_Dragon::AHD_Dragon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	// SetRootComponent(CapsuleComp);
	// CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));
	SetRootComponent(SkeletalComp);
	SkeletalComp->SetGenerateOverlapEvents(true);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeleton(TEXT(
		"/Script/Engine.PhysicsAsset'/Game/LHJ/UnkaDragon/Meshes/UnkaDragon/SK_Unka_Realistic_TEST_PhysicsAsset.SK_Unka_Realistic_TEST_PhysicsAsset'"));
	//(Pitch=0.000000,Yaw=-90.000000,Roll=0.000000)
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

	// 필드에 있는 캐릭터를 인지한다.
	for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
	{
		ACharacter* Character = *It;
		if (Character)
		{
			if (!Character->GetActorLabel().Contains("Dragon"))
				CharacterArr.Add(Character);
		}
	}
}

// Called every frame
void AHD_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
