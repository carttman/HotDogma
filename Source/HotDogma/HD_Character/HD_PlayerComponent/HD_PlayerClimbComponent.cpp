// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerClimbComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotDogma/HD_Character/HD_CharacterBase.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UHD_PlayerClimbComponent::UHD_PlayerClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA(TEXT("/Script/EnhancedInput.InputAction'/Game/CHJ/Input/IA_Player_Climb.IA_Player_Climb'"));
	if(TempIA.Succeeded())
	{
		IA_Player_Climb = TempIA.Object;
	}
	// ...
}


// Called when the game starts
void UHD_PlayerClimbComponent::BeginPlay()
{
	Super::BeginPlay();
	//Player = Cast<AHD_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	//Target_Location = Player->GetActorLocation();
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is: %s"), *Owner->GetName());

		Player = Cast<AHD_CharacterPlayer>(Owner);
		if (Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player cast successful"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player cast failed"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwner() returned null"));
	}
}


// Called every frame
void UHD_PlayerClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UHD_PlayerClimbComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(IA_Player_Climb, ETriggerEvent::Started, this,
	                                   &UHD_PlayerClimbComponent::Climb);

}

void UHD_PlayerClimbComponent::Climb()
{
	//TraceMovement();
	//ClimbMove();
	GrabWall();
}

void UHD_PlayerClimbComponent::TraceMovement()
{
	 // FHitResult OutHit;
	 // FVector Start = Player->GetActorLocation();
	 // FVector End = Player->GetActorLocation() + GetMovementDirection() * 1000 + Player->GetActorUpVector() * -500;
	 // ECollisionChannel TraceChannel = ECC_Visibility;
	 // FCollisionQueryParams  params;
	 // params.AddIgnoredActor(Player);
	 // bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, params);
	 //
	 // if(bHit)
	 // {
	 // 	DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
	 // 	Hit_F1 = OutHit;
	 // 	
	 // }
	// else
	// {
	// 	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 3.0f);
	// }
	//
	// FVector End_2 = Player->GetActorLocation() + GetMovementDirection() * 1100 + Player->GetActorUpVector() * -500;
	// bool bHit_2 = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End_2, TraceChannel, params);
	// if(bHit_2)
	// {
	// 	DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
	// 	Hit_F2 = OutHit;
	// }
	//
	// FVector End_3 = Player->GetActorLocation() + GetMovementDirection() * 1000 + Player->GetActorUpVector() * -500 + GetMovementDirection().RotateAngleAxis(90, Player->GetActorUpVector()) * 100;
	// bool bHit_3 = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End_3, TraceChannel, params);
	// if(bHit_3)
	// {
	// 	DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
	// 	Hit_R = OutHit;
	// }
	// Target_Location = Hit_F1.Location + Hit_F1.Normal * 60;
	//
	// Target_Rotation = UKismetMathLibrary::MakeRotationFromAxes(UKismetMathLibrary::GetDirectionUnitVector(Hit_F1.Location, Hit_F2.Location), UKismetMathLibrary::GetDirectionUnitVector(Hit_F1.Location, Hit_R.Location), Hit_F1.Normal);
}

// FVector UHD_PlayerClimbComponent::GetMovementDirection()
// {//Player->MovementVector.X
// 	// FVector Direction = Player->GetActorForwardVector() * Player->MovementVector.X + Player->GetActorRightVector() * Player->MovementVector.Y;
// 	// Direction.Normalize();
// 	// return Direction;
// }

void UHD_PlayerClimbComponent::ClimbMove()
{
	
	// if(UKismetMathLibrary::Abs(Player->MovementVector.X) + UKismetMathLibrary::Abs(Player->MovementVector.Y) > 0)
	// {
	// 	if(UKismetMathLibrary::Vector_Distance(Target_Location, Player->GetActorLocation()) > 2)
	// 	{
	// 		Player->AddMovementInput(UKismetMathLibrary::GetDirectionUnitVector(Player->GetActorLocation(), Target_Location));
	// 		Player->SetActorRotation(UKismetMathLibrary::RLerp(Player->GetActorRotation(), Target_Rotation, 0.01f, true));
	// 		
	// 	}
	// }
}

void UHD_PlayerClimbComponent::GrabWall()
{
	FHitResult OutHit;
	FVector Start = Player->GetMesh()->GetSocketLocation(FName("pelvis"));
	FVector End = Player->GetMesh()->GetSocketLocation(FName("pelvis")) + Player->GetActorForwardVector() * 200;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams params;
	params.AddIgnoredActor(Player);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, params);
	 
	if(bHit)
	{
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
		
		
		FVector Start_2 = Player->GetMesh()->GetSocketLocation(FName("head"));
		FVector End_2 = Player->GetMesh()->GetSocketLocation(FName("head")) + Player->GetActorForwardVector() * 200;
		bool bHit_2 = GetWorld()->LineTraceSingleByChannel(OutHit, Start_2, End_2, TraceChannel, params);
		
		if(bHit_2)
		{
			DrawDebugLine(GetWorld(), Start_2, OutHit.ImpactPoint, FColor::Red, false, 3.0f);
			Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			FRotator NewRot = UKismetMathLibrary::Conv_VectorToRotator(OutHit.Normal * -1);
			FVector NewLoc = OutHit.Location + OutHit.Normal * 60;
			FLatentActionInfo latentInfo;
			latentInfo.CallbackTarget = this;
			
			Player->GetCharacterMovement()->StopMovementImmediately();
			UKismetSystemLibrary::MoveComponentTo(Player->GetCapsuleComponent(),NewLoc, FRotator(0,NewRot.Yaw,0),
				false, false, 0.4f, false, EMoveComponentAction::Move, latentInfo);
		}
		 
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 3.0f);
	}
}
 
