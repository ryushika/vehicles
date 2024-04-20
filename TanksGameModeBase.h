// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/TankPawn.h"
#include "Pawns/TurretPawn.h"
#include "TanksGameModeBase.generated.h"

UENUM()
enum class EGameplayState : uint8
{
	Playing,
	GameOver,
	MainMenu,
	Victory,
	Paused
};

UCLASS()
class TANKS_API ATanksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

//  Functions

public:
	void SpawnEnemiesOnSpawnPoints(bool bTank = true);
	UFUNCTION()
	void SpawnEnemyTanks();
	void SpawnEnemy(const FVector& Location, bool bTank = true);

	UFUNCTION()
	FORCEINLINE EGameplayState GetCurrentGameState() { return CurrentGameState; };

	UFUNCTION()
	FORCEINLINE void SetCurrentGameState(EGameplayState NewState) { CurrentGameState = NewState; HandleGameState(EGameplayState::Paused); };

	void PawnKilled(APawn* PawnKilled);

protected:
	UFUNCTION()
	void HandleGameState(EGameplayState NewState);

	void EndGame(bool bPlayerWon);

private:

//  Variables

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawner")
	TSubclassOf<AActor> EnemyTankSpawnPointClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawner")
	TSubclassOf<AActor> EnemyTurretSpawnPointClass;

	UPROPERTY()
	TArray<AActor*> TankSpawnPoints;

    UPROPERTY()
	FTimerHandle SpawnTimerHandle;
   	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawner")
	TSubclassOf<ATankPawn> EnemyTankClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawner")
	TSubclassOf<ATurretPawn> EnemyTurretClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawner")
	TSubclassOf<ATankPawn> PlayerTank;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawner")
	TSubclassOf<ATurretPawn> PlayerTurret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Combat")
	int32 EnemiesMax = 2;

	UPROPERTY()
	int32 EnemiesSpawnedInRuntime = 0;

	UPROPERTY()
	int32 EnemiesAlive = 0;

private:
	UPROPERTY()
	EGameplayState CurrentGameState;

	UPROPERTY()
	bool bGamePaused = false;
};
