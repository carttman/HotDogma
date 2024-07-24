// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonFSM.h"

#include "HD_Dragon.h"
#include "HD_DragonAnim.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region [Constructor]
UHD_DragonFSM::UHD_DragonFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}
#pragma endregion

#pragma region [BeginPlay]
void UHD_DragonFSM::BeginPlay()
{
	Super::BeginPlay();

	NearTargetActor = UGameplayStatics::GetActorOfClass(GetWorld(), AHD_CharacterPlayer::StaticClass());
	if (!NearTargetActor)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NearTargetActor Is NullPtr"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("NearTargetActor Is Not NullPtr"));

	DragonActor = UGameplayStatics::GetActorOfClass(GetWorld(), AHD_Dragon::StaticClass());
	if (!DragonActor)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DragonActor Is NullPtr"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DragonActor Is Not NullPtr"));

	Dragon = Cast<AHD_Dragon>(GetOwner());
	if (!Dragon)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Dragon Is NullPtr"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Dragon Is Not NullPtr"));

	if(Dragon->SkeletalComp->GetAnimInstance())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("GetAnimInstance Is Not NullPtr"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GetAnimInstance Is NullPtr"));
	}
	
	if (Dragon)
		Anim = Cast<UHD_DragonAnim>(Dragon->SkeletalComp->GetAnimInstance());
	if (!Anim)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Anim Is NullPtr"));
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Anim Is Not NullPtr"));
		Anim->AnimState = State;
	}
}
#pragma endregion

#pragma region [Tick]
void UHD_DragonFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	double dot = ChkDirectionFromCharacter();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Player To Dragon Direction : %f"), dot));
}
#pragma endregion

#pragma region [State Function]
void UHD_DragonFSM::IdleState()
{
}

void UHD_DragonFSM::MoveState()
{
}
#pragma endregion

#pragma region [Check direction from character]
double UHD_DragonFSM::ChkDirectionFromCharacter()
{
	double dRtn = 0;

	if (NearTargetActor && DragonActor)
	{
		dir = NearTargetActor->GetActorLocation() - DragonActor->GetActorLocation();
		dir.Normalize();

		FVector forward = DragonActor->GetActorForwardVector();

		double dot = UKismetMathLibrary::Dot_VectorVector(forward, dir);
		dRtn = dot;
	}

	return dRtn;
}
#pragma endregion
