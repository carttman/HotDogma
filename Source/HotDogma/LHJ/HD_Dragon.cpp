// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_Dragon.h"
#include "HD_DragonFSM.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AHD_Dragon::AHD_Dragon()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));
	SkeletalComp->SetupAttachment(RootComponent);
	SkeletalComp->SetGenerateOverlapEvents(true);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeleton(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/LHJ/UnkaDragon/Meshes/UnkaDragon/SK_Unka_Realistic.SK_Unka_Realistic'"));
	if (tempSkeleton.Succeeded())
	{
		SkeletalComp->SetSkeletalMesh(tempSkeleton.Object);
	}

	fsm = CreateDefaultSubobject<UHD_DragonFSM>(TEXT("FSM"));
	//fsm->RegisterComponent();
}

void AHD_Dragon::BeginPlay()
{
	Super::BeginPlay();
}

void AHD_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어&동료 찾기
	if (CharacterArr.Num() == 0)
	{
		TArray<AActor*> OutActors;
		// 필드에 있는 캐릭터를 인지한다.
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACharacter::StaticClass(),
		                                             TEXT("CompanionCharacter"), OutActors);

		TArray<AActor*> OutCharacterActor;
		// 필드에 있는 캐릭터를 인지한다.
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACharacter::StaticClass(),
		                                             TEXT("PlayerCharacter"), OutCharacterActor);

		if (OutActors.Num() > 0)
		{
			for (auto othActor : OutActors)
			{
				ACharacter* charac = Cast<ACharacter>(othActor);
				if (charac)
					CharacterArr.Add(charac);
			}
		}

		if (OutCharacterActor.Num() > 0)
		{
			for (auto othActor : OutCharacterActor)
			{
				ACharacter* charac = Cast<ACharacter>(othActor);
				if (charac)
					CharacterArr.Add(charac);
			}
		}
	}
}

float AHD_Dragon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("%s Takes Damage : %f"), *GetName(), damage);
	CurrHP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("%s Takes Damage : %f"), *GetName(), CurrHP);
   
	return damage;
}
