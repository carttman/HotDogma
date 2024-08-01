// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerAnimInstance.h"
#include "HotDogma/LHJ/HD_Dragon.h"
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
	// 타겟의 위치를 업데이트
	
}

void UHD_PlayerAttackComponent::Cutting_GetTarget()
{
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation();
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel6;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Player);
	CuttingHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, 1000.f, UEngineTypes::ConvertToTraceType(CollisionChannel), false,
		ActorsToIgnore,EDrawDebugTrace::ForDuration,OutHits,true);

	for (auto& Hit : OutHits)
	{
		if (IsValid(Hit.GetActor()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s Cutting_TargetLoc : %.f, %.f, %.f"), *Hit.GetActor()->GetName(),Hit.GetActor()->GetActorLocation().X, Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z);
			FVector Cutting_TargetLoc = Hit.GetActor()->GetActorLocation(); //- Player->GetActorLocation();
			//UE_LOG(LogTemp, Warning, TEXT("Cutting_TargetLoc : %.f, %.f, %.f"), Cutting_TargetLoc.X, Cutting_TargetLoc.Y, Cutting_TargetLoc.Z);
			//UE_LOG(LogTemp, Warning, TEXT("Player->GetActorLocation() : %.f, %.f, %.f"), Player->GetActorLocation().X, Player->GetActorLocation().Y, Player->GetActorLocation().Z);
			FVector newLoc = Cutting_TargetLoc - Player->GetActorLocation();
			newLoc.Normalize();
			
			Cutting_Target_Rot = UKismetMathLibrary::MakeRotFromX(newLoc);
			//Cutting_Target_Rot = Cutting_TargetLoc;
			break;
		}
	}
}

void UHD_PlayerAttackComponent::PlayMontageNotifyBegin_Cutting(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if(NotifyName == FName("Cutting_Run_Start"))
	{
		FMotionWarpingTarget Cutting_MW_Target;
		Cutting_GetTarget();
		if(CuttingHit)
		{
			Player->SetActorRotation(Cutting_Target_Rot);
			Cutting_MW_Target.Rotation = Cutting_Target_Rot; //Player->GetActorRotation();
			UE_LOG(LogTemp, Warning, TEXT("Cutting_Target_Rot : %.f"), Cutting_Target_Rot.Yaw);
		}
		UE_LOG(LogTemp, Warning, TEXT("Notify %s has begun!"), *NotifyName.ToString());
		Cutting_MW_Target.Name = FName("Cutting_Boost");
		Cutting_MW_Target.Location = FVector( (Player->GetActorLocation().X + Player->GetActorForwardVector().X * 600) ,Player->GetActorLocation().Y + Player->GetActorForwardVector().Y * 600 ,Player->GetActorLocation().Z);
		Cutting_MW_Target.Rotation = Player->GetActorRotation();
		Player->MotionWarpingComponent->AddOrUpdateWarpTarget(Cutting_MW_Target);
		TargetFOV = 105;
		//만약 공격 범위 내에 타겟이 있다면
		//타겟 위치로 회전
		//if(Cutting_TargetLoc <= Cutting_Attack_Range)
	}
	if(NotifyName == FName("Cutting_Attack_Start")) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify %s has begun!"), *NotifyName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
		TargetFOV = 90;
	}
}
