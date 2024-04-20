// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types.h"

#include "ShootingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShootSignature, float, ReloadTime);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_API UShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShootingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//  Functions

public:
	void Initialize(APawn* NewInstigator, class USceneComponent* SpawnPoint);

	UFUNCTION(BlueprintCallable)
	void StartShooting();
	
	UFUNCTION(BlueprintCallable)
	void StopShooting();

	UFUNCTION(BlueprintCallable)
	void PullTrigger();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	void CycleProjectileTypes();
	void SetProjectileType(const FName& NewType);
	void SetProjectileType(int32 NewIndex);

	void CycleFireModes();

protected:

private:
	void SpawnProjectile(const FVector& Location);
	void SpawnShootSound(const FVector& Location);
	void SpawnShootParticles(const FVector& Location);

//  Variables

public:
	UPROPERTY(BlueprintAssignable)
	FOnShootSignature OnShoot;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<EFireModes> AvailableFireModes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> AvailableProjectileTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EFireModes CurrentFireMode = EFireModes::Single;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime = 2.f;

private:
	bool bIsShooting = false;
	FTimerHandle ShootingTimer;

	UPROPERTY()
	int32 CurrentProjectileIndex = 0;

	UPROPERTY()
	int32 CurrentFireModeIndex = 0;

	UPROPERTY()
	FProjectileStruct CurrentProjectileStruct;

	UPROPERTY()
	TObjectPtr<class USceneComponent> SpawnPointComponent;

	UPROPERTY()
	class APawn* Instigator = nullptr;
};
