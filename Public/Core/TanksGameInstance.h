// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Types.h"

#include "TanksGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTanksGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
//  Functions

public:
	FProjectileStruct GetProjectileData(const FName& RowName) const;
	void OpenMainLevel(const class UObject* WorldContextObject, EGameMode LevelGameMode);

protected:

private:

//  Variables

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Data")
	class UDataTable* ProjectileData;

private:
};
