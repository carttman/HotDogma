// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonFSM.h"

#include <random>

#include "AIController.h"
#include "HD_BreathCol.h"
#include "HD_Dragon.h"
#include "HD_DragonAnim.h"
#include "HD_DragonThunderCol.h"
#include "HD_Meteor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"

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

	FString myState = UEnum::GetValueOrBitfieldAsString(State);
	DrawDebugString(GetWorld(), Dragon->GetActorLocation(), myState, nullptr, FColor::Yellow, 0);
	if (Anim)
	{
		myState = UEnum::GetValueOrBitfieldAsString(Anim->AnimNormalAttackState);
		DrawDebugString(
			GetWorld(), FVector(Dragon->GetActorLocation().X, Dragon->GetActorLocation().Y,
			                    Dragon->GetActorLocation().Z - 50), myState, nullptr, FColor::Yellow, 0);
	}

	BreathTimeline.TickTimeline(DeltaTime);
	if (State != DragonState::Move)
		ai->StopMovement();


	if (NearTargetActor && !isAttack)
		RotateToTarget(DeltaTime);

	if (State == DragonState::Attack && normalAttackState == AttackState::Breath && bBreathAttack)
	{
		ProjectileBreathCollision(DeltaTime);
	}

	//공격중일 때는 상태 변환 x
	if (!isAttack)
	{
		if (RndAttackPattern.size() == 0)
		{
			ShuffleAttackPattern();
		}

		if (RndFlyAttackPattern.size() == 0)
		{
			ShuffleFlyAttackPattern();
		}

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
	// 일정거리 안에 플레이어가 들어오거나, 플레이어가 먼저 공격을 하면
	// SleepEnd 애니메이션 재생을 하고,
	// Idle 상태로 전환한다(노티파이 처리)
	if (!chkCharacterUsingSleep)
		ChkCharacterIntoRadian();
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

	// <><><> 공격 받는 부분에 어그로 이동 추가 (일단...보류)
	// <><><> Fly 수정중
	CurrIdleTime += DeltaTime;
	if (CurrIdleTime >= DuringIdleTime)
	{
		CurrIdleTime = 0.f;
		if (Dragon->MaxHP * 0.75 >= Dragon->CurrHP)
		{
			if (!chkOnceFly)
			{
				Anim->ChangeAttackState(AttackState::None);
				Anim->ChangeState(DragonState::Fly);
			}
			else
			{
				if (Anim->isFly)
				{
					F_NormalIdle(DeltaTime);
				}
				else
				{
					if (RequiredSkillCnt == CurrUsedSkillCnt)
					{
						Anim->ChangeAttackState(AttackState::None);
						Anim->ChangeState(DragonState::Fly);
					}
					else
					{
						F_NormalIdle(DeltaTime);
					}
				}
			}
		}
		else
		{
			F_NormalIdle(DeltaTime);
		}
	}
}

void UHD_DragonFSM::F_NormalIdle(const float& DeltaTime)
{
	// 타겟을 지정한다.
	ACharacter* ClosestCharacter = nullptr;
	float MinDistance = FLT_MAX;

	// 가까운 캐릭터를 공격대상으로 지정
	for (ACharacter* Character : Dragon->CharacterArr)
	{
		if (Dragon->CharacterArr.Num() > 1)
		{
			if (Character->GetName().Contains("Player"))
				continue;
		}

		float Distance = FVector::Dist(Character->GetActorLocation(), Dragon->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestCharacter = Character;
			NearTargetActor = ClosestCharacter;
		}
	}

	// 공격 범위 내에 들어오면
	if (NearTargetActor && MinDistance < AttackDist)
	{
		// 공중에서 스킬 사용개수 지정
		if (Anim->chkUsingSkillCnt && ApplySkillAsFly == 0)
			ApplySkillAsFly = FMath::RandRange(1, 2);

		// 공격 상태로 전이

		bool b = RotateToTarget(DeltaTime);
		if (b)
		{
			ChooseAttackState();
			Anim->ChangeState(DragonState::Attack);
		}
	}
	else
	{
		// 공격범위 밖이라면
		// 이동 상태로 전이
		Anim->ChangeState(DragonState::Move);
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

		float RadianValue = FMath::Acos(TargetDot);

		float DegreeValue = FMath::RadiansToDegrees(RadianValue);
		Anim->Direction = DegreeValue;

		//ai->MoveToActor(NearTargetActor);
		ai->MoveToLocation(NearTargetActor->GetActorLocation());

		float Distance = FVector::Dist(NearTargetActor->GetActorLocation(), Dragon->GetActorLocation());
		if (Distance <= AttackDist)
		{
			Anim->ChangeState(DragonState::Idle);
		}
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
	if (normalAttackState == AttackState::ThunderMagic)
	{
		if (bStartThunder)
			F_ThunderMagic(DeltaTime);
	}
	if (normalAttackState == AttackState::Meteor)
	{
		if (bStartMeteor)
			F_MeteorMagic(DeltaTime);
	}
	// if (normalAttackState == AttackState::Breath)
	// {
	// 	if (bBreathAttack)
	// 		ProjectileBreathCollision(DeltaTime);
	// }
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

		x = Dragon->GetActorLocation().X;
		y = Dragon->GetActorLocation().Y;
		z = Dragon->GetActorLocation().Z;

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

void UHD_DragonFSM::NormalBreath(const float& DeltaTime)
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

float UHD_DragonFSM::GetDegreeFromCharacter()
{
	FVector ForwardVec = Dragon->GetActorForwardVector();
	FVector CurrentLocation = Dragon->GetActorLocation();
	FVector TargetVec = NearTargetActor->GetActorLocation() - CurrentLocation;
	float TargetDot = FVector::DotProduct(ForwardVec, TargetVec);

	float RadianValue = FMath::Acos(TargetDot);

	float DegreeValue = FMath::RadiansToDegrees(RadianValue);
	//Anim->Direction = DegreeValue;
	return DegreeValue;
}
#pragma endregion


void UHD_DragonFSM::ShuffleAttackPattern()
{
	// Random number generator
	std::random_device rd;
	std::mt19937 g(rd());

	// 사용 스킬 목록을 복사
	RndAttackPattern = OrgAttackPattern;

	// Shuffle the vector
	std::shuffle(RndAttackPattern.begin(), RndAttackPattern.end(), g);
}

void UHD_DragonFSM::ShuffleFlyAttackPattern()
{
	// Random number generator
	std::random_device rd;
	std::mt19937 g(rd());

	// 사용 스킬 목록을 복사
	RndFlyAttackPattern = OrgFlyAttackPattern;

	// Shuffle the vector
	std::shuffle(RndFlyAttackPattern.begin(), RndFlyAttackPattern.end(), g);
}

void UHD_DragonFSM::ChooseAttackState()
{
	if (Anim->isFly)
	{
		// 공중날고 있을때 사용가능 스킬 - 3개
		// Breath, ThunderMagic, Meteor
		// 30 20 20
		//int_rand = FMath::RandRange(1, 70);
		// if (int_rand > 50)
		// {
		// 	//ThunderMagic
		// 	//Anim->ChangeAttackState(AttackState::ThunderMagic);
		// 	Anim->ChangeState(DragonState::Idle);
		// }
		// else if (int_rand > 30)
		// {
		// 	//Meteor
		// 	//Anim->ChangeAttackState(AttackState::Meteor);
		// 	Anim->ChangeState(DragonState::Idle);
		// }
		// else if (int_rand > 0)
		// if (int_rand > 0)
		// {
		// 	//Breath
		// 	Anim->ChangeAttackState(AttackState::Breath);
		// }

		if (RndFlyAttackPattern.size() > 0)
		{
			AttackState attack = RndFlyAttackPattern[0];
			Anim->ChangeAttackState(attack);
			RndFlyAttackPattern.erase(RndFlyAttackPattern.begin());
		}
		else
		{
			ShuffleFlyAttackPattern();
			AttackState attack = RndFlyAttackPattern[0];
			Anim->ChangeAttackState(attack);
			RndFlyAttackPattern.erase(RndFlyAttackPattern.begin());
		}
	}
	else
	{
		// 지상에 있을때 사용가능 스킬 - 8개
		// Breath, Shout, HandPress, Scratch, TailSlap, JumpPress, ThunderMagic, Meteor
		// 20		30		30			30		30			20		10				10
		//int_rand = FMath::RandRange(1, 180);
		// if (int_rand > 170)
		// {
		// 	//Meteor
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

		//=================================================

		// int_rand = FMath::RandRange(1, 160);
		// if (int_rand > 140)
		// {
		// 	//JumpPress
		// 	Anim->ChangeAttackState(AttackState::JumpPress);
		// }
		// else if (int_rand > 110)
		// {
		// 	//TailSlap
		// 	Anim->ChangeAttackState(AttackState::TailSlap);
		// 	// Anim->InnerAngle = GetRadianFromCharacter();
		// 	// Anim->chkAngle = true;
		// }
		// else if (int_rand > 80)
		// {
		// 	//Scratch
		// 	Anim->ChangeAttackState(AttackState::Scratch);
		// 	// Anim->InnerAngle = GetRadianFromCharacter();
		// 	// Anim->chkAngle = true;
		// }
		// else if (int_rand > 50)
		// {
		// 	//HandPress
		// 	Anim->ChangeAttackState(AttackState::HandPress);
		// }
		// else if (int_rand > 20)
		// {
		// 	//Shout
		// 	Anim->ChangeAttackState(AttackState::Shout);
		// }
		// else if (int_rand > 0)
		// {
		// 	//Breath
		// 	Anim->ChangeAttackState(AttackState::Breath);
		// }

		//=================================================TEST
		if (RndAttackPattern.size() > 0)
		{
			AttackState attack = RndAttackPattern[0];
			Anim->ChangeAttackState(attack);
			RndAttackPattern.erase(RndAttackPattern.begin());
		}
		else
		{
			ShuffleAttackPattern();
			AttackState attack = RndAttackPattern[0];
			Anim->ChangeAttackState(attack);
			RndAttackPattern.erase(RndAttackPattern.begin());
		}
	}
}

bool UHD_DragonFSM::RotateToTarget(const float& DeltaTime)
{
	FVector StartLoc = Dragon->GetActorLocation();
	FRotator curRot = Dragon->GetActorRotation();
	FVector TargetLoc = NearTargetActor->GetActorLocation();
	FRotator realRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, TargetLoc);
	bool EndRotate = false;

	al += DeltaTime / 100;

	float r = GetRadianFromCharacter();

	//UE_LOG(LogTemp, Warning, TEXT("%f"), r);
	if (al > 1 || (r > -0.5 && r <= 0.5))
	{
		al = 0;
		bRotate = false;
		EndRotate = true;
		if (Anim)
		{
			Anim->isRotate = false;
			Anim->InnerProductValue = 0;
		}
	}
	else
	{
		if (Anim)
		{
			Anim->isRotate = true;
			Anim->InnerProductValue = r;
		}
		FRotator realrealRot = UKismetMathLibrary::RLerp(curRot, realRot, al, true);
		Dragon->SetActorRotation(realrealRot);
	}

	return EndRotate;
}

void UHD_DragonFSM::BreathRStart(const float& Alpha)
{
	//Dragon->SetActorRotation(FRotator(NowRotator.Pitch, NowRotator.Yaw + Alpha * 6, NowRotator.Roll));

	// Value는 0에서 1까지의 값으로, 360도를 회전하도록 설정
	float NewYaw = FMath::Lerp(0.0f, 360.0f, Alpha);
	Dragon->SetActorRotation(FRotator(NowRotator.Pitch, NowRotator.Yaw + NewYaw, NowRotator.Roll));
}

void UHD_DragonFSM::BreathREnd()
{
}

void UHD_DragonFSM::ProjectileBreathCollision(const float& DeltaTime)
{
	auto FireTrans = Dragon->SkeletalComp->GetSocketTransform(FName("Fire_Socket"));
	AHD_BreathCol* BreathCol = GetWorld()->SpawnActor<AHD_BreathCol>(
		Breath_Projectile, FireTrans.GetLocation(), FRotator::ZeroRotator);
	BreathCol->SetTarget(FireTrans);
}

void UHD_DragonFSM::F_ThunderMagic(const float& DeltaTime)
{
	F_GetCharacterLoc_Casting();

	if (iCastingCnt == 0)
	{
		int_rand_Thunder = FMath::RandRange(1, 2);
		if (int_rand_Thunder == 1)
		{
			ThunderPatern.push_back(Dragon->ThunderPoint1->GetComponentLocation());
			ThunderPatern.push_back(Dragon->ThunderPoint3->GetComponentLocation());
			ThunderPatern.push_back(Dragon->ThunderPoint5->GetComponentLocation());
			ThunderPatern.push_back(Dragon->ThunderPoint7->GetComponentLocation());
		}
		else
		{
			ThunderPatern.push_back(Dragon->ThunderPoint2->GetComponentLocation());
			ThunderPatern.push_back(Dragon->ThunderPoint4->GetComponentLocation());
			ThunderPatern.push_back(Dragon->ThunderPoint6->GetComponentLocation());
			ThunderPatern.push_back(Dragon->ThunderPoint8->GetComponentLocation());
		}

		// 처음 떨어지는 번개는
		// 드래곤 주변 4곳과 플레이어 위치에 떨어트리고 싶다.
		for (auto ThunderPoint : ThunderPatern)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Dragon->ThunderVFX1, ThunderPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Dragon->ThunderVFX2, ThunderPoint);
			if (ThunderCol)
				GetWorld()->SpawnActor<AHD_DragonThunderCol>(ThunderCol, ThunderPoint, FRotator::ZeroRotator);
		}

		for (auto ThunderPoint : CastingAttack_CharacterLoc)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Dragon->ThunderVFX1, ThunderPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Dragon->ThunderVFX2, ThunderPoint);
			if (ThunderCol)
				GetWorld()->SpawnActor<AHD_DragonThunderCol>(ThunderCol, ThunderPoint, FRotator::ZeroRotator);
		}

		ThunderPatern.clear();
		iCastingCnt++;
	}
	else
	{
		if (iCastingCnt < 4)
		{
			CurrThunderTime += DeltaTime;
			if (CurrThunderTime >= MakeThunderTime)
			{
				CurrThunderTime = 0;
				iCastingCnt++;
				for (auto ThunderPoint : CastingAttack_CharacterLoc)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Dragon->ThunderVFX1, ThunderPoint);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Dragon->ThunderVFX2, ThunderPoint);
					if (ThunderCol)
						GetWorld()->SpawnActor<AHD_DragonThunderCol>(ThunderCol, ThunderPoint, FRotator::ZeroRotator);
				}

				if (iCastingCnt == 4)
				{
					bStartThunder = false;

					if (Dragon)
						Dragon->strDamageAttackType = "";
				}
			}
		}
	}
}

void UHD_DragonFSM::F_GetCharacterLoc_Casting()
{
	CastingAttack_CharacterLoc.clear();

	for (auto charac : Dragon->CharacterArr)
	{
		FVector CharacLoc = charac->GetActorLocation();
		CastingAttack_CharacterLoc.push_back(FVector(CharacLoc.X, CharacLoc.Y, 0));
	}
}

void UHD_DragonFSM::F_MeteorMagic(const float& DeltaTime)
{
	F_GetCharacterLoc_Casting();

	//FVector SpMeteorLoc = F_GetSpawnMeteorLoc();

	if (iCastingCnt < 4)
	{
		CurrMeteorTime += DeltaTime;
		if (CurrMeteorTime >= MakeMeteorTime)
		{
			CurrMeteorTime = 0.f;
			for (auto MeteorPoint : CastingAttack_CharacterLoc)
			{
				AHD_Meteor* meteor_prj = GetWorld()->SpawnActor<AHD_Meteor>(
					Meteor_Projectile, MeteorPoint, FRotator::ZeroRotator);
				meteor_prj->SetTarget(MeteorPoint);
			}

			iCastingCnt++;
		}
	}


	// if (iCastingCnt < 4)
	// {
	// 	CurrMeteorTime += DeltaTime;
	// 	if (CurrMeteorTime >= MakeMeteorTime)
	// 	{
	// 		CurrMeteorTime = 0.f;
	// 		for (auto MeteorPoint : CastingAttack_CharacterLoc)
	// 		{
	// 			UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	// 				this,
	// 				MeteorVFX,
	// 				MeteorPoint,
	// 				MeteorPoint.Rotation()
	// 			);
	// 		}
	// 		iCastingCnt++;
	// 	}
	// }
}

FVector UHD_DragonFSM::F_GetSpawnMeteorLoc()
{
	int rndLoc = FMath::RandRange(1, 4);
	FVector rtnVec = FVector::ZeroVector;

	switch (rndLoc)
	{
	case 1:
		rtnVec = Dragon->MeteorPoint1->GetComponentLocation();
		break;
	case 2:
		rtnVec = Dragon->MeteorPoint2->GetComponentLocation();
		break;
	case 3:
		rtnVec = Dragon->MeteorPoint3->GetComponentLocation();
		break;
	case 4:
		rtnVec = Dragon->MeteorPoint4->GetComponentLocation();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("%d"), rndLoc);
		break;
	}

	return rtnVec;
}
