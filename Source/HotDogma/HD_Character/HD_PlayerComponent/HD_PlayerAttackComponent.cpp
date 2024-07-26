// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerAttackComponent.h"

#include "EnhancedInputComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"

// Sets default values for this component's properties
UHD_PlayerAttackComponent::UHD_PlayerAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHD_PlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AHD_CharacterBase>(GetOwner());
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


