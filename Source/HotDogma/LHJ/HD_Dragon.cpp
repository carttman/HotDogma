// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_Dragon.h"

#include "HD_DragonFSM.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotDogma/HD_GameModeBase/CHJ_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

AHD_Dragon::AHD_Dragon()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));
	SkeletalComp->SetupAttachment(RootComponent);
	SkeletalComp->SetGenerateOverlapEvents(true);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkeleton(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/LHJ/UnkaDragon/Meshes/UnkaDragon/SK_Unka_Realistic.SK_Unka_Realistic'"));
	if (tempSkeleton.Succeeded())
	{
		SkeletalComp->SetSkeletalMesh(tempSkeleton.Object);
	}
	SkeletalComp->SetCollisionProfileName(FName("DragonMeshColl"));
	SkeletalComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	fsm = CreateDefaultSubobject<UHD_DragonFSM>(TEXT("FSM"));
	//RootComponent->RegisterComponent();

	TargetPoint1 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint1"));
	TargetPoint1->SetupAttachment(SkeletalComp,TEXT("L-HorseLink"));
	TargetPoint1->SetCapsuleHalfHeight(60.f);
	TargetPoint1->SetCapsuleRadius(30.f);
	TargetPoint1->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetPoint2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint2"));
	TargetPoint2->SetupAttachment(SkeletalComp,TEXT("R-HorseLink"));
	TargetPoint2->SetCapsuleHalfHeight(60.f);
	TargetPoint2->SetCapsuleRadius(30.f);
	TargetPoint2->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetPoint3 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint3"));
	TargetPoint3->SetupAttachment(SkeletalComp,TEXT("R-Forearm"));
	TargetPoint3->SetCapsuleHalfHeight(60.f);
	TargetPoint3->SetCapsuleRadius(30.f);
	TargetPoint3->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetPoint4 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetPoint4"));
	TargetPoint4->SetupAttachment(SkeletalComp,TEXT("L-Forearm"));
	TargetPoint4->SetCapsuleHalfHeight(60.f);
	TargetPoint4->SetCapsuleRadius(30.f);
	TargetPoint4->SetCollisionProfileName(FName("DragonMeshColl"));
	TargetPoint4->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Tongue에 SocketComponent 설치
	FireScene = CreateDefaultSubobject<USceneComponent>(TEXT("FireScene"));
	FireScene->SetupAttachment(SkeletalComp, TEXT("Fire_Socket"));

	FireCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FireCollision"));
	FireCollision->SetupAttachment(FireScene);
	FireCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HandCollision_R = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HandCollision_R"));
	HandCollision_R->SetupAttachment(SkeletalComp,TEXT("R-Finger2"));
	HandCollision_R->SetRelativeRotation(FRotator(90, 0, 0));
	HandCollision_R->SetCollisionProfileName(FName("DragonAttack"));
	HandCollision_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision_R->OnComponentBeginOverlap.AddDynamic(this, &AHD_Dragon::OnOverlapBegin_Scratch);

	HandCollision_L = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HandCollision_L"));
	HandCollision_L->SetupAttachment(SkeletalComp,TEXT("L-Finger2"));
	HandCollision_L->SetRelativeRotation(FRotator(90, 0, 0));
	HandCollision_L->SetCollisionProfileName(FName("DragonAttack"));
	HandCollision_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision_L->OnComponentBeginOverlap.AddDynamic(this, &AHD_Dragon::OnOverlapBegin_Scratch);

	TailCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TailCollision"));
	TailCollision->SetupAttachment(SkeletalComp,TEXT("Tail05"));
	TailCollision->SetRelativeLocationAndRotation(FVector(43, -15, 0), FRotator(90, 0, 0));
	TailCollision->SetCapsuleHalfHeight(157);
	TailCollision->SetCapsuleRadius(37);
	TailCollision->SetCollisionProfileName(FName("DragonAttack"));
	TailCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TailCollision->OnComponentBeginOverlap.AddDynamic(this, &AHD_Dragon::OnOverlapBegin_TailSlap);

	CreateThunderPoint();
	CreateClimbCollision();

	MeteorPoint1 = CreateDefaultSubobject<USceneComponent>(TEXT("MeteorPoint1"));
	MeteorPoint1->SetupAttachment(SkeletalComp);
	MeteorPoint1->SetRelativeLocation(FVector(-350, -150, 450));
	MeteorPoint2 = CreateDefaultSubobject<USceneComponent>(TEXT("MeteorPoint2"));
	MeteorPoint2->SetupAttachment(SkeletalComp);
	MeteorPoint2->SetRelativeLocation(FVector(-200, -150, 450));
	MeteorPoint3 = CreateDefaultSubobject<USceneComponent>(TEXT("MeteorPoint3"));
	MeteorPoint3->SetupAttachment(SkeletalComp);
	MeteorPoint3->SetRelativeLocation(FVector(200, -150, 450));
	MeteorPoint4 = CreateDefaultSubobject<USceneComponent>(TEXT("MeteorPoint4"));
	MeteorPoint4->SetupAttachment(SkeletalComp);
	MeteorPoint4->SetRelativeLocation(FVector(350, -150, 450));
}

void AHD_Dragon::CreateThunderPoint()
{
	ThunderPoint1 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint1"));
	ThunderPoint1->SetupAttachment(SkeletalComp);
	ThunderPoint1->SetRelativeLocation(FVector(200, 0, 0));

	ThunderPoint2 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint2"));
	ThunderPoint2->SetupAttachment(SkeletalComp);
	ThunderPoint2->SetRelativeLocation(FVector(150, -150, 0));

	ThunderPoint3 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint3"));
	ThunderPoint3->SetupAttachment(SkeletalComp);
	ThunderPoint3->SetRelativeLocation(FVector(15, -185.0, 0));

	ThunderPoint4 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint4"));
	ThunderPoint4->SetupAttachment(SkeletalComp);
	ThunderPoint4->SetRelativeLocation(FVector(-150.0, -150.0, 0));

	ThunderPoint5 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint5"));
	ThunderPoint5->SetupAttachment(SkeletalComp);
	ThunderPoint5->SetRelativeLocation(FVector(-200.0, 0, 0));

	ThunderPoint6 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint6"));
	ThunderPoint6->SetupAttachment(SkeletalComp);
	ThunderPoint6->SetRelativeLocation(FVector(-150.0, 150.0, 0));

	ThunderPoint7 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint7"));
	ThunderPoint7->SetupAttachment(SkeletalComp);
	ThunderPoint7->SetRelativeLocation(FVector(15, 185.0, 0));

	ThunderPoint8 = CreateDefaultSubobject<USceneComponent>(TEXT("ThunderPoint8"));
	ThunderPoint8->SetupAttachment(SkeletalComp);
	ThunderPoint8->SetRelativeLocation(FVector(150.0, 150.0, 0));
}

void AHD_Dragon::CreateClimbCollision()
{
	ClimbHand_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbHand_L"));
	ClimbHand_L->SetupAttachment(SkeletalComp, FName("L-Finger2"));
	ClimbArm1_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbArm1_L"));
	ClimbArm1_L->SetupAttachment(SkeletalComp, FName("L-Arm1"));
	ClimbArm2_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbArm2_L"));
	ClimbArm2_L->SetupAttachment(SkeletalComp, FName("L-Arm2"));

	ClimbHand_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbHand_R"));
	ClimbHand_R->SetupAttachment(SkeletalComp, FName("R-Finger2"));
	ClimbArm1_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbArm1_R"));
	ClimbArm1_R->SetupAttachment(SkeletalComp, FName("R-Arm1"));
	ClimbArm2_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbArm2_R"));
	ClimbArm2_R->SetupAttachment(SkeletalComp, FName("R-Arm2"));

	ClimbFoot_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbFoot_L"));
	ClimbFoot_L->SetupAttachment(SkeletalComp, FName("R-Toe1"));
	ClimbLeg1_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbLeg1_L"));
	ClimbLeg1_L->SetupAttachment(SkeletalComp, FName("R-Foot"));
	ClimbLeg2_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbLeg2_L"));
	ClimbLeg2_L->SetupAttachment(SkeletalComp, FName("R_Leg2"));
	ClimbLeg3_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbLeg3_L"));
	ClimbLeg3_L->SetupAttachment(SkeletalComp, FName("R_Leg3"));

	ClimbFoot_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbFoot_R"));
	ClimbFoot_R->SetupAttachment(SkeletalComp, FName("L-Toe1"));
	ClimbLeg1_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbLeg1_R"));
	ClimbLeg1_R->SetupAttachment(SkeletalComp, FName("L-Foot"));
	ClimbLeg2_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbLeg2_R"));
	ClimbLeg2_R->SetupAttachment(SkeletalComp, FName("L-Leg2"));
	ClimbLeg3_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbLeg3_R"));
	ClimbLeg3_R->SetupAttachment(SkeletalComp, FName("L-Leg3"));

	ClimbTail1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbTail1"));
	ClimbTail1->SetupAttachment(SkeletalComp, FName("Tail02"));
	ClimbTail2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbTail2"));
	ClimbTail2->SetupAttachment(SkeletalComp, FName("Tail04"));
	ClimbTail3 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbTail3"));
	ClimbTail3->SetupAttachment(SkeletalComp, FName("Tail06"));
	ClimbTail4 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbTail4"));
	ClimbTail4->SetupAttachment(SkeletalComp, FName("Tail08"));
	ClimbTail5 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbTail5"));
	ClimbTail5->SetupAttachment(SkeletalComp, FName("Tail10"));

	ClimbSpine1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbSpine1"));
	ClimbSpine1->SetupAttachment(SkeletalComp, FName("SpineSK1"));
	ClimbSpine2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbSpine2"));
	ClimbSpine2->SetupAttachment(SkeletalComp, FName("SpineSK2"));

	ClimbNeck1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbNeck1"));
	ClimbNeck1->SetupAttachment(SkeletalComp, FName("Neck"));
	ClimbNeck2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbNeck2"));
	ClimbNeck2->SetupAttachment(SkeletalComp, FName("Neck1"));
	ClimbNeck3 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbNeck3"));
	ClimbNeck3->SetupAttachment(SkeletalComp, FName("Neck2"));
	ClimbNeck4 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbNeck4"));
	ClimbNeck4->SetupAttachment(SkeletalComp, FName("Neck3"));
	ClimbNeck5 = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbNeck5"));
	ClimbNeck5->SetupAttachment(SkeletalComp, FName("Neck4"));

	ClimbHead = CreateDefaultSubobject<UChildActorComponent>(TEXT("ClimbHead"));
	ClimbHead->SetupAttachment(SkeletalComp, FName("HeadSK"));
}

void AHD_Dragon::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->GetMaxSpeed() * 0.75;

	if (ThunderVFX2)
		ThunderVFX2->Delay = 0.25f;

	gm = Cast<ACHJ_GameMode>(GetWorld()->GetAuthGameMode());

	LineHpValue = MaxHP / LineCnt;
}

void AHD_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어&동료 찾기
	if (CharacterArr.Num() == 0)
	{
		TArray<AActor*> OutActors;
		// 필드에 있는 캐릭터를 인지한다.
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACharacter::StaticClass(),
		                                             TEXT("HD_Player"), OutActors);
		//
		// TArray<AActor*> OutCharacterActor;
		// // 필드에 있는 캐릭터를 인지한다.
		// UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACharacter::StaticClass(),
		//                                              TEXT("PlayerCharacter"), OutCharacterActor);

		if (OutActors.Num() > 0)
		{
			for (auto othActor : OutActors)
			{
				ACharacter* charac = Cast<ACharacter>(othActor);
				if (charac&&!charac->GetName().Contains("Player"))
					CharacterArr.Add(charac);
			}
		}
	}
}

void AHD_Dragon::OnOverlapBegin_Scratch(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (!DamageActorSet.Contains(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Actor Name : %s"), *OtherActor->GetName());
		DamageActorSet.Add(OtherActor);

		strDamageAttackType = "Scratch";

		UGameplayStatics::ApplyDamage(OtherActor, fsm->Damage_Scratch, GetController(), this,
		                              UDamageType::StaticClass());
	}
}

void AHD_Dragon::OnOverlapBegin_TailSlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (!DamageActorSet.Contains(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Actor Name : %s"), *OtherActor->GetName());
		DamageActorSet.Add(OtherActor);

		strDamageAttackType = "TailSlap";

		UGameplayStatics::ApplyDamage(OtherActor, fsm->Damage_Scratch, GetController(), this,
		                              UDamageType::StaticClass());
	}
}

float AHD_Dragon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("%s Takes Damage : %f"), *GetName(), damage);
	CurrHP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("%s Takes Damage : %f"), *GetName(), CurrHP);

	if (gm)
	{

		float EmptyHpBarCnt = fmod((CurrHP - 1), (float)LineHpValue) + 1; // 남은 피통
		float CurrEmptyHPLine = (CurrHP - 1) / LineHpValue;	// 남은 피통 줄수
		
		float BarHp = CurrHP - (int)CurrEmptyHPLine * LineHpValue;

		gm->SetDragonHPUI(BarHp, LineHpValue,CurrEmptyHPLine);
		
	}

	return damage;
}
