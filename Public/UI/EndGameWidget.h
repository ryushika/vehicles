// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types.h"
#include "EndGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
//  Functions

public:
	void InitializeWidget(bool bVictory);

protected:
	virtual void NativeConstruct();

	UFUNCTION()
	void RestartAsTank();
	UFUNCTION()
	void RestartAsTurret();
	UFUNCTION()
	void ExitGame();

private:

//  Variables

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* EndGameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UButton* RestartAsTankButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* RestartAsTurretButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitButton;
private:
};
