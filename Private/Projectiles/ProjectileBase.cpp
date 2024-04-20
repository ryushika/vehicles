

#include "Projectiles/ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"


AProjectileBase::AProjectileBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->SetNotifyRigidBodyCollision(true);
	ProjectileMesh->SetCollisionObjectType(PROJECTILE_CHANNEL);
	ProjectileMesh->SetCollisionResponseToChannel(PROJECTILE_CHANNEL, ECR_Ignore);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	AddOwnedComponent(ProjectileMovement);
	ProjectileMovement->bRotationFollowsVelocity = true;

	Tags.Add("Projectile");
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::InitializeProjectile(const FProjectileStruct& NewStruct)
{
	ProjectileStruct = NewStruct;

	ProjectileMesh->SetStaticMesh(ProjectileStruct.ProjectileMesh);
	ParticleTrail->SetTemplate(ProjectileStruct.ParticleTrail);

	ProjectileMovement->InitialSpeed = ProjectileStruct.MovementSpeed;
	ProjectileMovement->MaxSpeed = ProjectileStruct.MovementSpeed;

	if (APawn* LocalPawn = GetInstigator())
	{
		bool bIsPlayerOwned = LocalPawn->GetController()->IsPlayerController();
		ProjectileMesh->SetCollisionResponseToChannel(ENEMY_CHANNEL, bIsPlayerOwned ? ECR_Ignore : ECR_Block);
		ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, bIsPlayerOwned ? ECR_Ignore : ECR_Ignore);
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		bool bShouldNotDealDamage = (GetOwner()->ActorHasTag("Enemy") && OtherActor->ActorHasTag("Enemy"))
									|| GetOwner() == OtherActor;
		if (bShouldNotDealDamage)
		{
			LOG(TEXT("Should not deal damage"));
			Destroy();
			return;
		}

		UGameplayStatics::ApplyDamage(OtherActor, ProjectileStruct.Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	
		SpawnHitParticle(Hit.Location, OtherActor->GetActorRotation());
		SpawnHitSound(Hit.Location);

		LOG(TEXT("Hit %s"), *UKismetSystemLibrary::GetDisplayName(OtherActor));
		Destroy();
	}
}

void AProjectileBase::SpawnHitParticle(const FVector& Location, const FRotator& Rotation)
{
	if (ProjectileStruct.HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileStruct.HitParticle, Location, Rotation);
	}
}

void AProjectileBase::SpawnHitSound(const FVector& Location)
{
	if (ProjectileStruct.HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileStruct.HitSound, Location);
	}
}

