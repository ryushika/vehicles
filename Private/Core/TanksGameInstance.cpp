// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TanksGameInstance.h"
#include "Engine/DataTable.h"

FProjectileStruct UTanksGameInstance::GetProjectileData(const FName& RowName) const
{
	if (ProjectileData)
	{
		FProjectileStruct* ProjectileInfo = ProjectileData->FindRow<FProjectileStruct>(RowName, TEXT("WeaponInfo Row Missing"), true);
		if (ProjectileInfo)
		{
			return *ProjectileInfo;
		}
	}

	return FProjectileStruct();
}

void UTanksGameInstance::OpenMainLevel(const class UObject* WorldContextObject, EGameMode LevelGameMode)
{
	FString Options = "?Game=";
	Options += PATH_TO_GAMEMODES;
	switch (LevelGameMode)
	{
	case EGameMode::Tank:
		Options += "BP_TankGameMode.BP_TankGameMode_C";
		break;
	case EGameMode::Turret:
		Options += "BP_TurretGameMode.BP_TurretGameMode_C";
		break;
	default:
		break;
	}
	UGameplayStatics::OpenLevel(WorldContextObject, TEXT("MainLevel"), true, Options);
}