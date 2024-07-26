// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_CharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HD_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HD_PlayerComponent/HD_PlayerAttackComponent.h"
#include "HD_PlayerComponent/PlayerStatusComponent.h"

// Sets default values
AHD_CharacterBase::AHD_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,500,0);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f; 
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// 플레이어 메쉬 프로필
	GetMesh()->SetCollisionProfileName(TEXT("PlayerMeshColl"));
	GetMesh()->SetGenerateOverlapEvents(true);
	// 플레이어 캡슐 프로필
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCapsuleColl"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// 메쉬 위치 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//SpringArm 셋팅
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeLocation(FVector(0, 0, 0));
	springArm->SetRelativeRotation(FRotator(0, 0, 0));
	springArm->TargetArmLength = 200;
	springArm->ProbeChannel = ECollisionChannel::ECC_Visibility;
	springArm->bUsePawnControlRotation = true;
	springArm->SocketOffset = FVector(40, 35, 155);
	springArm->bDoCollisionTest = false;
	
	// camera setting
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
	camera->SetRelativeLocation(FVector(0, 0, 0));
	camera->SetRelativeRotation(FRotator(-20, 0, 0));

	// Player 컴포넌트
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
		enhancedInputComponent->BindAction(ia_DH_Jump, ETriggerEvent::Started, this, &AHD_CharacterBase::EnhancedJump);
		enhancedInputComponent->BindAction(ia_DH_Order, ETriggerEvent::Started, this, &AHD_CharacterBase::EnhancedOrder);

		PlayerAttackComponent->SetupPlayerInputComponent(enhancedInputComponent);
		
	}
}

void AHD_CharacterBase::EnhancedMove(const FInputActionValue& InputActionValue)
{
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
}


void AHD_CharacterBase::EnhancedOrder(const FInputActionValue& InputActionValue)
{
}
