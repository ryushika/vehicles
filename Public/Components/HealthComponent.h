// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Types.h"

#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTakenSignature, float, Damage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//  Functions

public:
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetHealthPercentage() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintPure)
	FText GetHealthIntText(); 

	void UpdateHealth(float DeltaHealth);

protected:

private:

//  Variables

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;
	UPROPERTY(BlueprintAssignable)
	FOnDamageTakenSignature OnDamageTaken;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Health")
	FTimeline HealthTimeline;

	UPROPERTY(EditAnywhere, Category = "Health")
	FTimerHandle HealthTimerHandle;

	float CurveFloatValue;
	float TimelineValue;

private:
};
