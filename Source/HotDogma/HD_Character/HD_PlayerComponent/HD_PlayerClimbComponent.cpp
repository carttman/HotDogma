// Fill out your copyright notice in the Description page of Project Settings.


#include "../../HD_Character/HD_PlayerComponent/HD_PlayerClimbComponent.h"

// #include "CableComponent.h"
// #include "EnhancedInputComponent.h"
// #include "HD_PlayerAttackComponent.h"
// #include "MotionWarpingComponent.h"
// #include "Components/CapsuleComponent.h"
// #include "GameFramework/CharacterMovementComponent.h"
// #include "HotDogma/HD_Character/HD_CharacterBase.h"
// #include "HotDogma/HD_Character/HD_CharacterPlayer.h"
// #include "HotDogma/HD_Character/HD_PlayerAnimInstance.h"
// #include "HotDogma/LHJ/HD_Dragon.h"
// #include "Kismet/KismetMathLibrary.h"
// #include "Kismet/KismetSystemLibrary.h"

UHD_PlayerClimbComponent::UHD_PlayerClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// ConstructorHelpers::FObjectFinder<UInputAction> TempIA(TEXT("/Script/EnhancedInput.InputAction'/Game/CHJ/Input/IA_Player_Climb.IA_Player_Climb'"));
	// if(TempIA.Succeeded())
	// {
	// 	IA_Player_Climb = TempIA.Object; // ia 셋팅
	// }
	// ConstructorHelpers::FObjectFinder<UAnimMontage> Temp_CMontage(TEXT("/Script/Engine.AnimMontage'/Game/CHJ/Player_Animation/Move/Climb/AM_Ledge.AM_Ledge'"));
	// if(Temp_CMontage.Succeeded())
	// {
	// 	LedgeMontage = Temp_CMontage.Object;
	// }
	//
	// MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeline"));  // 타임라인 컴포넌트 생성
	// // 타임라인이 초기화될 때 호출되는 델리게이트 설정
	// InterpFunction.BindUFunction(this, FName("ClimbTimelineProgress")); // 타임라인 진행 함수 바인딩
	// TimelineFinished.BindUFunction(this, FName("OnClimbTimelineFinished")); // 타임라인 종료 함수 바인딩
}

void UHD_PlayerClimbComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// AActor* Owner = GetOwner();
	// if (Owner) // 플레이어, 플레이어 애님인스턴스 셋팅
	// {
	// 	//UE_LOG(LogTemp, Warning, TEXT("Owner is: %s"), *Owner->GetName());
	// 	Player = Cast<AHD_CharacterPlayer>(Owner);
	// 	PlayerAnim = Cast<UHD_PlayerAnimInstance>(Player->GetMesh()->GetAnimInstance());
	// 	if(Player) UE_LOG(LogTemp, Warning, TEXT("Player cast successful"))
	// 	else UE_LOG(LogTemp, Warning, TEXT("Player cast failed"));
	// }
	// else UE_LOG(LogTemp, Warning, TEXT("GetOwner() returned null"));
	//
	// // 타임라인 커브가 있다면
	// if (FloatCurve)
	// {
	// 	MyTimeline->AddInterpFloat(FloatCurve, InterpFunction); // 커브와 델리게이트 연결
	// 	MyTimeline->SetTimelineFinishedFunc(TimelineFinished); // 타임라인 종료 델리게이트 설정
	// 	MyTimeline->SetLooping(false);  // 타임라인이 반복되지 않도록 설정
	// 	MyTimeline->SetIgnoreTimeDilation(true); // 시간 변형을 무시하도록 설정
	// }
	// if(LedgeMontage)
	// {
	// 	PlayerAnim->OnPlayMontageNotifyBegin.AddDynamic(this, &UHD_PlayerClimbComponent::PlayMontageNotifyBegin);
	// }
}


// Called every frame
void UHD_PlayerClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr); // 타임라인을 매 프레임마다 업데이트
	// if(Dragon == nullptr)
	// {
	// 	Dragon = Cast<AHD_Dragon>(Player->Dragon);
	// 	if(Dragon)UE_LOG(LogTemp, Warning, TEXT("TO ClimbComp-> This is: %s"), *Dragon->GetName());
	// }
}

void UHD_PlayerClimbComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	// enhancedInputComponent->BindAction(IA_Player_Climb, ETriggerEvent::Started, this,
	//                                    &UHD_PlayerClimbComponent::Climb);
}

//void UHD_PlayerClimbComponent::Climb()
//{
	// if(Player->PlayerAttackComponent->IsSplitting || Player->PlayerAttackComponent->IsCutting) return;
	// // Flying 모드(벽타기 상태)가 아니라면 벽타기 진입
	// if(Player->GetCharacterMovement()->MovementMode != MOVE_Flying) 
	// {
	// 	if(IsClimbing) return;
	// 	//Attach_Distance만큼 라.트 쐈을때, 벽에 닿았다면
	// 	if(AttachToSurfaceCaculation(200.f, Climb_OutHit))
	// 	{	// Flying 모드로 변경
	// 		IsClimbing = true;
	// 		if(IsClimbing)
	// 		Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	// 		Player->GetCharacterMovement()->bOrientRotationToMovement = false;
	// 		
	// 		FVector TargetLocation;
	// 		FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(Climb_OutHit.Normal * -1); // 회전값 : 트레이스 노말의 반대방향(앞방향)의 Rotation
	// 		
	// 		FLatentActionInfo LatentInfo; //MoveComponentTo 인자중 하나
	// 		LatentInfo.CallbackTarget = this; // 현재 클래스의 인스턴스를 콜백 타겟으로 설정
	// 		//LatentInfo.ExecutionFunction = FName("OnMoveCompleted"); // 완료 시 호출할 함수 이름
	// 		//LatentInfo.Linkage = 0;
	// 		//LatentInfo.UUID = 1;
	// 		if(Climb_OutHit.GetActor() == Dragon) //드래곤이 맞았다면
	// 		{	UE_LOG(LogTemp, Warning, TEXT("Hit Dragon")); // 위치값 : 캡슐 컴포넌트 둘레? * 트레이스 노말 + 충돌된 위치
	// 			//TargetLocation = Player->GetCapsuleComponent()->GetScaledCapsuleRadius() + Player->GetCapsuleComponent()->SetRelativeTransform();
	// 			FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
	// 			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false);
	//
	// 			Player->GetCapsuleComponent()->AttachToComponent(Dragon->SkeletalComp, AttachmentTransformRules, Climb_OutHit.BoneName);
	// 			//Climb_OutHit.BoneName
	// 			// UKismetSystemLibrary::MoveComponentTo(Player->GetCapsuleComponent(),TargetLocation,TargetRotation,false,false,
	// 			// 0.2f,false, EMoveComponentAction::Move,LatentInfo);
	// 			Player->PlayerAttackComponent->TargetFOV = 120;
	// 		}
	// 		else TargetLocation = Player->GetCapsuleComponent()->GetRelativeLocation() * Climb_OutHit.Normal + Climb_OutHit.Location;
	// 		// 트레이스 충돌된 위치,회전값으로 플레이어 이동
	// 		UKismetSystemLibrary::MoveComponentTo(Player->GetCapsuleComponent(),TargetLocation,TargetRotation,false,false,
	// 			0.2f,false, EMoveComponentAction::Move,LatentInfo);
	// 	}
	// }
	// else StopClimbing(); //벽타기 취소
//}
//벽에 닿았는가? 를 계산하는 bool 함수 / OutHit 를 레퍼런스로 넘겨줌
//bool UHD_PlayerClimbComponent::AttachToSurfaceCaculation(float Attach_Distance, FHitResult& OutHit)
//{	
// 	FVector Start = Player->GetActorLocation();
// 	FVector End = Player->GetActorLocation() + Player->GetActorForwardVector() * Attach_Distance;
// 	ECollisionChannel TraceChannel = ECC_GameTraceChannel5; //ECC_GameTraceChannel5는 Climb Trace 채널임
// 	FCollisionQueryParams params;
// 	params.AddIgnoredActor(Player);
// 	
// 	//벽 앞에 있는지 판단하기 위한 라인 트레이스
// 	bHit_AttachToSurfaceCaculation = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, params);
// 	//if(bHit_AttachToSurfaceCaculation) DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f);
// 	//else DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
// 	
// 	return bHit_AttachToSurfaceCaculation; //충돌 됐는지 여부 넘겨줌
// }
// ClimbMovementEvent : Flying상태일 시, Character 무브먼트 컴포넌트에 넘겨줄 함수 
// void UHD_PlayerClimbComponent::ClimbMovementEvent(FVector WorldDirection, float ScaleValue)
// {
// 	//OnClimbMovement.Broadcast(); //딜리게이트
// 	//trace 성공했다면
// 	if(AttachToSurfaceCaculation(120.f, Climb_OutHit))
// 	{	// 플레이어 이동
// 		Player->AddMovementInput(WorldDirection, ScaleValue);
// 		//Ledge 할수있는지 계산하는 함수
// 		LedgeMantleCaculation();
// 		// 타임라인 시작 (원하는 시점에 호출 가능)
// 		MyTimeline->PlayFromStart();
// 	}
// 	else LedgeMantleCaculation();
// 	//else StopClimbing();
// }
// 타임라인이 진행될때 실행
// void UHD_PlayerClimbComponent::ClimbTimelineProgress(float Value) // output value
// {
// 	FRotator timline_Rotation;
// 	if(bHit_AttachToSurfaceCaculation) //충돌 됐으면 충돌지점 normal의 반대방향(앞방향)으로 방향 셋팅
// 	{	//플레이어 회전값 ~ 충돌 정방향
// 		timline_Rotation = UKismetMathLibrary::RLerp(Player->GetActorRotation(),
// 		   UKismetMathLibrary::MakeRotFromX(Climb_OutHit.Normal * -1), Value, false);
// 	}
// 	else // 충돌 안됐으면 NewRot의 방향을 플레이어의 앞 방향으로 셋팅
// 	{
// 		timline_Rotation = UKismetMathLibrary::MakeRotFromX(Player->GetActorForwardVector());
// 	}
// 	//벽 타고 있을 때 NewRot으로 방향 회전
// 	Player->SetActorRotation(timline_Rotation);
// }
// 타임라인이 완료될때 실행
// void UHD_PlayerClimbComponent::OnClimbTimelineFinished()
// {
// }
// // 벽타기 멈추기
// void UHD_PlayerClimbComponent::StopClimbing()
// {	
// 	Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
// 	Player->GetCharacterMovement()->bOrientRotationToMovement = true;
// 	Player->SetActorRotation(FRotator(0, Player->GetActorRotation().Yaw,  0));
// 	IsClimbing = false;
// 	Player->GetCapsuleComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
// 	Player->PlayerAttackComponent->TargetFOV = 90;
// }
// // 케이블 컴포넌트 붙이기
// void UHD_PlayerClimbComponent::AttachToCable()
// {
// 	FHitResult OutHit;
// 	FVector Start = Player->GetActorLocation();
// 	FVector End = Player->GetActorLocation() + Player->GetActorForwardVector() * 200;
// 	ECollisionChannel TraceChannel = ECC_GameTraceChannel5;
// 	FCollisionQueryParams params;
// 	
// 	params.AddIgnoredActor(Player);
// 	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, params);
// 	
// 	if(bHit)
// 	{
// 		//DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Purple, false, 3.0f);
// 		Player->CableCompoent->EndLocation = OutHit.ImpactPoint - Player->GetActorLocation();
// 	}
// 	//else DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
// }
// // 파쿠르 하는 함수
// void UHD_PlayerClimbComponent::LedgeMantleCaculation()
// {
// 	if(Climb_OutHit.GetActor() == Dragon) return;
// 	
// 	FHitResult ledge_OutHit;
// 	ECollisionChannel TraceChannel = ECC_GameTraceChannel5;
// 	FCollisionQueryParams ledge_params;
// 	ledge_params.AddIgnoredActor(Player);
// 	
// 	FVector ledge_Start = Player->GetActorLocation() + FVector(0,0,100);
// 	FVector ledge_End = Player->GetActorLocation() + FVector(0,0,100) + UKismetMathLibrary::GetForwardVector(Player->GetActorRotation()) * 100;
// 	
// 	bool ledge_bHit = GetWorld()->LineTraceSingleByChannel(ledge_OutHit, ledge_Start, ledge_End, TraceChannel, ledge_params);
// 	
// 	if(ledge_bHit){}//DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Purple, false, 3.0f);
// 	else // 벽타기 하다가 충돌이 안되고 있다면
// 	{
// 		//DrawDebugLine(GetWorld(), ledge_Start, ledge_End, FColor::Purple, false, 3.0f);
// 		
// 		FHitResult ledge_OutHit_2;
// 		FCollisionQueryParams ledge_params_2;
// 		ledge_params_2.AddIgnoredActor(Player);
// 		//모션 워핑 : 파쿠르 몽타지 이동시켜주는 컴포넌트
// 		FMotionWarpingTarget ledge_MW_Target;
// 		FMotionWarpingTarget ledge_MW_Target_2;
// 		
// 		for(int32 i=0; i<=20; i++) // 플레이어 앞으로 트레이스 반복 쏘기
// 		{ 
// 			FVector ledge_Start_2 = (ledge_OutHit.TraceStart + FVector(0, 0, 90)) + (UKismetMathLibrary::GetForwardVector(Player->GetActorRotation()) * i * 3);
// 			FVector ledge_End_2 =  (ledge_OutHit.TraceStart + FVector(0, 0, 90)) + (UKismetMathLibrary::GetForwardVector(Player->GetActorRotation()) * i * 3) - FVector(0,0,100);
//
// 			bool ledge_bHit_2 = GetWorld()->LineTraceSingleByChannel(ledge_OutHit_2, ledge_Start_2, ledge_End_2, TraceChannel, ledge_params_2);
// 			
// 			if(ledge_bHit_2) // 앞으로 쏜 것 중 하나라도 맞았으면 파쿠르 시작
// 			{
// 				//DrawDebugLine(GetWorld(), ledge_Start_2, ledge_End_2, FColor::Blue, false, 3.0f);
// 				switch (ledge_bHit_2)
// 				{
// 				case 1:	// 모션 워핑 
// 					ledge_MW_Target.Name = FName("Ledge Climb Up");
// 					ledge_MW_Target.Location = FVector(Player->GetActorLocation().X + Player->GetActorForwardVector().X * 48 ,Player->GetActorLocation().Y + Player->GetActorForwardVector().Y * 48, ledge_OutHit_2.Location.Z);
// 					ledge_MW_Target.Rotation = Player->GetActorRotation();
//
// 					ledge_MW_Target_2.Name = FName("Ledge Climb Forward");
// 					ledge_MW_Target_2.Location = FVector(ledge_OutHit_2.Location.X + Player->GetActorForwardVector().X * 60, ledge_OutHit_2.Location.Y + Player->GetActorForwardVector().Y * 60,Player->GetActorLocation().Z + 100.f);
// 					ledge_MW_Target_2.Rotation = Player->GetActorRotation();
// 				
// 					//DrawDebugSphere(GetWorld(), ledge_OutHit_2.Location, 30.f, 12, FColor::Black, false, 3.f);
// 					Player->MotionWarpingComponent->AddOrUpdateWarpTarget(ledge_MW_Target);
// 					Player->MotionWarpingComponent->AddOrUpdateWarpTarget(ledge_MW_Target_2);
// 					PlayerAnim->Montage_Play(LedgeMontage, 1.0f);	
// 					//Player->SetActorEnableCollision(false);
// 					Player->SetActorEnableCollision(false);
// 					//Player->springArm->bDoCollisionTest = false;
// 					return;
// 					
// 				default:
// 				break;
// 				}
// 			}
// 			else
// 			{
// 				//DrawDebugLine(GetWorld(), ledge_Start_2, ledge_End_2, FColor::Blue, false, 3.0f);
// 				
// 			}
// 		}
// 	}
// 	
// }
//
// void UHD_PlayerClimbComponent::PlayMontageNotifyBegin(FName NotifyName,
// 	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
// {
// 	if(NotifyName == FName("Climb"))
// 	{
// 	UE_LOG(LogTemp, Warning, TEXT("Notify %s has begun!"), *NotifyName.ToString());
// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
// 	Player->SetActorEnableCollision(true);
// 	//Player->springArm->bDoCollisionTest = true;
// 	Player->PlayerClimbComponent->StopClimbing();
// 	}
// }




