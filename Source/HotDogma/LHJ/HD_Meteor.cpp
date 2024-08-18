#include "../LHJ/HD_Meteor.h"

#include "HD_Dragon.h"
#include "HD_DragonFSM.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerComponent/PlayerStatusComponent.h"
#include "Kismet/GameplayStatics.h"

AHD_Meteor::AHD_Meteor()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("Meteor"));
	collisionComp->SetSphereRadius(55);
	RootComponent = collisionComp;
	collisionComp->OnComponentHit.AddDynamic(this, &AHD_Meteor::OnHit);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->SetUpdatedComponent(RootComponent);
	ProjectileComp->InitialSpeed = 500;
	ProjectileComp->MaxSpeed = speed;
	ProjectileComp->bShouldBounce = false;
	ProjectileComp->ProjectileGravityScale = 0.f;
}

void AHD_Meteor::BeginPlay()
{
	Super::BeginPlay();

	Dragon = Cast<AHD_Dragon>(UGameplayStatics::GetActorOfClass(GetWorld(), AHD_Dragon::StaticClass()));
}

void AHD_Meteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHD_Meteor::SetTarget(FVector target)
{
	FVector dir = target - GetActorLocation();
	FRotator dirRot = dir.Rotation();
	//dirRot.Yaw += 45.f;
	dir.Normalize();
	//SetActorRotation(dirRot);
	ProjectileComp->Velocity = dir * speed;
}

void AHD_Meteor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       FVector NormalImpulse, const FHitResult& Hit)
{
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	bool bRtn = false;

	if (OtherActor->Tags.Contains("Meteor"))
	{
		return;
	}

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		MeteorNia,
		GetActorLocation(),
		GetActorRotation()
	);

	if (MeteorSound)
		UGameplayStatics::PlaySound2D(GetWorld(), MeteorSound);

	// 플레이어가 카메라 쉐이크 범위 안에 있다면 카메라 쉐이크 효과를 호출한다.
	bRtn = GetAttackMeteor(CameraShakeDist);
	if (bRtn)
	{
		for (auto DamageOtherActor : DamageActorSet)
		{
			if (DamageOtherActor->GetName().Contains("Player"))
			{
				AHD_CharacterPlayer* player = Cast<AHD_CharacterPlayer>(DamageOtherActor);
				if (player)
				{
					if (player->PlayerStatusComponent->CurrHP > 0)
						player->GetPlayerCameraShake();
				}
			}
		}
		DamageActorSet.Empty();
	}

	// 일정 범위 안에 있는 캐릭터를 감지하여 데미지를 준다.
	bRtn = GetAttackMeteor(AttackDist);
	if (bRtn)
	{
		for (auto DamageOtherActor : DamageActorSet)
		{
			Dragon->strDamageAttackType = "Meteor";
			UGameplayStatics::ApplyDamage(DamageOtherActor, Dragon->fsm->Damage_Meteor, GetInstigatorController(),
			                              Dragon,
			                              UDamageType::StaticClass());
		}
		DamageActorSet.Empty();
	}
	Destroy();
}

bool AHD_Meteor::GetAttackMeteor(const float& AttackDistance)
{
	bool bRtn = false;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation();
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel4;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Dragon);
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, AttackDistance,
	                                       UEngineTypes::ConvertToTraceType(CollisionChannel), false,
	                                       ActorsToIgnore, EDrawDebugTrace::None, OutHits,
	                                       true);

	for (auto& Hit : OutHits)
	{
		if (IsValid(Hit.GetActor()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s TargetLoc : %.f, %.f, %.f"),
			       *Hit.GetActor()->GetName(), Hit.GetActor()->GetActorLocation().X,
			       Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z);
			FVector JumpPressTarget = Hit.GetActor()->GetActorLocation(); //- Player->GetActorLocation();
			//UE_LOG(LogTemp, Warning, TEXT("Cutting_TargetLoc : %.f, %.f, %.f"), Cutting_TargetLoc.X, Cutting_TargetLoc.Y, Cutting_TargetLoc.Z);
			//UE_LOG(LogTemp, Warning, TEXT("Player->GetActorLocation() : %.f, %.f, %.f"), Player->GetActorLocation().X, Player->GetActorLocation().Y, Player->GetActorLocation().Z);
			FVector newLoc = JumpPressTarget - Dragon->GetActorLocation();
			newLoc.Normalize();

			if (Hit.GetActor()->Tags.Num() > 0 && (Hit.GetActor()->Tags[0].ToString().Equals("HD_Player")))
			{
				if (!DamageActorSet.Contains(Hit.GetActor()))
				{
					DamageActorSet.Add(Hit.GetActor());
				}
			}
		}
	}

	if (DamageActorSet.Num() > 0)
		bRtn = true;

	return bRtn;
}
