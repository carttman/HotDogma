// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerAnimInstance.h"
#include "HotDogma/HD_Character/HD_PlayerItem/HD_PlayerWeaponBase.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


UHD_PlayerAttackComponent::UHD_PlayerAttackComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/CHJ/Input/IA_Player_Attack.IA_Player_Attack'"));
	if(TempIA_Attack.Succeeded())
	{
		IA_HD_Attack = TempIA_Attack.Object; // ia 셋팅
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Skill(TEXT("/Script/EnhancedInput.InputAction'/Game/CHJ/Input/IA_Player_Skill.IA_Player_Skill'"));
	if(TempIA_Skill.Succeeded())
	{
		IA_HD_Skill = TempIA_Skill.Object; // ia 셋팅
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> Temp_SPMontage(TEXT("/Script/Engine.AnimMontage'/Game/CHJ/Player_Animation/Attack/Skill_Splitter/AM_Splitter.AM_Splitter'"));
	if(Temp_SPMontage.Succeeded())
	{
		AM_Splitter = Temp_SPMontage.Object; 
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> Temp_BAMontage(TEXT("/Script/Engine.AnimMontage'/Game/CHJ/Player_Animation/Attack/BaseAttack/AM_BaseAttack.AM_BaseAttack'"));
	if(Temp_BAMontage.Succeeded())
	{
		AM_BaseAttack = Temp_BAMontage.Object; 
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> Temp_SCMontage(TEXT("/Script/Engine.AnimMontage'/Game/CHJ/Player_Animation/Attack/Cutting_Wind/AM_Cutting_Wind.AM_Cutting_Wind'"));
	if(Temp_SCMontage.Succeeded())
	{
		AM_Cutting = Temp_SCMontage.Object; 
	}
}



void UHD_PlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AHD_CharacterBase>(GetOwner());
	CharacterPlayer = Cast<AHD_CharacterPlayer>(Player);
	//Left_Weapon = Cast<AHD_PlayerWeaponBase>(CharacterPlayer->Left_Weapon);
	//Right_Weapon = Cast<AHD_PlayerWeaponBase>(CharacterPlayer->Right_Weapon);
	PlayerAnim = Cast<UHD_PlayerAnimInstance>(Player->GetMesh()->GetAnimInstance());
	Dragon = Cast<AHD_Dragon>(Player->Dragon);
	// ...
	if(AM_Splitter) PlayerAnim->OnPlayMontageNotifyBegin.AddDynamic(this, &UHD_PlayerAttackComponent::PlayMontageNotifyBegin_Splitter);
	if(AM_Cutting) PlayerAnim->OnPlayMontageNotifyBegin.AddDynamic(this, &UHD_PlayerAttackComponent::PlayMontageNotifyBegin_Cutting);
}


// Called every frame
void UHD_PlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePlayerAttack(DeltaTime);
	if(IsSplitting)Update_Skill_Splitter();
	if(IsCutting)Update_Skill_Cuttring();
	Player->camera->FieldOfView = FMath::Lerp(Player->camera->FieldOfView, TargetFOV, DeltaTime * 7);
}

void UHD_PlayerAttackComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(IA_HD_Attack, ETriggerEvent::Started, this, &UHD_PlayerAttackComponent::PlayerAttack);
	enhancedInputComponent->BindAction(IA_HD_Skill, ETriggerEvent::Started, this, &UHD_PlayerAttackComponent::EnhancedSkill);
}



void UHD_PlayerAttackComponent::EnhancedSkill(const FInputActionValue& InputActionValue)
{
	float value = InputActionValue.Get<float>();
	if (FMath::IsNearlyEqual(value, 1.f))
	{
		Skill_Splitter();
	}
	else if (FMath::IsNearlyEqual(value, 2.f))
	{
		Skill_Cutting();
	}
	else if (FMath::IsNearlyEqual(value, 3.f))
	{
	}
	else if (FMath::IsNearlyEqual(value, 4.f))
	{
	}
}

void UHD_PlayerAttackComponent::PlayerAttack()
{
	if (ComboCount == 0)
	{
		ComboCount++;
		CurrComboTime = 0;

		// 단검 아이템에 있는 애님몽타주 실행
		Player->PlayAnimMontage(AM_BaseAttack, 1, FName("Attack_1"));
	}
	else if (ComboCount == 1)
	{
		// 콤보최소시간 <= 현재시간 이고 현재시간 <= 최대시간
		if (MinComboTime <= CurrComboTime && CurrComboTime <= MaxComboTime)
		{
			ComboCount++;
			CurrComboTime = 0;

			// 단검 아이템에 있는 애님몽타주 실행
			Player->PlayAnimMontage(AM_BaseAttack, 1, FName("Attack_2"));
		}
	}
	else if (ComboCount == 2)
	{
		// 콤보최소시간 <= 현재시간 이고 현재시간 <= 최대시간
		if (MinComboTime <= CurrComboTime && CurrComboTime <= MaxComboTime)
		{
			ComboCount++;
			CurrComboTime = 0;

			// 단검 아이템에 있는 애님몽타주 실행
			Player->PlayAnimMontage(AM_BaseAttack, 1, FName("Attack_3"));
		}
	}
	else if (ComboCount == 3)
	{
		// 콤보최소시간 <= 현재시간 이고 현재시간 <= 최대시간
		if (MinComboTime <= CurrComboTime && CurrComboTime <= MaxComboTime)
		{
			ComboCount++;
			CurrComboTime = 0;

			// 단검 아이템에 있는 애님몽타주 실행
			Player->PlayAnimMontage(AM_BaseAttack, 1, FName("Attack_4"));
		}
	}
	else if (ComboCount == 4)
	{
		// 콤보최소시간 <= 현재시간 이고 현재시간 <= 최대시간
		if (MinComboTime <= CurrComboTime && CurrComboTime <= MaxComboTime)
		{
			ComboCount++;
			CurrComboTime = 0;

			// 단검 아이템에 있는 애님몽타주 실행
			Player->PlayAnimMontage(AM_BaseAttack, 1, FName("Attack_5"));
		}
	}
	
}

void UHD_PlayerAttackComponent::UpdatePlayerAttack(float DeltaTime)
{
	// 콤보 카운트가 0보다 크면 (콤보 시작)
	if (ComboCount > 0)
	{
		//콤보 현재시간 = 콤보 현재시간 + 델타타임 -> 콤보 현재시간을 증가시켜라
		CurrComboTime += DeltaTime;
		//콤보 현재 시간이 최대시간보다 커졌을 때
		if (CurrComboTime > MaxComboTime)
		{
			// 콤보 카운트를 초기화 시켜라 -> 콤보 처음부터 시작하기 위해서
			ComboCount = 0;
		}
	}
}

void UHD_PlayerAttackComponent::Skill_Splitter()
{	// 루트모션을 위한 flying 모드 변경
	IsSplitting = true;
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	PlayerAnim->Montage_Play(AM_Splitter, 1);
}

void UHD_PlayerAttackComponent::Update_Skill_Splitter()
{
	if(Player->GetCharacterMovement()->Velocity.IsZero()) // 착지했을 때
	{
		PlayerAnim->Montage_JumpToSection(FName("Splitter_End"), AM_Splitter);
		Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		Player->GetCharacterMovement()->GravityScale = 1.75f;
		IsSplitting = false;
	}
}

void UHD_PlayerAttackComponent::PlayMontageNotifyBegin_Splitter(FName NotifyName,
                                                                const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if(NotifyName == FName("Splitter_Start"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify %s has begun!"), *NotifyName.ToString());
	}
	if(NotifyName == FName("Falling")) //롤링 시작할 때
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify %s has begun!"), *NotifyName.ToString());
		Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		Player->GetCharacterMovement()->GravityScale = 0.5f;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
		TargetFOV = 105;
	}
	if(NotifyName == FName("Splitter_Land"))
	{
		TargetFOV = 90;
	}
	
}



void UHD_PlayerAttackComponent::Skill_Cutting()
{
	PlayerAnim->Montage_Play(AM_Cutting, 1);
}

void UHD_PlayerAttackComponent::Update_Skill_Cuttring()
{
	FHitResult HitResult;
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation();
	TArray<AActor*> ActorsToIgnore;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6);
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),Start, End, 100.f, TraceType,false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 3.0f);
	if(bHit)
	{
		PlayerAnim->Montage_JumpToSection(FName("Cutting_Attack"), AM_Cutting);
	}
}

void UHD_PlayerAttackComponent::Cutting_GetTarget()
{
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation();
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel6;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Player);
	CuttingHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, 900.f, UEngineTypes::ConvertToTraceType(CollisionChannel), false,
		ActorsToIgnore,EDrawDebugTrace::ForDuration,OutHits,true);

	for (auto& Hit : OutHits)
	{
		if (IsValid(Hit.GetActor()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s Cutting_TargetLoc : %.f, %.f, %.f"), *Hit.GetActor()->GetName(),Hit.GetActor()->GetActorLocation().X, Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z);
			FVector newLoc = Hit.GetActor()->GetActorLocation() - Player->GetActorLocation();//히트 된 상대의 위치
			newLoc.Normalize();
			Cutting_Target_Rot = UKismetMathLibrary::MakeRotFromX(newLoc); //타겟의 방향벡터
			// if(UKismetMathLibrary::Vector_Distance(Hit.GetActor()->GetActorLocation(), Player->GetActorLocation()) <= Cutting_Attack_Range)
			// {	//근처에 타겟 있으면 바로 공격 시작
			// 	PlayerAnim->Montage_JumpToSection(FName("Cutting_Attack"), AM_Cutting);
			// }
			break;
		}
	}
}


void UHD_PlayerAttackComponent::PlayMontageNotifyBegin_Cutting(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if(NotifyName == FName("Cutting_Run_Start"))
	{
		IsCutting = true;
		Cutting_GetTarget();
		FMotionWarpingTarget Cutting_MW_Target;
		if(CuttingHit)
		{
			Player->SetActorRotation(FRotator(Cutting_Target_Rot.Pitch, Cutting_Target_Rot.Yaw * 1.3, Cutting_Target_Rot.Roll));
		}
		
		Cutting_MW_Target.Name = FName("Cutting_Boost");
		Cutting_MW_Target.Location = FVector( (Player->GetActorLocation().X + Player->GetActorForwardVector().X * 600) ,Player->GetActorLocation().Y + Player->GetActorForwardVector().Y * 600 ,Player->GetActorLocation().Z);
		//Cutting_MW_Target.Rotation = Player->GetActorRotation();
		Player->MotionWarpingComponent->AddOrUpdateWarpTarget(Cutting_MW_Target);
		
		TargetFOV = 105;
	}
	if(NotifyName == FName("Cutting_Attack_Start")) 
	{
		FMotionWarpingTarget Cutting_MW_Closed_Target;
		Cutting_MW_Closed_Target.Name = FName("Cutting_Closed_Target");
		Cutting_MW_Closed_Target.Location = FVector( (Player->GetActorLocation().X + Player->GetActorForwardVector().X * 400) ,Player->GetActorLocation().Y + Player->GetActorForwardVector().Y * 400 ,Player->GetActorLocation().Z);
		//Cutting_MW_Closed_Target.Rotation = Player->GetActorRotation();
		Player->MotionWarpingComponent->AddOrUpdateWarpTarget(Cutting_MW_Closed_Target);
		// 시간을 0.5배로 0.1초간 느리게 설정
		//SlowDownTime(0.5f, 0.1f);
		TargetFOV = 90;
	}
	if(NotifyName == FName("Cutting_Tick_End"))
	{
		IsCutting = false;
	}
	if(NotifyName == FName("Damage_On_Cutter"))
	{
		CharacterPlayer->Left_Weapon->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CharacterPlayer->Right_Weapon->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//Right_Weapon->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if(NotifyName == FName("Damage_Off_Cutter"))
	{
		CharacterPlayer->Left_Weapon->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterPlayer->Right_Weapon->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UHD_PlayerAttackComponent::SlowDownTime(float SlowDownFactor, float Duration)
{
	// 현재 글로벌 타임 딜레이션 저장
	OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	// 새로운 글로벌 타임 딜레이션 설정
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowDownFactor);

	// 원래 타임 딜레이션을 복원하는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(TimeDilationTimerHandle, this, &UHD_PlayerAttackComponent::RestoreTimeDilation, Duration, false);

}

void UHD_PlayerAttackComponent::RestoreTimeDilation()
{
	// 원래 글로벌 타임 딜레이션 복원
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTimeDilation);
}
