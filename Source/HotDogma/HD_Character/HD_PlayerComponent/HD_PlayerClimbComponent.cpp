// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerClimbComponent.h"

#include "EnhancedInputComponent.h"
#include "HotDogma/HD_Character/HD_CharacterBase.h"

// Sets default values for this component's properties
UHD_PlayerClimbComponent::UHD_PlayerClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHD_PlayerClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AHD_CharacterBase>(GetOwner());
	
}


// Called every frame
void UHD_PlayerClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHD_PlayerClimbComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(IA_Player_Climb, ETriggerEvent::Started, this,
	                                   &UHD_PlayerClimbComponent::Climb);

}

void UHD_PlayerClimbComponent::Climb()
{
	TraceMovement();
}

void UHD_PlayerClimbComponent::TraceMovement()
{
	FHitResult OutHit;
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + GetMovementDirection() * 1000 + Player->GetActorUpVector() * -500;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams  params;
	params.AddIgnoredActor(Player);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, params);

	if(bHit)
	{
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
		Hit_F1 = OutHit;
		
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 3.0f);
	}
	
	FVector End_2 = Player->GetActorLocation() + GetMovementDirection() * 1100 + Player->GetActorUpVector() * -500;
	bool bHit_2 = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End_2, TraceChannel, params);
	if(bHit_2)
	{
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
		Hit_F2 = OutHit;
	}
	
	FVector End_3 = Player->GetActorLocation() + GetMovementDirection() * 1000 + Player->GetActorUpVector() * -500 + GetMovementDirection().RotateAngleAxis(90, Player->GetActorUpVector()) * 100;
	bool bHit_3 = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End_3, TraceChannel, params);
	if(bHit_3)
	{
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
		Hit_F2 = OutHit;
	}
}

FVector UHD_PlayerClimbComponent::GetMovementDirection()
{
	FVector Direction = Player->GetActorForwardVector() * Player->MovementVector.X + Player->GetActorRightVector() * Player->MovementVector.Y;
	Direction.Normalize();
	return Direction;
}

