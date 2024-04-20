

#include "Components/ShootingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/TanksGameInstance.h"
#include "Projectiles/ProjectileBase.h"
#include "Sound/SoundCue.h"

UShootingComponent::UShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AvailableProjectileTypes.AddUnique("Default");
	AvailableFireModes.AddUnique(EFireModes::Single);
	AvailableFireModes.AddUnique(EFireModes::Auto);
}

void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	SetProjectileType(0);
}

void UShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UShootingComponent::Initialize(APawn* NewInstigator, USceneComponent* SpawnPoint)
{
	Instigator = NewInstigator;
	SpawnPointComponent = SpawnPoint;
}

void UShootingComponent::StartShooting()
{
	bIsShooting = true;

	if (!GetWorld()->GetTimerManager().IsTimerActive(ShootingTimer))
	{
		switch (CurrentFireMode)
		{
		case EFireModes::Single:
			PullTrigger();
			GetWorld()->GetTimerManager().SetTimer(ShootingTimer, ReloadTime, false);
			break;
		case EFireModes::Auto:
			GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UShootingComponent::PullTrigger, ReloadTime, true, 0.f);
			break;
		default:
			break;
		}
	}
}

void UShootingComponent::StopShooting()
{
	bIsShooting = false;
}

void UShootingComponent::PullTrigger()
{
	if (bIsShooting)
	{
		Shoot();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootingTimer);
		return;
	}
}

void UShootingComponent::Shoot()
{
	if (SpawnPointComponent)
	{
		FVector Location = SpawnPointComponent->GetComponentLocation();

		SpawnShootSound(Location);
		SpawnShootParticles(Location);
		SpawnProjectile(Location);
		OnShoot.Broadcast(ReloadTime);
	}
}

void UShootingComponent::CycleProjectileTypes()
{
	CurrentProjectileIndex = (CurrentProjectileIndex + 1) % AvailableProjectileTypes.Num();
	if (AvailableProjectileTypes.IsValidIndex(CurrentProjectileIndex))
	{
		SetProjectileType(AvailableProjectileTypes[CurrentProjectileIndex]);
	}
}

void UShootingComponent::SetProjectileType(const FName& NewType)
{
	UTanksGameInstance* GameInstance = GetWorld()->GetGameInstance<UTanksGameInstance>();
	if (GameInstance)
	{
		CurrentProjectileStruct = GameInstance->GetProjectileData(NewType);
	}
}

void UShootingComponent::SetProjectileType(int32 NewIndex)
{
	if (AvailableProjectileTypes.IsValidIndex(NewIndex))
	{
		SetProjectileType(AvailableProjectileTypes[NewIndex]);
	}
}

void UShootingComponent::CycleFireModes()
{
	CurrentFireModeIndex = (CurrentFireModeIndex + 1) % AvailableFireModes.Num();
	if (AvailableFireModes.IsValidIndex(CurrentFireModeIndex))
	{
		CurrentFireMode = AvailableFireModes[CurrentFireModeIndex];
	}
}

void UShootingComponent::SpawnProjectile(const FVector& Location)
{
	if (CurrentProjectileStruct.ProjectileClass)
	{
		FTransform SpawnTransform = FTransform(SpawnPointComponent->GetComponentRotation(), SpawnPointComponent->GetComponentLocation(), FVector(1.f, 1.f, 1.f));
		AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(CurrentProjectileStruct.ProjectileClass, SpawnTransform, GetOwner());
		if (Projectile)
		{
			Instigator->MoveIgnoreActorAdd(Projectile);
			Projectile->SetInstigator(Instigator);
			Projectile->InitializeProjectile(CurrentProjectileStruct);
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
		}
	}
}

void UShootingComponent::SpawnShootSound(const FVector& Location)
{
	const int R = FMath::RandRange(0, CurrentProjectileStruct.ShootingSounds.Num() - 1);

	if (CurrentProjectileStruct.ShootingSounds.IsValidIndex(R))
	{
		UGameplayStatics::SpawnSoundAtLocation
		(
			GetWorld(),
			CurrentProjectileStruct.ShootingSounds[R],
			Location,
			FRotator::ZeroRotator,
			FMath::RandRange(0.9, 1.1),
			1,
			0,
			nullptr,
			false
		);
	}
}

void UShootingComponent::SpawnShootParticles(const FVector& Location)
{
	if (CurrentProjectileStruct.ShootingParticles)
	{
		FVector Direction = SpawnPointComponent->GetForwardVector();
		FRotator Rotation = (-Direction).Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentProjectileStruct.ShootingParticles, Location, Rotation);
	}
}
