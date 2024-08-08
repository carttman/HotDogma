// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_Meteor.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHD_Meteor::AHD_Meteor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("DragonAttack"));
	collisionComp->SetSphereRadius(55);
	RootComponent = collisionComp;
	collisionComp->OnComponentHit.AddDynamic(this, &AHD_Meteor::OnHit);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->SetUpdatedComponent(RootComponent);
	ProjectileComp->InitialSpeed = 500;
	ProjectileComp->MaxSpeed = speed;
	ProjectileComp->bShouldBounce = false;
	ProjectileComp->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AHD_Meteor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHD_Meteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHD_Meteor::SetTarget(FVector target)
{
	FVector dir = target - GetActorLocation();
	dir.Normalize();
	SetActorRotation(dir.Rotation());
	ProjectileComp->Velocity = dir * speed;
}

void AHD_Meteor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       FVector NormalImpulse, const FHitResult& Hit)
{
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());

	UGameplayStatics::ApplyDamage(OtherActor, 1, GetInstigatorController(), this, UDamageType::StaticClass());
	Destroy();
}
