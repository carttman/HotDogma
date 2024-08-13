// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_CharacterBase.h"


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HD_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CableComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/ArrowComponent.h"
#include "HD_PlayerComponent/PlayerStatusComponent.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "HotDogma/LHJ/HD_Dragon.h"

// Sets default values
AHD_CharacterBase::AHD_CharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 500, 0);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->SetFixedBrakingDistance(200.f);
	GetCharacterMovement()->MaxFlySpeed = 150.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 3000.f;

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
	springArm->SocketOffset = FVector(-100, 0, 200);
	springArm->bEnableCameraLag = true;
	springArm->bEnableCameraRotationLag = true;
	springArm->CameraLagSpeed = 6.0f;
	springArm->CameraRotationLagSpeed = 6.0f;

	// camera setting
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
	camera->SetRelativeLocation(FVector(0, 0, 0));
	camera->SetRelativeRotation(FRotator(-20, 0, 0));

	//PlayerClimbComponent = CreateDefaultSubobject<UHD_PlayerClimbComponent>(TEXT("PlayerClimbComponent"));
	PlayerStatusComponent = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatusComponent"));

	CableCompoent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableCompoent->SetupAttachment(GetMesh(), TEXT("pelvis"));
	CableCompoent->EndLocation = FVector(0, 0, 0);

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	CameraPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraPoint"));
	CameraPoint->SetupAttachment(RootComponent);
	CameraPoint->SetRelativeLocation(FVector(-440, 0, 170));
	CameraPoint->SetRelativeRotation(FRotator(-10, 0, 0));
}

// Called when the game starts or when spawned
void AHD_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//get APlayerController
	AHD_PlayerController* playerContoller = Cast<AHD_PlayerController>(GetController());
	if (playerContoller == nullptr) return;
	//get subSystem
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		playerContoller->GetLocalPlayer());
	//서브시스템을 가져왔다면
	if (subSystem)
	{
		//imc 맵핑
		subSystem->AddMappingContext(imc_HDMapping, 0);
	}

	PlayerGameMode = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());
	if (PlayerGameMode != nullptr)
	{
		Dragon = PlayerGameMode->GetEnemy(GetOwner()->GetActorLocation());
		if (Dragon != nullptr) UE_LOG(LogTemp, Warning, TEXT("TO CharacterBase -> Get : %s"), *Dragon->GetName());
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
		enhancedInputComponent->BindAction(IA_HD_Move, ETriggerEvent::Triggered, this,
		                                   &AHD_CharacterBase::EnhancedMove);
		enhancedInputComponent->BindAction(IA_HD_Look, ETriggerEvent::Triggered, this,
		                                   &AHD_CharacterBase::EnhancedLook);
		enhancedInputComponent->BindAction(IA_HD_Jump, ETriggerEvent::Started, this, &AHD_CharacterBase::EnhancedJump);
		enhancedInputComponent->BindAction(IA_HD_Order, ETriggerEvent::Started, this,
		                                   &AHD_CharacterBase::EnhancedOrder);
		enhancedInputComponent->BindAction(IA_HD_Run, ETriggerEvent::Triggered, this, &AHD_CharacterBase::EnhancedRun);
		enhancedInputComponent->BindAction(IA_HD_Run, ETriggerEvent::Completed, this, &AHD_CharacterBase::EnhancedRun);


		//PlayerClimbComponent->SetupPlayerInputComponent(enhancedInputComponent);
	}
}


void AHD_CharacterBase::EnhancedMove(const FInputActionValue& InputActionValue)
{
	MovementVector = InputActionValue.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		switch (GetCharacterMovement()->MovementMode)
		{
		case MOVE_Walking:// Walking일 때
			if(IsKnockDown) return;
			if(IsHit) return;
			if(IsDeath) return;
			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
			
			break;
		case MOVE_Falling:

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
			break;
		case MOVE_Flying:
			// if (PlayerClimbComponent->IsClimbing)
			// {
			// 	//climb movement : 무중력 상태일 때, 매달리는 함수 -> inputX값을 UpVector로 바꾸고 Z축으로 이동시킴
			// 	PlayerClimbComponent->ClimbMovementEvent(GetActorRightVector(), MovementVector.X);
			// 	Climb_LeftRight = MovementVector.X;
			// 	PlayerClimbComponent->ClimbMovementEvent(GetActorUpVector(), MovementVector.Y);
			// 	Climb_UpDown = MovementVector.Y;
			// }
			break;
		default:
			break;
		}
	}
}

void AHD_CharacterBase::EnhancedJump(const FInputActionValue& InputActionValue)
{
	if(IsKnockDown) return;
	if(IsHit) return;
	if(IsDeath) return;
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
	if(IsDeath) return;
	float InputKey = InputActionValue.Get<float>();
	// gamemode를 가져온다.

	if (PlayerGameMode == nullptr) return;
	float value = InputActionValue.Get<float>();
	if (FMath::IsNearlyEqual(value, 1.f))
	{
		PlayerGameMode->CommandCompanion(0);
	}
	else if (FMath::IsNearlyEqual(value, 2.f))
	{
		PlayerGameMode->CommandCompanion(1);
	}
	else if (FMath::IsNearlyEqual(value, 3.f))
	{
		PlayerGameMode->CommandCompanion(2);
	}
	else if (FMath::IsNearlyEqual(value, 4.f))
	{
		PlayerGameMode->CommandCompanion(3);
	}
}

void AHD_CharacterBase::EnhancedRun(const FInputActionValue& InputActionValue)
{
	if(IsDeath) return;
	bool IsRun = InputActionValue.Get<bool>();
	if (IsRun)GetCharacterMovement()->MaxWalkSpeed = 800.f;
	else GetCharacterMovement()->MaxWalkSpeed = 500.f;
}


float AHD_CharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                    AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return damage;
}
