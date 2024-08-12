// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/HD_GamePlayWidget.h"

#include "Components/AudioComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UHD_GamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHD_GamePlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHD_GamePlayWidget::ChangeSwitcher(int32 index)
{
	WidgetSwitcher->SetActiveWidgetIndex(index);
}

void UHD_GamePlayWidget::PlayBGM_Crow()
{
	//PlaySound(BGM_Crow);
	CrowAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BGM_Crow);       // 사운드를 붙일 컴포넌트 (여기서는 루트 컴포넌트에 붙임)
}

void UHD_GamePlayWidget::StopBGM_Crow()
{
	if (CrowAudioComponent && CrowAudioComponent->IsPlaying())CrowAudioComponent->Stop();
}
