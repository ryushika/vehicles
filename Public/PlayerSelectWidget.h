// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSelectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartButtonHoveredSignature, EGameMode, ButtonGameMode);

class ATanksGameModeBase;
/**
 * 
 */
UCLASS()
class TANKS_API UPlayerSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	
public:

//  Functions

public:

protected:
	UFUNCTION()
	void OnTankButtonReleased();
	UFUNCTION()
	void OnTurretButtonReleased();
	UFUNCTION()
	void OnExitButtonReleased();

	UFUNCTION()
	void OnTankButtonHovered();
	UFUNCTION()
	void OnTurretButtonHovered();
	UFUNCTION()
	void OnExitButtonHovered();


private:

//  Variables

public:
	UPROPERTY(BlueprintAssignable)
	FOnStartButtonHoveredSignature OnStartButtonHovered;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* TankButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TurretButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
private:
	
};
