// Fill out your copyright notice in the Description page of Project Settings.


#include "HotDogma/Ksw/HD_CompanionWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <HotDogma/LHJ/HD_Dragon.h>

// Sets default values
AHD_CompanionWeapon::AHD_CompanionWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComp"));
	SetRootComponent(WeaponMeshComp);
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(WeaponMeshComp);
}

// Called when the game starts or when spawned
void AHD_CompanionWeapon::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AHD_CompanionWeapon::OnOverlapBegin);
	OnCollision(false);
}

// Called every frame
void AHD_CompanionWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHD_CompanionWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AHD_Dragon>())
	{
		UGameplayStatics::ApplyDamage(OtherActor, AttackDamage, GetInstigatorController(), this, UDamageType::StaticClass());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, GetActorLocation(), GetActorRotation());

		// 로그 추가
		UE_LOG(LogTemp, Warning, TEXT("CompanionWeapon Overlap Begin"));

		OnCollision(false);
	}
}

void AHD_CompanionWeapon::SetDamage(float Damage)
{
	this->AttackDamage = Damage;
}

void AHD_CompanionWeapon::OnCollision(bool bCollision)
{
	if (bCollision)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

