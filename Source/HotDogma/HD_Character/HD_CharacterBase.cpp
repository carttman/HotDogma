// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_CharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HD_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "HD_PlayerComponent/HD_PlayerAttackComponent.h"
#include "HD_PlayerComponent/PlayerStatusComponent.h"

// Sets default values
AHD_CharacterBase::AHD_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 플레이어 메쉬 프로필
	//GetMesh()->SetCollisionProfileName(TEXT("PlayerMeshColl"));
	GetMesh()->SetGenerateOverlapEvents(true);
	// 플레이어 캡슐 프로필
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerColl"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	PlayerAttackComponent = CreateDefaultSubobject<UHD_PlayerAttackComponent>(TEXT("PlayerAttackComponent"));
	PlayerStatusComponent = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatusComponent"));
	
}

// Called when the game starts or when spawned
void AHD_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//get APlayerController
	AHD_PlayerController* playerContoller = Cast<AHD_PlayerController>(GetController());
	if (playerContoller == nullptr) return;
	//get subSystem
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());

	//서브시스템을 가져왔다면
	if (subSystem)
	{
		//imc 맵핑
		subSystem->AddMappingContext(imc_HDMapping, 0);
	}

}

// Called every frame
void AHD_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AHD_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_DH_Move, ETriggerEvent::Triggered, this, &AHD_CharacterBase::EnhancedMove);
		enhancedInputComponent->BindAction(ia_DH_Look, ETriggerEvent::Triggered, this, &AHD_CharacterBase::EnhancedLook);
		//enhancedInputComponent->BindAction(ia_DB_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		enhancedInputComponent->BindAction(ia_DH_Jump, ETriggerEvent::Started, this, &AHD_CharacterBase::EnhancedJump);
		enhancedInputComponent->BindAction(ia_DH_Attack, ETriggerEvent::Started, this, &AHD_CharacterBase::EnhancedAttack);
		enhancedInputComponent->BindAction(ia_DH_Order, ETriggerEvent::Started, this, &AHD_CharacterBase::EnhancedOrder);
		
	}
}

void AHD_CharacterBase::EnhancedMove(const FInputActionValue& InputActionValue)
{
	// FVector2D dir = InputActionValue.Get<FVector2D>();
	// FVector originVec = FVector(dir.Y, dir.X, 0);
	// //FVector newVec = GetTransform().TransformVector(originVec);
	//
	// AddMovementInput(originVec);
	// input is a Vector2D
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AHD_CharacterBase::EnhancedJump(const FInputActionValue& InputActionValue)
{
	Jump();
}

void AHD_CharacterBase::EnhancedLook(const FInputActionValue& InputActionValue)
{
	FVector2D dir = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(dir.X);
	AddControllerPitchInput(dir.Y);
	//UE_LOG(LogTemp, , TEXT("%d"), x);
}

void AHD_CharacterBase::EnhancedAttack(const FInputActionValue& InputActionValue)
{
}

void AHD_CharacterBase::EnhancedOrder(const FInputActionValue& InputActionValue)
{
}
