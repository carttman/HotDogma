// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonFSM.h"

#include "AIController.h"
#include "HD_Dragon.h"
#include "HD_DragonAnim.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	// else
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DragonActor Is Not NullPtr"));

	Dragon = Cast<AHD_Dragon>(GetOwner());
	if (!Dragon)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Dragon Is NullPtr"));
	// else
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Dragon Is Not NullPtr"));

	if (!Dragon->SkeletalComp->GetAnimInstance())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GetAnimInstance Is NullPtr"));
	}
	// else
	// {
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("GetAnimInstance Is Not NullPtr"));
	// }

	if (Dragon)
	{
		Anim = Cast<UHD_DragonAnim>(Dragon->SkeletalComp->GetAnimInstance());
		ai = Cast<AAIController>(Dragon->Controller);
	}

	if (!Anim)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Anim Is NullPtr"));
	else
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Anim Is Not NullPtr"));

		if (Anim)
		{
			Anim->ChangeState(DragonState::Sleep);
			Anim->ChangeAttackState(AttackState::None);
		}
	}

	FOnTimelineFloat ProgressUpdate;
	ProgressUpdate.BindUFunction(this, FName("BreathRStart"));

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("BreathREnd"));

	BreathTimeline.AddInterpFloat(BreathCurve, ProgressUpdate);
	BreathTimeline.SetTimelineFinishedFunc(FinishedEvent);
}
#pragma endregion

#pragma region [Tick]
void UHD_DragonFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//double dot = GetRadianFromCharacter();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Player To Dragon Direction : %f"), dot));
	FString myState = UEnum::GetValueOrBitfieldAsString(State);
	DrawDebugString(GetWorld(), Dragon->GetActorLocation(), myState, 0, FColor::Yellow, 0);
	if (Anim)
	{
		myState = UEnum::GetValueOrBitfieldAsString(Anim->AnimNormalAttackState);
		DrawDebugString(
			GetWorld(), FVector(Dragon->GetActorLocation().X, Dragon->GetActorLocation().Y,
			                    Dragon->GetActorLocation().Z - 50), myState, 0, FColor::Yellow, 0);
	}

	BreathTimeline.TickTimeline(DeltaTime);
	if (State != DragonState::Move)
		ai->StopMovement();

	// 전체 체력 75프로 이하로 깍이면 Fly State진입
	// 스킬 1~2개 사용하고 내려오도록 설정
	// 내려온 다음, 땅에서 최소 스킬을 4개 이상 써야 다시 올라갈수 있도록 설정
	// if (!chkOnceFly)
	// {
	// 	// 최초로 75%보다 낮아지면 하늘로 날아오른다.
	// 	if (Dragon->MaxHP * 0.75 >= Dragon->CurrHP)
	// 	{
	// 		State = DragonState::Fly;
	// 		CurrUsedSkillCnt = 0;
	// 		chkOnceFly = true;
	// 	}
	// }
	// else
	// {
	// 	// 날고있는 상태이고, 정해진 개수만큼 스킬을 사용했을 때
	// 	if (bFly && CurrUsedSkillCnt == ApplySkillAsFly)
	// 	{
	// 		State = DragonState::Fly;
	// 		CurrUsedSkillCnt = 0;
	// 	}
	//
	// 	if(!bFly)
	// 		CurrUsedSkillCnt = 0;
	// }

	//공격중일 때는 상태 변환 x
	if (!isAttack)
	{
		switch (State)
		{
		case DragonState::Sleep:
			SleepState();
			break;
		case DragonState::Shout:
			break;
		case DragonState::Idle:
			IdleState(DeltaTime);
			break;
		case DragonState::Move:
			MoveState(DeltaTime);
			break;
		case DragonState::Attack:
			F_NormalAttackState(DeltaTime);
			break;
		case DragonState::Fly:
			Anim->isFly = true;
			break;
		case DragonState::FlyDown:
			Anim->isFly = false;
			break;
		case DragonState::Groggy:
		case DragonState::Death:
			break;
		}
	}
}
#pragma endregion

#pragma region [State Function]
void UHD_DragonFSM::SleepState()
{
	// 테스트용
	// if (Dragon->CharacterArr.Num() == 0)
	// {
	// 	NearTargetActor = UGameplayStatics::GetActorOfClass(GetWorld(), AHD_CharacterPlayer::StaticClass());
	// }

	// 일정거리 안에 플레이어가 들어오거나, 플레이어가 먼저 공격을 하면
	// SleepEnd 애니메이션 재생을 하고,
	// Idle 상태로 전환한다(노티파이 처리)
	if (!chkCharacterUsingSleep)
		ChkCharacterIntoRadian();

	//State = DragonState::Idle;
}

void UHD_DragonFSM::IdleState(const float& DeltaTime)
{
	if (Anim)
	{
		// 꼬리치기 변수 초기화
		if (Anim->chkAngle)
			Anim->chkAngle = false;

		if (!isAttack)
			Anim->ChangeAttackState(AttackState::None);
	}

	// 타겟을 지정한다.
	ACharacter* ClosestCharacter = nullptr;
	float MinDistance = FLT_MAX;

	// 가까운 캐릭터를 공격대상으로 지정
	for (ACharacter* Character : Dragon->CharacterArr)
	{
		float Distance = FVector::Dist(Character->GetActorLocation(), Dragon->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestCharacter = Character;
			NearTargetActor = ClosestCharacter;
		}
	}

	// <><><> 공격 받는 부분에 어그로 이동 추가
	CurrIdleTime += DeltaTime;
	if (CurrIdleTime >= DuringIdleTime)
	{
		CurrIdleTime = 0.f;

		// 공격 범위 내에 들어오면
		if (NearTargetActor && MinDistance < AttackDist)
		{
			// 공중에서 스킬 사용개수 지정
			if (Anim->chkUsingSkillCnt)
				ApplySkillAsFly = FMath::RandRange(1, 2);

			// 공격 상태로 전이
			ChooseAttackState();
			Anim->ChangeState(DragonState::Attack);
		}
		else
		{
			// 공격범위 밖이라면
			// 이동 상태로 전이
			Anim->ChangeState(DragonState::Move);
		}
	}
}


void UHD_DragonFSM::MoveState(const float& DeltaTime)
{
	if (NearTargetActor)
	{
		//Distance랑 Speed만 동기화

		Anim->Speed = 0.75f;
		FVector ForwardVec = Dragon->GetActorForwardVector();
		FVector CurrentLocation = Dragon->GetActorLocation();
		FVector TargetVec = NearTargetActor->GetActorLocation() - CurrentLocation;
		float TargetDot = FVector::DotProduct(ForwardVec, TargetVec);

		float MagnitudeCurrent = CurrentLocation.Size();
		//UE_LOG(LogTemp, Warning, TEXT("MagnitudeCurrent %f"), MagnitudeCurrent);
		float MagnitudeDirection = TargetVec.Size();
		float RadianValue = FMath::Acos(TargetDot);

		float DegreeValue = FMath::RadiansToDegrees(RadianValue);
		Anim->Direction = DegreeValue;

		// if (bRotate)
		// {
		// 	RotateToTarget(DeltaTime);
		// }

		// FVector StartLoc = Dragon->GetActorLocation();
		// FRotator curRot = Dragon->GetActorRotation();
		// FVector TargetLoc = NearTargetActor->GetActorLocation();
		// FRotator realRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, TargetLoc);
		//
		//
		// al += DeltaTime / 5;
		//
		//
		// if (al > 1)
		// {
		// 	al = 0;
		// }
		// else
		// {
		// 	FRotator realrealROt = UKismetMathLibrary::RLerp(curRot, realRot, al, true);
		// 	Dragon->SetActorRotation(realrealROt);
		// }

		// else
		// {
		//ai->MoveToActor(NearTargetActor);
		ai->MoveToLocation(NearTargetActor->GetActorLocation());

		float Distance = FVector::Dist(NearTargetActor->GetActorLocation(), Dragon->GetActorLocation());
		if (Distance <= AttackDist)
		{
			Anim->ChangeState(DragonState::Idle);
		}
		//}


		//Dragon->SetActorRotation(realRot); 
	}
}


void UHD_DragonFSM::F_NormalAttackState(const float& DeltaTime)
{
	// 정해진 공격 패턴의 스킬 쿨타임이 남아 있다면 다시 패턴 지정
	if (normalAttackState == AttackState::Scratch || normalAttackState == AttackState::TailSlap)
	{
		Anim->InnerAngle = GetRadianFromCharacter();
		Anim->chkAngle = true;
	}
}
#pragma endregion

#pragma region Normal Attack Function
void UHD_DragonFSM::FlyPress(float DeltaTime)
{
	if (!Dragon || !Anim)
		return;

	if (bStartFlyPress)
	{
		// 캐릭터의 movement가 flying이 아니면 설정한다.
		if (!Dragon->GetCharacterMovement()->GetMovementName().Contains("Flying"))
		{
			Dragon->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}

		UKismetMathLibrary::BreakVector(Dragon->GetActorLocation(), x, y, z);

		if (z < FlyPressHeight)
		{
			Dragon->AddMovementInput(FVector(0, 0, 1000));
		}
		else
		{
			Anim->bEndFlyUp = true;
			bStartFlyPress = false;
		}
	}

	if (Anim->bFlyPress)
	{
		// 낙하 시작
		//Alpha += DeltaTime;
		FallSpeed += 980.f * DeltaTime * 1.5; // 중력 가속도 적용 (m/s^2)
		FVector FallVelocity = FVector(0.f, 0.f, -FallSpeed); // 낙하 속도 설정
		//FVector FallVelocity = FVector(0.f, 0.f, -3000.f); // 낙하 속도 설정
		FVector CurrentLocation = Dragon->GetActorLocation();
		FVector NewLocation = CurrentLocation + FallVelocity * DeltaTime;

		Dragon->SetActorLocation(NewLocation);

		// 땅에 닿았는지 체크 (여기서는 Z 축 0을 기준으로)
		if (NewLocation.Z <= 89.f)
		{
			Dragon->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			Anim->bEndFlyUp = false;
			bStartFlyPress = false;
			Anim->bFlyPress = false; // 낙하 완료 상태로 전환
		}
	}
}

void UHD_DragonFSM::NormalBreath(float DeltaTime)
{
}
#pragma endregion

#pragma region [Check direction from character]
float UHD_DragonFSM::GetRadianFromCharacter()
{
	float fRtn = 0;

	if (NearTargetActor && DragonActor)
	{
		dir = NearTargetActor->GetActorLocation() - DragonActor->GetActorLocation();
		dir.Normalize();
		FVector rightVec = DragonActor->GetActorRightVector();

		double dot = UKismetMathLibrary::Dot_VectorVector(rightVec, dir);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
		                                 FString::Printf(TEXT("Player To Dragon Direction : %f"), dot));

		fRtn = dot;
	}

	return fRtn;
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
				chkCharacterUsingSleep = true;
				break;
			}
		}
	}
	return bRtn;
}
#pragma endregion

void UHD_DragonFSM::ChooseAttackState()
{
	if (Anim->isFly)
	{
		// 공중날고 있을때 사용가능 스킬 - 3개
		// Breath, ThunderMagic, Methor
		// 30 20 20
		int_rand = FMath::RandRange(1, 70);
		// if (int_rand > 50)
		// {
		// 	//ThunderMagic
		// 	//Anim->ChangeAttackState(AttackState::ThunderMagic);
		// 	Anim->ChangeState(DragonState::Idle);
		// }
		// else if (int_rand > 30)
		// {
		// 	//Methor
		// 	//Anim->ChangeAttackState(AttackState::Meteor);
		// 	Anim->ChangeState(DragonState::Idle);
		// }
		// else if (int_rand > 0)
		if (int_rand > 0)
		{
			//Breath
			Anim->ChangeAttackState(AttackState::Breath);
		}
	}
	else
	{
		// 지상에 있을때 사용가능 스킬 - 8개
		// Breath, Shout, HandPress, Scratch, TailSlap, JumpPress, ThunderMagic, Methor
		// 20		30		30			30		30			20		10				10
		//int_rand = FMath::RandRange(1, 180);
		// if (int_rand > 170)
		// {
		// 	//Methor
		// 	//Anim->ChangeAttackState(AttackState::Meteor);
		// 	Anim->ChangeState(DragonState::Idle);
		// }
		// else if (int_rand > 160)
		// {
		// 	//ThunderMagic
		// 	//Anim->ChangeAttackState(AttackState::ThunderMagic);
		// 	Anim->ChangeState(DragonState::Idle);
		// }
		//else if (int_rand > 140)
		
		int_rand = FMath::RandRange(1, 160);
		if (int_rand > 140)
		{
			//JumpPress
			Anim->ChangeAttackState(AttackState::JumpPress);
		}
		else if (int_rand > 110)
		{
			//TailSlap
			Anim->ChangeAttackState(AttackState::TailSlap);
			// Anim->InnerAngle = GetRadianFromCharacter();
			// Anim->chkAngle = true;
		}
		else if (int_rand > 80)
		{
			//Scratch
			Anim->ChangeAttackState(AttackState::Scratch);
			// Anim->InnerAngle = GetRadianFromCharacter();
			// Anim->chkAngle = true;
		}
		else if (int_rand > 50)
		{
			//HandPress
			Anim->ChangeAttackState(AttackState::HandPress);
		}
		else if (int_rand > 20)
		{
			//Shout
			Anim->ChangeAttackState(AttackState::Shout);
		}
		else if (int_rand > 0)
		{
			//Breath
			Anim->ChangeAttackState(AttackState::Breath);
		}
	}
}

void UHD_DragonFSM::RotateToTarget(const float& DeltaTime)
{
	FVector StartLoc = Dragon->GetActorLocation();
	FRotator curRot = Dragon->GetActorRotation();
	FVector TargetLoc = NearTargetActor->GetActorLocation();
	FRotator realRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, TargetLoc);


	al += DeltaTime / 10;


	if (al > 1)
	{
		al = 0;
		bRotate = false;
	}
	else
	{
		FRotator realrealROt = UKismetMathLibrary::RLerp(curRot, realRot, al, true);
		Dragon->SetActorRotation(realrealROt);
	}
}

void UHD_DragonFSM::BreathRStart(float Alpha)
{
	//Dragon->SetActorRotation(FRotator(NowRotator.Pitch, NowRotator.Yaw + Alpha * 6, NowRotator.Roll));

	// Value는 0에서 1까지의 값으로, 360도를 회전하도록 설정
	float NewYaw = FMath::Lerp(0.0f, 360.0f, Alpha);
	Dragon->SetActorRotation(FRotator(NowRotator.Pitch, NowRotator.Yaw + NewYaw, NowRotator.Roll));
}

void UHD_DragonFSM::BreathREnd()
{
}
