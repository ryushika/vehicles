// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TanksGameModeBase.h"
#include "PlayerSelectWidget.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Pawns/BasePawn.h"
#include "Pawns/TankPawn.h"
#include "Pawns/TurretPawn.h"

void ATanksGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentGameState(EGameplayState::Playing);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATanksGameModeBase::SpawnEnemyTanks, 2, true);

	SpawnEnemiesOnSpawnPoints(true);
	SpawnEnemiesOnSpawnPoints(false);
}

void ATanksGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATanksGameModeBase::SpawnEnemiesOnSpawnPoints(bool bTank)
{
	TSubclassOf<ABasePawn> EnemyClass;
	if (bTank)
	{
		EnemyClass = EnemyTankClass;
	}
	else
	{
		EnemyClass = EnemyTurretClass;
	}
	TSubclassOf<AActor> SpawnPointClass;
	if (bTank)
	{
		SpawnPointClass = EnemyTankSpawnPointClass;
	}
	else
	{
		SpawnPointClass = EnemyTurretSpawnPointClass;
	}

	if (SpawnPointClass)
	{
		TArray<AActor*> SpawnPoints;
		UGameplayStatics::GetAllActorsOfClass(this, SpawnPointClass, SpawnPoints);
		for (AActor* SpawnPoint : SpawnPoints)
		{
			SpawnEnemy(SpawnPoint->GetActorLocation(), bTank);
		}
	}
}

void ATanksGameModeBase::SpawnEnemyTanks()
{
	int32 RandomIndex = FMath::RandRange(0, TankSpawnPoints.Num() - 1);
	if (TankSpawnPoints.IsValidIndex(RandomIndex))
	{
		AActor* SpawnPoint = TankSpawnPoints[RandomIndex];
		SpawnEnemy(SpawnPoint->GetActorLocation(), true);

		EnemiesSpawnedInRuntime++;
		if (EnemiesSpawnedInRuntime >= EnemiesMax)
		{
			GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		}
	}
}

void ATanksGameModeBase::SpawnEnemy(const FVector& Location, bool bTank)
{
	if (bTank)
	{
		if (EnemyTankClass)
		{
			GetWorld()->SpawnActor<ATankPawn>(EnemyTankClass, Location, FRotator{});
			EnemiesAlive++;
		}
	}
	else
	{
		if (EnemyTurretClass)
		{
			GetWorld()->SpawnActor<ATurretPawn>(EnemyTurretClass, Location, FRotator{});
			EnemiesAlive++;
		}
	}
}

void ATanksGameModeBase::HandleGameState(EGameplayState NewState)
{
	switch (NewState)
	{
	case EGameplayState::Playing:
		if (!bGamePaused) UGameplayStatics::SetGamePaused(GetWorld(), false);
		break;
	case EGameplayState::MainMenu:
		
		break;
	case EGameplayState::GameOver:
		EndGame(false);
		break;
	case EGameplayState::Victory:
		EndGame(true);
		break;
	case EGameplayState::Paused:
		if (bGamePaused) UGameplayStatics::SetGamePaused(GetWorld(), true);
		break;
	default:
		break;
	}
}

void ATanksGameModeBase::EndGame(bool bPlayerWon)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bPlayerWon;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}

void ATanksGameModeBase::PawnKilled(APawn* PawnKilled)
{
	if (PawnKilled)
	{
		if (PawnKilled->Tags.Find(TEXT("Player")) != INDEX_NONE)
		{
			HandleGameState(EGameplayState::GameOver);
		}
		else if (PawnKilled->Tags.Find(TEXT("Enemy")) != INDEX_NONE)
		{
			PawnKilled->DetachFromControllerPendingDestroy();
			EnemiesAlive--;
			if (EnemiesAlive <= 0)
			{
				HandleGameState(EGameplayState::Victory);
			}
			PawnKilled->Destroy();
		}
	}
}

