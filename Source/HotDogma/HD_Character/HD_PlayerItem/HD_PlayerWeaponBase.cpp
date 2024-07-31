// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerItem/HD_PlayerWeaponBase.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerAnimInstance.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "Kismet/GameplayStatics.h"

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
	WeaponMeshComp->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
}

// Called when the game starts or when spawned
void AHD_PlayerWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AHD_PlayerWeaponBase::OnOverlapBegin);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Player = Cast<AHD_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerAnim = Cast<UHD_PlayerAnimInstance>(Player->GetMesh()->GetAnimInstance());
	if(Player) UE_LOG(LogTemp, Warning, TEXT("Weapon : Player cast successful"))
	else UE_LOG(LogTemp, Warning, TEXT(" Weapon : Player cast failed"));
	
}

// Called every frame
void AHD_PlayerWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHD_PlayerWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == Player->Dragon)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Dragon"));
		FDamageEvent DamageEvent;
		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, Player->GetController(), this, UDamageType::StaticClass());
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

