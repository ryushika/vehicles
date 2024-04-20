#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

#include "Types.generated.h"

#define LOG(Format, ...) UE_LOG(LogTemp, Warning, Format, ##__VA_ARGS__)
#define DRAW_SPHERE(Location, Color, LifeTime) DrawDebugSphere(GetWorld(), Location, 25.f, 8, Color, false, LifeTime)

#define PROJECTILE_CHANNEL ECC_GameTraceChannel1
#define ENEMY_CHANNEL ECC_GameTraceChannel2

#define PATH_TO_GAMEMODES "/Game/Blueprints/Core/"

UENUM(BlueprintType)
enum class EFireModes : uint8
{
	Single,
	Auto,

	MAX
};

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	Tank,
	Turret,
	Exit,

	MAX
};

USTRUCT(BlueprintType)
struct FProjectileStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Options")
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Options")
	TObjectPtr<class UStaticMesh> ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Options")
	TObjectPtr<class UParticleSystem> ParticleTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit effects")
	TObjectPtr<class UParticleSystem> HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit effects")
	TObjectPtr<class USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn effects")
	TObjectPtr<class UParticleSystem> ShootingParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn effects")
	TArray<TObjectPtr<class USoundBase>> ShootingSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 1300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 30.f;
};

