// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerCamera.h"

#include "Components/ArrowComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"

AHD_PlayerCamera::AHD_PlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHD_PlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AHD_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(Player) UE_LOG(LogTemp, Warning, TEXT("camera : Player cast successful"))
	else UE_LOG(LogTemp, Warning, TEXT("camera : Player cast failed"));
	
}

void AHD_PlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// SetActorLocation(GetActorLocation() + GetActorForwardVector() *  CameraSpeed * DeltaTime);
	SetActorLocation(Player->CameraPoint->GetRelativeLocation() + Player->GetActorForwardVector() * CameraSpeed * DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("1"));
}

void AHD_PlayerCamera::AutoActivateForPlayer(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		PlayerController->SetViewTarget(this); // 컨트롤러의 시점을 정한다
	}
}
