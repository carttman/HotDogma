// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/HD_GameClearWidget.h"

void UHD_GameClearWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayCreditAnim();
}

void UHD_GameClearWidget::PlayCreditAnim()
{
	PlayAnimation(Credits);
}
