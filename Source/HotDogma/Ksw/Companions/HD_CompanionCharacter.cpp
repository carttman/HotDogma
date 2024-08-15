// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_CompanionCharacter.h"
#include "HotDogma/Ksw/CompanionComponents/HD_CompanionStateComponent.h"
#include "Components/ArrowComponent.h"
#include "../CompanionComponents/HD_WarriorStateComponent.h"
#include "../CompanionComponents/HD_SorcererStateComponent.h"
#include "HotDogma/HD_Character/HD_PlayerComponent/PlayerStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "HotDogma/UI/HD_CompanionWidget.h"
#include "HotDogma/Ksw/HD_CompanionWeapon.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "HotDogma/Ksw/Companions/HD_WarriorAnimInstance.h"
#include "Components/BoxComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <HotDogma/HD_GameModeBase/CHJ_GameMode.h>

// Sets default values
AHD_CompanionCharacter::AHD_CompanionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerStatusComp = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatusComp"));

	HPComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPComp"));
	HPComp->SetupAttachment(RootComponent);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UUserWidget> TempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_CompanionHP.WBP_CompanionHP_C'"));
	if (TempWidget.Succeeded())
	{
		HPComp->SetWidgetClass(TempWidget.Class);
		HPComp->SetDrawSize(FVector2D(200.0f, 51.0f));
		HPComp->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	}

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHD_CompanionCharacter::BeginPlay()
{
	Super::BeginPlay();

	Widget = Cast<UHD_CompanionWidget>(HPComp->GetUserWidgetObject());
	if (Widget)
	{
		Widget->SetImage(CompanionImage);
		Widget->SetText(CompanionName);
		Widget->SetHPBar(1, 1);
	}

	CompanionWeapon = GetWorld()->SpawnActor<AHD_CompanionWeapon>(CompanionWeaponFactory, GetActorLocation(), GetActorRotation());
	CompanionWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHD_CompanionCharacter::OnOverlapBegin);
}

// Called every frame
void AHD_CompanionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Target = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector Dir = Target - HPComp->GetComponentLocation();
	FRotator Rot = Dir.ToOrientationRotator();
	HPComp->SetWorldRotation(Rot);

	// Target과의 거리가 멀수록 HP바가 커진다.
	float Distance = FVector::Distance(Target, HPComp->GetComponentLocation());
	float Scale = FMath::Clamp(Distance / 750, 0.1f, 2.0f);
	HPComp->SetWorldScale3D(FVector(Scale, Scale, Scale));
}

// Called to bind functionality to input
void AHD_CompanionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AHD_CompanionCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (!IsDeath)
	{
		PlayerStatusComp->CurrHP -= Damage;
		if (PlayerStatusComp->MaxHP < PlayerStatusComp->CurrHP)
		{
			PlayerStatusComp->CurrHP = PlayerStatusComp->MaxHP;
		}

		PlayerStatusComp->CurrHP <= 0 ? IsDeath = true : IsDeath = false;
		Widget->SetHPBar(PlayerStatusComp->CurrHP, PlayerStatusComp->MaxHP);
		if (!CompanionStateComp->bCasting)
		{
			if (DamageCauser)
			{
				AHD_Dragon* Dragon = Cast<AHD_Dragon>(DamageCauser);
				if (Dragon)
				{
					if (Dragon->strDamageAttackType.Equals("Shout"))
					{
						GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1);
						GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Stun"), HitMontage);
						CompanionStateComp->DoStun(0.2f);
					}
					if (Dragon->strDamageAttackType.Equals("JumpPress"))
					{
						auto* gm = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
						if (gm  && CompanionId == 2)
						{
							gm->PlaySoundAtIndex(26);
							gm->PlaySoundAtIndex(27);
						}

						CompanionStateComp->DoStun(1.0f);
						GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1);
						GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("CriticalHit"), HitMontage);
					}
					if (Dragon->strDamageAttackType.Equals("TailSlap"))
					{
						GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1);
						GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("KnockBack"), HitMontage);
						CompanionStateComp->DoStun(0.5f);
					}
					if (Dragon->strDamageAttackType.Equals("Scratch"))
					{
						GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1);
						GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("KnockBack"), HitMontage);
						CompanionStateComp->DoStun(0.5f);
					}
					if (Dragon->strDamageAttackType.Equals("HandPress"))
					{
						GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1);
						GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Hit"), HitMontage);
					}
					if (Dragon->strDamageAttackType.Equals("Thunder"))
					{
						GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1);
						GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Hit"), HitMontage);
					}
					if (Dragon->strDamageAttackType.Equals("Meteor"))
					{
						auto* gm = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
						if (gm && CompanionId == 2)
						{
							gm->PlaySoundAtIndex(26);
							gm->PlaySoundAtIndex(27);
						}

						CompanionStateComp->DoStun(1.0f);
						GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1);
						GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("CriticalHit"), HitMontage);
					}
				}
			}
		}
	}
	return Damage;
}

void AHD_CompanionCharacter::ToggleHandIK(bool enable)
{
	UHD_WarriorAnimInstance* anim = Cast<UHD_WarriorAnimInstance>(GetMesh()->GetAnimInstance());
	if (anim)
	{
		// 플레이어의 오른손 위치
		ACharacter* Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
		FVector Target = Player->GetMesh()->GetBoneLocation("hand_r");

		anim->ToggleHandIK(enable, Target);
	}

	if (enable)
	{
		// 사운드
		auto* gm = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			if (CompanionId == 1)
			{
				gm->PlaySoundAtIndex(28);
			}
			else if (CompanionId == 2)
			{
				gm->PlaySoundAtIndex(29);
			}
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HighfiveSound, GetActorLocation());
	}
}

class UHD_CompanionStateComponent* AHD_CompanionCharacter::SetupCompanionStateComp(bool isWarrior)
{
	if (isWarrior)
	{
		CompanionStateComp = NewObject<UHD_WarriorStateComponent>(this, UHD_WarriorStateComponent::StaticClass(), TEXT("CompanionStateComp"));
		CompanionStateComp->RegisterComponent();
	}
	else
	{
		CompanionStateComp = NewObject<UHD_SorcererStateComponent>(this, UHD_SorcererStateComponent::StaticClass(), TEXT("CompanionStateComp"));
		CompanionStateComp->RegisterComponent();
	}

	return CompanionStateComp;
}

void AHD_CompanionCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHD_CharacterPlayer* Player = Cast<AHD_CharacterPlayer>(OtherActor);
	if (Player && CompanionStateComp->IsHighfive())
	{
		CompanionStateComp->Highfive();
		ToggleHandIK(true);
		UHD_PlayerAnimInstance* PlayerAnim = Cast< UHD_PlayerAnimInstance>(Player->GetMesh()->GetAnimInstance());
		
		PlayerAnim->PlayHighfive();
		PlayerAnim->ToggleHandIK(true, this);
		Player->GetCharacterMovement()->StopMovementImmediately();
		// 회전시킨다.
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation());
		Player->SetActorRotation(Rot);

		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
		SetActorRotation(LookAt);

		// 콜리전 제거
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
