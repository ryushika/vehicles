// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
//  Functions

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	virtual void BeginPlay() override;

private:

//  Variables

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UEndGameWidget> EndGameWidgetClass;

	UPROPERTY()
	UEndGameWidget* EndGameWidget;

private:
};
