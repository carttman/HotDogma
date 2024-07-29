// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "HD_PlayerClimbComponent.generated.h"

// 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClimbMovement);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOTDOGMA_API UHD_PlayerClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHD_PlayerClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Player_Climb;
	UPROPERTY(EditAnywhere)
	class AHD_CharacterPlayer* Player;
	
	UPROPERTY(EditAnywhere)
	class AHD_Dragon* Dragon;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHD_Dragon> DragonClass;
public:
	FHitResult Climb_OutHit;
	void Climb();
	
	bool AttachToSurfaceCaculation(float Attach_Distance, FHitResult& OutHit);

	// 델리게이트 인스턴스
	UPROPERTY(BlueprintAssignable, Category = "CustomEvent")
	FClimbMovement OnClimbMovement;

	void ClimbMovementEvent(FVector WorldDirection, float ScaleValue);
	
public:
	UPROPERTY()
	UTimelineComponent* MyTimeline;

	UPROPERTY()
	FTimeline CurveTimeline;
	
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* FloatCurve;

	FOnTimelineFloat InterpFunction; // 타임라인의 값이 변경될 때 호출될 델리게이트
	FOnTimelineEvent TimelineFinished;  // 타임라인이 끝났을 때 호출될 델리게이트

	UFUNCTION()
	void ClimbTimelineProgress(float Value); // 타임라인 진행 함수

	UFUNCTION()
	void OnClimbTimelineFinished(); // 타임라인 종료 함수

	void StopClimbing();

	void AttachToCable();
};
