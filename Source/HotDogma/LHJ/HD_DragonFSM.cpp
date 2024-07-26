// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonFSM.h"

#include "AIController.h"
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

	if (Dragon->SkeletalComp->GetAnimInstance())
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
		changeState(State);
	}
}
#pragma endregion

#pragma region [Tick]
void UHD_DragonFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//double dot = GetRadianFromCharacter();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Player To Dragon Direction : %f"), dot));

	switch (State)
	{
	case DragonState::Sleep:
		SleepState();
		break;
	case DragonState::Idle:
		IdleState(DeltaTime);
		break;
	case DragonState::Move:
		MoveState(DeltaTime);
		break;
	}
}
#pragma endregion

#pragma region [State Function]
void UHD_DragonFSM::SleepState()
{
	if (Dragon->CharacterArr.Num() == 0)
	{
	}
	NearTargetActor = UGameplayStatics::GetActorOfClass(GetWorld(), AHD_CharacterPlayer::StaticClass());
	
	// 일정거리 안에 플레이어가 들어오거나, 플레이어가 먼저 공격을 하면
	// SleepEnd 애니메이션 재생을 하고,
	// Idle 상태로 전환한다(노티파이 처리)
	ChkCharacterIntoRadian();

	State = DragonState::Idle;
}

void UHD_DragonFSM::IdleState(float DeltaTime)
{
	// 타겟을 지정한다.
	ACharacter* ClosestCharacter = nullptr;
	float MinDistance = FLT_MAX;

	for (ACharacter* Character : Dragon->CharacterArr)
	{
		if (Character && Dragon)
		{
			float Distance = FVector::Dist(Character->GetActorLocation(), Dragon->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestCharacter = Character;
			}
		}
	}

	changeState(DragonState::Move);
}

void UHD_DragonFSM::MoveState(float DeltaTime)
{
	if (NearTargetActor)
	{
		// 날고있지 않을 때는 AI MOVE로 이동
		if (!bFly)
			MoveToLocation(NearTargetActor->GetActorLocation());
		else
		{
			//날고있을 때는 P=P0+VT 사용
		}
	}
}
#pragma endregion

#pragma region [Check direction from character]
double UHD_DragonFSM::GetRadianFromCharacter()
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

// 일정 범위 내에 플레이어가 있는지 확인
bool UHD_DragonFSM::ChkCharacterIntoRadian()
{
	bool bRtn = false;
	// 월드에 있는 캐릭터를 가지고 와서 배열에 저장(BeginPlay에 작성)
	// 임시로 캐릭터로 지정
	// 일정 거리 안에 들어온 캐릭터가 있으면
	for (int i = 0; i < Dragon->CharacterArr.Num(); i++)
	{
		int distanceSize = (Dragon->CharacterArr[i]->GetActorLocation() - Dragon->GetActorLocation()).Size();
		if (distanceSize <= ThresholdRadian)
		{
			if (Anim)
			{
				// true 리턴
				Anim->bSleepEnd = true;
				break;
			}
		}
	}
	return bRtn;
}
#pragma endregion

EPathFollowingRequestResult::Type UHD_DragonFSM::MoveToLocation(FVector targetLoc)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(targetLoc);
	MoveRequest.SetAcceptanceRadius(5.0f);

	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = Dragon->AIController->MoveTo(MoveRequest, &NavPath);

	return Result.Code;
}

void UHD_DragonFSM::changeState(DragonState NextState)
{
	if (Anim)
		Anim->AnimState = NextState;

	State = NextState;
}
