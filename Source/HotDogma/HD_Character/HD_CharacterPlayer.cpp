// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_CharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "HD_PlayerController.h"
#include "HD_PlayerComponent/HD_PlayerAttackComponent.h"
#include "HD_PlayerComponent/PlayerStatusComponent.h"
#include "HD_PlayerItem/HD_PlayerWeaponBase.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHD_CharacterPlayer::AHD_CharacterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Left_WeaponScene = CreateDefaultSubobject<USceneComponent>(TEXT("Left_WeaponScene"));
	Left_WeaponScene->SetupAttachment(GetMesh(), TEXT("middle_01_l"));
	Left_WeaponScene->SetRelativeLocation(FVector(7.2,-3, -26.6));
	Left_WeaponScene->SetRelativeRotation(FRotator(0,-180,180));
	
	Right_WeaponScene = CreateDefaultSubobject<USceneComponent>(TEXT("Right_WeaponScene"));
	Right_WeaponScene->SetupAttachment(GetMesh(), TEXT("middle_01_r"));
	Right_WeaponScene->SetRelativeLocation(FVector(-4,1, 26));

	// Player 컴포넌트
	PlayerAttackComponent = CreateDefaultSubobject<UHD_PlayerAttackComponent>(TEXT("PlayerAttackComponent"));
}

// Called when the game starts or when spawned
void AHD_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerContoller = Cast<AHD_PlayerController>(GetController());
	AttachWeapon();
	if(AM_KnockDown_Montage) GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AHD_CharacterPlayer::PlayMontageNotifyBegin_KnockDown);
}

// Called every frame
void AHD_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerStatusComponent->CurrHP <= 0)
	{
		if(IsDeath) return;
		IsDeath = true;
		if(IsDeath)
		{
			DeathProcess();
		}
	}
}

// Called to bind functionality to input
void AHD_CharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		PlayerAttackComponent->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

float AHD_CharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	UE_LOG(LogTemp, Warning, TEXT("%s Takes Damage : %f"), *GetName(), damage);
	PlayerStatusComponent->CurrHP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("%s Takes Damage : %f"), *GetName(), PlayerStatusComponent->CurrHP);
	
	if(DamageCauser)
	{
		AHD_Dragon* HJ_Dragon = Cast<AHD_Dragon>(DamageCauser);
		if(HJ_Dragon)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *HJ_Dragon->strDamageAttackType);
			if(!HJ_Dragon->strDamageAttackType.Equals("Shout"))
			{
				PlayerStatusComponent->CurrHP -= damage;
			}
			if(HJ_Dragon->strDamageAttackType.Equals("Shout"))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(AM_KnockDown_Montage, 1);
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("KnockDown_Start"), AM_KnockDown_Montage);
				
			}
			if(HJ_Dragon->strDamageAttackType.Equals("JumpPress"))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(AM_Hit_Montage, 1);
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Hit_Large"), AM_Hit_Montage);
			}
			if(HJ_Dragon->strDamageAttackType.Equals("TailSlap"))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(AM_Hit_Montage, 1);
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Hit_Large"), AM_Hit_Montage);
			}
			if(HJ_Dragon->strDamageAttackType.Equals("Scratch"))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(AM_Hit_Montage, 1);
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Hit_Normal"), AM_Hit_Montage);
			}
			if(HJ_Dragon->strDamageAttackType.Equals("HandPress"))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(AM_Hit_Montage, 1);
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Hit_Normal"), AM_Hit_Montage);
			}
		}
	}
	return damage;
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

void AHD_CharacterPlayer::DeathProcess()
{
	GetMesh()->GetAnimInstance()->Montage_Play(AM_Hit_Montage, 1);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Hit_Death"), AM_Hit_Montage);
	
	DisableInput(PlayerContoller);
}

void AHD_CharacterPlayer::PlayMontageNotifyBegin_KnockDown(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if(NotifyName == FName("KnockDown_Start"))
	{
		IsKnockDown = true;
	}
	if(NotifyName == FName("KnockDown_End"))
	{
		IsKnockDown = false;
	}
}

