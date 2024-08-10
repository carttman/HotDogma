// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonThunderCol.h"

#include "HD_Dragon.h"
#include "HD_DragonFSM.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHD_DragonThunderCol::AHD_DragonThunderCol()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetRelativeScale3D(FVector(1, 1, 5));
	BoxComp->SetCollisionProfileName(FName("DragonAttack"));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHD_DragonThunderCol::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHD_DragonThunderCol::BeginPlay()
{
	Super::BeginPlay();

	Dragon = Cast<AHD_Dragon>(UGameplayStatics::GetActorOfClass(GetWorld(),AHD_Dragon::StaticClass()));
}

// Called every frame
void AHD_DragonThunderCol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrTime += DeltaTime;
	if (CurrTime >= CollisionOnTime)
	{
		CurrTime = 0.f;
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		   this,
		   ThunderSmog,
		   GetActorLocation(),
		   GetActorRotation()
	   );
	}

	if (BoxComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
	{
		if (CurrTime >= DestroyTime)
		{
			Destroy();
		}
	}
}

void AHD_DragonThunderCol::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	Dragon->strDamageAttackType = "Thunder";
	UGameplayStatics::ApplyDamage(OtherActor, Dragon->fsm->Damage_Thunder, Dragon->GetController(), Dragon,
								  UDamageType::StaticClass());
}
