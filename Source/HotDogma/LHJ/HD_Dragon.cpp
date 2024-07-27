// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_Dragon.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "HD_DragonFSM.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"

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
		"/Script/Engine.SkeletalMesh'/Game/LHJ/UnkaDragon/Meshes/UnkaDragon/SK_Unka_Realistic.SK_Unka_Realistic'"));
	//(Pitch=0.000000,Yaw=-90.000000,Roll=0.000000)
	if (tempSkeleton.Succeeded())
	{
		SkeletalComp->SetSkeletalMesh(tempSkeleton.Object);
	}
	
	// FloatingPawnMovement 컴포넌트 생성 및 추가
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 800.f;

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

	AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), GetActorLocation(), GetActorRotation());
		AIController->Possess(this);
	}
}

// Called every frame
void AHD_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
