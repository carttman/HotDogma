// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/HD_GameOverWidget.h"

#include "Components/Button.h"
#include "HotDogma/HD_Character/HD_CharacterPlayer.h"
#include "HotDogma/HD_Character/HD_PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UHD_GameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Restart->OnClicked.AddDynamic(this, &UHD_GameOverWidget::OnMyReStart);
	Button_BackTitle->OnClicked.AddDynamic(this, &UHD_GameOverWidget::OnMyBackTitle);
	Player = Cast<AHD_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UHD_GameOverWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UHD_GameOverWidget::OnMyReStart()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GameOverReverseFade();
	GetWorld()->GetTimerManager().SetTimer(PlayTimerHandle, this, &UHD_GameOverWidget::OpenLevelTimer, 2.f, false);
}

void UHD_GameOverWidget::OnMyBackTitle()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GameOverReverseFade();
	GetWorld()->GetTimerManager().SetTimer(PlayTimerHandle, this, &UHD_GameOverWidget::OpenTitle, 2.f, false);
}

void UHD_GameOverWidget::OpenLevelTimer()
{
	FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(*mapName));
	Player->PlayerContoller->SetShowMouseCursor(false);
	Player->PlayerContoller->SetInputMode(FInputModeGameOnly());
}

void UHD_GameOverWidget::OpenTitle()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void UHD_GameOverWidget::GameOverPlayFade()
{
	PlayAnimation(Fade_Out);
}

void UHD_GameOverWidget::GameOverReverseFade()
{
	
	PlayAnimation(Fade_In);
}
