// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_Dragon.h"

#include "HD_DragonFSM.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	SkeletalComp->SetCollisionProfileName(FName("DragonMeshColl"));
	SkeletalComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	fsm = CreateDefaultSubobject<UHD_DragonFSM>(TEXT("FSM"));
	//RootComponent->RegisterComponent();

	TargetPoint1 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint1"));
	TargetPoint1->SetupAttachment(SkeletalComp,TEXT("L-HorseLink"));
	TargetPoint1->SetCapsuleHalfHeight(60.f);
	TargetPoint1->SetCapsuleRadius(30.f);
	TargetPoint1->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetPoint2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint2"));
	TargetPoint2->SetupAttachment(SkeletalComp,TEXT("R-HorseLink"));
	TargetPoint2->SetCapsuleHalfHeight(60.f);
	TargetPoint2->SetCapsuleRadius(30.f);
	TargetPoint2->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetPoint3 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint3"));
	TargetPoint3->SetupAttachment(SkeletalComp,TEXT("R-Forearm"));
	TargetPoint3->SetCapsuleHalfHeight(60.f);
	TargetPoint3->SetCapsuleRadius(30.f);
	TargetPoint3->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetPoint4 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint4"));
	TargetPoint4->SetupAttachment(SkeletalComp,TEXT("L-Forearm"));
	TargetPoint4->SetCapsuleHalfHeight(60.f);
	TargetPoint4->SetCapsuleRadius(30.f);
	TargetPoint4->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint4->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Tongue에 SocketComponent 설치
	FireScene = CreateDefaultSubobject<USceneComponent>(TEXT("FireScene"));
	FireScene->SetupAttachment(SkeletalComp, TEXT("Fire_Socket"));

	FireCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FireCollision"));
	FireCollision->SetupAttachment(FireScene);
	FireCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HandCollision_R = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HandCollision_R"));
	HandCollision_R->SetupAttachment(SkeletalComp,TEXT("R-Finger2"));
	HandCollision_R->SetRelativeRotation(FRotator(90, 0, 0));
	HandCollision_R->SetCollisionProfileName(FName("DragonAttack"));
	HandCollision_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision_R->OnComponentBeginOverlap.AddDynamic(this, &AHD_Dragon::OnOverlapBegin);

	HandCollision_L = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HandCollision_L"));
	HandCollision_L->SetupAttachment(SkeletalComp,TEXT("L-Finger2"));
	HandCollision_L->SetRelativeRotation(FRotator(90, 0, 0));
	HandCollision_L->SetCollisionProfileName(FName("DragonAttack"));
	HandCollision_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision_L->OnComponentBeginOverlap.AddDynamic(this, &AHD_Dragon::OnOverlapBegin);

	TailCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TailCollision"));
	TailCollision->SetupAttachment(SkeletalComp,TEXT("Tail05"));
	TailCollision->SetRelativeLocationAndRotation(FVector(43, -15, 0), FRotator(90, 0, 0));
	TailCollision->SetCapsuleHalfHeight(157);
	TailCollision->SetCapsuleRadius(37);
	TailCollision->SetCollisionProfileName(FName("DragonAttack"));
	TailCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TailCollision->OnComponentBeginOverlap.AddDynamic(this, &AHD_Dragon::OnOverlapBegin);
}

void AHD_Dragon::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->GetMaxSpeed() * 0.75;
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
		                                             TEXT("HD_Player"), OutActors);
		//
		// TArray<AActor*> OutCharacterActor;
		// // 필드에 있는 캐릭터를 인지한다.
		// UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACharacter::StaticClass(),
		//                                              TEXT("PlayerCharacter"), OutCharacterActor);

		if (OutActors.Num() > 0)
		{
			for (auto othActor : OutActors)
			{
				ACharacter* charac = Cast<ACharacter>(othActor);
				if (charac)
					CharacterArr.Add(charac);
			}
		}
	}
}

void AHD_Dragon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageActorSet.Contains(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Actor Name : %s"), *OtherActor->GetName());
		DamageActorSet.Add(OtherActor);

		UGameplayStatics::ApplyDamage(OtherActor, fsm->Damage_Scratch, GetController(), this, UDamageType::StaticClass());
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
