// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_BreathCol.h"

#include "HD_Dragon.h"
#include "HD_DragonAnim.h"
#include "HD_DragonFSM.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHD_BreathCol::AHD_BreathCol()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("DragonAttack"));
	collisionComp->SetSphereRadius(80.0);
	RootComponent = collisionComp;
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AHD_BreathCol::OnOverlapBegin);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->SetUpdatedComponent(RootComponent);
	ProjectileComp->InitialSpeed = 500;
	ProjectileComp->MaxSpeed = speed;
	ProjectileComp->bShouldBounce = false;
	ProjectileComp->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AHD_BreathCol::BeginPlay()
{
	Super::BeginPlay();

	Dragon = Cast<AHD_Dragon>(UGameplayStatics::GetActorOfClass(GetWorld(), AHD_Dragon::StaticClass()));
	if(Dragon)
		Anim = Cast<UHD_DragonAnim>(Dragon->SkeletalComp->GetAnimInstance());
}

// Called every frame
void AHD_BreathCol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHD_BreathCol::SetTarget(FTransform target)
{
	SetLifeSpan(.225f);
	FRotator FireSocketRotation = target.GetRotation().Rotator();
	if (Anim)
	{
		if (Anim->isFly)
		{
			
			FireSocketRotation.Pitch -= 40.f;
		}
		else
		{
			
			FireSocketRotation.Pitch += 20.f;
		}
	}
	SetActorRotation(FireSocketRotation);
	ProjectileComp->Velocity = FireSocketRotation.Vector() * speed;
}

void AHD_BreathCol::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ACharacter>())
	{
		//ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);
		Dragon->strDamageAttackType = "Breath";
		UGameplayStatics::ApplyDamage(OtherActor, Dragon->fsm->Damage_Breath, GetInstigatorController(), Dragon,
		                              UDamageType::StaticClass());
	}
}
