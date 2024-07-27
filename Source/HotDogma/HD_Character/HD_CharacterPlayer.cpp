// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_CharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "HD_PlayerComponent/HD_PlayerAttackComponent.h"
#include "HD_PlayerComponent/HD_PlayerClimbComponent.h"
#include "HD_PlayerComponent/PlayerStatusComponent.h"
#include "HD_PlayerItem/HD_PlayerWeaponBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHD_CharacterPlayer::AHD_CharacterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Left_Dagger = CreateDefaultSubobject<UChildActorComponent>(TEXT("Left_Dagger"));
	// Left_Dagger->SetupAttachment(GetMesh(), TEXT("middle_01_l"));
	// Left_Dagger->SetRelativeLocation(FVector(3.2,-3, -4.6));
	// Left_Dagger->SetRelativeRotation(FRotator(0,-180,180));
	// Left_Dagger->SetChildActorClass(PlayerWeaponBase);
	//
	// Right_Dagger = CreateDefaultSubobject<UChildActorComponent>(TEXT("Right_Dagger"));
	// Right_Dagger->SetupAttachment(GetMesh(), TEXT("middle_01_r"));
	// Right_Dagger->SetRelativeLocation(FVector(-4,2, 5));
	// Right_Dagger->SetChildActorClass(PlayerWeaponBase);
	Left_WeaponScene = CreateDefaultSubobject<USceneComponent>(TEXT("Left_WeaponScene"));
	Left_WeaponScene->SetupAttachment(GetMesh(), TEXT("middle_01_l"));
	Left_WeaponScene->SetRelativeLocation(FVector(3.2,-3, -4.6));
	Left_WeaponScene->SetRelativeRotation(FRotator(0,-180,180));

	Right_WeaponScene = CreateDefaultSubobject<USceneComponent>(TEXT("Right_WeaponScene"));
	Right_WeaponScene->SetupAttachment(GetMesh(), TEXT("middle_01_r"));
	Right_WeaponScene->SetRelativeLocation(FVector(-4,2, 5));

	// Player 컴포넌트
	PlayerAttackComponent = CreateDefaultSubobject<UHD_PlayerAttackComponent>(TEXT("PlayerAttackComponent"));
	PlayerStatusComponent = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatusComponent"));
	PlayerClimbComponent = CreateDefaultSubobject<UHD_PlayerClimbComponent>(TEXT("PlayerClimbComponent"));
}

// Called when the game starts or when spawned
void AHD_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	AttachWeapon();
}

// Called every frame
void AHD_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHD_CharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		PlayerAttackComponent->SetupPlayerInputComponent(enhancedInputComponent);
		PlayerClimbComponent->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

void AHD_CharacterPlayer::AttachWeapon()
{
	// SpawnActorDeferred : BeginPlay
	Left_Weapon = GetWorld()->SpawnActorDeferred<AHD_PlayerWeaponBase>(PlayerWeaponFactory, Left_WeaponScene->GetComponentTransform(), GetOwner());
	Right_Weapon = GetWorld()->SpawnActorDeferred<AHD_PlayerWeaponBase>(PlayerWeaponFactory, Right_WeaponScene->GetComponentTransform(), GetOwner());
	
	UGameplayStatics::FinishSpawningActor(Left_Weapon, Left_WeaponScene->GetComponentTransform());
	UGameplayStatics::FinishSpawningActor(Right_Weapon, Right_WeaponScene->GetComponentTransform());

	Left_Weapon->AttachToComponent(Left_WeaponScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Right_Weapon->AttachToComponent(Right_WeaponScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Left_Weapon->SetOwner(GetOwner());
	Right_Weapon->SetOwner(GetOwner());
}

