// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HD_DragonFSM.generated.h"

UENUM()
enum class DragonState:uint8
{
	Idle,			// 대기
	Shout,			// 포효
	Walk,			// 걷기
	Scratch,		// 할퀴기	
	TailSlap,		// 꼬리치기
	Bress,			// 브레스
	FlyUp,			// 이륙
	FlyDown,		// 착륙
	FlyPress,		// 날아 올랐다가 찍기
	FlyBress,		// 공중 브레스
	ThunderMagic,	// 전기마법공격
	Meteor,			// 메테오
	Groggy			// 그로기
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOTDOGMA_API UHD_DragonFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHD_DragonFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	DragonState State = DragonState::Idle;	//Default State를 Idle로 설정

	// 가까운 플레이어
	UPROPERTY(EditAnywhere)
	class AActor* NearTarget;
	
	// 내 위치
	UPROPERTY(EditAnywhere)
	class AActor* Dragon;
};
