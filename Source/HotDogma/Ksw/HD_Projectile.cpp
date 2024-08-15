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

	//1. �浹ü ���
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	//2. �浹 �������� ����
	collisionComp->SetCollisionProfileName(TEXT("OverlapAll"));

	//3 �浹ü ũ�� ����
	collisionComp->SetSphereRadius(13);
	//4. ��Ʈ�� ���
	RootComponent = collisionComp;

	collisionComp->OnComponentHit.AddDynamic(this, &AHD_Projectile::OnHit);

	//5. �ܰ� ������Ʈ ���
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	//6. �θ�������Ʈ ����
	bodyMeshComp->SetupAttachment(collisionComp);
	//7. �޽� �浹ü ��Ȱ��ȭ
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//8. �޽� �ܰ� ũ�� ����
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	//9. �߻�ü ������Ʈ
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	//10. movement ������Ʈ�� ���Ž�ų ������Ʈ ����
	movementComp->SetUpdatedComponent(collisionComp);
	//11. �߻�ü �ʱ�ӵ� ����
	movementComp->InitialSpeed = 500;
	//12. �߻�ü �ִ�ӵ� ����
	movementComp->MaxSpeed = 500;
	//13. �߻�ü �ݵ�����
	movementComp->bShouldBounce = true;
	//14. �߻�ü �ݵ���
	movementComp->Bounciness = 0.3f;
	// �߷� ����
	movementComp->ProjectileGravityScale = 0.0f;

	//15. �߻�ü ����Ʈ
	trailEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("TrailEffect"));
	//16. ������ ����Ʈ
	explosionEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("ExplosionEffect"));
}

void AHD_Projectile::Die()
{
	Destroy();
}

void AHD_Projectile::SetTarget(FVector target)
{
	//���� ����
	FVector dir = target - GetActorLocation();
	dir.Normalize();
	//���� ����
	SetActorRotation(dir.Rotation());
	//�̵� ����
	movementComp->Velocity = dir * speed;
}

void AHD_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//�ǰ� ����Ʈ
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());

	//�ǰ� ����
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
	UGameplayStatics::ApplyDamage(OtherActor, 5, GetInstigatorController(), this, UDamageType::StaticClass());
	//�Ѿ� ����
	Destroy();
}

