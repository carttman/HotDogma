// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerAnimInstance.h"


UHD_PlayerAttackComponent::UHD_PlayerAttackComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



void UHD_PlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AHD_CharacterBase>(GetOwner());
	PlayerAnim = Cast<UHD_PlayerAnimInstance>(Player->GetMesh()->GetAnimInstance());
	// ...
	
}


// Called every frame
void UHD_PlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePlayerAttack(DeltaTime);
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
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	PlayerAnim->Montage_Play(AM_Splitter, 1);
}


