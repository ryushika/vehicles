// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EndGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Core/TanksGameInstance.h"


void UEndGameWidget::InitializeWidget(bool bVictory)
{
	if (bVictory)
	{
		EndGameText->SetText(FText::FromString("Victory!"));
		EndGameText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	}
	else
	{
		EndGameText->SetText(FText::FromString("Defeat!"));
		EndGameText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
}

void UEndGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartAsTankButton->OnReleased.AddDynamic(this, &UEndGameWidget::RestartAsTank);
	RestartAsTurretButton->OnReleased.AddDynamic(this, &UEndGameWidget::RestartAsTurret);
	ExitButton->OnReleased.AddDynamic(this, &UEndGameWidget::ExitGame);
}

void UEndGameWidget::RestartAsTank()
{
	UTanksGameInstance* GameInstance = GetGameInstance<UTanksGameInstance>();
	if (GameInstance)
	{
		GameInstance->OpenMainLevel(this, EGameMode::Tank);
	}
}

void UEndGameWidget::RestartAsTurret()
{
	UTanksGameInstance* GameInstance = GetGameInstance<UTanksGameInstance>();
	if (GameInstance)
	{
		GameInstance->OpenMainLevel(this, EGameMode::Turret);
	}
}

void UEndGameWidget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
