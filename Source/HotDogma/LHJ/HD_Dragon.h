#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HD_Dragon.generated.h"

UCLASS()
class HOTDOGMA_API AHD_Dragon : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHD_Dragon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
							 class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnOverlapBegin_Scratch(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapBegin_TailSlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);	

	// 더미 콜리전
	// NoCollision 처리할건데 충돌처리 문제나면 변경하기
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	// 스켈레탈 매쉬
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SkeletalComp;

#pragma region Body Material Getter&Setter
	UFUNCTION()
	UMaterialInterface* GetMaterial();
	UFUNCTION()
	void SetMaterial(UMaterialInstanceDynamic* & NewMaterial);
#pragma endregion

#pragma region Init
#pragma region 공격용 콜리전
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* HandCollision_R;
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* HandCollision_L;
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* TailCollision;
#pragma endregion

#pragma region 플레이어 공격 추적용 콜리전
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* TargetPoint4;
#pragma endregion

#pragma region Thunder Point
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ThunderPoint8;
#pragma endregion

#pragma region Meteor Point
	UPROPERTY(EditAnywhere)
	class USceneComponent* MeteorPoint;
#pragma endregion

#pragma region 클라이밍
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbHand_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm1_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm2_L;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbHand_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm1_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbArm2_R;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbFoot_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg1_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg2_L;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg3_L;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbFoot_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg1_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg2_R;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbLeg3_R;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail1;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail2;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail3;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail4;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbTail5;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbSpine1;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbSpine2;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck1;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck2;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck3;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck4;
	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbNeck5;

	UPROPERTY(EditAnywhere)
	class UChildActorComponent* ClimbHead;
#pragma endregion

	UFUNCTION()
	void CreateSlashPoint();
	UFUNCTION()
	void CreateAttackPoint();
	UFUNCTION()
	void CreateClimbCollision();
#pragma endregion

	UPROPERTY()
	class UHD_DragonFSM* fsm; // fsm

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 3500; // 체력
	UPROPERTY()
	float CurrHP;

	UPROPERTY()
	TArray<class ACharacter*> CharacterArr; // 공격 대상 리스트
	UPROPERTY()
	TSet<AActor*> DamageActorSet; // 중복 데미지 처리 방지용
	UPROPERTY()
	FString strDamageAttackType; // 공격 종류

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ThunderVFX1;
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ThunderVFX2;
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* MeteorVFX;

	//UI
	int LineCnt = 8; // 피통 줄 수
	float LineHpValue; // 한줄 당 피 값

	UPROPERTY(EditAnywhere)
	float TimeDilation = 0.5f;
	UPROPERTY(EditAnywhere)
	float Duration = 1.25f;
	UPROPERTY(EditAnywhere)
	float BeforeTimeToCredit = 35.f;
	void CallCredit();
	void DeathNarr();

	UPROPERTY()
	class ACHJ_GameMode* gm; // 게임모드
	
	UPROPERTY()
	class AHD_CharacterPlayer* Player;
};