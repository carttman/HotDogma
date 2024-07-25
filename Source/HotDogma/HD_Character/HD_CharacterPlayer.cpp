// Fill out your copyright notice in the Description page of Project Settings.


#include "../HD_Character/HD_CharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AHD_CharacterPlayer::AHD_CharacterPlayer()
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
	// 메쉬 위치 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//SpringArm 컴포넌트 생성
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	//springArm 을 RootComponent 의 자식 (루프 컴포넌트는 디폴트 기본 자식임)
	springArm->SetupAttachment(RootComponent);
	//springArm 위치를 바꾸자
	springArm->SetRelativeLocation(FVector(0, 0, 0));
	//springArm 각도 변경
	springArm->SetRelativeRotation(FRotator(0, 0, 0));
	springArm->TargetArmLength = 200;
	springArm->ProbeChannel = ECollisionChannel::ECC_Visibility;
	springArm->bUsePawnControlRotation = true;
	springArm->SocketOffset = FVector(40, 35, 155);
	springArm->bDoCollisionTest = false;
	// camera setting
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//camera 를 springArm 의 자식으로 셋팅
	camera->SetupAttachment(springArm);
	//camera->SetupAttachment(GetMesh(), FName(TEXT("headSocket")));
	camera->SetRelativeLocation(FVector(0, 0, 0));
	camera->SetRelativeRotation(FRotator(-20, 0, 0));

	Left_DaggerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left_DaggerMesh"));
	Left_DaggerMesh->SetupAttachment(GetMesh(), TEXT("middle_01_l"));
	Left_DaggerMesh->SetRelativeLocation(FVector(3.2,-3, -4.6));
	Left_DaggerMesh->SetRelativeRotation(FRotator(0,-180,180));

	Right_DaggerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right_DaggerMesh"));
	Right_DaggerMesh->SetupAttachment(GetMesh(), TEXT("middle_01_r"));
	Right_DaggerMesh->SetRelativeLocation(FVector(-4,2, 5));
	//Left_DaggerMesh->SetRelativeRotation(FRotator(0,-180,180));
}

// Called when the game starts or when spawned
void AHD_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
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

}

