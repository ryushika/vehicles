// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainPlayerController.h"
#include "UI/EndGameWidget.h"

void AMainPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (EndGameWidgetClass && !EndGameWidget)
	{
		EndGameWidget = CreateWidget<UEndGameWidget>(this, EndGameWidgetClass);
		if (EndGameWidget)
		{
			EndGameWidget->InitializeWidget(bIsWinner);
			EndGameWidget->AddToViewport();

			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = true;

			//GetPawn()->DisableInput(this);
			GetPawn()->Destroy();
		}
	}
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
}
