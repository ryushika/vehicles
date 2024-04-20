// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSelectWidget.h"
#include "../TanksGameModeBase.h"
#include "UMG/Public/Components/Button.h"
#include "Core/TanksGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UPlayerSelectWidget::NativeConstruct()
{
	if (TankButton)
	{
		TankButton->OnReleased.AddDynamic(this, &UPlayerSelectWidget::OnTankButtonReleased);
		TankButton->OnHovered.AddDynamic(this, &UPlayerSelectWidget::OnTankButtonHovered);
	}
	if (TurretButton)
	{
		TurretButton->OnReleased.AddDynamic(this, &UPlayerSelectWidget::OnTurretButtonReleased);
		TurretButton->OnHovered.AddDynamic(this, &UPlayerSelectWidget::OnTurretButtonHovered);
	}
	if (ExitButton)
	{
		ExitButton->OnReleased.AddDynamic(this, &UPlayerSelectWidget::OnExitButtonReleased);
		ExitButton->OnHovered.AddDynamic(this, &UPlayerSelectWidget::OnExitButtonHovered);
	}
}

void UPlayerSelectWidget::OnTankButtonReleased()
{
	UTanksGameInstance* GameInstance = GetGameInstance<UTanksGameInstance>();
	if (GameInstance)
	{
		GameInstance->OpenMainLevel(this, EGameMode::Tank);
	}

	RemoveFromParent();

	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->bShowMouseCursor = false;
}

void UPlayerSelectWidget::OnTurretButtonReleased()
{
	UTanksGameInstance* GameInstance = GetGameInstance<UTanksGameInstance>();
	if (GameInstance)
	{
		GameInstance->OpenMainLevel(this, EGameMode::Turret);
	}

	RemoveFromParent();

	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->bShowMouseCursor = false;
}

void UPlayerSelectWidget::OnExitButtonReleased()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UPlayerSelectWidget::OnTankButtonHovered()
{
	OnStartButtonHovered.Broadcast(EGameMode::Tank);
}

void UPlayerSelectWidget::OnTurretButtonHovered()
{
	OnStartButtonHovered.Broadcast(EGameMode::Turret);
}

void UPlayerSelectWidget::OnExitButtonHovered()
{
	OnStartButtonHovered.Broadcast(EGameMode::Exit);
}

