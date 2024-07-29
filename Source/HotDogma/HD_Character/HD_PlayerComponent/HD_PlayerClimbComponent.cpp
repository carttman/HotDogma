// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerClimbComponent.h"

#include "CableComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotDogma/HD_Character/HD_CharacterBase.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/LHJ/HD_Dragon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UHD_PlayerClimbComponent::UHD_PlayerClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA(TEXT("/Script/EnhancedInput.InputAction'/Game/CHJ/Input/IA_Player_Climb.IA_Player_Climb'"));
	if(TempIA.Succeeded())
	{
		IA_Player_Climb = TempIA.Object;
	}
	
	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeline"));  // 타임라인 컴포넌트 생성
	// 타임라인이 초기화될 때 호출되는 델리게이트 설정
	InterpFunction.BindUFunction(this, FName("ClimbTimelineProgress")); // 타임라인 진행 함수 바인딩
	TimelineFinished.BindUFunction(this, FName("OnClimbTimelineFinished")); // 타임라인 종료 함수 바인딩

}


// Called when the game starts
void UHD_PlayerClimbComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is: %s"), *Owner->GetName());

		Player = Cast<AHD_CharacterPlayer>(Owner);
		if(Player) UE_LOG(LogTemp, Warning, TEXT("Player cast successful"))
		else UE_LOG(LogTemp, Warning, TEXT("Player cast failed"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("GetOwner() returned null"));

	
	Dragon = Cast<AHD_Dragon>(UGameplayStatics::GetActorOfClass(GetWorld(), DragonClass));
	if(Dragon)UE_LOG(LogTemp, Warning, TEXT("This is: %s"), *Dragon->GetName())
	else UE_LOG(LogTemp, Warning, TEXT("This is: None "));
	
	if (FloatCurve)
	{
		MyTimeline->AddInterpFloat(FloatCurve, InterpFunction); // 커브와 델리게이트 연결
		MyTimeline->SetTimelineFinishedFunc(TimelineFinished); // 타임라인 종료 델리게이트 설정
		MyTimeline->SetLooping(false);  // 타임라인이 반복되지 않도록 설정
		MyTimeline->SetIgnoreTimeDilation(true); // 시간 변형을 무시하도록 설정
		
	}
}


// Called every frame
void UHD_PlayerClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr); // 타임라인을 매 프레임마다 업데이트

	// if(Climb_OutHit.GetActor() == Dragon) //Dragon이 맞았다면
	// {
	// 	FVector TargetLocation = Player->GetCapsuleComponent()->GetScaledCapsuleRadius() * Climb_OutHit.Normal + Climb_OutHit.Location;
	// 	FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(Climb_OutHit.Normal * -1);
	// 	bool bEaseIn = false;
	// 	bool bEaseOut = false;
	// 	float OverTime = 0.2f;
	// 	bool bForceShortestRotationPath = false;
	// 	
	// 	FLatentActionInfo LatentInfo;
	// 	LatentInfo.CallbackTarget = this; // 현재 클래스의 인스턴스를 콜백 타겟으로 설정
	// 	
	// 	//Player->SetActorLocation(TargetLocation);
	// 	UKismetSystemLibrary::MoveComponentTo(Player->GetCapsuleComponent(),Climb_OutHit.TraceEnd, TargetRotation,bEaseIn,bEaseOut,
	// 		OverTime,bForceShortestRotationPath, EMoveComponentAction::Move,LatentInfo);
	// }
}

void UHD_PlayerClimbComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(IA_Player_Climb, ETriggerEvent::Started, this,
	                                   &UHD_PlayerClimbComponent::Climb);
	
}

void UHD_PlayerClimbComponent::Climb()
{
	
	if(Player->GetCharacterMovement()->MovementMode != MOVE_Flying)
	{	
		if(AttachToSurfaceCaculation(200.f, Climb_OutHit)) //닿았다면
		{
			Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			Player->GetCharacterMovement()->bOrientRotationToMovement = false;
			
			FVector TargetLocation;
			if(Climb_OutHit.GetActor() == Dragon) //드래곤이 맞았다면
			{	UE_LOG(LogTemp, Warning, TEXT("Hit Dragon"));
				TargetLocation = Player->GetCapsuleComponent()->GetScaledCapsuleRadius() * Climb_OutHit.Normal + Climb_OutHit.Location;
				//Player->CableCompoent->EndLocation = Climb_OutHit.ImpactPoint;
				//Player->GetCapsuleComponent()->AttachToComponent(Dragon->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName(Climb_OutHit.BoneName));
				Player->CableCompoent->SetAttachEndToComponent(Dragon->GetMesh(), Climb_OutHit.BoneName);
			}
			else
			{
				//Player->CableCompoent->EndLocation = Climb_OutHit.ImpactPoint;
				TargetLocation = Player->GetCapsuleComponent()->GetScaledCapsuleRadius() * Climb_OutHit.Normal + Climb_OutHit.Location;
			}
			FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(Climb_OutHit.Normal * -1);
			bool bEaseIn = false;
			bool bEaseOut = false;
			float OverTime = 0.2f;
			bool bForceShortestRotationPath = false;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this; // 현재 클래스의 인스턴스를 콜백 타겟으로 설정
			//LatentInfo.ExecutionFunction = FName("OnMoveCompleted"); // 완료 시 호출할 함수 이름
			//LatentInfo.Linkage = 0;
			//LatentInfo.UUID = 1;

			UKismetSystemLibrary::MoveComponentTo(Player->GetCapsuleComponent(),TargetLocation,TargetRotation,bEaseIn,bEaseOut,
				OverTime,bForceShortestRotationPath, EMoveComponentAction::Move,LatentInfo);
		}
	}
	else StopClimbing();
}
//벽에 닿았는가?
bool UHD_PlayerClimbComponent::AttachToSurfaceCaculation(float Attach_Distance, FHitResult& OutHit)
{
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + Player->GetActorForwardVector() * Attach_Distance;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams params;
	
	params.AddIgnoredActor(Player);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, params);
	
	if(bHit)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
	}
	return bHit; //hit 성공여부 리턴
}

void UHD_PlayerClimbComponent::ClimbMovementEvent(FVector WorldDirection, float ScaleValue)
{
	//딜리게이트
	//OnClimbMovement.Broadcast();
	//trace 성공했다면
	if(AttachToSurfaceCaculation(120.f, Climb_OutHit))
	{
		Player->AddMovementInput(WorldDirection, ScaleValue);
		// 타임라인 시작 (원하는 시점에 호출 가능)
		MyTimeline->PlayFromStart();
	}
	else StopClimbing();
}

void UHD_PlayerClimbComponent::ClimbTimelineProgress(float Value) // output value
{
	FRotator NewRot = UKismetMathLibrary::RLerp(Player->GetActorRotation(),
		UKismetMathLibrary::MakeRotFromX(Climb_OutHit.Normal * -1.0f), Value, false);
	//UE_LOG(LogTemp, Warning, TEXT("%.2f"), Value);
	//벽 타고 있을 때 방향 회전
	Player->SetActorRotation(NewRot);
}

void UHD_PlayerClimbComponent::OnClimbTimelineFinished()
{
}

void UHD_PlayerClimbComponent::StopClimbing()
{
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	Player->GetCharacterMovement()->bOrientRotationToMovement = true;
	Player->SetActorRotation(FRotator(0, Player->GetActorRotation().Yaw,  0));
}

void UHD_PlayerClimbComponent::AttachToCable()
{
	FHitResult OutHit;
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + Player->GetActorForwardVector() * 200;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams params;
	
	params.AddIgnoredActor(Player);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, params);
	
	if(bHit)
	{
		Player->CableCompoent->EndLocation = OutHit.ImpactPoint - Player->GetActorLocation();
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Purple, false, 3.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
	}
}


