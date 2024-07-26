// Fill out your copyright notice in the Description page of Project Settings.


#include "../LHJ/HD_DragonAnim.h"

#include "HD_Dragon.h"

void UHD_DragonAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		fsm = OwnerActor->FindComponentByClass<UHD_DragonFSM>();
		//middleBoss = OwnerActor->FindComponentByClass<AHJ_MiddleBoss>();
		Dragon = Cast<AHD_Dragon>(OwnerActor);
	}
}

void UHD_DragonAnim::PlayShoutAnim()
{
	bPlayShoutAnim = true;
}
