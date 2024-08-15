// Fill out your copyright notice in the Description page of Project Settings.


#include "HotDogma/Ksw/HD_Projectile.h"

#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Particles/ParticleSystemComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
AHD_Projectile::AHD_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//1. 충돌체 등록
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	//2. 충돌 프로파일 설정
	collisionComp->SetCollisionProfileName(TEXT("OverlapAll"));

	//3 충돌체 크기 설정
	collisionComp->SetSphereRadius(13);
	//4. 루트로 등록
	RootComponent = collisionComp;

	collisionComp->OnComponentHit.AddDynamic(this, &AHD_Projectile::OnHit);

	//5. 외관 컴포넌트 등록
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	//6. 부모컴포넌트 지정
	bodyMeshComp->SetupAttachment(collisionComp);
	//7. 메시 충돌체 비활성화
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//8. 메시 외관 크기 설정
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	//9. 발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	//10. movement 컴포넌트가 갱신시킬 컴포넌트 지정
	movementComp->SetUpdatedComponent(collisionComp);
	//11. 발사체 초기속도 설정
	movementComp->InitialSpeed = 500;
	//12. 발사체 최대속도 설정
	movementComp->MaxSpeed = 500;
	//13. 발사체 반동여부
	movementComp->bShouldBounce = true;
	//14. 발사체 반동값
	movementComp->Bounciness = 0.3f;
	// 중력 제거
	movementComp->ProjectileGravityScale = 0.0f;

	//15. 발사체 이펙트
	trailEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("TrailEffect"));
	//16. 터질때 이펙트
	explosionEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("ExplosionEffect"));
}

void AHD_Projectile::Die()
{
	Destroy();
}

void AHD_Projectile::SetTarget(FVector target)
{
	//방향 설정
	FVector dir = target - GetActorLocation();
	dir.Normalize();
	//방향 설정
	SetActorRotation(dir.Rotation());
	//이동 설정
	movementComp->Velocity = dir * speed;
}

void AHD_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//피격 이펙트
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());

	//피격 사운드
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
	UGameplayStatics::ApplyDamage(OtherActor, 5, GetInstigatorController(), this, UDamageType::StaticClass());
	//총알 제거
	Destroy();
}

